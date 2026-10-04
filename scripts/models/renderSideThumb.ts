// The side-elevation thumbnail of an XML-built vehicle, rendered at
// `npm run export` from the GLB the export has just written.
//
// An UPLOADED vehicle's thumbnail is rendered in the uploader's browser by Rail
// Island's src/creator/glbThumb.ts: three.js, a WebGLRenderer, MeshStandardMaterial from the
// GLTFLoader, an ambient light and a sun, a side camera. The build has no browser
// and no GPU, so this file reproduces that render in software, from the same
// numbers (thumbView.ts, a copy of Rail Island's table):
//
//   - the same frame (320x120), field of view, framing rule and camera position;
//   - the same opaque backdrop, the same bright front lights, every material
//     drawn as paint, and the same final sharpen -- the look tuned to match the
//     hand-drawn side views these models used to ship with;
//   - three's direct lighting for a MeshStandardMaterial, per pixel: Lambert
//     diffuse over (1 - metalness) of the albedo from the ambient light and the
//     sun, plus the sun's GGX specular (Smith-correlated visibility, Schlick
//     Fresnel, F0 = 0.04 mixed toward the albedo by metalness) -- the BRDF three's
//     shader chunks implement, with no tone mapping and an sRGB output, which is
//     what a default WebGLRenderer does;
//   - the base colour texture sampled per pixel (bilinear, clamped, sRGB-decoded)
//     at the perspective-correct UV;
//   - back faces culled, as the default FrontSide material does -- so a model the
//     game draws with a hole shows the same hole here.
//
// So the shipped fleet's pictures replace the hand-drawn image.bmp side views the
// XML packs came with, and sit beside an upload's without reading as a second
// catalogue. Pure Node (no three, no DOM): pixels out, the caller encodes them.

import { PNG } from "pngjs";
import {
  type Gltf,
  type Mat4,
  type Vec3,
  cross,
  dot,
  ident,
  mul,
  norm,
  parseGlb,
  readAccessor,
  sub,
  trs,
  xformDir,
  xformPoint,
} from "./glbParse.ts";
import {
  SIDE_AMBIENT,
  SIDE_DIST_FACTOR,
  SIDE_ELEVATION,
  SIDE_FLAT_METAL,
  SIDE_FOV_DEG,
  SIDE_SHARPEN,
  SIDE_SUN,
  SIDE_SUN_POSITION,
  SIDE_THUMB,
  THUMB_BACKGROUND,
  sharpenRgba,
} from "./thumbView.ts";

/** Supersampling per axis; three's MSAA is approximated by a box filter. */
const SS = 3;

export interface RenderedThumb {
  width: number;
  height: number;
  /** RGBA, 8 bits per channel, opaque. */
  data: Uint8Array;
}

// ------------------------------------------------------------------- colour

function srgbToLinear(c: number): number {
  return c <= 0.04045 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4);
}

function linearToSrgb(v: number): number {
  const c = Math.min(1, Math.max(0, v));
  return c <= 0.0031308 ? c * 12.92 : 1.055 * Math.pow(c, 1 / 2.4) - 0.055;
}

function hexToLinear(hex: string): Vec3 {
  const n = parseInt(hex.replace("#", ""), 16);
  return [srgbToLinear(((n >> 16) & 255) / 255), srgbToLinear(((n >> 8) & 255) / 255), srgbToLinear((n & 255) / 255)];
}

// ------------------------------------------------------------------- texture

/** A decoded texture in linear RGB, sampled bilinearly with clamp-to-edge. */
class Texture {
  private readonly rgb: Float32Array;
  constructor(
    readonly width: number,
    readonly height: number,
    rgba: Uint8Array,
  ) {
    // Decoded once: a lookup table beats a pow() per texel per sample.
    const lut = new Float32Array(256);
    for (let i = 0; i < 256; i++) lut[i] = srgbToLinear(i / 255);
    this.rgb = new Float32Array(width * height * 3);
    for (let i = 0; i < width * height; i++) {
      this.rgb[i * 3] = lut[rgba[i * 4]!]!;
      this.rgb[i * 3 + 1] = lut[rgba[i * 4 + 1]!]!;
      this.rgb[i * 3 + 2] = lut[rgba[i * 4 + 2]!]!;
    }
  }

  sample(u: number, v: number, out: Vec3): void {
    // glTF UV origin is the top-left of the image, which is row 0 of the PNG.
    const x = Math.min(this.width - 1, Math.max(0, u * this.width - 0.5));
    const y = Math.min(this.height - 1, Math.max(0, v * this.height - 0.5));
    const x0 = Math.floor(x), y0 = Math.floor(y);
    const x1 = Math.min(this.width - 1, x0 + 1), y1 = Math.min(this.height - 1, y0 + 1);
    const fx = x - x0, fy = y - y0;
    for (let c = 0; c < 3; c++) {
      const a = this.rgb[(y0 * this.width + x0) * 3 + c]!;
      const b = this.rgb[(y0 * this.width + x1) * 3 + c]!;
      const d = this.rgb[(y1 * this.width + x0) * 3 + c]!;
      const e = this.rgb[(y1 * this.width + x1) * 3 + c]!;
      out[c] = (a * (1 - fx) + b * fx) * (1 - fy) + (d * (1 - fx) + e * fx) * fy;
    }
  }
}

function decodeTexture(json: Gltf, bin: Uint8Array, texIdx: number): Texture | null {
  const src = json.textures?.[texIdx]?.source;
  const view = src != null ? json.images?.[src]?.bufferView : undefined;
  const bv = view != null ? json.bufferViews?.[view] : undefined;
  if (!bv) return null;
  const start = bv.byteOffset ?? 0;
  try {
    const png = PNG.sync.read(Buffer.from(bin.subarray(start, start + bv.byteLength)));
    return new Texture(png.width, png.height, png.data);
  } catch {
    return null; // not a PNG pngjs reads: the base colour factor alone stands in
  }
}

// ------------------------------------------------------------------- geometry

interface Material {
  color: Vec3; // linear baseColorFactor
  metalness: number;
  roughness: number;
  texture: Texture | null;
}

interface Tri {
  p: [Vec3, Vec3, Vec3];
  n: [Vec3, Vec3, Vec3];
  uv: [number, number, number, number, number, number] | null;
  mat: Material;
}

function collect(json: Gltf, bin: Uint8Array): Tri[] {
  const tris: Tri[] = [];
  const textures = new Map<number, Texture | null>();
  const materials = new Map<number, Material>();
  // glTF's defaults, which are also the GLTFLoader's: white, fully metallic, fully rough.
  const fallback: Material = { color: [1, 1, 1], metalness: 1, roughness: 1, texture: null };
  const materialFor = (index: number | undefined): Material => {
    if (index == null) return fallback;
    const cached = materials.get(index);
    if (cached) return cached;
    const pbr = json.materials?.[index]?.pbrMetallicRoughness;
    const texIdx = pbr?.baseColorTexture?.index;
    if (texIdx != null && !textures.has(texIdx)) textures.set(texIdx, decodeTexture(json, bin, texIdx));
    const f = pbr?.baseColorFactor ?? [1, 1, 1, 1];
    const mat: Material = {
      color: [f[0]!, f[1]!, f[2]!],
      metalness: pbr?.metallicFactor ?? 1,
      roughness: pbr?.roughnessFactor ?? 1,
      texture: texIdx != null ? (textures.get(texIdx) ?? null) : null,
    };
    materials.set(index, mat);
    return mat;
  };

  const visit = (nodeIndex: number, parent: Mat4): void => {
    const node = json.nodes?.[nodeIndex];
    if (!node) return;
    const world = mul(parent, node.matrix ? (node.matrix as Mat4) : trs(node.translation, node.rotation, node.scale));
    if (node.mesh != null) {
      for (const prim of json.meshes?.[node.mesh]?.primitives ?? []) {
        if ((prim.mode ?? 4) !== 4 || prim.attributes.POSITION == null) continue;
        const pos = readAccessor(json, bin, prim.attributes.POSITION);
        const nrm = prim.attributes.NORMAL != null ? readAccessor(json, bin, prim.attributes.NORMAL) : null;
        const mat = materialFor(prim.material);
        const uvs = mat.texture && prim.attributes.TEXCOORD_0 != null ? readAccessor(json, bin, prim.attributes.TEXCOORD_0) : null;
        const idx = prim.indices != null ? readAccessor(json, bin, prim.indices) : Array.from({ length: pos.length / 3 }, (_, i) => i);
        for (let i = 0; i + 2 < idx.length; i += 3) {
          const vs = [idx[i]!, idx[i + 1]!, idx[i + 2]!];
          const p = vs.map((v) => xformPoint(world, [pos[v * 3]!, pos[v * 3 + 1]!, pos[v * 3 + 2]!])) as [Vec3, Vec3, Vec3];
          let n: [Vec3, Vec3, Vec3];
          if (nrm) {
            n = vs.map((v) => norm(xformDir(world, [nrm[v * 3]!, nrm[v * 3 + 1]!, nrm[v * 3 + 2]!]))) as [Vec3, Vec3, Vec3];
          } else {
            const face = norm(cross(sub(p[1], p[0]), sub(p[2], p[0])));
            n = [face, face, face];
          }
          const uv = uvs
            ? ([uvs[vs[0]! * 2]!, uvs[vs[0]! * 2 + 1]!, uvs[vs[1]! * 2]!, uvs[vs[1]! * 2 + 1]!, uvs[vs[2]! * 2]!, uvs[vs[2]! * 2 + 1]!] as Tri["uv"])
            : null;
          tris.push({ p, n, uv, mat });
        }
      }
    }
    for (const c of node.children ?? []) visit(c, world);
  };
  const scene = json.scenes?.[json.scene ?? 0];
  for (const n of scene?.nodes ?? []) visit(n, ident());
  return tris;
}

// ------------------------------------------------------------------- shading

// three's BRDF_GGX for a direct light: Schlick Fresnel, Smith-correlated
// visibility, GGX distribution.
function ggx(nl: number, nv: number, nh: number, vh: number, roughness: number, f0: Vec3, out: Vec3): void {
  const alpha = roughness * roughness;
  const a2 = alpha * alpha;
  const fresnel = Math.pow(1 - vh, 5);
  const gv = nl * Math.sqrt(a2 + (1 - a2) * nv * nv);
  const gl = nv * Math.sqrt(a2 + (1 - a2) * nl * nl);
  const vis = 0.5 / Math.max(gv + gl, 1e-6);
  const denom = nh * nh * (a2 - 1) + 1;
  const d = a2 / (Math.PI * denom * denom);
  for (let c = 0; c < 3; c++) out[c] = (f0[c]! + (1 - f0[c]!) * fresnel) * vis * d;
}

// -------------------------------------------------------------------- render

/** Render the side thumbnail of an (unencoded) GLB, or throw if it has no geometry. */
export function renderSideThumb(glbBytes: Uint8Array): RenderedThumb {
  const { json, bin } = parseGlb(glbBytes);
  const tris = collect(json, bin);
  if (tris.length === 0) throw new Error("no triangles");

  const { width, height } = SIDE_THUMB;
  const aspect = width / height;

  // Framing, exactly as glbThumb.ts frames a side view: on the silhouette the
  // camera actually sees (length x height), from the +Z flank, a touch above.
  const mn: Vec3 = [Infinity, Infinity, Infinity];
  const mx: Vec3 = [-Infinity, -Infinity, -Infinity];
  for (const t of tris) for (const p of t.p) for (let c = 0; c < 3; c++) {
    mn[c] = Math.min(mn[c]!, p[c]!);
    mx[c] = Math.max(mx[c]!, p[c]!);
  }
  const center: Vec3 = [(mn[0] + mx[0]) / 2, (mn[1] + mx[1]) / 2, (mn[2] + mx[2]) / 2];
  const extent = Math.max((mx[0] - mn[0]) / aspect, mx[1] - mn[1]) || 1;
  const fov = (SIDE_FOV_DEG * Math.PI) / 180;
  // Fitted at the NEAR flank, not the centre: the side facing the camera is half
  // the body's width closer and draws that much larger, which with a tight frame
  // clipped a wide vehicle's ends.
  const dist = (extent / (2 * Math.tan(fov / 2))) * SIDE_DIST_FACTOR + (mx[2] - mn[2]) / 2;
  const near = extent / 100;
  const far = extent * 100;
  const eye: Vec3 = [center[0], center[1] + dist * SIDE_ELEVATION, center[2] + dist];

  // lookAt(center) with +Y up: the camera looks down its own -Z.
  const zAxis = norm(sub(eye, center));
  const xAxis = norm(cross([0, 1, 0], zAxis));
  const yAxis = cross(zAxis, xAxis);
  const view: Mat4 = [
    xAxis[0], yAxis[0], zAxis[0], 0,
    xAxis[1], yAxis[1], zAxis[1], 0,
    xAxis[2], yAxis[2], zAxis[2], 0,
    -dot(xAxis, eye), -dot(yAxis, eye), -dot(zAxis, eye), 1,
  ];
  const f = 1 / Math.tan(fov / 2);

  const sun = norm([...SIDE_SUN_POSITION] as Vec3);
  const ambient = SIDE_AMBIENT / Math.PI; // irradiance x Lambert's 1/pi
  const background = hexToLinear(THUMB_BACKGROUND);

  const W = width * SS;
  const H = height * SS;
  const depth = new Float64Array(W * H).fill(Infinity);
  const rgb = new Float32Array(W * H * 3);
  for (let i = 0; i < W * H; i++) {
    rgb[i * 3] = background[0];
    rgb[i * 3 + 1] = background[1];
    rgb[i * 3 + 2] = background[2];
  }

  const albedo: Vec3 = [0, 0, 0];
  const spec: Vec3 = [0, 0, 0];
  const f0: Vec3 = [0, 0, 0];
  for (const tri of tris) {
    const vs = tri.p.map((p) => xformPoint(view, p));
    if (vs.some((v) => -v[2] <= near || -v[2] >= far)) continue;
    const sx: number[] = [], sy: number[] = [], iw: number[] = [];
    for (const v of vs) {
      const w = -v[2];
      iw.push(1 / w);
      sx.push(((v[0] * f) / aspect / w * 0.5 + 0.5) * W);
      sy.push((0.5 - (v[1] * f) / w * 0.5) * H);
    }
    const area = (sx[1]! - sx[0]!) * (sy[2]! - sy[0]!) - (sx[2]! - sx[0]!) * (sy[1]! - sy[0]!);
    // Counter-clockwise on screen is the front (y runs down here, hence < 0);
    // FrontSide culls the rest.
    if (area >= 0) continue;
    const x0 = Math.max(0, Math.floor(Math.min(sx[0]!, sx[1]!, sx[2]!)));
    const x1 = Math.min(W - 1, Math.ceil(Math.max(sx[0]!, sx[1]!, sx[2]!)));
    const y0 = Math.max(0, Math.floor(Math.min(sy[0]!, sy[1]!, sy[2]!)));
    const y1 = Math.min(H - 1, Math.ceil(Math.max(sy[0]!, sy[1]!, sy[2]!)));
    const m = SIDE_FLAT_METAL ? { ...tri.mat, metalness: 0 } : tri.mat;
    const rough = Math.max(m.roughness, 0.0525); // three's floor
    for (let y = y0; y <= y1; y++) {
      const py = y + 0.5;
      for (let x = x0; x <= x1; x++) {
        const px = x + 0.5;
        let b0 = ((sx[1]! - px) * (sy[2]! - py) - (sx[2]! - px) * (sy[1]! - py)) / area;
        let b1 = ((sx[2]! - px) * (sy[0]! - py) - (sx[0]! - px) * (sy[2]! - py)) / area;
        let b2 = 1 - b0 - b1;
        if (b0 < 0 || b1 < 0 || b2 < 0) continue;
        const wsum = b0 * iw[0]! + b1 * iw[1]! + b2 * iw[2]!;
        const z = 1 / wsum;
        const at = y * W + x;
        if (z >= depth[at]!) continue;
        depth[at] = z;
        b0 = (b0 * iw[0]!) / wsum;
        b1 = (b1 * iw[1]!) / wsum;
        b2 = 1 - b0 - b1;

        const nrm = norm([
          tri.n[0][0] * b0 + tri.n[1][0] * b1 + tri.n[2][0] * b2,
          tri.n[0][1] * b0 + tri.n[1][1] * b1 + tri.n[2][1] * b2,
          tri.n[0][2] * b0 + tri.n[1][2] * b1 + tri.n[2][2] * b2,
        ]);
        const world: Vec3 = [
          tri.p[0][0] * b0 + tri.p[1][0] * b1 + tri.p[2][0] * b2,
          tri.p[0][1] * b0 + tri.p[1][1] * b1 + tri.p[2][1] * b2,
          tri.p[0][2] * b0 + tri.p[1][2] * b1 + tri.p[2][2] * b2,
        ];

        albedo[0] = m.color[0];
        albedo[1] = m.color[1];
        albedo[2] = m.color[2];
        if (tri.uv && m.texture) {
          const u = tri.uv[0] * b0 + tri.uv[2] * b1 + tri.uv[4] * b2;
          const v = tri.uv[1] * b0 + tri.uv[3] * b1 + tri.uv[5] * b2;
          m.texture.sample(u, v, spec);
          albedo[0] *= spec[0];
          albedo[1] *= spec[1];
          albedo[2] *= spec[2];
        }

        const toEye = norm(sub(eye, world));
        const nl = Math.max(0, dot(nrm, sun));
        const nv = Math.min(1, Math.max(1e-4, Math.abs(dot(nrm, toEye))));
        const half = norm([sun[0] + toEye[0], sun[1] + toEye[1], sun[2] + toEye[2]]);
        const nh = Math.min(1, Math.max(0, dot(nrm, half)));
        const vh = Math.min(1, Math.max(0, dot(toEye, half)));
        for (let c = 0; c < 3; c++) f0[c] = 0.04 * (1 - m.metalness) + albedo[c]! * m.metalness;
        if (nl > 0) ggx(nl, nv, nh, vh, rough, f0, spec);
        else spec[0] = spec[1] = spec[2] = 0;

        const diffuseScale = (1 - m.metalness) * (ambient + (SIDE_SUN * nl) / Math.PI);
        for (let c = 0; c < 3; c++) {
          rgb[at * 3 + c] = albedo[c]! * diffuseScale + SIDE_SUN * nl * spec[c]!;
        }
      }
    }
  }

  // Box-filter the supersampled buffer down, in linear light, then encode sRGB.
  const data = new Uint8Array(width * height * 4);
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      let r = 0, g = 0, b = 0;
      for (let dy = 0; dy < SS; dy++) {
        for (let dx = 0; dx < SS; dx++) {
          const at = (y * SS + dy) * W + (x * SS + dx);
          r += rgb[at * 3]!;
          g += rgb[at * 3 + 1]!;
          b += rgb[at * 3 + 2]!;
        }
      }
      const o = (y * width + x) * 4;
      const n = SS * SS;
      data[o] = Math.round(linearToSrgb(r / n) * 255);
      data[o + 1] = Math.round(linearToSrgb(g / n) * 255);
      data[o + 2] = Math.round(linearToSrgb(b / n) * 255);
      data[o + 3] = 255;
    }
  }
  sharpenRgba(data, width, height, SIDE_SHARPEN);
  return { width, height, data };
}
