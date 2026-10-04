// Parse a model/index.xml <model> section into intermediate polygon soup,
// resolving <inlines>/<inline> sub-models (pantographs etc.) with their
// scale/translation transforms.
import fs from "node:fs";
import path from "node:path";
import { parseXmlSafe, asArray, attrNum, attrStr, type XmlNode } from "./xml.ts";
import { generateModel } from "./simplemodel/generate.ts";

export interface ModelVertex {
  x: number;
  y: number;
  z: number;
  s: number;
  t: number;
}
export interface ModelPolygon {
  textureKey: string | null;
  normal: [number, number, number];
  color: [number, number, number];
  transparent: boolean;
  verts: ModelVertex[];
}
export type TextureSource =
  | { type: "pack"; dir: string; name: string }
  | { type: "material"; name: string }
  | { type: "unknown" };
export interface ParsedModel {
  polygons: ModelPolygon[];
  textureSources: Map<string, TextureSource>;
}

interface ParseOptions {
  includeInlines?: boolean;
  /** Reports sub-models the walk could not resolve. Called once per model. */
  onWarn?: (message: string) => void;
  /** Internal: names already reported as unresolved, so each is warned once. */
  missing?: Set<string>;
}

/**
 * Parse a model directory into intermediate polygons + texture sources.
 * `modelDir` contains index.xml. Pass `includeInlines: true` to merge nested
 * <inline name="..."> sub-models with their transforms applied.
 */
export function parseModel(modelDir: string, options: ParseOptions = {}): ParsedModel {
  const polygons: ModelPolygon[] = [];
  const textureSources = new Map<string, TextureSource>();
  const missing = new Set<string>();
  walk(modelDir, "", [1, 1, 1], [0, 0, 0], polygons, textureSources, 0, { ...options, missing });
  // Sub-models named with a leading slash ("/pant", "/pantdbl") are jt7 GLOBAL
  // objects that live outside db/ -- every tram's pantograph is one of them, as
  // is the electric db/train stock's. They resolve to nothing here and their
  // geometry is simply absent; say so rather than dropping them silently.
  if (missing.size) {
    options.onWarn?.(`unresolved <inline> sub-model(s): ${[...missing].sort().join(", ")}`);
  }
  return { polygons, textureSources };
}

/** Read a model's committed `options.json` (per-side texture flips etc.). */
export function readOptions(modelDir: string): { flipS?: Record<string, boolean> } {
  const p = path.join(modelDir, "options.json");
  if (!fs.existsSync(p)) return {};
  try {
    return JSON.parse(fs.readFileSync(p, "utf8"));
  } catch {
    return {};
  }
}

function walk(
  dir: string, keyPrefix: string, scale: [number, number, number], translate: [number, number, number],
  polygons: ModelPolygon[], textureSources: Map<string, TextureSource>, depth: number, options: ParseOptions,
): void {
  if (depth > 8) return; // guard against pathological recursion
  const xmlPath = path.join(dir, "index.xml");
  if (!fs.existsSync(xmlPath)) return;

  let root: XmlNode;
  try {
    root = parseXmlSafe(xmlPath);
  } catch {
    return;
  }
  const sm = (root.simplemodel as XmlNode | undefined) ?? root;
  const flipS = readOptions(dir).flipS ?? {};
  // Regenerate <model> from the source description when present; fall back to the
  // pre-baked <model> for baked-only models.
  const modelEl = generateModel(sm, { flipS }) ?? (sm.model as XmlNode | undefined);

  const [sx, sy, sz] = scale;
  const flip = sx * sy * sz < 0; // negative determinant flips winding

  if (modelEl) {
    for (const polygon of asArray((modelEl as XmlNode).polygon as XmlNode | XmlNode[] | undefined)) {
      const texRef = attrStr(polygon, "texture");
      const textureKey = registerTexture(texRef, dir, keyPrefix, textureSources);

      let verts: ModelVertex[] = asArray(polygon.vertex as XmlNode | XmlNode[] | undefined).map((v) => ({
        x: attrNum(v, "x") * sx + translate[0],
        y: attrNum(v, "y") * sy + translate[1],
        z: attrNum(v, "z") * sz + translate[2],
        s: attrNum(v, "s"),
        t: attrNum(v, "t"),
      }));
      if (verts.length < 3) continue;
      if (flip) verts = verts.slice().reverse();

      const tnx = attrNum(polygon, "nx") * Math.sign(sx || 1);
      const tny = attrNum(polygon, "ny") * Math.sign(sy || 1);
      const tnz = attrNum(polygon, "nz") * Math.sign(sz || 1);
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

  if (!options.includeInlines) return;

  // Recurse into sub-models.
  const inlines = (sm.inlines as XmlNode | undefined)?.inline as XmlNode | XmlNode[] | undefined;
  for (const inl of asArray(inlines)) {
    const name = attrStr(inl, "name");
    if (!name) continue;
    const sxc = attrNum(inl, "sx", 1);
    const syc = attrNum(inl, "sy", 1);
    const szc = attrNum(inl, "sz", 1);
    const txc = attrNum(inl, "tx", 0);
    const tyc = attrNum(inl, "ty", 0);
    const tzc = attrNum(inl, "tz", 0);
    const childScale: [number, number, number] = [sx * sxc, sy * syc, sz * szc];
    const childTranslate: [number, number, number] = [
      sx * txc + translate[0], sy * tyc + translate[1], sz * tzc + translate[2],
    ];
    if (!fs.existsSync(path.join(dir, name, "index.xml"))) {
      options.missing?.add(name);
      continue;
    }
    walk(
      path.join(dir, name),
      keyPrefix ? `${keyPrefix}${name}/` : `${name}/`,
      childScale,
      childTranslate,
      polygons,
      textureSources,
      depth + 1,
      options,
    );
  }
}

function registerTexture(
  texRef: string, dir: string, keyPrefix: string, textureSources: Map<string, TextureSource>,
): string | null {
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
