// Golden tests: regenerate <model> from source and compare against the pre-baked
// <model> that the original C++ editor wrote into each index.xml.
//
// Comparison is order/winding-insensitive (a bijection between baked and
// generated polygons), since float-driven tie-breaks in vertex merging may
// reorder polygons/vertices without changing the geometry.
import { describe, it, expect } from "vitest";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { parseXmlSafe, asArray, attrNum, attrStr, type XmlNode } from "../scripts/models/xml.ts";
import { generateModel } from "../scripts/models/simplemodel/generate.ts";
import { parseModel } from "../scripts/models/geometry.ts";

const repoRoot = path.resolve(fileURLToPath(import.meta.url), "../..");

interface Vert { x: number; y: number; z: number; s: number; t: number }
interface PolyInfo { texture: string; r: number; g: number; b: number; n: number[]; verts: Vert[] }

const POS_TOL = 5e-3; // baked floats carry ~6 significant figures
const ST_TOL = 5e-3;

function polyInfo(p: XmlNode): PolyInfo {
  return {
    texture: attrStr(p, "texture"),
    r: attrNum(p, "r", 1),
    g: attrNum(p, "g", 1),
    b: attrNum(p, "b", 1),
    n: [attrNum(p, "nx"), attrNum(p, "ny"), attrNum(p, "nz")],
    verts: asArray(p.vertex as XmlNode | XmlNode[]).map((v) => ({
      x: attrNum(v, "x"), y: attrNum(v, "y"), z: attrNum(v, "z"),
      s: attrNum(v, "s"), t: attrNum(v, "t"),
    })),
  };
}

const centroid = (info: PolyInfo) => {
  const c = { x: 0, y: 0, z: 0 };
  for (const v of info.verts) { c.x += v.x; c.y += v.y; c.z += v.z; }
  const n = info.verts.length || 1;
  return { x: c.x / n, y: c.y / n, z: c.z / n };
};

// Bucket by texture only — a coarse centroid bucket can split a true pair across
// a rounding boundary. Within a texture bucket, polyMatch (positions + s/t +
// normal + color) uniquely identifies the partner.
function bucketKey(info: PolyInfo): string {
  return info.texture;
}

const veq = (a: Vert, b: Vert) =>
  Math.abs(a.x - b.x) < POS_TOL && Math.abs(a.y - b.y) < POS_TOL && Math.abs(a.z - b.z) < POS_TOL &&
  Math.abs(a.s - b.s) < ST_TOL && Math.abs(a.t - b.t) < ST_TOL;

// True if g's vertex ring matches b's under some rotation in either direction.
function vertsAlign(bv: Vert[], gv: Vert[]): boolean {
  const n = bv.length;
  if (gv.length !== n) return false;
  for (const dir of [1, -1]) {
    for (let off = 0; off < n; off++) {
      let ok = true;
      for (let i = 0; i < n; i++) {
        const j = (((off + dir * i) % n) + n) % n;
        if (!veq(bv[i]!, gv[j]!)) { ok = false; break; }
      }
      if (ok) return true;
    }
  }
  return false;
}

function polyMatch(b: PolyInfo, g: PolyInfo): boolean {
  if (b.texture !== g.texture) return false;
  if (Math.abs(b.r - g.r) > 1e-4 || Math.abs(b.g - g.g) > 1e-4 || Math.abs(b.b - g.b) > 1e-4) return false;
  if ([0, 1, 2].some((i) => Math.abs(b.n[i]! - g.n[i]!) > POS_TOL)) return false;
  return vertsAlign(b.verts, g.verts);
}

function assertBijection(bakedPolys: XmlNode[], genPolys: XmlNode[]): void {
  expect(genPolys.length).toBe(bakedPolys.length);

  const baked = bakedPolys.map(polyInfo);
  const gen = genPolys.map(polyInfo);

  const buckets = new Map<string, number[]>();
  gen.forEach((g, idx) => {
    const k = bucketKey(g);
    if (!buckets.has(k)) buckets.set(k, []);
    buckets.get(k)!.push(idx);
  });

  const used = new Array(gen.length).fill(false);
  const unmatched: PolyInfo[] = [];
  for (const b of baked) {
    const candidates = buckets.get(bucketKey(b)) ?? [];
    let found = -1;
    for (const idx of candidates) {
      if (used[idx]) continue;
      if (polyMatch(b, gen[idx]!)) { found = idx; break; }
    }
    if (found < 0) unmatched.push(b);
    else used[found] = true;
  }
  if (unmatched.length) {
    const sample = unmatched.slice(0, 3).map((u) => `${u.texture} c=${JSON.stringify(centroid(u))}`);
    throw new Error(`${unmatched.length}/${baked.length} baked polygons had no generated match. e.g. ${sample.join("; ")}`);
  }
}

function runCase(relPath: string): void {
  const root = parseXmlSafe(path.join(repoRoot, relPath));
  const sm = (root.simplemodel ?? root) as XmlNode;
  const model = sm.model as XmlNode | undefined;
  // <inline> elements aren't <polygon>, naturally excluded
  const baked = asArray(model?.polygon as XmlNode | XmlNode[] | undefined);
  // No flipS: faithful reproduction of the original C++ baked UVs.
  const generated = generateModel(sm);
  expect(generated, "generateModel returned null").not.toBeNull();
  assertBijection(baked, generated!.polygon);
}

describe("simplemodel generator vs baked <model> (faithful, no flips)", () => {
  it("cd_163 — clip CSG + colorize sides + material", () => {
    runCase("db/train/loco/electric/cd_163/model/index.xml");
  });

  it("n_raj — rotate solid", () => {
    runCase("db/train/truck/n_raj/model/index.xml");
  });

  it("panelak — stamps (decals)", () => {
    // No vehicle's baked <model> reproduces a stamp faithfully, so the building this
    // case was written against is kept as a fixture (db/building is gone).
    runCase("test/fixtures/panelak/model/index.xml");
  });
});

describe("side-logo mirroring fix", () => {
  // Pick the longest-spanning rect face of a given texture and report its
  // s as a function of world x (min-x vertex and max-x vertex).
  function sByX(polys: XmlNode[], texture: string): { sAtMinX: number; sAtMaxX: number } | null {
    let best: XmlNode[] | null = null;
    let span = -1;
    for (const p of polys) {
      if (attrStr(p, "texture") !== texture) continue;
      const vs = asArray(p.vertex as XmlNode | XmlNode[]);
      const xs = vs.map((v) => attrNum(v, "x"));
      const s = Math.max(...xs) - Math.min(...xs);
      if (s > span) { span = s; best = vs; }
    }
    if (!best) return null;
    let lo = best[0]!, hi = best[0]!;
    for (const v of best) {
      if (attrNum(v, "x") < attrNum(lo, "x")) lo = v;
      if (attrNum(v, "x") > attrNum(hi, "x")) hi = v;
    }
    return { sAtMinX: attrNum(lo, "s"), sAtMaxX: attrNum(hi, "s") };
  }

  // Largest matching face's s at its lowest- and highest- world-`axis` vertex.
  function sByAxis(polys: XmlNode[], pred: (p: XmlNode) => boolean, axis: string): { sLo: number; sHi: number } | null {
    let best: XmlNode[] | null = null;
    let span = -1;
    for (const p of polys) {
      if (!pred(p)) continue;
      const vs = asArray(p.vertex as XmlNode | XmlNode[]);
      const c = vs.map((v) => attrNum(v, axis));
      const s = Math.max(...c) - Math.min(...c);
      if (s > span) { span = s; best = vs; }
    }
    if (!best) return null;
    let lo = best[0]!, hi = best[0]!;
    for (const v of best) {
      if (attrNum(v, axis) < attrNum(lo, axis)) lo = v;
      if (attrNum(v, axis) > attrNum(hi, axis)) hi = v;
    }
    return { sLo: attrNum(lo, "s"), sHi: attrNum(hi, "s") };
  }

  it("flips only the sides named in flipS (s → 1 − s), leaving others untouched", () => {
    const root = parseXmlSafe(path.join(repoRoot, "db/train/loco/electric/cd_163/model/index.xml"));
    const sm = (root.simplemodel ?? root) as XmlNode;
    const faithful = generateModel(sm)!.polygon;
    const fixed = generateModel(sm, { flipS: { "x-": true, "z+": true } })!.polygon;
    const near = (a: number, b: number) => Math.abs(a - b) < 1e-3;

    // --- long sides (z): z_dec = z+ (dir 4) flips; z_inc = z- (dir 5) unchanged ---
    const zdecF = sByX(faithful, "pack:z_dec.bmp")!;
    const zdecX = sByX(fixed, "pack:z_dec.bmp")!;
    const zincF = sByX(faithful, "pack:z_inc.bmp")!;
    const zincX = sByX(fixed, "pack:z_inc.bmp")!;
    expect(near(zdecF.sAtMaxX, zincF.sAtMaxX)).toBe(true);        // faithful: same mapping (bug)
    expect(near(zdecX.sAtMaxX, 1 - zdecF.sAtMaxX)).toBe(true);    // z+ flipped
    expect(near(zincX.sAtMaxX, zincF.sAtMaxX)).toBe(true);        // z- unchanged
    expect(Math.abs(zdecX.sAtMaxX - zincX.sAtMaxX)).toBeGreaterThan(0.5);

    // --- cab ends (x, both on pack:x_both.bmp): x- (dir 1) flips; x+ (dir 0) unchanged.
    // x faces project s←world z, so measure along the z axis.
    const xpos = (p: XmlNode) => attrStr(p, "texture") === "pack:x_both.bmp" && attrNum(p, "nx") > 0.5;
    const xneg = (p: XmlNode) => attrStr(p, "texture") === "pack:x_both.bmp" && attrNum(p, "nx") < -0.5;
    const xposF = sByAxis(faithful, xpos, "z")!;
    const xposX = sByAxis(fixed, xpos, "z")!;
    const xnegF = sByAxis(faithful, xneg, "z")!;
    const xnegX = sByAxis(fixed, xneg, "z")!;
    expect(near(xposX.sHi, xposF.sHi)).toBe(true);               // x+ unchanged
    expect(near(xnegX.sHi, 1 - xnegF.sHi)).toBe(true);           // x- flipped
  });

  it("parseModel reads the committed options.json (cd_163 ships x-/z+ flips)", () => {
    const modelDir = path.join(repoRoot, "db/train/loco/electric/cd_163/model");
    // cd_163 ships options.json enabling x-/z+, so its z+ side comes out flipped
    // vs the faithful baked mapping.
    const { polygons } = parseModel(modelDir);
    const zplus = polygons.filter((p) => p.textureKey?.endsWith("z_dec.bmp"));
    expect(zplus.length).toBeGreaterThan(0);
    // Faithful z+ maps x-min→s≈0; with the committed flip it maps x-min→s≈1.
    const lo = zplus.flatMap((p) => p.verts).reduce((a, v) => (v.x < a.x ? v : a));
    expect(lo.s).toBeGreaterThan(0.5);
  });
});
