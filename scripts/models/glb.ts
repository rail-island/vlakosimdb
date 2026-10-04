// Build a GLB (glTF 2.0 binary) from intermediate polygons + a texture atlas,
// with UV remapping into the atlas and an embedded PNG. Returns { buffer, stats }.
//
// A model is built from one or more named parts. A single unnamed part is the
// ordinary rigid model and produces one mesh under one node. Several named parts
// (an articulated vehicle: section0 / section1 / bellows0, see articulate.ts)
// produce one mesh + node each under a shared root node, so the render layer can
// pose them independently. Geometry, UVs and the atlas are identical either way.
import type { ModelPolygon } from "./geometry.ts";
import type { ModelPart } from "./articulate.ts";
import type { AtlasMapping } from "./atlas.ts";

export interface GlbStats {
  vertices: number;
  faces: number;
  bbox: { min: number[]; max: number[] };
  length: number;
}
export interface GlbResult {
  buffer: Buffer;
  stats: GlbStats;
}

function pad4(buf: Buffer): Buffer {
  const rem = buf.length % 4;
  if (rem === 0) return buf;
  return Buffer.concat([buf, Buffer.alloc(4 - rem)]);
}

// One part's triangulated geometry, ready to be written into the binary chunk.
interface PartMesh {
  name: string | null;
  positions: number[];
  normals: number[];
  texcoords: number[];
  indices: number[];
  vertices: number;
  triangles: number;
  min: number[];
  max: number[];
}

function triangulate(polygons: ModelPolygon[], atlasMap: Map<string, AtlasMapping>, name: string | null): PartMesh {
  const positions: number[] = [];
  const normals: number[] = [];
  const texcoords: number[] = [];
  const indices: number[] = [];
  let vertices = 0;
  let triangles = 0;

  for (const poly of polygons) {
    const mapping = poly.textureKey ? atlasMap.get(poly.textureKey) : undefined;
    const [nx, ny, nz] = poly.normal;
    const verts = poly.verts;
    if (verts.length < 3) continue;

    const base = vertices;
    for (const v of verts) {
      let u: number, vv: number;
      if (mapping) {
        const [uOff, vOff, uSc, vSc] = mapping;
        u = uOff + v.s * uSc;
        vv = vOff + (1.0 - v.t) * vSc;
      } else {
        u = v.s;
        vv = 1.0 - v.t;
      }
      positions.push(v.x, v.y, v.z);
      normals.push(nx, ny, nz);
      texcoords.push(u, vv);
      vertices++;
    }
    for (let j = 1; j < verts.length - 1; j++) {
      indices.push(base, base + j, base + j + 1);
      triangles++;
    }
  }

  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < vertices; i++) {
    for (let c = 0; c < 3; c++) {
      const val = positions[i * 3 + c]!;
      if (val < min[c]!) min[c] = val;
      if (val > max[c]!) max[c] = val;
    }
  }

  return { name, positions, normals, texcoords, indices, vertices, triangles, min, max };
}

export function buildGLB(
  parts: ModelPart[], atlasMap: Map<string, AtlasMapping>, atlasPng: Buffer,
  pbr: [number, number] = [0.0, 0.8],
): GlbResult | null {
  const meshes = parts.map((p) => triangulate(p.polygons, atlasMap, p.name));
  for (const m of meshes) {
    if (m.vertices === 0 && m.name !== null) throw new Error(`part "${m.name}" has no geometry`);
  }
  const kept = meshes.filter((m) => m.vertices > 0);
  if (kept.length === 0) return null;

  const vertexCount = kept.reduce((n, m) => n + m.vertices, 0);
  const triangleCount = kept.reduce((n, m) => n + m.triangles, 0);
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (const m of kept) {
    for (let c = 0; c < 3; c++) {
      if (m.min[c]! < min[c]!) min[c] = m.min[c]!;
      if (m.max[c]! > max[c]!) max[c] = m.max[c]!;
    }
  }

  // Per part: POSITION, NORMAL, TEXCOORD_0 and index buffer views/accessors, in
  // that order, so a single-part model keeps the historical 0..3 layout.
  const chunks: Buffer[] = [];
  const bufferViews: Record<string, number>[] = [];
  const accessors: Record<string, unknown>[] = [];
  const gltfMeshes: Record<string, unknown>[] = [];
  let offset = 0;
  const pushView = (data: Buffer, target: number): number => {
    const padded = pad4(data);
    bufferViews.push({ buffer: 0, byteOffset: offset, byteLength: data.length, target });
    chunks.push(padded);
    offset += padded.length;
    return bufferViews.length - 1;
  };

  for (const m of kept) {
    const useUint32 = m.vertices > 65535;
    const idxArray = useUint32 ? new Uint32Array(m.indices) : new Uint16Array(m.indices);
    const posView = pushView(Buffer.from(new Float32Array(m.positions).buffer), 34962);
    const normView = pushView(Buffer.from(new Float32Array(m.normals).buffer), 34962);
    const uvView = pushView(Buffer.from(new Float32Array(m.texcoords).buffer), 34962);
    const idxView = pushView(Buffer.from(idxArray.buffer, idxArray.byteOffset, m.indices.length * (useUint32 ? 4 : 2)), 34963);
    const base = accessors.length;
    accessors.push(
      { bufferView: posView, componentType: 5126, count: m.vertices, type: "VEC3", min: m.min, max: m.max },
      { bufferView: normView, componentType: 5126, count: m.vertices, type: "VEC3" },
      { bufferView: uvView, componentType: 5126, count: m.vertices, type: "VEC2" },
      { bufferView: idxView, componentType: useUint32 ? 5125 : 5123, count: m.indices.length, type: "SCALAR" },
    );
    gltfMeshes.push({
      ...(m.name ? { name: m.name } : {}),
      primitives: [{
        attributes: { POSITION: base, NORMAL: base + 1, TEXCOORD_0: base + 2 },
        indices: base + 3,
        material: 0,
      }],
    });
  }

  const hasTexture = atlasPng && atlasPng.length > 0;
  let materials: Record<string, unknown>[];
  let texturesGltf: Record<string, number>[];
  let imagesGltf: Record<string, unknown>[];
  let samplersGltf: Record<string, number>[];

  if (hasTexture) {
    const imgView = bufferViews.length;
    bufferViews.push({ buffer: 0, byteOffset: offset, byteLength: atlasPng.length });
    chunks.push(pad4(atlasPng));
    offset += pad4(atlasPng).length;
    materials = [{
      pbrMetallicRoughness: {
        baseColorTexture: { index: 0 },
        metallicFactor: pbr[0],
        roughnessFactor: pbr[1],
      },
    }];
    texturesGltf = [{ source: 0, sampler: 0 }];
    imagesGltf = [{ bufferView: imgView, mimeType: "image/png" }];
    samplersGltf = [{ magFilter: 9729, minFilter: 9987, wrapS: 33071, wrapT: 33071 }];
  } else {
    materials = [{
      pbrMetallicRoughness: {
        baseColorFactor: [0.7, 0.7, 0.7, 1.0],
        metallicFactor: pbr[0],
        roughnessFactor: pbr[1],
      },
    }];
    texturesGltf = [];
    imagesGltf = [];
    samplersGltf = [];
  }

  const binData = Buffer.concat(chunks);

  // One unnamed part keeps the historical single-node scene; named parts hang off
  // a shared root so the runtime can look each up by name and pose it.
  const single = kept.length === 1 && kept[0]!.name === null;
  const nodes: Record<string, unknown>[] = single
    ? [{ mesh: 0 }]
    : [
        { name: "root", children: kept.map((_, i) => i + 1) },
        ...kept.map((m, i) => ({ name: m.name ?? `part${i}`, mesh: i })),
      ];

  const gltf: Record<string, unknown> = {
    asset: { version: "2.0", generator: "rail-island-web" },
    scene: 0,
    scenes: [{ nodes: [0] }],
    nodes,
    meshes: gltfMeshes,
    materials,
    accessors,
    bufferViews,
    buffers: [{ byteLength: binData.length }],
  };
  if (texturesGltf.length) gltf.textures = texturesGltf;
  if (imagesGltf.length) gltf.images = imagesGltf;
  if (samplersGltf.length) gltf.samplers = samplersGltf;

  let jsonStr = JSON.stringify(gltf);
  while (jsonStr.length % 4 !== 0) jsonStr += " ";
  const jsonData = Buffer.from(jsonStr, "utf-8");

  const totalLength = 12 + 8 + jsonData.length + 8 + binData.length;
  const header = Buffer.alloc(12);
  header.writeUInt32LE(0x46546c67, 0); // "glTF"
  header.writeUInt32LE(2, 4);
  header.writeUInt32LE(totalLength, 8);
  const jsonHeader = Buffer.alloc(8);
  jsonHeader.writeUInt32LE(jsonData.length, 0);
  jsonHeader.writeUInt32LE(0x4e4f534a, 4); // "JSON"
  const binHeader = Buffer.alloc(8);
  binHeader.writeUInt32LE(binData.length, 0);
  binHeader.writeUInt32LE(0x004e4942, 4); // "BIN\0"

  const buffer = Buffer.concat([header, jsonHeader, jsonData, binHeader, binData]);

  return {
    buffer,
    stats: {
      vertices: vertexCount,
      faces: triangleCount,
      bbox: { min, max },
      length: Math.round((max[0]! - min[0]!) * 1000) / 1000,
    },
  };
}
