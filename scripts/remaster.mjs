#!/usr/bin/env node
// Remaster one model's textures 4x (gpt-image-2). Writes per-texture
// model/<name>_4x.png files next to the originals plus debug artifacts under
// generated/<slug>/. atlas_4x is derived separately (scripts/lib/atlas4x.mjs).
//
//   node scripts/remaster.mjs <model-folder|name> [--dry-run] [--quality high] [--pad 16]
//
// <model-folder|name> may be a repo path (db/train/loco/electric/cd_163) or a bare
// model name (cd_163) resolved under db/train/.
import fs from "node:fs";
import path from "node:path";
import { loadEnv, repoRoot } from "./lib/env.mjs";
import { remasterModel } from "./lib/remaster.mjs";

const ROOT = repoRoot();
const MATERIALS = path.join(ROOT, "materials");
const DB_SEARCH_ROOT = path.join(ROOT, "db", "train");

function isModelDir(dir) {
  return fs.existsSync(path.join(dir, "model", "index.xml"));
}

// Resolve a model NAME (cd_163) or repo path to its directory.
function resolveModel(nameOrPath) {
  const direct = path.resolve(ROOT, nameOrPath);
  if (isModelDir(direct)) return direct;

  const matches = [];
  const stack = [DB_SEARCH_ROOT];
  while (stack.length) {
    const dir = stack.pop();
    let entries;
    try { entries = fs.readdirSync(dir, { withFileTypes: true }); } catch { continue; }
    for (const e of entries) {
      if (!e.isDirectory()) continue;
      const child = path.join(dir, e.name);
      if (e.name === nameOrPath && isModelDir(child)) matches.push(child);
      stack.push(child);
    }
  }
  if (matches.length === 0) {
    console.error(`Error: no model named '${nameOrPath}' found under db/train/ (needs model/index.xml).`);
    process.exit(2);
  }
  if (matches.length > 1) {
    console.error(`Ambiguous name '${nameOrPath}' — ${matches.length} matches:`);
    for (const m of matches) console.error(`  ${path.relative(ROOT, m)}`);
    process.exit(2);
  }
  return matches[0];
}

function parseArgs(argv) {
  const args = { dryRun: false, quality: "high", pad: 16, model: null };
  for (let i = 0; i < argv.length; i++) {
    const a = argv[i];
    if (a === "--dry-run") args.dryRun = true;
    else if (a === "--quality") args.quality = argv[++i];
    else if (a === "--pad") args.pad = Number(argv[++i]);
    else if (!a.startsWith("--") && !args.model) args.model = a;
  }
  return args;
}

async function main() {
  loadEnv();
  const args = parseArgs(process.argv.slice(2));
  if (!args.model) {
    console.error("Usage: node scripts/remaster.mjs <model-folder|name> [--dry-run] [--quality high] [--pad 16]");
    process.exit(1);
  }
  const modelDir = resolveModel(args.model);
  console.log(`Model: ${path.relative(ROOT, modelDir)}`);

  const result = await remasterModel(modelDir, {
    repoRoot: ROOT,
    materialsDir: MATERIALS,
    quality: args.quality,
    pad: args.pad,
    dryRun: args.dryRun,
    onLog: (m) => console.log(`  ${m}`),
  });

  console.log(`\nDone. ${result.tiles.length} textures -> model/*_4x.png` +
    (args.dryRun ? " (dry-run)" : ` | cost ~$${result.cost_usd.toFixed(4)}`));
  console.log(`Debug: ${path.relative(ROOT, result.outDir)}/`);
}

main().catch((e) => { console.error(e.stack || String(e)); process.exit(1); });
