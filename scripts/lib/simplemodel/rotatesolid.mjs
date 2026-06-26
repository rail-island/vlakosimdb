// Port of the rotation-solid generator (jt7_src/jlib/glo/rotatesolid.cpp).
// Rotates a profile polyline around the axis (last-first) into a closed solid,
// emitting cap triangles and side quads with cylindric texture coords.

import { sub, add, scale, cross, dot, normalize, dist3d, rotateMatrix, matMulVec } from "./geom.mjs";
import { ColorCond } from "./coloralg.mjs";

// CylindricColorizeItem.load
export function loadCylindricColorize(items) {
  return items.map((xml) => {
    const fnum = (name) => {
      const v = xml["@_" + name];
      const n = v === undefined || v === "" ? 0 : Number(v);
      return Number.isFinite(n) ? n : 0;
    };
    const fstr = (name) => (xml["@_" + name] === undefined ? "" : String(xml["@_" + name]));
    let xscale = fnum("xscale");
    let yscale = fnum("yscale");
    if (xscale < 0.00001) xscale = 1;
    if (yscale < 0.00001) yscale = 1;
    return {
      texture: fstr("texture"),
      cond: new ColorCond().load(fstr("cond")),
      material: fnum("material") !== 0,
      xscale,
      yscale,
      transp: fnum("transp") !== 0,
      reflectx: fnum("reflectx") !== 0,
      reflecty: fnum("reflecty") !== 0,
    };
  });
}

function findCize(items, faceid) {
  for (const it of items) if (it.cond.accept(faceid)) return it;
  return null;
}

function countNormalTri(a, b, c, mid, ospt) {
  const s1 = sub(b, a);
  const s2 = sub(b, c);
  let n = cross(s1, s2);
  const v = sub(ospt, mid);
  if (dot(v, n) < 0) n = { x: -n.x, y: -n.y, z: -n.z };
  return normalize(n);
}

function countNormal(coords, faceVrs, ospt) {
  const p = faceVrs.map((r) => coords[r.id]);
  if (p.length === 3) {
    const mid = scale(add(add(p[0], p[1]), p[2]), 1 / 3);
    return countNormalTri(p[0], p[1], p[2], mid, ospt);
  }
  if (p.length === 4) {
    const mid = scale(add(add(p[0], p[1]), add(p[2], p[3])), 1 / 4);
    return countNormalTri(p[0], p[1], p[2], mid, ospt);
  }
  return { x: 0, y: 0, z: 0 };
}

function repaircolorize(face, c) {
  if (c.transp) for (const r of face.vrs) [r.t1, r.t2] = [r.t2, r.t1];
  if (c.reflectx) for (const r of face.vrs) r.t1 = 1 - r.t1;
  if (c.reflecty) for (const r of face.vrs) r.t2 = 1 - r.t2;
}

const rec = (id, t1 = 0, t2 = 0) => ({ id, t1, t2 });

/**
 * @param {{x,y,z}[]} pts   profile points
 * @param {number} cnt      rotation segments
 * @param {object[]} cize   cylindric colorize items (from loadCylindricColorize)
 * @param {string[]} faceids one per profile edge (pts.length - 1)
 * @returns {{vrs:{x,y,z}[], faces:object[]}}
 */
export function generateRotateSolid(pts, cnt, cize, faceids) {
  const vrs = [];
  const faces = [];
  if (pts.length < 3) return { vrs, faces };
  if (cnt < 3) return { vrs, faces };
  if (faceids.length !== pts.length - 1) throw new Error("rotatesolid: bad faceids count");

  let angle = 0;
  const dangle = (2 * Math.PI) / cnt;
  const first = pts[0];
  const last = pts[pts.length - 1];
  const os = sub(last, first);
  const ospt = scale(add(last, first), 0.5);

  vrs.push({ ...first });
  vrs.push({ ...last });

  for (let i = 0; i < cnt; i++, angle += dangle) {
    const R = rotateMatrix(os, angle);
    for (let j = 1; j < pts.length - 1; j++) {
      const rotated = matMulVec(R, sub(pts[j], first));
      vrs.push(add(rotated, first));
    }
  }

  let sumdist = 0;
  for (let i = 0; i < pts.length - 1; i++) sumdist += dist3d(pts[i], pts[i + 1]);

  const A = pts.length - 2;
  const mkFace = (ids) => ({ vrs: ids.map((id) => rec(id)), n: { x: 0, y: 0, z: 0 }, faceid: "", texture: "", culling: 1 });

  for (let i = 0; i < cnt; i++) {
    const i1 = (i + 1) % cnt;

    { // top cap
      const face = mkFace([0, 2 + i * A, 2 + i1 * A]);
      faces.push(face);
      face.n = countNormal(vrs, face.vrs, ospt);
      const c = findCize(cize, faceids[0]);
      if (c) {
        face.faceid = faceids[0];
        face.texture = c.texture;
        const tpom = dist3d(pts[0], pts[1]) / sumdist;
        face.vrs[0].t1 = ((i + 0.5) / cnt) * c.xscale; face.vrs[0].t2 = 0 * c.yscale;
        face.vrs[1].t1 = (i / cnt) * c.xscale; face.vrs[1].t2 = tpom * c.yscale;
        face.vrs[2].t1 = ((i + 1) / cnt) * c.xscale; face.vrs[2].t2 = tpom * c.yscale;
        repaircolorize(face, c);
      }
    }
    { // bottom cap
      const face = mkFace([1, 2 + i * A + A - 1, 2 + i1 * A + A - 1]);
      faces.push(face);
      face.n = countNormal(vrs, face.vrs, ospt);
      const c = findCize(cize, faceids[faceids.length - 1]);
      if (c) {
        face.faceid = faceids[faceids.length - 1];
        face.texture = c.texture;
        const tpom = 1 - dist3d(pts[pts.length - 2], pts[pts.length - 1]) / sumdist;
        face.vrs[0].t1 = ((i + 0.5) / cnt) * c.xscale; face.vrs[0].t2 = 1 * c.yscale;
        face.vrs[1].t1 = (i / cnt) * c.xscale; face.vrs[1].t2 = tpom * c.yscale;
        face.vrs[2].t1 = ((i + 1) / cnt) * c.xscale; face.vrs[2].t2 = tpom * c.yscale;
        repaircolorize(face, c);
      }
    }

    let pomarg = dist3d(pts[0], pts[1]);
    for (let j = 0; j < A - 1; j++) {
      const face = mkFace([2 + i * A + j, 2 + i1 * A + j, 2 + i1 * A + j + 1, 2 + i * A + j + 1]);
      faces.push(face);
      face.n = countNormal(vrs, face.vrs, ospt);
      const c = findCize(cize, faceids[j + 1]);
      if (c) {
        face.faceid = faceids[j + 1];
        face.texture = c.texture;
        const dpom = dist3d(pts[j + 1], pts[j + 2]);
        const tp1 = pomarg / sumdist;
        const tp2 = (pomarg + dpom) / sumdist;
        face.vrs[0].t1 = (i / cnt) * c.xscale; face.vrs[0].t2 = tp1 * c.yscale;
        face.vrs[1].t1 = ((i + 1) / cnt) * c.xscale; face.vrs[1].t2 = tp1 * c.yscale;
        face.vrs[2].t1 = ((i + 1) / cnt) * c.xscale; face.vrs[2].t2 = tp2 * c.yscale;
        face.vrs[3].t1 = (i / cnt) * c.xscale; face.vrs[3].t2 = tp2 * c.yscale;
        pomarg += dpom;
        repaircolorize(face, c);
      }
    }
  }

  return { vrs, faces };
}
