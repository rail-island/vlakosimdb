// Build one model's GLB (geometry + embedded texture atlas). Shared by the web
// build (build-models.mjs), the Roblox uploader (upload-roblox.mjs) and the dev
// upscale plugin so they always produce the same GLB.
import path from "node:path";
import { parseModel } from "./geometry.mjs";
import { buildAtlas, detectMaterialPBR } from "./atlas.mjs";
import { buildAtlas4x } from "./atlas4x.mjs";
import { buildGLB } from "./glb.mjs";

/**
 * @param {string} dir          model folder (contains model/index.xml)
 * @param {string} materialsDir
 * @returns {{ glb, hires, atlas4xPng, atlasMap }|null}  glb = { buffer, stats }
 */
export function buildModelGlb(dir, materialsDir) {
  const { polygons, textureSources } = parseModel(path.join(dir, "model"), { includeInlines: true });
  if (polygons.length === 0) return null;

  const { pngBuffer, atlasMap, width, height } = buildAtlas(textureSources, materialsDir);
  // 4x atlas built in-memory from the per-texture model/*_4x.png files (source of
  // truth); falls back to the compact atlas when no *_4x.png exist.
  const { pngBuffer: atlas4xPng, hires } = buildAtlas4x(atlasMap, width, height, textureSources, materialsDir);
  const glbAtlas = hires ? atlas4xPng : pngBuffer;

  const pbr = detectMaterialPBR(textureSources.keys());
  const glb = buildGLB(polygons, atlasMap, glbAtlas, pbr);
  if (!glb) return null;

  return { glb, hires, atlas4xPng, atlasMap };
}
