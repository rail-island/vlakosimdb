// Shelf-pack a model's source textures into one RGBA atlas. Model-level 4x atlas
// files are handled by atlas4x.ts as whole-atlas replacements.
import fs from "node:fs";
import path from "node:path";
import { loadImageRGBA, solidImage, blit, encodePNG, type RgbaImage } from "./images.ts";
import type { TextureSource } from "./geometry.ts";

// atlasMap value: [uOff, vOff, uScale, vScale] (normalized 0..1).
export type AtlasMapping = [number, number, number, number];
export interface AtlasResult {
  pngBuffer: Buffer;
  atlasMap: Map<string, AtlasMapping>;
  width: number;
  height: number;
}

// Built-in colors for "material:xxx.bmp" refs with no file.
const MATERIAL_COLORS: Record<string, number[]> = {
  black: [0, 0, 0, 255],
  white: [255, 255, 255, 255],
  grey: [128, 128, 128, 255],
  gray: [128, 128, 128, 255],
  red: [255, 0, 0, 255],
  green: [0, 128, 0, 255],
  blue: [0, 0, 255, 255],
  wall: [180, 150, 120, 255],
  roof: [100, 80, 60, 255],
  glass: [200, 220, 255, 200],
};

// metallic/roughness inferred from material name substrings.
const MATERIAL_PBR: Record<string, [number, number]> = {
  metal: [1.0, 0.35], chrome: [1.0, 0.1], steel: [1.0, 0.4], iron: [0.9, 0.5],
  copper: [1.0, 0.4], gold: [1.0, 0.3], silver: [1.0, 0.2],
  aluminum: [1.0, 0.3], aluminium: [1.0, 0.3],
};

const MAGENTA = [255, 0, 255, 255];

export function resolveImage(src: TextureSource, materialsDir: string): RgbaImage {
  try {
    if (src.type === "pack") {
      const p = path.join(src.dir, src.name);
      if (fs.existsSync(p)) return loadImageRGBA(p);
      return solidImage(16, 16, MAGENTA);
    }
    if (src.type === "material") {
      const p = path.join(materialsDir, src.name);
      if (fs.existsSync(p)) return loadImageRGBA(p);
      const upper = path.join(materialsDir, src.name.toUpperCase());
      if (fs.existsSync(upper)) return loadImageRGBA(upper);
      const matName = src.name.replace(/\.bmp$/i, "").toLowerCase();
      return solidImage(64, 64, MATERIAL_COLORS[matName] ?? [128, 128, 128, 255]);
    }
  } catch {
    // decode failed — fall through to placeholder
  }
  return solidImage(16, 16, MAGENTA);
}

/** Infer dominant PBR metallic/roughness from material refs. */
export function detectMaterialPBR(textureKeys: Iterable<string>): [number, number] {
  let metallicSum = 0, roughnessSum = 0, matched = 0;
  const keysArr = [...textureKeys];
  for (const key of keysArr) {
    if (key.startsWith("material:")) {
      const name = key.slice(9).replace(/\.bmp$/i, "").toLowerCase();
      for (const [pat, [mf, rf]] of Object.entries(MATERIAL_PBR)) {
        if (name.includes(pat)) { metallicSum += mf; roughnessSum += rf; matched++; break; }
      }
    }
  }
  if (matched === 0) return [0.0, 0.8];
  const packCount = keysArr.filter((k) => k.startsWith("pack:")).length;
  const total = matched + packCount;
  return [metallicSum / total, (roughnessSum + packCount * 0.8) / total];
}

interface Shelf {
  y: number;
  h: number;
  items: { key: string; x: number }[];
}

/** Build a combined atlas. */
export function buildAtlas(textureSources: Map<string, TextureSource>, materialsDir: string): AtlasResult {
  // Sort alphabetically first; the height sort below is stable, so equal-height
  // tiles break ties in this order — keeping packing deterministic.
  const keys = [...textureSources.keys()].sort();
  if (keys.length === 0) {
    return { pngBuffer: Buffer.alloc(0), atlasMap: new Map(), width: 0, height: 0 };
  }

  const images = new Map<string, RgbaImage>();
  for (const key of keys) images.set(key, resolveImage(textureSources.get(key)!, materialsDir));

  const padding = 1;
  const sortedKeys = keys.slice().sort((a, b) => images.get(b)!.height - images.get(a)!.height);
  const all = [...images.values()];
  const maxW = Math.max(...all.map((i) => i.width));
  const totalW = all.reduce((s, i) => s + i.width + padding, 0);
  const maxH = Math.max(...all.map((i) => i.height));
  const targetWidth = Math.max(maxW + padding, Math.floor(Math.sqrt(totalW * maxH)));

  const shelves: Shelf[] = [];
  for (const key of sortedKeys) {
    const img = images.get(key)!;
    let placed = false;
    for (const shelf of shelves) {
      const last = shelf.items[shelf.items.length - 1];
      const usedW = last ? last.x + images.get(last.key)!.width + padding : 0;
      if (usedW + img.width <= targetWidth && img.height <= shelf.h) {
        shelf.items.push({ key, x: usedW });
        placed = true;
        break;
      }
    }
    if (!placed) {
      const last = shelves[shelves.length - 1];
      const y = last ? last.y + last.h + padding : 0;
      shelves.push({ y, h: img.height, items: [{ key, x: 0 }] });
    }
  }

  const atlasW = Math.max(
    ...shelves.map((s) => {
      const last = s.items[s.items.length - 1]!;
      return last.x + images.get(last.key)!.width;
    }),
  );
  const lastShelf = shelves[shelves.length - 1]!;
  const atlasH = lastShelf.y + lastShelf.h;

  const atlas = solidImage(atlasW, atlasH, [0, 0, 0, 0]);
  const atlasMap = new Map<string, AtlasMapping>();
  for (const shelf of shelves) {
    for (const { key, x } of shelf.items) {
      const img = images.get(key)!;
      blit(atlas, img, x, shelf.y);
      atlasMap.set(key, [x / atlasW, shelf.y / atlasH, img.width / atlasW, img.height / atlasH]);
    }
  }

  return { pngBuffer: encodePNG(atlas), atlasMap, width: atlasW, height: atlasH };
}
