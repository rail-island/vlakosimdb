// Parse a model/index.xml <model> section into intermediate polygon soup,
// resolving <inlines>/<inline> sub-models (pantographs etc.) with their
// scale/translation transforms — an improvement over the Python pipeline,
// which ignores inlines.
import fs from "node:fs";
import path from "node:path";
import { parseXmlSafe, asArray, attrNum, attrStr } from "./xml.mjs";

/**
 * Parse a model directory into:
 *   { polygons: [{ textureKey, normal:[nx,ny,nz], color:[r,g,b], transparent,
 *                  verts:[{x,y,z,s,t}, ...] }],
 *     textureSources: Map<key, {type:'pack', dir, name} | {type:'material', name}> }
 *
 * `modelDir` contains index.xml. Sub-models are nested folders referenced by
 * <inline name="..."> and are merged in with their transforms applied; their
 * pack textures are namespaced by folder so they don't collide with the parent.
 */
export function parseModel(modelDir) {
  const polygons = [];
  const textureSources = new Map();
  walk(modelDir, "", [1, 1, 1], [0, 0, 0], polygons, textureSources, 0);
  return { polygons, textureSources };
}

function walk(dir, keyPrefix, scale, translate, polygons, textureSources, depth) {
  if (depth > 8) return; // guard against pathological recursion
  const xmlPath = path.join(dir, "index.xml");
  if (!fs.existsSync(xmlPath)) return;

  let root;
  try {
    root = parseXmlSafe(xmlPath);
  } catch {
    return;
  }
  const sm = root.simplemodel ?? root;
  const modelEl = sm.model;

  const [sx, sy, sz] = scale;
  const flip = sx * sy * sz < 0; // negative determinant flips winding

  if (modelEl) {
    for (const polygon of asArray(modelEl.polygon)) {
      const texRef = attrStr(polygon, "texture");
      const textureKey = registerTexture(texRef, dir, keyPrefix, textureSources);

      let verts = asArray(polygon.vertex).map((v) => ({
        x: attrNum(v, "x") * sx + translate[0],
        y: attrNum(v, "y") * sy + translate[1],
        z: attrNum(v, "z") * sz + translate[2],
        s: attrNum(v, "s"),
        t: attrNum(v, "t"),
      }));
      if (verts.length < 3) continue;
      if (flip) verts = verts.slice().reverse();

      // Transform the face normal by the sign of each scale axis, then normalize.
      let tnx = attrNum(polygon, "nx") * Math.sign(sx || 1);
      let tny = attrNum(polygon, "ny") * Math.sign(sy || 1);
      let tnz = attrNum(polygon, "nz") * Math.sign(sz || 1);
      const len = Math.hypot(tnx, tny, tnz) || 1;

      polygons.push({
        textureKey,
        normal: [tnx / len, tny / len, tnz / len],
        color: [attrNum(polygon, "r", 1), attrNum(polygon, "g", 1), attrNum(polygon, "b", 1)],
        transparent: attrStr(polygon, "texture.transparent", "0") === "1",
        verts,
      });
    }
  }

  // Recurse into sub-models.
  for (const inl of asArray(sm.inlines?.inline)) {
    const name = attrStr(inl, "name");
    if (!name) continue;
    const sxc = attrNum(inl, "sx", 1);
    const syc = attrNum(inl, "sy", 1);
    const szc = attrNum(inl, "sz", 1);
    const txc = attrNum(inl, "tx", 0);
    const tyc = attrNum(inl, "ty", 0);
    const tzc = attrNum(inl, "tz", 0);
    // Compose child transform into parent space.
    const childScale = [sx * sxc, sy * syc, sz * szc];
    const childTranslate = [sx * txc + translate[0], sy * tyc + translate[1], sz * tzc + translate[2]];
    walk(
      path.join(dir, name),
      keyPrefix ? `${keyPrefix}${name}/` : `${name}/`,
      childScale,
      childTranslate,
      polygons,
      textureSources,
      depth + 1
    );
  }
}

function registerTexture(texRef, dir, keyPrefix, textureSources) {
  if (!texRef) return null;
  if (texRef.startsWith("pack:")) {
    const name = texRef.slice(5);
    const key = `pack:${keyPrefix}${name}`;
    if (!textureSources.has(key)) textureSources.set(key, { type: "pack", dir, name });
    return key;
  }
  if (texRef.startsWith("material:")) {
    const name = texRef.slice(9);
    const key = `material:${name}`;
    if (!textureSources.has(key)) textureSources.set(key, { type: "material", name });
    return key;
  }
  if (!textureSources.has(texRef)) textureSources.set(texRef, { type: "unknown" });
  return texRef;
}
