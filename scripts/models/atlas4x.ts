// Build a 4x atlas from the per-texture *_4x.png files (written next to each
// source texture by the remaster step), repacked onto the EXACT compact-atlas
// layout at 4x scale. The compact atlasMap (normalized UVs) is unchanged, so a 4x
// atlas whose tiles sit at 4x the compact pixel positions and are 4x the compact
// tile size maps onto the existing GLB UVs exactly.
import fs from "node:fs";
import path from "node:path";
import { resolveImage, type AtlasMapping } from "./atlas.ts";
import { solidImage, blit, upscaleNearest, encodePNG, type RgbaImage } from "./images.ts";
import { loadImageRGBA } from "./images.ts";
import type { TextureSource } from "./geometry.ts";

const SCALE = 4;

/** x_inc.bmp -> x_inc_4x.png (the AI-upscaled per-texture file). */
export function fourxName(name: string): string {
  return name.replace(/\.[^.]+$/, "") + "_4x.png";
}

export interface Atlas4xResult {
  pngBuffer: Buffer;
  hires: boolean;
  width: number;
  height: number;
}

export function buildAtlas4x(
  atlasMap: Map<string, AtlasMapping>, compactWidth: number, compactHeight: number,
  textureSources: Map<string, TextureSource>, materialsDir: string,
): Atlas4xResult {
  if (!compactWidth || !compactHeight) {
    return { pngBuffer: Buffer.alloc(0), hires: false, width: 0, height: 0 };
  }
  const W = compactWidth * SCALE;
  const H = compactHeight * SCALE;
  const canvas = solidImage(W, H, [0, 0, 0, 255]);
  let hires = false;

  for (const [key, mapping] of atlasMap) {
    const src = textureSources.get(key);
    if (!src) continue;
    const cx = Math.round(mapping[0] * compactWidth) * SCALE;
    const cy = Math.round(mapping[1] * compactHeight) * SCALE;

    let tile: RgbaImage | null = null;
    if (src.type === "pack") {
      const p = path.join(src.dir, fourxName(src.name));
      if (fs.existsSync(p)) {
        try {
          tile = loadImageRGBA(p);
          hires = true;
        } catch {
          tile = null;
        }
      }
    }
    // Fallback: nearest-upscale the resolved compact tile by 4 so the slot is filled.
    if (!tile) {
      try {
        tile = upscaleNearest(resolveImage(src, materialsDir), SCALE);
      } catch {
        continue;
      }
    }

    // Bounds guard — a *_4x.png that isn't exactly 4x its source must not overflow.
    if (cx + tile.width <= W && cy + tile.height <= H) {
      blit(canvas, tile, cx, cy);
    }
  }

  return { pngBuffer: encodePNG(canvas), hires, width: W, height: H };
}
