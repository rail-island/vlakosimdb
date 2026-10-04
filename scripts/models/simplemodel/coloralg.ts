// Port of the colorize/stamp algorithms (jt7 jlib/glo/coloralg.cpp + .h).
//
// ColorAlg assigns a texture and per-vertex s/t to each generated polygon by
// matching the polygon normal to the best-aligned colorize "side". StampAlg
// stamps decal polygons clipped to a texture rectangle. Polygons are mutated in
// fast-xml-parser shape (`@_` attributes, `vertex` array).

import { EPS, normalize, cross, pointInPolygon2d, type Vec2, type Vec3 } from "../geom.ts";

// A polygon / vertex node in fast-xml-parser shape (`@_`-prefixed attributes).
export type XmlPolygon = Record<string, unknown>;

// ---- ColorCond: faceid condition mini-language ( * ^ + - ) --------------------

interface ColorCondItem {
  s: string;
  mustbe: boolean;
  mustnot: boolean;
  canbe: boolean;
}

export class ColorCond {
  start = true;
  items: ColorCondItem[] = [];

  load(s: string | undefined): this {
    s = s ?? "";
    let i = 0;
    const isWs = (ch: string) => ch === " " || ch === "\t" || ch === "\n" || ch === "\r";
    const readToken = (): string => {
      let out = "";
      while (i < s!.length && !isWs(s![i]!)) out += s![i++];
      return out;
    };
    while (i < s.length) {
      while (i < s.length && isWs(s[i]!)) i++;
      if (i >= s.length) break;
      const ch = s[i];
      if (ch === "*") {
        i++;
        this.start = true;
      } else if (ch === "^") {
        i++;
        this.start = false;
      } else if (ch === "+") {
        i++;
        this.items.push({ s: readToken(), mustbe: true, mustnot: false, canbe: false });
      } else if (ch === "-") {
        i++;
        this.items.push({ s: readToken(), mustbe: false, mustnot: true, canbe: false });
      } else {
        this.items.push({ s: readToken(), mustbe: false, mustnot: false, canbe: true });
      }
    }
    return this;
  }

  accept(faceid: string): boolean {
    let res = this.start;
    for (const it of this.items) {
      if (it.s === faceid) {
        if (it.canbe && !this.start) res = true;
        if (it.mustnot) return false;
      } else {
        if (it.mustbe) return false;
      }
    }
    return res;
  }
}

// Side key per direction index (0..5). Used to look up per-side flip flags in
// ColorAlg.flipS (from a model's options.json).
export const DIR_KEYS = ["x+", "x-", "y+", "y-", "z+", "z-"];

export function normalFromDir(dir: number): Vec3 {
  switch (dir) {
    case 0: return { x: 1, y: 0, z: 0 };
    case 1: return { x: -1, y: 0, z: 0 };
    case 2: return { x: 0, y: 1, z: 0 };
    case 3: return { x: 0, y: -1, z: 0 };
    case 4: return { x: 0, y: 0, z: 1 };
    case 5: return { x: 0, y: 0, z: -1 };
    default: return { x: 0, y: 0, z: 0 };
  }
}

// Projection of a 3D point onto the 2D plane / depth axis for d in {0,1,2}.
const getx = (p: Vec3, d: number): number => (d === 0 ? p.z : p.x);
const gety = (p: Vec3, d: number): number => (d === 1 ? p.z : p.y);
const getz = (p: Vec3, d: number): number => (d === 0 ? p.x : d === 1 ? p.y : p.z);
function setProjected(d: number, x: number, y: number, z: number): Vec3 {
  // inverse of getx/gety/getz
  if (d === 0) return { x: z, y, z: x };
  if (d === 1) return { x, y: z, z: y };
  return { x, y, z };
}

// ---- ColorAlg ----------------------------------------------------------------

interface ColorSide {
  material: boolean;
  dir: number;
  x1: number; y1: number; x2: number; y2: number;
  dx?: number; dy?: number; xscale?: number; yscale?: number;
  name: string;
  transparent: boolean;
  transp?: boolean;
  cond: ColorCond;
  rkoef: string; gkoef: string; bkoef: string;
  denylight: boolean;
  n: Vec3;
  loaded: boolean;
}

export class ColorAlg {
  items: ColorSide[] = [];
  // Per-side flip flags (keyed by DIR_KEYS, e.g. { "x-": true }). A true entry
  // mirrors `s` (s -> 1-s) on that side so an opposite-face pair does not render
  // its logo reversed. From the model's options.json.
  flipS: Record<string, boolean> = {};

  addSide(
    dir: number, x1: number, y1: number, x2: number, y2: number, name: string,
    transparent: boolean, cond: string, rkoef: string, gkoef: string, bkoef: string,
    denylight: boolean,
  ): void {
    if (dir < 0 || dir >= 6) return;
    const it: ColorSide = {
      material: false,
      dir,
      x1, y1, x2, y2,
      name,
      transparent,
      cond: new ColorCond().load(cond),
      rkoef, gkoef, bkoef,
      denylight,
      n: normalFromDir(dir),
      loaded: true,
    };
    if (Math.abs(it.x1 - it.x2) < EPS || Math.abs(it.y1 - it.y2) < EPS) it.loaded = false;
    this.items.push(it);
  }

  addMaterialSide(
    dir: number, dx: number, dy: number, xscale: number, yscale: number, name: string,
    transparent: boolean, rkoef: string, gkoef: string, bkoef: string, transp: boolean,
    cond: string, denylight: boolean,
  ): void {
    if (dir < 0 || dir >= 6) return;
    // Faithful to the C++ quirk in addmaterialside: yscale is set to xscale.
    yscale = xscale;
    const it: ColorSide = {
      material: true,
      dir,
      x1: 0, y1: 0, x2: 0, y2: 0,
      dx, dy, xscale, yscale,
      name,
      transparent,
      transp,
      cond: new ColorCond().load(cond),
      rkoef, gkoef, bkoef,
      denylight,
      n: normalFromDir(dir),
      loaded: true,
    };
    if (Math.abs(it.xscale!) < EPS) it.xscale = 1;
    if (Math.abs(it.yscale!) < EPS) it.yscale = 1;
    this.items.push(it);
  }

  // Mutate a polygon node in place; returns true if it was colored.
  run(polygon: XmlPolygon): boolean {
    let n: Vec3 = { x: num(polygon, "nx"), y: num(polygon, "ny"), z: num(polygon, "nz") };
    if (Math.hypot(n.x, n.y, n.z) < EPS) return false;
    n = normalize(n);
    const faceid = str(polygon, "faceid");

    let maxi = -1;
    let maxv = 0;
    for (let i = 0; i < this.items.length; i++) {
      const it = this.items[i]!;
      if (!it.loaded) continue;
      if (!it.cond.accept(faceid)) continue;
      const act = it.n.x * n.x + it.n.y * n.y + it.n.z * n.z;
      if (act > maxv) {
        maxv = act;
        maxi = i;
      }
    }
    if (maxi < 0) return false;
    if (maxv < 0.7) return false;

    const ci = this.items[maxi]!;
    const verts = polygon.vertex as XmlPolygon[];
    const d = ci.dir >> 1;
    const pt2 = verts.map((v) => ({ x: getx(pt3(v), d), y: gety(pt3(v), d) }));

    if (ci.material) {
      if (ci.name) {
        for (let i = 0; i < verts.length; i++) {
          const sKey = ci.transp ? "@_t" : "@_s";
          const tKey = ci.transp ? "@_s" : "@_t";
          verts[i]![sKey] = pt2[i]!.x * ci.xscale! + ci.dx!;
          verts[i]![tKey] = pt2[i]!.y * ci.yscale! + ci.dy!;
        }
      }
    } else {
      for (const p of pt2) {
        if (p.x < Math.min(ci.x1, ci.x2) - EPS) return false;
        if (p.x > Math.max(ci.x1, ci.x2) + EPS) return false;
        if (p.y < Math.min(ci.y1, ci.y2) - EPS) return false;
        if (p.y > Math.max(ci.y1, ci.y2) + EPS) return false;
      }
      const mirror = !!this.flipS[DIR_KEYS[ci.dir]!];
      for (let i = 0; i < verts.length; i++) {
        const s = (pt2[i]!.x - ci.x1) / (ci.x2 - ci.x1);
        verts[i]!["@_s"] = mirror ? 1 - s : s;
        verts[i]!["@_t"] = (pt2[i]!.y - ci.y1) / (ci.y2 - ci.y1);
      }
    }
    if (ci.name) {
      polygon["@_texture"] = ci.name;
      polygon["@_texture.transparent"] = ci.transparent ? 1 : 0;
    }
    if (ci.denylight) polygon["@_denylight"] = 1;
    polygon["@_r"] = ci.rkoef;
    polygon["@_g"] = ci.gkoef;
    polygon["@_b"] = ci.bkoef;
    return true;
  }
}

// ---- 2D polygon intersection (for StampAlg) ----------------------------------

const dist2 = (a: Vec2, b: Vec2): number => Math.hypot(a.x - b.x, a.y - b.y);
const sub2 = (a: Vec2, b: Vec2): Vec2 => ({ x: a.x - b.x, y: a.y - b.y });
const fsize2 = (a: Vec2): number => Math.hypot(a.x, a.y);
const dot2 = (a: Vec2, b: Vec2): number => a.x * b.x + a.y * b.y;

interface LinePos {
  pos: number;
  t?: number;
  s?: number;
}

// Classify two segments: -1 unknown, 0 identical line, 1 parallel, 2 crossing.
function linespos(a: Vec2, b: Vec2, c: Vec2, d: Vec2): LinePos {
  const u = sub2(b, a);
  const v = sub2(d, c);
  const jm = fsize2(u) * fsize2(v);
  if (jm < EPS) return { pos: -1 };
  const od = Math.acos(Math.min(1, Math.abs(dot2(u, v)) / jm));
  if (od < EPS) {
    const w = sub2(c, a);
    const jm2 = fsize2(u) * fsize2(w);
    if (jm2 < EPS) return { pos: 0 };
    const od2 = Math.acos(Math.min(1, Math.abs(dot2(u, w)) / jm2));
    if (od2 < EPS) return { pos: 0 };
    return { pos: 1 };
  }
  const m = b.x - a.x, o = b.y - a.y, nn = d.x - c.x, p = d.y - c.y;
  const den = m * p - o * nn;
  const s = (m * a.y - o * a.x - m * c.y + o * c.x) / den;
  const t = (nn * a.y - p * a.x - nn * c.y + p * c.x) / den;
  return { pos: 2, t, s };
}

function between(x: Vec2, a: Vec2, b: Vec2): boolean {
  if (x.x >= a.x && x.y >= a.y && x.x <= b.x && x.y <= b.y) return true;
  if (x.x <= a.x && x.y <= a.y && x.x >= b.x && x.y >= b.y) return true;
  return false;
}

interface CompPt {
  pt: Vec2;
  in: boolean;
}

function condadd(pts: CompPt[], pt: Vec2): void {
  for (const e of pts) if (dist2(e.pt, pt) < EPS) return;
  pts.push({ pt, in: false });
}

function makecompts(pl1: Vec2[], pl2: Vec2[], res: CompPt[]): void {
  for (let i = 0; i < pl1.length; i++) {
    const a = pl1[i]!;
    const b = pl1[(i + 1) % pl1.length]!;
    condadd(res, a);
    const coms: { t: number; s: number }[] = [];
    for (let j = 0; j < pl2.length; j++) {
      const c = pl2[j]!;
      const d = pl2[(j + 1) % pl2.length]!;
      const r = linespos(a, b, c, d);
      if (r.pos === 0) {
        if (between(c, a, b)) {
          if (between(d, a, c)) { condadd(res, d); condadd(res, c); }
          else if (between(d, b, c)) { condadd(res, c); condadd(res, d); }
          else condadd(res, c);
        } else if (between(d, a, b)) condadd(res, d);
      }
      if (r.pos === 2 && r.t! >= 0 && r.s! >= 0 && r.t! <= 1 && r.s! <= 1) {
        coms.push({ t: r.t!, s: r.s! });
      }
    }
    coms.sort((x, y) => x.t - y.t);
    const v = sub2(b, a);
    for (const cm of coms) condadd(res, { x: a.x + v.x * cm.t, y: a.y + v.y * cm.t });
  }
}

function isnear(a1: CompPt[], i1: number, a2: CompPt[], i2: number): boolean {
  i1 = ((i1 % a1.length) + a1.length) % a1.length;
  i2 = ((i2 % a2.length) + a2.length) % a2.length;
  return dist2(a1[i1]!.pt, a2[i2]!.pt) < EPS;
}
function issoused(p1: CompPt[], p2: CompPt[], i1: number, i2: number): boolean {
  if (p1 === p2) {
    if (Math.abs(i1 - i2) <= 1) return true;
    if (Math.abs(i1 - i2) === p1.length - 1) return true;
    return false;
  }
  if (isnear(p1, i1 - 1, p2, i2)) return true;
  if (isnear(p1, i1 + 1, p2, i2)) return true;
  if (isnear(p1, i1, p2, i2 - 1)) return true;
  if (isnear(p1, i1, p2, i2 + 1)) return true;
  return false;
}
function containsPt(pts: Vec2[], pt: Vec2): boolean {
  for (const p of pts) if (dist2(p, pt) < EPS) return true;
  return false;
}

function intersection(pl1: Vec2[], pl2: Vec2[]): Vec2[] {
  const p1: CompPt[] = [];
  const p2: CompPt[] = [];
  makecompts(pl1, pl2, p1);
  makecompts(pl2, pl1, p2);
  for (const ip of p1) ip.in = pointInPolygon2d(pl2, ip.pt);
  for (const ip of p2) ip.in = pointInPolygon2d(pl1, ip.pt);

  let a1 = p1;
  let a2 = p2;
  let idx = -1;
  for (let i = 0; i < p1.length; i++) if (p1[i]!.in) { idx = i; break; }
  if (idx < 0) for (let i = 0; i < p2.length; i++) if (p2[i]!.in) { idx = i; [a1, a2] = [a2, a1]; break; }
  if (idx < 0) return [];

  const res: Vec2[] = [a1[idx]!.pt];
  for (;;) {
    let advanced = false;
    for (let i = 0; i < a1.length; i++) {
      const ip = a1[i]!;
      if (!ip.in || containsPt(res, ip.pt)) continue;
      if (issoused(a1, a1, i, idx)) {
        res.push(a1[i]!.pt);
        idx = i;
        advanced = true;
        break;
      }
    }
    if (advanced) continue;
    for (let i = 0; i < a2.length; i++) {
      const ip = a2[i]!;
      if (!ip.in || containsPt(res, ip.pt)) continue;
      if (issoused(a2, a1, i, idx)) {
        res.push(a2[i]!.pt);
        idx = i;
        [a1, a2] = [a2, a1];
        advanced = true;
        break;
      }
    }
    if (!advanced) return res;
  }
}

// ---- StampAlg ----------------------------------------------------------------

export class StampAlg {
  texture = "";
  x1 = 0; y1 = 0; x2 = 0; y2 = 0;
  dir = 0;
  maxdist = Math.PI / 2;
  transp = false;
  reflects = false;
  reflectt = false;
  xscale = 1;
  yscale = 1;
  material = false;
  rkoef = "1";
  gkoef = "1";
  bkoef = "1";
  dist = 0;
  cond = new ColorCond();

  setCondition(value: string): void {
    this.cond = new ColorCond().load(value);
  }

  // Emit a stamped decal polygon (fast-xml-parser shape) into `out`; returns true.
  run(polygon: XmlPolygon, out: XmlPolygon[]): boolean {
    if (this.dist < 0.01) this.dist = 0.01;
    const faceid = str(polygon, "faceid");
    if (!this.cond.accept(faceid)) return false;
    if (Math.abs(this.x1 - this.x2) < EPS || Math.abs(this.y1 - this.y2) < EPS) return false;

    let n: Vec3 = { x: num(polygon, "nx"), y: num(polygon, "ny"), z: num(polygon, "nz") };
    if (Math.hypot(n.x, n.y, n.z) < EPS) return false;
    n = normalize(n);

    const pts = (polygon.vertex as XmlPolygon[]).map(pt3);
    const ndir = normalFromDir(this.dir);
    const distDot = ndir.x * n.x + ndir.y * n.y + ndir.z * n.z;
    if (Math.cos(this.maxdist) >= distDot + EPS) return false;

    const d2 = this.dir >> 1;
    const pt2 = pts.map((p) => ({ x: getx(p, d2), y: gety(p, d2) }));
    const rect: Vec2[] = [
      { x: this.x1, y: this.y1 },
      { x: this.x2, y: this.y1 },
      { x: this.x2, y: this.y2 },
      { x: this.x1, y: this.y2 },
    ];
    const inter = intersection(pt2, rect);
    if (inter.length < 3) return false;
    if (pts.length < 3) return false;

    const ra = { x: getx(pts[0]!, d2), y: gety(pts[0]!, d2), z: getz(pts[0]!, d2) };
    const rb = { x: getx(pts[1]!, d2), y: gety(pts[1]!, d2), z: getz(pts[1]!, d2) };
    let u: Vec3 = { x: 0, y: 0, z: 0 };
    let v: Vec3 = { x: 0, y: 0, z: 0 };
    for (let i = 2; i < pts.length; i++) {
      const rc = { x: getx(pts[i]!, d2), y: gety(pts[i]!, d2), z: getz(pts[i]!, d2) };
      u = { x: ra.x - rb.x, y: ra.y - rb.y, z: ra.z - rb.z };
      v = { x: rc.x - rb.x, y: rc.y - rb.y, z: rc.z - rb.z };
      const cr = cross(u, v);
      if (Math.hypot(cr.x, cr.y, cr.z) > EPS) break;
    }
    const rov = cross(u, v);
    if (Math.hypot(rov.x, rov.y, rov.z) < EPS) return false;
    const dPlane = -(rov.x * ra.x + rov.y * ra.y + rov.z * ra.z);
    if (Math.abs(rov.z) < EPS) return false;

    let dzVisible = -this.dist;
    if (this.dir % 2) dzVisible = -dzVisible;

    const np: XmlPolygon = { vertex: [] as XmlPolygon[] };
    for (const ip of inter) {
      const zproj = -(rov.x * ip.x + rov.y * ip.y + dPlane) / rov.z + dzVisible;
      const pt = setProjected(d2, ip.x, ip.y, zproj);
      let s = (ip.x - this.x1) / (this.x2 - this.x1);
      let t = (ip.y - this.y1) / (this.y2 - this.y1);
      if (this.material) {
        if (this.xscale > 0) s = (ip.x - this.x1) / this.xscale;
        if (this.yscale > 0) t = (ip.y - this.y1) / this.yscale;
      }
      if (this.transp) [s, t] = [t, s];
      if (this.reflectt) t = 1 - t;
      if (this.reflects) s = 1 - s;
      (np.vertex as XmlPolygon[]).push({ "@_x": pt.x, "@_y": pt.y, "@_z": pt.z, "@_t": t, "@_s": s });
    }
    if (this.texture) np["@_texture"] = this.texture;
    np["@_r"] = this.rkoef;
    np["@_g"] = this.gkoef;
    np["@_b"] = this.bkoef;
    // Faithful to StampAlg::run — no nx/ny/nz/culling on the decal polygon.
    out.push(np);
    return true;
  }
}

// ---- small fast-xml-parser accessors -----------------------------------------

function num(obj: XmlPolygon, name: string): number {
  const v = obj["@_" + name];
  if (v === undefined || v === null || v === "") return 0;
  const n = Number(v);
  return Number.isFinite(n) ? n : 0;
}
function str(obj: XmlPolygon, name: string): string {
  const v = obj["@_" + name];
  return v === undefined || v === null ? "" : String(v);
}
const pt3 = (v: XmlPolygon): Vec3 => ({ x: num(v, "x"), y: num(v, "y"), z: num(v, "z") });
