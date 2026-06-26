// Golden tests: regenerate <model> from source and compare against the pre-baked
// <model> that the original C++ editor wrote into each index.xml.
//
// Comparison is order/winding-insensitive (a bijection between baked and
// generated polygons), since float-driven tie-breaks in vertex merging may
// reorder polygons/vertices without changing the geometry.
import { describe, it, expect } from "vitest";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { parseXmlSafe, asArray, attrNum, attrStr } from "../../xml.mjs";
import { generateModel } from "../generate.mjs";

const repoRoot = path.resolve(fileURLToPath(import.meta.url), "../../../../..");

const POS_TOL = 5e-3; // baked floats carry ~6 significant figures
const ST_TOL = 5e-3;

function polyInfo(p) {
  return {
    texture: attrStr(p, "texture"),
    r: attrNum(p, "r", 1),
    g: attrNum(p, "g", 1),
    b: attrNum(p, "b", 1),
    n: [attrNum(p, "nx"), attrNum(p, "ny"), attrNum(p, "nz")],
    verts: asArray(p.vertex).map((v) => ({
      x: attrNum(v, "x"), y: attrNum(v, "y"), z: attrNum(v, "z"),
      s: attrNum(v, "s"), t: attrNum(v, "t"),
    })),
  };
}

const centroid = (info) => {
  const c = { x: 0, y: 0, z: 0 };
  for (const v of info.verts) { c.x += v.x; c.y += v.y; c.z += v.z; }
  const n = info.verts.length || 1;
  return { x: c.x / n, y: c.y / n, z: c.z / n };
};

// Bucket by texture only — a coarse centroid bucket can split a true pair across
// a rounding boundary. Within a texture bucket, polyMatch (positions + s/t +
// normal + color) uniquely identifies the partner.
function bucketKey(info) {
  return info.texture;
}

const veq = (a, b) =>
  Math.abs(a.x - b.x) < POS_TOL && Math.abs(a.y - b.y) < POS_TOL && Math.abs(a.z - b.z) < POS_TOL &&
  Math.abs(a.s - b.s) < ST_TOL && Math.abs(a.t - b.t) < ST_TOL;

// True if g's vertex ring matches b's under some rotation in either direction.
function vertsAlign(bv, gv) {
  const n = bv.length;
  if (gv.length !== n) return false;
  for (const dir of [1, -1]) {
    for (let off = 0; off < n; off++) {
      let ok = true;
      for (let i = 0; i < n; i++) {
        const j = (((off + dir * i) % n) + n) % n;
        if (!veq(bv[i], gv[j])) { ok = false; break; }
      }
      if (ok) return true;
    }
  }
  return false;
}

function polyMatch(b, g) {
  if (b.texture !== g.texture) return false;
  if (Math.abs(b.r - g.r) > 1e-4 || Math.abs(b.g - g.g) > 1e-4 || Math.abs(b.b - g.b) > 1e-4) return false;
  if (Math.abs(b.n[0] - g.n[0]) > POS_TOL || Math.abs(b.n[1] - g.n[1]) > POS_TOL || Math.abs(b.n[2] - g.n[2]) > POS_TOL) return false;
  return vertsAlign(b.verts, g.verts);
}

function assertBijection(bakedPolys, genPolys) {
  expect(genPolys.length).toBe(bakedPolys.length);

  const baked = bakedPolys.map(polyInfo);
  const gen = genPolys.map(polyInfo);

  const buckets = new Map();
  gen.forEach((g, idx) => {
    const k = bucketKey(g);
    if (!buckets.has(k)) buckets.set(k, []);
    buckets.get(k).push(idx);
  });

  const used = new Array(gen.length).fill(false);
  const unmatched = [];
  for (const b of baked) {
    const candidates = buckets.get(bucketKey(b)) ?? [];
    let found = -1;
    for (const idx of candidates) {
      if (used[idx]) continue;
      if (polyMatch(b, gen[idx])) { found = idx; break; }
    }
    if (found < 0) unmatched.push(b);
    else used[found] = true;
  }
  if (unmatched.length) {
    const sample = unmatched.slice(0, 3).map((u) => `${u.texture} c=${JSON.stringify(centroid(u))}`);
    throw new Error(`${unmatched.length}/${baked.length} baked polygons had no generated match. e.g. ${sample.join("; ")}`);
  }
}

function runCase(relPath) {
  const root = parseXmlSafe(path.join(repoRoot, relPath));
  const sm = root.simplemodel ?? root;
  const baked = asArray(sm.model?.polygon); // <inline> elements aren't <polygon>, naturally excluded
  const generated = generateModel(sm);
  expect(generated, "generateModel returned null").not.toBeNull();
  assertBijection(baked, generated.polygon);
}

describe("simplemodel generator vs baked <model>", () => {
  it("cd_163 — clip CSG + colorize sides + material", () => {
    runCase("db/train/loco/electric/cd_163/model/index.xml");
  });

  it("n_raj — rotate solid", () => {
    runCase("db/train/truck/n_raj/model/index.xml");
  });

  it("panelak — stamps (decals)", () => {
    runCase("db/building/panelak/model/index.xml");
  });
});
