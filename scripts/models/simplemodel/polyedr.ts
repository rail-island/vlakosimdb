// Port of the prPolyedr flatten step (jt7 jlib/jmath/polyedr.cpp):
// re-run normalize_culling, then emit flat <polygon>s with per-vertex x,y,z and
// s,t (= the face VrRecord's t1,t2).
//
// A polyhedron here is { vrs:[{x,y,z}], faces:[{ vrs:[{id,t1,t2}], n, faceid,
// texture, culling }] }.
import type { Vec3 } from "../geom.ts";
import type { XmlPolygon } from "./coloralg.ts";

export interface FaceLike {
  vrs: { id: number; t1: number; t2: number }[];
  n: Vec3;
  faceid?: string;
  texture?: string;
  culling?: number;
}
export interface PolyedrLike {
  vrs: Vec3[];
  faces: FaceLike[];
}

// Emulate the engine's float serialization (setvar(float) ~ printf "%g", i.e. 6
// significant digits). In the original pipeline every polygon was written to XML
// here and re-parsed before ColorAlg runs, so colorize sees 6-sig-fig values.
// Matching that precision makes exact-45-degree colorize ties resolve as the C++ did.
function g6(x: number): number {
  if (!Number.isFinite(x) || x === 0) return 0;
  return Number(x.toPrecision(6));
}

// prFace::normalize_culling — reorder vertices to match the stored normal.
function normalizeCullingFace(coords: Vec3[], face: FaceLike): void {
  const ids = face.vrs;
  const n = ids.length;
  let normala: Vec3 | null = null;
  let ncsize = -1;
  for (let i = 0; i < n; i++) {
    const x = coords[ids[((i - 1) % n + n) % n]!.id]!;
    const y = coords[ids[i]!.id]!;
    const z = coords[ids[(i + 1) % n]!.id]!;
    const a = { x: x.x - y.x, y: x.y - y.y, z: x.z - y.z };
    const b = { x: z.x - y.x, y: z.y - y.y, z: z.z - y.z };
    const actsize = Math.abs(a.x * b.x + a.y * b.y + a.z * b.z);
    if (ncsize < 0 || actsize < ncsize) {
      normala = { x: a.y * b.z - a.z * b.y, y: a.z * b.x - a.x * b.z, z: a.x * b.y - a.y * b.x };
      ncsize = actsize;
    }
  }
  const mul = normala!.x * face.n.x + normala!.y * face.n.y + normala!.z * face.n.z;
  if (mul < 0) face.vrs.reverse();
}

/**
 * Flatten a polyhedron into polygon objects in fast-xml-parser shape so the
 * existing geometry walker consumes them like parsed <polygon> nodes.
 */
export function polyedrToPolygons({ vrs, faces }: PolyedrLike): XmlPolygon[] {
  const polygons: XmlPolygon[] = [];
  for (const face of faces) {
    normalizeCullingFace(vrs, face);
    const polygon: XmlPolygon = {
      "@_nx": g6(face.n.x),
      "@_ny": g6(face.n.y),
      "@_nz": g6(face.n.z),
      "@_culling": face.culling ?? 1,
      vertex: face.vrs.map((r) => {
        const v = vrs[r.id]!;
        return { "@_x": g6(v.x), "@_y": g6(v.y), "@_z": g6(v.z), "@_s": g6(r.t1), "@_t": g6(r.t2) };
      }),
    };
    if (face.faceid) polygon["@_faceid"] = face.faceid;
    if (face.texture) polygon["@_texture"] = face.texture;
    polygons.push(polygon);
  }
  return polygons;
}
