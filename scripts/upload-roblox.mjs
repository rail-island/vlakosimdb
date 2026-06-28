#!/usr/bin/env node
// Upload locomotive assets to Roblox and emit loco_data.lua (Node port of
// upload_loco.py). Reuses the shared model builder, so the mesh embeds the same
// 4x atlas (built from model/*_4x.png) as the web build.
//
//   node scripts/upload-roblox.mjs [--dry-run] [--hires-only] [--input INPUT_LOCO] [--output loco_data.lua]
//
// --hires-only uploads only hi-res models (those with model/*_4x.png remastered
// textures). Non-hires models are left untouched: their existing thumbnailAssetId
// and modelAssetId are read back from the current loco_data.lua and kept.
//
// Env (.env or environment): ROBLOX_UPLOAD_ASSETS_KEY (falls back to
// ROBLOX_API_KEY) and ROBLOX_CREATOR_ID. Flags --api-key / --creator-id override.
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import { loadEnv, repoRoot } from "./lib/env.mjs";
import { buildModelGlb } from "./lib/model-build.mjs";
import { parseMetadata } from "./lib/metadata.mjs";
import { loadImageRGBA, encodePNG } from "./lib/images.mjs";
import {
  loadLockfile, saveLockfile, fileContentHash, bufferHash, makeLockKey,
  uploadMeshToRoblox, uploadImageToRoblox,
} from "./lib/roblox.mjs";

const ROOT = repoRoot();
const MATERIALS = path.join(ROOT, "materials");
const INPUT_FILE = "INPUT_LOCO";
const OUTPUT_LUA = "loco_data.lua";

// ---------------------------------------------------------------------------
// INPUT_LOCO resolution (port of resolve_input_dirs)
// ---------------------------------------------------------------------------

function isDir(p) { try { return fs.statSync(p).isDirectory(); } catch { return false; } }
function hasIndex(p) { return fs.existsSync(path.join(p, "index.xml")); }
function subdirs(p) {
  return fs.readdirSync(p, { withFileTypes: true })
    .filter((e) => e.isDirectory()).map((e) => path.join(p, e.name)).sort();
}

function resolveInputDirs(inputFile) {
  if (!fs.existsSync(inputFile)) {
    console.error(`Error: ${inputFile} not found`);
    process.exit(1);
  }
  const dirs = [];
  const skip = new Set();
  for (const rawLine of fs.readFileSync(inputFile, "utf-8").split(/\r?\n/)) {
    let line = rawLine.trim();
    if (!line || line.startsWith("#")) continue;
    if (line.toUpperCase().startsWith("SKIP:")) {
      skip.add(path.resolve(ROOT, line.slice(5)));
      continue;
    }
    let override = null;
    if (line.startsWith("!")) {
      const idx = line.indexOf(":");
      override = line.slice(1, idx);
      line = line.slice(idx + 1);
    }
    if (line.endsWith("/*/*")) {
      const parent = path.resolve(ROOT, line.slice(0, -4));
      if (!isDir(parent)) { console.warn(`  [WARN] Directory not found: ${parent}`); continue; }
      for (const child of subdirs(parent))
        for (const gc of subdirs(child))
          if (hasIndex(gc) && !skip.has(path.resolve(gc))) dirs.push([override, gc]);
    } else if (line.endsWith("/*")) {
      const parent = path.resolve(ROOT, line.slice(0, -2));
      if (!isDir(parent)) { console.warn(`  [WARN] Directory not found: ${parent}`); continue; }
      for (const child of subdirs(parent))
        if (hasIndex(child) && !skip.has(path.resolve(child))) dirs.push([override, child]);
    } else {
      const d = path.resolve(ROOT, line);
      if (isDir(d)) { if (!skip.has(d)) dirs.push([override, d]); }
      else console.warn(`  [WARN] Directory not found: ${d}`);
    }
  }
  return dirs;
}

// ---------------------------------------------------------------------------
// Per-loco processing
// ---------------------------------------------------------------------------

function thumbnailPngBytes(locoDir) {
  const fourx = path.join(locoDir, "image_4x.png");
  const bmp = path.join(locoDir, "image.bmp");
  const src = fs.existsSync(fourx) ? fourx : (fs.existsSync(bmp) ? bmp : null);
  if (!src) return null;
  // Roblox needs PNG; image_4x.png is already PNG, image.bmp gets converted.
  return { name: "thumbnail.png", bytes: encodePNG(loadImageRGBA(src)), src };
}

async function processLoco(locoDir, lockdata, apiKey, creatorId, dryRun, opts = {}) {
  const { hiresOnly = false, existingAssets = {} } = opts;
  const indexXml = path.join(locoDir, "index.xml");
  const rel = path.relative(ROOT, locoDir);
  const meta = parseMetadata(indexXml);
  console.log(`  Name: ${meta.name}  Title: ${meta.title}`);

  const built = buildModelGlb(locoDir, MATERIALS);
  let length = 0;
  let glbBytes = null;
  let atlas4xPng = null;
  let hires = false;
  if (built) {
    glbBytes = built.glb.buffer;
    length = built.glb.stats.length;
    atlas4xPng = built.atlas4xPng;
    hires = built.hires;
    fs.writeFileSync(path.join(locoDir, "model.glb"), glbBytes);
  }

  // In hires-only mode, fall back to the previously uploaded asset IDs (read back
  // from the existing loco_data.lua) so non-hires models — and hires models on a
  // dry run, where nothing is actually uploaded — keep their IDs in the DB.
  const prev = hiresOnly ? (existingAssets[luaStr(meta.name || "unknown")] || {}) : {};
  if (hiresOnly && !hires) {
    console.log("  [SKIP] not a hi-res model — keeping existing asset IDs");
    return {
      metadata: meta,
      thumbnailAssetId: prev.thumbnailAssetId || "PENDING_UPLOAD",
      modelAssetId: prev.modelAssetId || "PENDING_UPLOAD",
      length,
      hires,
    };
  }

  let thumbnailAssetId = prev.thumbnailAssetId || "PENDING_UPLOAD";
  let modelAssetId = prev.modelAssetId || "PENDING_UPLOAD";

  if (!dryRun) {
    // Thumbnail
    const thumb = thumbnailPngBytes(locoDir);
    if (thumb) {
      const key = makeLockKey(`${rel}/${path.basename(thumb.src)}`, fileContentHash(thumb.src));
      if (lockdata[key]) {
        thumbnailAssetId = lockdata[key];
      } else {
        const id = await uploadImageToRoblox(
          thumb.bytes, thumb.name, apiKey, creatorId,
          `${meta.title || "Loco"} Thumbnail`,
          `Thumbnail for ${meta.title || ""} by ${meta.author || ""}`,
        );
        if (id) { thumbnailAssetId = id; lockdata[key] = id; saveLockfile(lockdata); }
      }
    }
    // Mesh — key on the model XML + the embedded 4x atlas bytes, so regenerating
    // textures re-uploads the mesh.
    if (glbBytes) {
      const modelHash = fileContentHash(path.join(locoDir, "model", "index.xml"));
      const atlasHash = atlas4xPng && atlas4xPng.length ? bufferHash(atlas4xPng) : "noatlas";
      const combined = crypto.createHash("sha256").update(modelHash + atlasHash).digest("hex");
      const key = makeLockKey(`${rel}/model.glb`, combined);
      if (lockdata[key]) {
        modelAssetId = lockdata[key];
      } else {
        const id = await uploadMeshToRoblox(
          glbBytes, "model.glb", apiKey, creatorId,
          `${meta.title || "Loco"} Mesh`,
          `3D mesh for ${meta.title || ""} by ${meta.author || ""}`,
        );
        if (id) { modelAssetId = id; lockdata[key] = id; saveLockfile(lockdata); }
      }
    }
  }

  return { metadata: meta, thumbnailAssetId, modelAssetId, length, hires };
}

// ---------------------------------------------------------------------------
// loco_data.lua (port of generate_combined_lua)
// ---------------------------------------------------------------------------

function luaStr(v) { return String(v ?? "").replace(/\\/g, "\\\\").replace(/"/g, '\\"'); }
function luaInt(v) { const n = parseInt(v, 10); return Number.isFinite(n) ? n : 0; }

// Read back thumbnailAssetId / modelAssetId from an existing loco_data.lua,
// keyed by the (lua-escaped) loco name so processLoco can preserve them.
function parseExistingAssets(luaPath) {
  const map = {};
  if (!fs.existsSync(luaPath)) return map;
  const text = fs.readFileSync(luaPath, "utf-8");
  const entry = /\["((?:[^"\\]|\\.)*)"\]\s*=\s*\{([\s\S]*?)\n\t\},/g;
  let m;
  while ((m = entry.exec(text))) {
    const body = m[2];
    const t = /thumbnailAssetId\s*=\s*"((?:[^"\\]|\\.)*)"/.exec(body);
    const md = /modelAssetId\s*=\s*"((?:[^"\\]|\\.)*)"/.exec(body);
    map[m[1]] = { thumbnailAssetId: t ? t[1] : null, modelAssetId: md ? md[1] : null };
  }
  return map;
}

function generateLua(allLocos, outputPath, noun = "locomotive") {
  const lines = [
    `-- Auto-generated ${noun} database`,
    `-- ${allLocos.length} ${noun}(s)`,
    "",
    "local LocoDatabase = {",
  ];
  for (const loco of allLocos) {
    const m = loco.metadata;
    lines.push(`\t["${luaStr(m.name || "unknown")}"] = {`);
    lines.push(`\t\tname = "${luaStr(m.name)}",`);
    lines.push(`\t\ttitle = "${luaStr(m.title)}",`);
    lines.push(`\t\tauthor = "${luaStr(m.author)}",`);
    lines.push(`\t\temail = "${luaStr(m.email)}",`);
    for (const f of ["maxspeed", "weight", "power"]) lines.push(`\t\t${f} = ${luaInt(m[f])},`);
    lines.push(`\t\tpath = "${luaStr(m.path)}",`);
    lines.push(`\t\tlength = ${loco.length || 0},`);
    lines.push(`\t\tthumbnailAssetId = "${luaStr(loco.thumbnailAssetId)}",`);
    lines.push(`\t\tmodelAssetId = "${luaStr(loco.modelAssetId)}",`);
    lines.push(`\t\thiresTextures = ${loco.hires ? "true" : "false"},`);
    lines.push("\t},");
  }
  lines.push("}", "", "return LocoDatabase", "");
  fs.writeFileSync(outputPath, lines.join("\n"));
  console.log(`\nLua file written to: ${outputPath}`);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

function parseArgs(argv) {
  const a = { dryRun: false, hiresOnly: false, input: null, output: null, apiKey: null, creatorId: null };
  for (let i = 0; i < argv.length; i++) {
    const x = argv[i];
    if (x === "--dry-run") a.dryRun = true;
    else if (x === "--hires-only") a.hiresOnly = true;
    else if (x === "--input") a.input = argv[++i];
    else if (x === "--output") a.output = argv[++i];
    else if (x === "--api-key") a.apiKey = argv[++i];
    else if (x === "--creator-id") a.creatorId = argv[++i];
  }
  return a;
}

async function main() {
  loadEnv();
  const args = parseArgs(process.argv.slice(2));
  const apiKey = args.apiKey || process.env.ROBLOX_UPLOAD_ASSETS_KEY || process.env.ROBLOX_API_KEY;
  const creatorId = args.creatorId || process.env.ROBLOX_CREATOR_ID;
  if (!args.dryRun && (!apiKey || !creatorId)) {
    console.error("Error: --api-key and --creator-id are required (or use --dry-run)");
    process.exit(1);
  }

  const inputFile = path.resolve(ROOT, args.input || INPUT_FILE);
  console.log(`Reading input from: ${inputFile}`);
  const locoDirs = resolveInputDirs(inputFile);
  if (!locoDirs.length) { console.error("No locomotive directories found."); process.exit(1); }
  console.log(`Found ${locoDirs.length} locomotive(s) to process.\n`);

  const outputPath = path.resolve(ROOT, args.output || OUTPUT_LUA);
  const existingAssets = args.hiresOnly ? parseExistingAssets(outputPath) : {};
  if (args.hiresOnly) console.log("Hi-res only: non-hires models keep their existing asset IDs.\n");

  const lockdata = loadLockfile();
  const allLocos = [];
  for (let i = 0; i < locoDirs.length; i++) {
    const [override, locoDir] = locoDirs[i];
    console.log(`[${i + 1}/${locoDirs.length}] Processing ${path.relative(ROOT, locoDir)}…`);
    if (!fs.existsSync(path.join(locoDir, "index.xml"))) { console.log("  [SKIP] No index.xml"); continue; }
    const result = await processLoco(locoDir, lockdata, apiKey, creatorId, args.dryRun, {
      hiresOnly: args.hiresOnly, existingAssets,
    });
    if (override !== null) result.metadata.path = override;
    allLocos.push(result);
  }

  const noun = path.basename(outputPath).startsWith("moto") ? "motocar" : "locomotive";
  generateLua(allLocos, outputPath, noun);
  console.log(`\nDone! Processed ${allLocos.length} ${noun}(s).`);
}

main().catch((e) => { console.error(e.stack || String(e)); process.exit(1); });
