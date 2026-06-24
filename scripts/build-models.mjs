// Build pipeline: walk db/train/**, convert each proprietary XML+BMP model into
// a GLB (geometry + embedded texture atlas) plus a PNG thumbnail, and emit a
// manifest.json the web frontend consumes. Pure Node — no Python needed.
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { parseModel } from "./lib/geometry.mjs";
import { buildAtlas, detectMaterialPBR } from "./lib/atlas.mjs";
import { buildGLB } from "./lib/glb.mjs";
import { parseMetadata } from "./lib/metadata.mjs";
import { loadImageRGBA, encodePNG } from "./lib/images.mjs";

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");
const DB_TRAIN = path.join(ROOT, "db", "train");
const MATERIALS = path.join(ROOT, "materials");
const OUT = path.join(ROOT, "public");
const MODELS_OUT = path.join(OUT, "models");
const THUMBS_OUT = path.join(OUT, "thumbs");

function cleanDir(dir) {
  fs.rmSync(dir, { recursive: true, force: true });
  fs.mkdirSync(dir, { recursive: true });
}

// Recursively find every directory that contains model/index.xml.
function findModelDirs(base) {
  const result = [];
  const stack = [base];
  while (stack.length) {
    const dir = stack.pop();
    let entries;
    try {
      entries = fs.readdirSync(dir, { withFileTypes: true });
    } catch {
      continue;
    }
    if (fs.existsSync(path.join(dir, "model", "index.xml"))) {
      result.push(dir);
      // don't descend into a model's own subfolders (sub-models handled internally)
      continue;
    }
    for (const e of entries) {
      if (e.isDirectory()) stack.push(path.join(dir, e.name));
    }
  }
  return result.sort();
}

function slug(s) {
  return s.toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-+|-+$/g, "");
}

function stripDollar(s) {
  return s && s.startsWith("$") ? s.slice(1) : s;
}

function makeThumbnail(dir, outPath) {
  const candidates = ["image_4x.png", "image.bmp", "image.png"];
  for (const c of candidates) {
    const p = path.join(dir, c);
    if (fs.existsSync(p)) {
      try {
        fs.writeFileSync(outPath, encodePNG(loadImageRGBA(p)));
        return true;
      } catch {
        // try next candidate
      }
    }
  }
  return false;
}

function main() {
  if (!fs.existsSync(DB_TRAIN)) {
    console.error(`db/train not found at ${DB_TRAIN}`);
    process.exit(1);
  }
  cleanDir(MODELS_OUT);
  cleanDir(THUMBS_OUT);

  const dirs = findModelDirs(DB_TRAIN);
  console.log(`Found ${dirs.length} model directories under db/train`);

  const models = [];
  const usedIds = new Set();
  let built = 0;
  const skipped = [];

  for (const dir of dirs) {
    const rel = path.relative(DB_TRAIN, dir).split(path.sep);
    const modelName = rel[rel.length - 1];
    const categoryParts = rel.slice(0, -1);
    const category = categoryParts[0] || "other";
    const subcategory = categoryParts[1] || null;

    let id = slug(rel.join("-"));
    if (!id) id = slug(modelName) || `model-${models.length}`;
    while (usedIds.has(id)) id = `${id}-2`;
    usedIds.add(id);

    try {
      // Metadata (top-level index.xml is optional).
      let meta = { name: modelName, title: modelName, author: "", email: "", weight: null, power: null, maxspeed: null, path: "" };
      const indexXml = path.join(dir, "index.xml");
      if (fs.existsSync(indexXml)) {
        try {
          meta = { ...meta, ...clean(parseMetadata(indexXml)) };
        } catch { /* keep defaults */ }
      }

      // Geometry + atlas + GLB.
      const { polygons, textureSources } = parseModel(path.join(dir, "model"));
      if (polygons.length === 0) {
        skipped.push(`${id} (no polygons)`);
        continue;
      }
      const { pngBuffer, atlasMap, width: atlasWidth, height: atlasHeight } = buildAtlas(textureSources, MATERIALS);
      const glbAtlas = selectAtlasForGlb(dir, pngBuffer, atlasWidth, atlasHeight);
      const pbr = detectMaterialPBR(textureSources.keys());
      const glb = buildGLB(polygons, atlasMap, glbAtlas, pbr);
      if (!glb) {
        skipped.push(`${id} (no vertices)`);
        continue;
      }

      fs.writeFileSync(path.join(MODELS_OUT, `${id}.glb`), glb.buffer);
      const hasThumb = makeThumbnail(dir, path.join(THUMBS_OUT, `${id}.png`));

      models.push({
        id,
        name: stripDollar(meta.name) || modelName,
        title: stripDollar(meta.title) || stripDollar(meta.name) || modelName,
        category,
        subcategory,
        author: meta.author || "",
        email: meta.email || "",
        weight: meta.weight,
        power: meta.power,
        maxspeed: meta.maxspeed,
        length: glb.stats.length,
        bbox: glb.stats.bbox,
        vertices: glb.stats.vertices,
        faces: glb.stats.faces,
        glb: `models/${id}.glb`,
        thumb: hasThumb ? `thumbs/${id}.png` : null,
      });
      built++;
    } catch (e) {
      skipped.push(`${id} (${e.message})`);
    }
  }

  models.sort((a, b) => a.title.localeCompare(b.title));
  const manifest = { generatedAt: new Date().toISOString(), count: models.length, models };
  fs.writeFileSync(path.join(OUT, "manifest.json"), JSON.stringify(manifest));

  console.log(`\nBuilt ${built} models, skipped ${skipped.length}.`);
  if (skipped.length) {
    console.log("Skipped:");
    for (const s of skipped.slice(0, 40)) console.log("  - " + s);
    if (skipped.length > 40) console.log(`  ... and ${skipped.length - 40} more`);
  }
  console.log(`Manifest: ${path.relative(ROOT, path.join(OUT, "manifest.json"))} (${models.length} models)`);
}

function selectAtlasForGlb(dir, compactAtlas, compactWidth, compactHeight) {
  if (!compactAtlas.length) return compactAtlas;

  const atlas4xPath = path.join(dir, "atlas_4x.png");
  if (!fs.existsSync(atlas4xPath)) return compactAtlas;

  try {
    const image = loadImageRGBA(atlas4xPath);
    const scaleX = compactWidth > 0 ? image.width / compactWidth : 0;
    const scaleY = compactHeight > 0 ? image.height / compactHeight : 0;
    if (scaleX === scaleY && Number.isInteger(scaleX) && scaleX > 0) {
      return fs.readFileSync(atlas4xPath);
    }
    console.warn(
      `  [WARN] Ignoring atlas_4x.png for ${path.relative(DB_TRAIN, dir)}: ` +
      `${image.width}x${image.height} does not match compact atlas ${compactWidth}x${compactHeight}`
    );
  } catch (e) {
    console.warn(`  [WARN] Could not read atlas_4x.png for ${path.relative(DB_TRAIN, dir)}: ${e.message}`);
  }
  return compactAtlas;
}

function clean(obj) {
  const out = {};
  for (const [k, v] of Object.entries(obj)) if (v !== "" && v !== null && v !== undefined) out[k] = v;
  return out;
}

main();
