// Port of the colorize/stamp algorithms (jt7_src/jlib/glo/coloralg.cpp + .h).
//
// ColorAlg assigns a texture and per-vertex s/t to each generated polygon by
// matching the polygon normal to the best-aligned colorize "side". StampAlg
// stamps decal polygons clipped to a texture rectangle. Polygons are mutated in
// fast-xml-parser shape (`@_` attributes, `vertex` array).

import { EPS, normalize, cross, pointInPolygon2d } from "./geom.mjs";

// ---- ColorCond: faceid condition mini-language ( * ^ + - ) --------------------

export class ColorCond {
  constructor() {
    this.start = true;
    this.items = []; // { s, mustbe, mustnot, canbe }
  }
  load(s) {
    s = s ?? "";
    let i = 0;
    const isWs = (ch) => ch === " " || ch === "\t" || ch === "\n" || ch === "\r";
    const readToken = () => {
      let out = "";
      while (i < s.length && !isWs(s[i])) out += s[i++];
      return out;
    };
    while (i < s.length) {
      while (i < s.length && isWs(s[i])) i++;
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
  accept(faceid) {
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

export function normalFromDir(dir) {
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

// Projection of a 3D point onto the 2D plane / depth axis for d2 in {0,1,2}.
const getx = (p, d) => (d === 0 ? p.z : p.x);
const gety = (p, d) => (d === 1 ? p.z : p.y);
const getz = (p, d) => (d === 0 ? p.x : d === 1 ? p.y : p.z);
function setProjected(d, x, y, z) {
  // inverse of getx/gety/getz
  if (d === 0) return { x: z, y, z: x };
  if (d === 1) return { x, y: z, z: y };
  return { x, y, z };
}

// ---- ColorAlg ----------------------------------------------------------------

export class ColorAlg {
  constructor() {
    this.items = [];
  }
  addSide(dir, x1, y1, x2, y2, name, transparent, cond, rkoef, gkoef, bkoef, denylight) {
    if (dir < 0 || dir >= 6) return;
    const it = {
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
  addMaterialSide(dir, dx, dy, xscale, yscale, name, transparent, rkoef, gkoef, bkoef, transp, cond, denylight) {
    if (dir < 0 || dir >= 6) return;
    // NOTE: faithful to the C++ quirk in addmaterialside — yscale is set to xscale.
    yscale = xscale;
    const it = {
      material: true,
      dir,
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
    if (Math.abs(it.xscale) < EPS) it.xscale = 1;
    if (Math.abs(it.yscale) < EPS) it.yscale = 1;
    this.items.push(it);
  }

  // Mutate a polygon node in place; returns true if it was colored.
  run(polygon) {
    let n = { x: num(polygon, "nx"), y: num(polygon, "ny"), z: num(polygon, "nz") };
    if (Math.hypot(n.x, n.y, n.z) < EPS) return false;
    n = normalize(n);
    const faceid = str(polygon, "faceid");

    let maxi = -1;
    let maxv = 0;
    for (let i = 0; i < this.items.length; i++) {
      const it = this.items[i];
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

    const ci = this.items[maxi];
    const verts = polygon.vertex;
    const d = ci.dir >> 1;
    const pt2 = verts.map((v) => ({ x: getx(pt3(v), d), y: gety(pt3(v), d) }));

    if (ci.material) {
      if (ci.name) {
        for (let i = 0; i < verts.length; i++) {
          const sKey = ci.transp ? "@_t" : "@_s";
          const tKey = ci.transp ? "@_s" : "@_t";
          verts[i][sKey] = pt2[i].x * ci.xscale + ci.dx;
          verts[i][tKey] = pt2[i].y * ci.yscale + ci.dy;
        }
      }
    } else {
      for (const p of pt2) {
        if (p.x < Math.min(ci.x1, ci.x2) - EPS) return false;
        if (p.x > Math.max(ci.x1, ci.x2) + EPS) return false;
        if (p.y < Math.min(ci.y1, ci.y2) - EPS) return false;
        if (p.y > Math.max(ci.y1, ci.y2) + EPS) return false;
      }
      for (let i = 0; i < verts.length; i++) {
        verts[i]["@_s"] = (pt2[i].x - ci.x1) / (ci.x2 - ci.x1);
        verts[i]["@_t"] = (pt2[i].y - ci.y1) / (ci.y2 - ci.y1);
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

const dist2 = (a, b) => Math.hypot(a.x - b.x, a.y - b.y);
const sub2 = (a, b) => ({ x: a.x - b.x, y: a.y - b.y });
const fsize2 = (a) => Math.hypot(a.x, a.y);
const dot2 = (a, b) => a.x * b.x + a.y * b.y;

// Classify two segments: -1 unknown, 0 identical line, 1 parallel, 2 crossing.
function linespos(a, b, c, d) {
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

function between(x, a, b) {
  if (x.x >= a.x && x.y >= a.y && x.x <= b.x && x.y <= b.y) return true;
  if (x.x <= a.x && x.y <= a.y && x.x >= b.x && x.y >= b.y) return true;
  return false;
}

function condadd(pts, pt) {
  for (const e of pts) if (dist2(e.pt, pt) < EPS) return;
  pts.push({ pt, in: false });
}

function makecompts(pl1, pl2, res) {
  for (let i = 0; i < pl1.length; i++) {
    const a = pl1[i];
    const b = pl1[(i + 1) % pl1.length];
    condadd(res, a);
    const coms = [];
    for (let j = 0; j < pl2.length; j++) {
      const c = pl2[j];
      const d = pl2[(j + 1) % pl2.length];
      const r = linespos(a, b, c, d);
      if (r.pos === 0) {
        if (between(c, a, b)) {
          if (between(d, a, c)) { condadd(res, d); condadd(res, c); }
          else if (between(d, b, c)) { condadd(res, c); condadd(res, d); }
          else condadd(res, c);
        } else if (between(d, a, b)) condadd(res, d);
      }
      if (r.pos === 2 && r.t >= 0 && r.s >= 0 && r.t <= 1 && r.s <= 1) {
        coms.push({ t: r.t, s: r.s });
      }
    }
    coms.sort((x, y) => x.t - y.t);
    const v = sub2(b, a);
    for (const cm of coms) condadd(res, { x: a.x + v.x * cm.t, y: a.y + v.y * cm.t });
  }
}

function isnear(a1, i1, a2, i2) {
  i1 = ((i1 % a1.length) + a1.length) % a1.length;
  i2 = ((i2 % a2.length) + a2.length) % a2.length;
  return dist2(a1[i1].pt, a2[i2].pt) < EPS;
}
function issoused(p1, p2, i1, i2) {
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
function containsPt(pts, pt) {
  for (const p of pts) if (dist2(p, pt) < EPS) return true;
  return false;
}

function intersection(pl1, pl2) {
  const p1 = [];
  const p2 = [];
  makecompts(pl1, pl2, p1);
  makecompts(pl2, pl1, p2);
  for (const ip of p1) ip.in = pointInPolygon2d(pl2, ip.pt);
  for (const ip of p2) ip.in = pointInPolygon2d(pl1, ip.pt);

  let a1 = p1;
  let a2 = p2;
  let idx = -1;
  for (let i = 0; i < p1.length; i++) if (p1[i].in) { idx = i; break; }
  if (idx < 0) for (let i = 0; i < p2.length; i++) if (p2[i].in) { idx = i; [a1, a2] = [a2, a1]; break; }
  if (idx < 0) return [];

  const res = [a1[idx].pt];
  for (;;) {
    let advanced = false;
    for (let i = 0; i < a1.length; i++) {
      const ip = a1[i];
      if (!ip.in || containsPt(res, ip.pt)) continue;
      if (issoused(a1, a1, i, idx)) {
        res.push(a1[i].pt);
        idx = i;
        advanced = true;
        break;
      }
    }
    if (advanced) continue;
    for (let i = 0; i < a2.length; i++) {
      const ip = a2[i];
      if (!ip.in || containsPt(res, ip.pt)) continue;
      if (issoused(a2, a1, i, idx)) {
        res.push(a2[i].pt);
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
  constructor() {
    this.texture = "";
    this.x1 = this.y1 = this.x2 = this.y2 = 0;
    this.dir = 0;
    this.maxdist = Math.PI / 2;
    this.transp = false;
    this.reflects = false;
    this.reflectt = false;
    this.xscale = 1;
    this.yscale = 1;
    this.material = false;
    this.rkoef = "1";
    this.gkoef = "1";
    this.bkoef = "1";
    this.dist = 0;
    this.cond = new ColorCond();
  }
  setCondition(value) {
    this.cond = new ColorCond().load(value);
  }

  // Emit a stamped decal polygon (fast-xml-parser shape) into `out`; returns true.
  run(polygon, out) {
    if (this.dist < 0.01) this.dist = 0.01;
    const faceid = str(polygon, "faceid");
    if (!this.cond.accept(faceid)) return false;
    if (Math.abs(this.x1 - this.x2) < EPS || Math.abs(this.y1 - this.y2) < EPS) return false;

    let n = { x: num(polygon, "nx"), y: num(polygon, "ny"), z: num(polygon, "nz") };
    if (Math.hypot(n.x, n.y, n.z) < EPS) return false;
    n = normalize(n);

    const pts = polygon.vertex.map(pt3);
    const ndir = normalFromDir(this.dir);
    const distDot = ndir.x * n.x + ndir.y * n.y + ndir.z * n.z;
    if (Math.cos(this.maxdist) >= distDot + EPS) return false;

    const d2 = this.dir >> 1;
    const pt2 = pts.map((p) => ({ x: getx(p, d2), y: gety(p, d2) }));
    const rect = [
      { x: this.x1, y: this.y1 },
      { x: this.x2, y: this.y1 },
      { x: this.x2, y: this.y2 },
      { x: this.x1, y: this.y2 },
    ];
    const inter = intersection(pt2, rect);
    if (inter.length < 3) return false;
    if (pts.length < 3) return false;

    // Face plane in projected coords.
    const ra = { x: getx(pts[0], d2), y: gety(pts[0], d2), z: getz(pts[0], d2) };
    const rb = { x: getx(pts[1], d2), y: gety(pts[1], d2), z: getz(pts[1], d2) };
    let u = { x: 0, y: 0, z: 0 };
    let v = { x: 0, y: 0, z: 0 };
    for (let i = 2; i < pts.length; i++) {
      const rc = { x: getx(pts[i], d2), y: gety(pts[i], d2), z: getz(pts[i], d2) };
      u = { x: ra.x - rb.x, y: ra.y - rb.y, z: ra.z - rb.z };
      v = { x: rc.x - rb.x, y: rc.y - rb.y, z: rc.z - rb.z };
      if (Math.hypot(...vec(cross(u, v))) > EPS) break;
    }
    const rov = cross(u, v);
    if (Math.hypot(rov.x, rov.y, rov.z) < EPS) return false;
    const dPlane = -(rov.x * ra.x + rov.y * ra.y + rov.z * ra.z);
    if (Math.abs(rov.z) < EPS) return false;

    let dzVisible = -this.dist;
    if (this.dir % 2) dzVisible = -dzVisible;

    const np = { vertex: [] };
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
      np.vertex.push({ "@_x": pt.x, "@_y": pt.y, "@_z": pt.z, "@_t": t, "@_s": s });
    }
    if (this.texture) np["@_texture"] = this.texture;
    np["@_r"] = this.rkoef;
    np["@_g"] = this.gkoef;
    np["@_b"] = this.bkoef;
    // NOTE: faithful to StampAlg::run — no nx/ny/nz/culling are written on the
    // decal polygon (matches the baked <model>).
    out.push(np);
    return true;
  }
}

// ---- small fast-xml-parser accessors -----------------------------------------

function num(obj, name) {
  const v = obj["@_" + name];
  if (v === undefined || v === null || v === "") return 0;
  const n = Number(v);
  return Number.isFinite(n) ? n : 0;
}
function str(obj, name) {
  const v = obj["@_" + name];
  return v === undefined || v === null ? "" : String(v);
}
const pt3 = (v) => ({ x: num(v, "x"), y: num(v, "y"), z: num(v, "z") });
const vec = (a) => [a.x, a.y, a.z];
