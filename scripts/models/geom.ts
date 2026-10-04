// Shared 3D/2D math for the simplemodel generator (ported from jt7 jlib/jmath).
// Vectors are plain {x,y,z} objects; matrices are flat row-major [a11..a33].

export const EPS = 0.0001;

export interface Vec3 {
  x: number;
  y: number;
  z: number;
}
export interface Vec2 {
  x: number;
  y: number;
}

export const sub = (a: Vec3, b: Vec3): Vec3 => ({ x: a.x - b.x, y: a.y - b.y, z: a.z - b.z });
export const add = (a: Vec3, b: Vec3): Vec3 => ({ x: a.x + b.x, y: a.y + b.y, z: a.z + b.z });
export const scale = (a: Vec3, k: number): Vec3 => ({ x: a.x * k, y: a.y * k, z: a.z * k });
export const dot = (a: Vec3, b: Vec3): number => a.x * b.x + a.y * b.y + a.z * b.z;
export const cross = (a: Vec3, b: Vec3): Vec3 => ({
  x: a.y * b.z - a.z * b.y,
  y: a.z * b.x - a.x * b.z,
  z: a.x * b.y - a.y * b.x,
});
export const len = (a: Vec3): number => Math.sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
export function normalize(a: Vec3): Vec3 {
  const s = len(a);
  return s > 0 ? { x: a.x / s, y: a.y / s, z: a.z / s } : { x: a.x, y: a.y, z: a.z };
}
export const dist3d = (a: Vec3, b: Vec3): number => len(sub(a, b));

/** 2D point distance. */
export const dist2 = (a: Vec2, b: Vec2): number => Math.hypot(a.x - b.x, a.y - b.y);

// Rodrigues rotation matrix about axis v by angle (radians).
// Port of matrix3d::dorotate (jt7 jlib/jmath/geom3d.cpp).
export function rotateMatrix(v: Vec3, angle: number): number[] {
  v = normalize(v);
  const c = Math.cos(angle);
  const s = Math.sin(angle);
  const { x, y, z } = v;
  return [
    x * x + c * (1 - x * x), x * y + c * (0 - x * y) - z * s, x * z + c * (0 - x * z) + y * s,
    y * x + c * (0 - y * x) + z * s, y * y + c * (1 - y * y), y * z + c * (0 - y * z) - x * s,
    z * x + c * (0 - z * x) - y * s, z * y + c * (0 - z * y) + x * s, z * z + c * (1 - z * z),
  ];
}

export function matMulVec(m: number[], v: Vec3): Vec3 {
  return {
    x: m[0]! * v.x + m[1]! * v.y + m[2]! * v.z,
    y: m[3]! * v.x + m[4]! * v.y + m[5]! * v.z,
    z: m[6]! * v.x + m[7]! * v.y + m[8]! * v.z,
  };
}

// 2D point-in-polygon "inverse fill" parity test.
// Port of Polygon::haspoint (pedralg.cpp) — also used by ColorAlg projections.
export function pointInPolygon2d(poly: Vec2[], p: Vec2): boolean {
  const n = poly.length;
  if (n < 3) return false;
  for (let i = 0; i < n; i++) {
    const a = poly[i]!;
    const b = poly[(i + 1) % n]!;
    if (dist2(a, p) + dist2(p, b) < dist2(a, b) + 2 * EPS) return true; // on edge
  }
  let invert = 0;
  for (let i = 0; i < n; i++) {
    let a = poly[i]!;
    let b = poly[(i + 1) % n]!;
    if (a.y === b.y) continue; // exact equality, intentional (matches C++)
    if (a.y > b.y) [a, b] = [b, a];
    if (p.y < a.y || p.y >= b.y) continue;
    if (Math.abs(a.x - b.x) < EPS) {
      if (p.x >= (a.x + b.x) / 2) invert++;
      continue;
    }
    const k = (b.y - a.y) / (b.x - a.x);
    if (b.x > a.x) {
      if (k * (p.x - a.x) + a.y > p.y) invert++;
    } else {
      if (k * (p.x - a.x) + a.y < p.y) invert++;
    }
  }
  return (invert & 1) === 1;
}
