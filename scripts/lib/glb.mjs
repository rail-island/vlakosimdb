// Build a GLB (glTF 2.0 binary) from intermediate polygons + a texture atlas.
// Port of convert_model_xml_to_glb (upload_utils.py) with UV remapping into the
// atlas and an embedded PNG. Returns { buffer, stats }.

function pad4(buf) {
  const rem = buf.length % 4;
  if (rem === 0) return buf;
  return Buffer.concat([buf, Buffer.alloc(4 - rem)]);
}

/**
 * @param {Array} polygons  from geometry.parseModel()
 * @param {Map} atlasMap    key -> [uOff,vOff,uScale,vScale]
 * @param {Buffer} atlasPng atlas PNG bytes (may be empty)
 * @param {[number,number]} pbr [metallicFactor, roughnessFactor]
 */
export function buildGLB(polygons, atlasMap, atlasPng, pbr = [0.0, 0.8]) {
  const positions = [];
  const normals = [];
  const texcoords = [];
  const indices = [];
  let vertexCount = 0;
  let triangleCount = 0;

  for (const poly of polygons) {
    const mapping = poly.textureKey ? atlasMap.get(poly.textureKey) : undefined;
    const [nx, ny, nz] = poly.normal;
    const verts = poly.verts;
    if (verts.length < 3) continue;

    const base = vertexCount;
    for (const v of verts) {
      let u, vv;
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
      vertexCount++;
    }
    for (let j = 1; j < verts.length - 1; j++) {
      indices.push(base, base + j, base + j + 1);
      triangleCount++;
    }
  }

  if (vertexCount === 0) return null;

  const useUint32 = vertexCount > 65535;

  const posData = pad4(Buffer.from(new Float32Array(positions).buffer));
  const normData = pad4(Buffer.from(new Float32Array(normals).buffer));
  const uvData = pad4(Buffer.from(new Float32Array(texcoords).buffer));
  const idxArray = useUint32 ? new Uint32Array(indices) : new Uint16Array(indices);
  const idxData = pad4(Buffer.from(idxArray.buffer));
  const idxComponentType = useUint32 ? 5125 : 5123;
  const idxByteLen = indices.length * (useUint32 ? 4 : 2);

  // Bounding box.
  const min = [Infinity, Infinity, Infinity];
  const max = [-Infinity, -Infinity, -Infinity];
  for (let i = 0; i < vertexCount; i++) {
    for (let c = 0; c < 3; c++) {
      const val = positions[i * 3 + c];
      if (val < min[c]) min[c] = val;
      if (val > max[c]) max[c] = val;
    }
  }

  const hasTexture = atlasPng && atlasPng.length > 0;

  let binData, bufferViews, materials, texturesGltf, imagesGltf, samplersGltf;
  const posOff = 0;
  const normOff = posOff + posData.length;
  const uvOff = normOff + normData.length;
  const idxOff = uvOff + uvData.length;

  if (hasTexture) {
    const imgData = pad4(atlasPng);
    const imgOff = idxOff + idxData.length;
    binData = Buffer.concat([posData, normData, uvData, idxData, imgData]);
    materials = [{
      pbrMetallicRoughness: {
        baseColorTexture: { index: 0 },
        metallicFactor: pbr[0],
        roughnessFactor: pbr[1],
      },
    }];
    texturesGltf = [{ source: 0, sampler: 0 }];
    imagesGltf = [{ bufferView: 4, mimeType: "image/png" }];
    samplersGltf = [{ magFilter: 9729, minFilter: 9987, wrapS: 33071, wrapT: 33071 }];
    bufferViews = [
      { buffer: 0, byteOffset: posOff, byteLength: vertexCount * 12, target: 34962 },
      { buffer: 0, byteOffset: normOff, byteLength: vertexCount * 12, target: 34962 },
      { buffer: 0, byteOffset: uvOff, byteLength: vertexCount * 8, target: 34962 },
      { buffer: 0, byteOffset: idxOff, byteLength: idxByteLen, target: 34963 },
      { buffer: 0, byteOffset: imgOff, byteLength: atlasPng.length },
    ];
  } else {
    binData = Buffer.concat([posData, normData, uvData, idxData]);
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
    bufferViews = [
      { buffer: 0, byteOffset: posOff, byteLength: vertexCount * 12, target: 34962 },
      { buffer: 0, byteOffset: normOff, byteLength: vertexCount * 12, target: 34962 },
      { buffer: 0, byteOffset: uvOff, byteLength: vertexCount * 8, target: 34962 },
      { buffer: 0, byteOffset: idxOff, byteLength: idxByteLen, target: 34963 },
    ];
  }

  const gltf = {
    asset: { version: "2.0", generator: "vlakosimdb-web" },
    scene: 0,
    scenes: [{ nodes: [0] }],
    nodes: [{ mesh: 0 }],
    meshes: [{ primitives: [{
      attributes: { POSITION: 0, NORMAL: 1, TEXCOORD_0: 2 },
      indices: 3,
      material: 0,
    }] }],
    materials,
    accessors: [
      { bufferView: 0, componentType: 5126, count: vertexCount, type: "VEC3", min, max },
      { bufferView: 1, componentType: 5126, count: vertexCount, type: "VEC3" },
      { bufferView: 2, componentType: 5126, count: vertexCount, type: "VEC2" },
      { bufferView: 3, componentType: idxComponentType, count: indices.length, type: "SCALAR" },
    ],
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
      length: Math.round((max[0] - min[0]) * 1000) / 1000,
    },
  };
}
