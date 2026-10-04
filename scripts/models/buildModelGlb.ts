// Build one model's GLB (geometry + embedded texture atlas).
import fs from "node:fs";
import path from "node:path";
import { parseModel } from "./geometry.ts";
import { buildAtlas, detectMaterialPBR, type AtlasMapping } from "./atlas.ts";
import { buildAtlas4x } from "./atlas4x.ts";
import { buildGLB, type GlbResult } from "./glb.ts";
import { parseMetadata } from "./metadata.ts";
import { planArticulation, type Articulation, type ModelPart } from "./articulate.ts";

export interface BuiltModel {
  glb: GlbResult;
  hires: boolean;
  atlas4xPng: Buffer;
  atlasMap: Map<string, AtlasMapping>;
  /** Set when the model was built as several bodies rather than one solid. */
  articulation: Articulation | null;
}

/**
 * @param dir          model folder (contains model/index.xml)
 * @param materialsDir directory of shared material textures (may not exist)
 * @returns null if the model has no geometry
 */
export function buildModelGlb(dir: string, materialsDir: string): BuiltModel | null {
  const modelDir = path.join(dir, "model");
  const warn = (msg: string): void => console.warn(`  ${path.basename(dir)}: ${msg}`);
  const { polygons, textureSources } = parseModel(modelDir, { includeInlines: true, onWarn: warn });
  if (polygons.length === 0) return null;

  // Articulated vehicles (style="bus2c"/"tram2c"/"tram3c") split into sections
  // plus a synthesized bellows per joint, and a coupled tram pair ("tram2v")
  // duplicates its one body; everything else stays one rigid part. A declared
  // style whose geometry does not match falls back to solid, like jt7's
  // jtMotocar::load.
  const indexXml = path.join(dir, "index.xml");
  const meta = fs.existsSync(indexXml) ? parseMetadata(indexXml) : null;
  const plan = meta ? planArticulation(polygons, meta, warn) : null;
  const parts: ModelPart[] = plan ? plan.parts : [{ name: null, polygons }];
  // The bellows reuses the body's own side/roof textures, but register them
  // anyway so a model that paints its joint with an otherwise unused bitmap
  // still gets it into the atlas.
  for (const name of plan?.textureFiles ?? []) {
    const key = `pack:${name}`;
    if (!textureSources.has(key)) textureSources.set(key, { type: "pack", dir: modelDir, name });
  }

  const { pngBuffer, atlasMap, width, height } = buildAtlas(textureSources, materialsDir);
  // 4x atlas built in-memory from the per-texture model/*_4x.png files; falls back
  // to the compact atlas when no *_4x.png exist.
  const { pngBuffer: atlas4xPng, hires } = buildAtlas4x(atlasMap, width, height, textureSources, materialsDir);
  const glbAtlas = hires ? atlas4xPng : pngBuffer;

  const pbr = detectMaterialPBR(textureSources.keys());
  const glb = buildGLB(parts, atlasMap, glbAtlas, pbr);
  if (!glb) return null;

  return { glb, hires, atlas4xPng, atlasMap, articulation: plan?.articulation ?? null };
}
