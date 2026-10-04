// The model export: walk db/motocar (buses), db/train (trains) and db/tram (trams),
// convert every XML+BMP model into a GLB (geometry + embedded texture atlas),
// render its side thumbnail, and write one folder per tree:
//
//   export/<tree>/glb/<id>.glb          the raw, unencoded GLB
//   export/<tree>/thumbs/<id>.webp      the rendered side view (a .png drawn by
//                                       hand only when the render fails)
//   export/<tree>/<tree>.meta.json      one row per vehicle: everything the GLB
//                                       itself cannot state (name, author,
//                                       physics, articulation, smoke anchor...)
//
// That is the whole contract with Rail Island: its `pnpm import:vehicles` runs this
// and mirrors export/<tree> into its own db/<tree>. Everything that depends on the
// game's runtime (LOD proxies, meshopt encoding, hashed file names, the archive
// list, the manifest) happens on the Rail Island side, from these files.
//
// Ids are the game's save-file keys, so they must never change: a category prefix
// ("moto" / "train" / "tram") plus the model's path below its tree, slugged, with
// one id namespace across all three trees. Pure Node -- run with `npm run export`.
import fs from "node:fs";
import path from "node:path";
import { execFileSync } from "node:child_process";
import { fileURLToPath } from "node:url";
import sharp from "sharp";
import { buildModelGlb } from "./buildModelGlb.ts";
import { parseMetadata, type ModelMetadata } from "./metadata.ts";
import type { Articulation } from "./articulate.ts";
import { readOptions } from "./geometry.ts";
import { loadImageRGBA, encodePNG } from "./images.ts";
import { renderSideThumb } from "./renderSideThumb.ts";
import { THUMB_WEBP_QUALITY } from "./thumbView.ts";

const HERE = path.dirname(fileURLToPath(import.meta.url));
const REPO_ROOT = path.resolve(HERE, "..", "..");
// No material bitmaps ship with the database: a `material:` texture reference
// resolves to its built-in colour (see atlas.ts resolveImage).
const MATERIALS = path.join(REPO_ROOT, "materials");

/** The three trees, in the order their ids are allocated. */
export const TREES = [
  { tree: "motocar", category: "moto" },
  { tree: "train", category: "train" },
  { tree: "tram", category: "tram" },
] as const;

/** One row of <tree>.meta.json. */
export interface ExportedVehicle {
  id: string;
  /** File name in glb/. */
  glb: string;
  /** File name in thumbs/, or null when neither a render nor a drawing exists. */
  thumb: string | null;
  name: string;
  title: string;
  // First path level below the tree (loco, coach, emu, ...); null for a model
  // sitting directly in it (every bus and tram).
  subcategory: string | null;
  // Locomotive traction (diesel/electric/steam/muv), from the folder below `loco`;
  // dmu -> diesel, emu -> electric; null for everything else.
  traction: string | null;
  author: string;
  email: string;
  weight: number | null;
  power: number | null;
  maxspeed: number | null;
  /** Native model units, measured on the GLB as written. */
  length: number;
  bbox: { min: number[]; max: number[] };
  vertices: number;
  faces: number;
  /** True when the atlas was built from the AI-remastered *_4x.png textures. */
  hires: boolean;
  /** The model's folder in this repository, e.g. "db/train/loco/diesel/t478". */
  source: string;
  /** Texture-side flips from model/options.json. */
  flip: Record<string, boolean>;
  articulation?: Articulation;
  smoke?: { x: number; y: number };
}

export interface TreeMeta {
  _comment: string[];
  category: string;
  vehicles: ExportedVehicle[];
}

function slug(s: string): string {
  return s.toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-+|-+$/g, "");
}

/** The vehicle id: category plus the model's path below its tree, slugged. */
export function modelId(base: string, dir: string, category: string): string {
  const rel = path.relative(base, dir).split(path.sep);
  return slug([category, ...rel].join("-")) || slug(rel[rel.length - 1] || "") || "model";
}

/** Every directory under `base` that holds a model/index.xml, sorted. */
export function findModelDirs(base: string): string[] {
  const result: string[] = [];
  const stack = [base];
  while (stack.length) {
    const dir = stack.pop()!;
    let entries: fs.Dirent[];
    try {
      entries = fs.readdirSync(dir, { withFileTypes: true });
    } catch {
      continue;
    }
    if (fs.existsSync(path.join(dir, "model", "index.xml"))) {
      result.push(dir);
      continue; // don't descend into a model's own subfolders
    }
    for (const e of entries) {
      if (e.isDirectory()) stack.push(path.join(dir, e.name));
    }
  }
  return result.sort();
}

function stripDollar(s: string): string {
  return s && s.startsWith("$") ? s.slice(1) : s;
}

function cleanMeta(obj: ModelMetadata): Partial<ModelMetadata> {
  const out: Record<string, unknown> = {};
  for (const [k, v] of Object.entries(obj)) if (v !== "" && v !== null && v !== undefined) out[k] = v;
  return out as Partial<ModelMetadata>;
}

/** The side view drawn by hand that ships beside the model, as a PNG. */
function drawnThumbnail(dir: string): Buffer | null {
  for (const c of ["image_4x.png", "image.bmp", "image.png"]) {
    const p = path.join(dir, c);
    if (!fs.existsSync(p)) continue;
    try {
      return encodePNG(loadImageRGBA(p));
    } catch {
      // try the next candidate
    }
  }
  return null;
}

/**
 * The thumbnail: rendered from the GLB just written (renderSideThumb.ts), so a
 * shipped vehicle and one uploaded in the game look alike. Only a model the
 * renderer cannot draw falls back to its hand-drawn side view.
 */
async function writeThumbnail(id: string, dir: string, glb: Buffer, thumbsOut: string): Promise<string | null> {
  try {
    const thumb = renderSideThumb(new Uint8Array(glb.buffer, glb.byteOffset, glb.byteLength));
    const file = `${id}.webp`;
    await sharp(Buffer.from(thumb.data), { raw: { width: thumb.width, height: thumb.height, channels: 4 } })
      .webp({ quality: Math.round(THUMB_WEBP_QUALITY * 100) })
      .toFile(path.join(thumbsOut, file));
    return file;
  } catch (e) {
    console.warn(`  ! ${id}: thumbnail render failed (${(e as Error).message}), using the drawn image`);
    const png = drawnThumbnail(dir);
    if (!png) return null;
    const file = `${id}.png`;
    fs.writeFileSync(path.join(thumbsOut, file), png);
    return file;
  }
}

async function exportOne(
  dir: string, id: string, base: string, outDir: string,
): Promise<ExportedVehicle | null> {
  const rel = path.relative(base, dir).split(path.sep);
  const modelName = rel[rel.length - 1]!;
  const categoryParts = rel.slice(0, -1);
  const subcategory = categoryParts[0] ?? null;
  const traction =
    subcategory === "loco" ? categoryParts[1] ?? null
    : subcategory === "dmu" ? "diesel"
    : subcategory === "emu" ? "electric"
    : null;

  let meta: ModelMetadata = {
    name: modelName, title: modelName, author: "", email: "",
    weight: null, power: null, maxspeed: null, path: "",
    style: "", bellows: null, tramspace: null, smoke: null,
  };
  const indexXml = path.join(dir, "index.xml");
  if (fs.existsSync(indexXml)) {
    try {
      meta = { ...meta, ...cleanMeta(parseMetadata(indexXml)) };
    } catch { /* keep defaults */ }
  }

  const result = buildModelGlb(dir, MATERIALS);
  if (!result) return null;
  const { glb, hires } = result;

  const glbFile = `${id}.glb`;
  fs.writeFileSync(path.join(outDir, "glb", glbFile), glb.buffer);
  const thumb = await writeThumbnail(id, dir, glb.buffer, path.join(outDir, "thumbs"));

  return {
    id,
    glb: glbFile,
    thumb,
    name: stripDollar(meta.name) || modelName,
    title: stripDollar(meta.title) || stripDollar(meta.name) || modelName,
    subcategory,
    traction,
    author: meta.author || "",
    email: meta.email || "",
    weight: meta.weight,
    power: meta.power,
    maxspeed: meta.maxspeed,
    length: glb.stats.length,
    bbox: glb.stats.bbox,
    vertices: glb.stats.vertices,
    faces: glb.stats.faces,
    hires,
    source: path.relative(REPO_ROOT, dir).split(path.sep).join("/"),
    flip: readOptions(path.join(dir, "model")).flipS ?? {},
    ...(result.articulation ? { articulation: result.articulation } : {}),
    ...(meta.smoke ? { smoke: meta.smoke } : {}),
  };
}

function gitCommit(): string | null {
  try {
    return execFileSync("git", ["rev-parse", "HEAD"], { cwd: REPO_ROOT, encoding: "utf8" }).trim();
  } catch {
    return null;
  }
}

/** One model folder and the id it is exported under. */
export interface AllocatedModel {
  tree: string;
  category: string;
  dir: string;
  id: string;
}

/**
 * Every model folder of every tree, with its id. One namespace across the trees,
 * allocated in TREES order, so a clash (none today) is resolved the same way on
 * every run. test/export.test.ts pins the result.
 */
export function allocateIds(dbRoot = path.join(REPO_ROOT, "db")): AllocatedModel[] {
  const usedIds = new Set<string>();
  const out: AllocatedModel[] = [];
  for (const { tree, category } of TREES) {
    const base = path.join(dbRoot, tree);
    for (const dir of findModelDirs(base)) {
      let id = modelId(base, dir, category);
      while (usedIds.has(id)) id = `${id}-2`;
      usedIds.add(id);
      out.push({ tree, category, dir, id });
    }
  }
  return out;
}

/** Export every tree into `outRoot`, replacing whatever was there. */
export async function exportAll(outRoot: string): Promise<{ built: number; skipped: string[] }> {
  fs.rmSync(outRoot, { recursive: true, force: true });
  const skipped: string[] = [];
  const commit = gitCommit();
  const models = allocateIds();
  let built = 0;

  for (const { tree, category } of TREES) {
    const base = path.join(REPO_ROOT, "db", tree);
    const outDir = path.join(outRoot, tree);
    fs.mkdirSync(path.join(outDir, "glb"), { recursive: true });
    fs.mkdirSync(path.join(outDir, "thumbs"), { recursive: true });

    const mine = models.filter((m) => m.tree === tree);
    console.log(`Found ${mine.length} model directories under db/${tree}`);
    const vehicles: ExportedVehicle[] = [];
    for (const { dir, id } of mine) {
      try {
        const v = await exportOne(dir, id, base, outDir);
        if (!v) {
          skipped.push(`${id} (no polygons/vertices)`);
          continue;
        }
        vehicles.push(v);
        built++;
      } catch (e) {
        skipped.push(`${id} (${(e as Error).message})`);
      }
    }

    const meta: TreeMeta = {
      _comment: [
        `Generated by vlakosimdb \`npm run export\` from db/${tree}${commit ? ` at commit ${commit}` : ""}.`,
        "Do not edit by hand: the next import overwrites this whole folder.",
      ],
      category,
      vehicles,
    };
    fs.writeFileSync(path.join(outDir, `${tree}.meta.json`), JSON.stringify(meta, null, 2) + "\n");
  }
  return { built, skipped };
}

async function main(): Promise<void> {
  const outRoot = path.resolve(process.argv[2] ?? path.join(REPO_ROOT, "export"));
  const { built, skipped } = await exportAll(outRoot);
  console.log(`\nExported ${built} models to ${path.relative(process.cwd(), outRoot) || "."}, skipped ${skipped.length}.`);
  for (const s of skipped) console.log("  - " + s);
  if (skipped.length) process.exitCode = 1;
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) void main();
