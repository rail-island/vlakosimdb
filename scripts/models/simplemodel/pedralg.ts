// Port of the clip CSG algorithm (jt7 jlib/jmath/pedralg.cpp + pedralg.h).
//
// A solid is carved from a box by cutting it, from three orthogonal directions,
// along 2D silhouette polygons. The body is kept as a union of convex
// sub-polyhedra; each clip edge defines a plane that splits every sub-polyhedron;
// sub-polyhedra outside the silhouette are dropped; the surviving surface is the
// set of faces that occur exactly once.
//
// Coordinates use JS doubles throughout (the C++ used `double` here).

import { EPS, pointInPolygon2d, type Vec2, type Vec3 } from "../geom.ts";

const dist = (a: Vec3, b: Vec3): number => Math.hypot(a.x - b.x, a.y - b.y, a.z - b.z);

// A vertex reference (index into PedrAlg.vrs) carrying interpolated texture coords
// t1/t2 (0 for the pure-clip path; meaningful when a rotate solid is re-clipped).
interface VrRecord {
  id: number;
  t1: number;
  t2: number;
}
const vrec = (id: number, t1 = 0, t2 = 0): VrRecord => ({ id, t1, t2 });
const cloneRec = (r: VrRecord): VrRecord => ({ id: r.id, t1: r.t1, t2: r.t2 });

interface ClipPolygon {
  vrs: Vec2[];
  faceids: string[];
}
interface PolyedrData {
  vrs: Vec3[];
  faces: Face[];
}

class Face {
  faceid: string;
  texture = "";
  culling = 1;
  n: Vec3 = { x: 0, y: 0, z: 0 };
  vrs: VrRecord[] = [];

  constructor(faceid = "", v1?: number, v2?: number, v3?: number, v4?: number) {
    this.faceid = faceid;
    if (v1 !== undefined) this.vrs.push(vrec(v1));
    if (v2 !== undefined) this.vrs.push(vrec(v2));
    if (v3 !== undefined) this.vrs.push(vrec(v3));
    if (v4 !== undefined && v4 >= 0) this.vrs.push(vrec(v4));
  }

  // Wrapped access (matches Face::vr with its (index+count*16)%count trick).
  vr(index: number): VrRecord {
    const n = this.vrs.length;
    return this.vrs[((index % n) + n) % n]!;
  }
  id(index: number): number {
    return this.vr(index).id;
  }

  clone(): Face {
    const f = new Face(this.faceid);
    f.vrs = this.vrs.map(cloneRec);
    f.n = { ...this.n };
    f.texture = this.texture;
    f.culling = this.culling;
    return f;
  }

  countnormal(alg: PedrAlg): void {
    const p0 = alg.vrs[this.vrs[0]!.id]!;
    const p1 = alg.vrs[this.vrs[1]!.id]!;
    const p2 = alg.vrs[this.vrs[2]!.id]!;
    const a = { x: p0.x - p1.x, y: p0.y - p1.y, z: p0.z - p1.z };
    const b = { x: p0.x - p2.x, y: p0.y - p2.y, z: p0.z - p2.z };
    let n: Vec3 = { x: a.y * b.z - a.z * b.y, y: a.z * b.x - a.x * b.z, z: a.x * b.y - a.y * b.x };
    const s = Math.hypot(n.x, n.y, n.z);
    if (s > 0) n = { x: n.x / s, y: n.y / s, z: n.z / s };
    this.n = n;
  }

  // True if some vertex is on the positive side of the plane.
  gt(alg: PedrAlg, a: number, b: number, c: number, d: number): boolean {
    for (const r of this.vrs) {
      const p = alg.vrs[r.id]!;
      if (a * p.x + b * p.y + c * p.z + d > EPS) return true;
    }
    return false;
  }

  // True if the face crosses the plane / lies on the opposite side of *dir.
  // dirObj.v carries the running side sign (0/1/-1).
  violates(alg: PedrAlg, dirObj: { v: number }, a: number, b: number, c: number, d: number): boolean {
    for (const r of this.vrs) {
      const v = alg.vrs[r.id]!;
      const x = a * v.x + b * v.y + c * v.z + d;
      if (dirObj.v) {
        if (dirObj.v * x < -EPS) return true;
      } else {
        if (x < -EPS) dirObj.v = -1;
        if (x > EPS) dirObj.v = 1;
      }
    }
    return false;
  }

  // Intersect with plane ax+by+cz+d=0, splitting the face into two parts pushed to
  // `faces`. Records the cut segment in `fms`/`cpp.dbls`. Returns true if cut.
  prusecik(
    faceindex: number, alg: PedrAlg, fms: { v1: number; v2: number; face: number }[],
    a: number, b: number, c: number, d: number, faces: Face[], cpp: ComPointProtocol,
  ): boolean {
    const vrs = this.vrs;
    const n = vrs.length;
    let bp1 = -1;
    let bp2 = -1;
    for (let i = 0; i < n; i++) {
      const v = alg.vrs[this.id(i)]!;
      if (Math.abs(a * v.x + b * v.y + c * v.z + d) / Math.sqrt(a * a + b * b + c * c) < EPS) {
        if (bp1 < 0) bp1 = i;
        else bp2 = i;
      }
    }

    const prs: { p: VrRecord; vridx: number }[] = []; // edge intersections
    for (let i = 0; i < n; i++) {
      const v1 = this.id(i);
      const v2 = this.id(i + 1);
      const vr1 = this.vr(i);
      const vr2 = this.vr(i + 1);
      if (i === bp1 || i + 1 === bp1) continue;
      if (i === bp2 || i + 1 === bp2) continue;

      const existing = cpp.getb(v1, v2);
      if (existing) {
        prs.push({ p: existing, vridx: i });
        continue;
      }

      const A = alg.vrs[v1]!;
      const B = alg.vrs[v2]!;
      const D = { x: B.x - A.x, y: B.y - A.y, z: B.z - A.z };
      const jm = a * D.x + b * D.y + c * D.z;
      if (Math.abs(jm) < EPS) continue; // parallel
      const cit = a * A.x + b * A.y + c * A.z + d;
      const t = -cit / jm;
      if (t < EPS || t > 1 - EPS) continue; // outside the segment
      const newpt = { x: A.x + D.x * t, y: A.y + D.y * t, z: A.z + D.z * t };
      const p = vrec(alg.addvr(newpt));
      p.t1 = vr1.t1 + (vr2.t1 - vr1.t1) * t;
      p.t2 = vr1.t2 + (vr2.t2 - vr1.t2) * t;
      prs.push({ p, vridx: i });
      cpp.add(v1, v2, p);
    }

    if (prs.length > 2) throw new Error("pedralg: more than 2 edge intersections");

    if (bp2 >= 0) {
      if (Math.abs(bp1 - bp2) === 1 || Math.abs(bp1 - bp2) === n - 1) {
        cpp.dbls.add(this.id(bp1), this.id(bp2));
        faces.push(this.clone());
        return false;
      }
      const f1 = new Face(this.faceid);
      const f2 = new Face(this.faceid);
      f1.texture = this.texture;
      f2.texture = this.texture;
      f1.n = { ...this.n };
      f2.n = { ...this.n };
      for (let i = bp1; i <= bp2; i++) f1.vrs.push(cloneRec(this.vr(i)));
      for (let i = bp2; i <= bp1 + n; i++) f2.vrs.push(cloneRec(this.vr(i)));
      faces.push(f1, f2);
      fms.push({ v1: this.id(bp1), v2: this.id(bp2), face: faceindex });
      cpp.dbls.add(this.id(bp1), this.id(bp2));
      return true;
    } else if (bp1 >= 0) {
      if (prs.length === 0) {
        faces.push(this.clone());
        return false;
      }
      const f1 = new Face(this.faceid);
      const f2 = new Face(this.faceid);
      f1.texture = this.texture;
      f2.texture = this.texture;
      f1.n = { ...this.n };
      f2.n = { ...this.n };
      const pr = prs[0]!;
      let cnt1 = 0;
      let cnt2 = 0;
      if (pr.vridx < bp1) cnt2 = n;
      else cnt1 = n;

      f1.vrs.push(cloneRec(pr.p));
      for (let i = pr.vridx + 1; i <= bp1 + cnt1; i++) f1.vrs.push(cloneRec(this.vr(i)));

      for (let i = bp1; i <= pr.vridx + cnt2; i++) f2.vrs.push(cloneRec(this.vr(i)));
      f2.vrs.push(cloneRec(pr.p));

      faces.push(f1, f2);
      fms.push({ v1: this.id(bp1), v2: pr.p.id, face: faceindex });
      cpp.dbls.add(this.id(bp1), pr.p.id);
      return true;
    } else if (prs.length === 2) {
      const f1 = new Face(this.faceid);
      const f2 = new Face(this.faceid);
      f1.texture = this.texture;
      f2.texture = this.texture;
      f1.n = { ...this.n };
      f2.n = { ...this.n };
      let cnt1 = 0;
      let cnt2 = 0;
      if (prs[0]!.vridx < prs[1]!.vridx) cnt2 = n;
      else cnt1 = n;

      f1.vrs.push(cloneRec(prs[0]!.p));
      for (let i = prs[0]!.vridx + 1; i <= prs[1]!.vridx + cnt1; i++) f1.vrs.push(cloneRec(this.vr(i)));
      f1.vrs.push(cloneRec(prs[1]!.p));

      f2.vrs.push(cloneRec(prs[1]!.p));
      for (let i = prs[1]!.vridx + 1; i <= prs[0]!.vridx + cnt2; i++) f2.vrs.push(cloneRec(this.vr(i)));
      f2.vrs.push(cloneRec(prs[0]!.p));

      faces.push(f1, f2);
      fms.push({ v1: prs[0]!.p.id, v2: prs[1]!.p.id, face: faceindex });
      cpp.dbls.add(prs[0]!.p.id, prs[1]!.p.id);
      return true;
    } else {
      faces.push(this.clone());
    }
    return false;
  }

  // Reorder vertices so culling/winding is consistent with the normal `n`.
  normalizeCulling(alg: PedrAlg): void {
    let normala: Vec3 | null = null;
    let ncsize = -1;
    const n = this.vrs.length;
    for (let i = 0; i < n; i++) {
      const x = alg.vrs[this.id(i - 1)]!;
      const y = alg.vrs[this.id(i)]!;
      const z = alg.vrs[this.id(i + 1)]!;
      const a = { x: x.x - y.x, y: x.y - y.y, z: x.z - y.z };
      const b = { x: z.x - y.x, y: z.y - y.y, z: z.z - y.z };
      const actsize = Math.abs(a.x * b.x + a.y * b.y + a.z * b.z);
      if (ncsize < 0 || actsize < ncsize) {
        normala = { x: a.y * b.z - a.z * b.y, y: a.z * b.x - a.x * b.z, z: a.x * b.y - a.y * b.x };
        ncsize = actsize;
      }
    }
    const mul = normala!.x * this.n.x + normala!.y * this.n.y + normala!.z * this.n.z;
    if (mul < 0) this.vrs.reverse();
  }

  // Canonical vertex order (rotate to min id) + orient normal inward.
  normalize(alg: PedrAlg, poly: Polyedr): void {
    const n = this.vrs.length;
    let minidx = 0;
    let minvalue = this.vrs[0]!.id;
    for (let i = 1; i < n; i++) {
      if (this.vrs[i]!.id < minvalue) {
        minidx = i;
        minvalue = this.vrs[i]!.id;
      }
    }
    let d = 1;
    if (this.id(minidx - 1) < this.id(minidx + 1)) d = -1;
    const buf: VrRecord[] = [];
    for (let i = 0; i < n; i++) buf.push(cloneRec(this.vr(minidx + i * d)));
    this.vrs = buf;

    const v0 = alg.vrs[this.vrs[0]!.id]!;
    let sum = 0;
    for (const face of poly.faces) {
      for (const r of face.vrs) {
        const v = alg.vrs[r.id]!;
        sum += (v.x - v0.x) * this.n.x + (v.y - v0.y) * this.n.y + (v.z - v0.z) * this.n.z;
      }
    }
    if (sum < 0) {
      this.n.x = -this.n.x;
      this.n.y = -this.n.y;
      this.n.z = -this.n.z;
    }
  }

  equalto(face: Face): boolean {
    if (face.vrs.length !== this.vrs.length) return false;
    for (let i = 0; i < this.vrs.length; i++) if (face.vrs[i]!.id !== this.vrs[i]!.id) return false;
    return true;
  }
}

class Polyedr {
  faces: Face[] = [];

  clone(): Polyedr {
    const p = new Polyedr();
    p.faces = this.faces.map((f) => f.clone());
    return p;
  }

  cut(alg: PedrAlg, pds: Polyedr[], a: number, b: number, c: number, d: number, cpp: ComPointProtocol, faceid: string): void {
    if (this.faces.length === 0) {
      pds.push(this.clone());
      return;
    }
    const sideDir = { v: 0 };
    let mustCut = false;
    for (const f of this.faces) {
      if (f.violates(alg, sideDir, a, b, c, d)) {
        mustCut = true;
        break;
      }
    }
    if (!mustCut) {
      pds.push(this.clone());
      return;
    }

    const fms: { v1: number; v2: number; face: number }[] = [];
    const newfaces: Face[] = [];
    for (let fi = 0; fi < this.faces.length; fi++) {
      this.faces[fi]!.prusecik(fi, alg, fms, a, b, c, d, newfaces, cpp);
    }

    const comface = new Face(faceid);
    const s = Math.sqrt(a * a + b * b + c * c);
    comface.n = { x: a / s, y: b / s, z: c / s };

    let res = false;
    for (const dbl of cpp.dbls.items) {
      if (findnextvdbl(dbl.v1, dbl.v1, comface.vrs, cpp)) {
        res = true;
        break;
      }
      if (findnextvdbl(dbl.v2, dbl.v2, comface.vrs, cpp)) {
        res = true;
        break;
      }
    }
    if (!res) {
      pds.push(this.clone());
      return;
    }

    const res1 = new Polyedr();
    const res2 = new Polyedr();
    for (const f of newfaces) {
      if (f.gt(alg, a, b, c, d)) res1.faces.push(f);
      else res2.faces.push(f);
    }
    res1.faces.push(comface.clone());
    res2.faces.push(comface);
    pds.push(res1, res2);
  }

  // Every vertex must project inside the silhouette polygon.
  isinpolygon(dir: number, alg: PedrAlg, poly: ClipPolygon): boolean {
    const tested = new Uint8Array(alg.vrs.length);
    for (const face of this.faces) {
      for (const r of face.vrs) {
        const id = r.id;
        if (tested[id]) continue;
        tested[id] = 1;
        const v = alg.vrs[id]!;
        let p: Vec2;
        switch (dir) {
          case 0: p = { x: v.z, y: v.y }; break;
          case 1: p = { x: v.x, y: v.z }; break;
          default: p = { x: v.x, y: v.y }; break;
        }
        if (!pointInPolygon2d(poly.vrs, p)) return false;
      }
    }
    return true;
  }

  normalize(alg: PedrAlg): void {
    for (const f of this.faces) f.normalize(alg, this);
  }
  hasface(face: Face): boolean {
    for (const f of this.faces) if (f.equalto(face)) return true;
    return false;
  }
}

// Recursive backtracking to assemble the common cut face from `cpp.dbls` edges.
function findnextvdbl(startv: number, v: number, cfv: VrRecord[], cpp: ComPointProtocol): boolean {
  if (v === startv && cfv.length >= 3) return true;
  if (cfv.some((r) => r.id === v)) return false;
  cfv.push(vrec(v));
  for (const dbl of cpp.dbls.items) {
    const nextv = dbl.other(v);
    if (nextv >= 0 && findnextvdbl(startv, nextv, cfv, cpp)) return true;
  }
  cfv.pop();
  return false;
}

interface DoubleEdge {
  v1: number;
  v2: number;
  other: (v: number) => number;
}

class Doubles {
  items: DoubleEdge[] = [];
  getd(v1: number, v2: number): number {
    return this.items.findIndex((d) => (d.v1 === v1 && d.v2 === v2) || (d.v1 === v2 && d.v2 === v1));
  }
  add(v1: number, v2: number): void {
    if (this.getd(v1, v2) < 0) {
      this.items.push({ v1, v2, other: (v) => (v === v1 ? v2 : v === v2 ? v1 : -1) });
    }
  }
  clear(): void {
    this.items = [];
  }
}

class ComPointProtocol {
  points: { v1: number; v2: number; p: VrRecord }[] = [];
  dbls = new Doubles();
  getb(v1: number, v2: number): VrRecord | null {
    for (const cp of this.points) {
      if ((cp.v1 === v1 && cp.v2 === v2) || (cp.v1 === v2 && cp.v2 === v1)) return cp.p;
    }
    return null;
  }
  add(v1: number, v2: number, p: VrRecord): void {
    this.points.push({ v1, v2, p });
  }
}

export class PedrAlg {
  vrs: Vec3[] = [];
  polys: Polyedr[] = [];
  faces: Face[] = [];
  clipx: ClipPolygon[] = [];
  clipy: ClipPolygon[] = [];
  clipz: ClipPolygon[] = [];

  addvr(v: Vec3): number {
    for (let i = 0; i < this.vrs.length; i++) if (dist(v, this.vrs[i]!) < EPS) return i;
    this.vrs.push({ x: v.x, y: v.y, z: v.z });
    return this.vrs.length - 1;
  }

  // Add a clip silhouette polygon for a direction.
  addclip(dir: number, points: { x: number; y: number; faceid?: string }[]): void {
    const poly: ClipPolygon = { vrs: [], faceids: [] };
    for (const pt of points) {
      poly.vrs.push({ x: pt.x, y: pt.y });
      poly.faceids.push(pt.faceid ?? "");
    }
    if (dir === 0) this.clipx.push(poly);
    else if (dir === 1) this.clipy.push(poly);
    else this.clipz.push(poly);
  }

  initBox(left: number, right: number, bottom: number, top: number, deep: number, tome: number): void {
    this.vrs = [];
    this.polys = [];
    const v1 = this.addvr({ x: left, y: bottom, z: tome });
    const v2 = this.addvr({ x: right, y: bottom, z: tome });
    const v3 = this.addvr({ x: right, y: top, z: tome });
    const v4 = this.addvr({ x: left, y: top, z: tome });
    const v5 = this.addvr({ x: left, y: bottom, z: deep });
    const v6 = this.addvr({ x: right, y: bottom, z: deep });
    const v7 = this.addvr({ x: right, y: top, z: deep });
    const v8 = this.addvr({ x: left, y: top, z: deep });
    const poly = new Polyedr();
    poly.faces.push(new Face("", v1, v2, v3, v4));
    poly.faces.push(new Face("", v5, v6, v7, v8));
    poly.faces.push(new Face("", v1, v2, v6, v5));
    poly.faces.push(new Face("", v4, v3, v7, v8));
    poly.faces.push(new Face("", v1, v4, v8, v5));
    poly.faces.push(new Face("", v2, v3, v7, v6));
    this.polys.push(poly);
    for (const f of poly.faces) f.countnormal(this);
  }

  initalg(): void {
    let edge = 0;
    const scan = (polys: ClipPolygon[]) => {
      for (const p of polys) for (const v of p.vrs) {
        if (Math.abs(v.x) > edge) edge = Math.abs(v.x);
        if (Math.abs(v.y) > edge) edge = Math.abs(v.y);
      }
    };
    scan(this.clipx);
    scan(this.clipy);
    scan(this.clipz);
    edge += 1;
    this.initBox(-edge, edge, -edge, edge, -edge, edge);
  }

  // Load a prebuilt polyhedron (for re-clipping a rotate solid). Accepts any
  // structurally-compatible source (PedrAlg output or a rotate solid).
  loadInput(pedr: { vrs: Vec3[]; faces: { faceid: string; texture: string; vrs: VrRecord[] }[] }): void {
    for (const v of pedr.vrs) this.vrs.push({ x: v.x, y: v.y, z: v.z });
    const poly = new Polyedr();
    for (const f of pedr.faces) {
      const nf = new Face(f.faceid);
      nf.texture = f.texture;
      nf.vrs = f.vrs.map(cloneRec);
      poly.faces.push(nf);
    }
    for (const f of poly.faces) f.countnormal(this);
    this.polys.push(poly);
  }

  // Cut all sub-polyhedra by plane ax+by+cz+d=0.
  cutPlane(a: number, b: number, c: number, d: number, faceid: string): void {
    const newpds: Polyedr[] = [];
    for (const poly of this.polys) {
      const cpp = new ComPointProtocol();
      poly.cut(this, newpds, a, b, c, d, cpp, faceid);
    }
    this.polys = newpds;
  }

  // Cut by every edge of a silhouette polygon in a given direction.
  cutPolygon(dir: number, poly: ClipPolygon): void {
    const n = poly.vrs.length;
    for (let i = 0; i < n; i++) {
      const A = poly.vrs[i % n]!;
      const B = poly.vrs[(i + 1) % n]!;
      const a = B.y - A.y;
      const b = A.x - B.x;
      const c = -(a * A.x + b * A.y);
      const fid = poly.faceids[i]!;
      if (dir === 0) this.cutPlane(0, b, a, c, fid);
      else if (dir === 1) this.cutPlane(a, 0, b, c, fid);
      else this.cutPlane(a, b, 0, c, fid);
    }
  }

  filter(dir: number, poly: ClipPolygon): void {
    this.polys = this.polys.filter((p) => p.isinpolygon(dir, this, poly));
  }

  makeside(dir: number, cutPolys: ClipPolygon[]): void {
    if (cutPolys.length === 0) return;
    const polys0 = this.polys.map((p) => p.clone());
    const respolys: Polyedr[] = [];
    for (const cp of cutPolys) {
      this.polys = polys0.map((p) => p.clone());
      this.cutPolygon(dir, cp);
      this.filter(dir, cp);
      for (const p of this.polys) respolys.push(p.clone());
    }
    this.polys = respolys;
  }

  uniquefaces(): void {
    for (let i = 0; i < this.polys.length; i++) {
      const poly = this.polys[i]!;
      for (const face of poly.faces) {
        let shared = false;
        for (let k = i + 1; k < this.polys.length; k++) {
          if (this.polys[k]!.hasface(face)) {
            shared = true;
            break;
          }
        }
        if (!shared) this.faces.push(face.clone());
      }
    }
  }

  normalize(): void {
    for (const p of this.polys) p.normalize(this);
  }
  normalizeCulling(): void {
    for (const f of this.faces) f.normalizeCulling(this);
  }

  run(): void {
    this.makeside(0, this.clipx);
    this.makeside(1, this.clipy);
    this.makeside(2, this.clipz);
    this.normalize();
    this.uniquefaces();
    this.normalizeCulling();
  }

  // The surviving surface as a {vrs, faces} polyhedron (faces reference vrs).
  toPolyedr(): PolyedrData {
    return { vrs: this.vrs, faces: this.faces };
  }
}

export type { PolyedrData, Face };
