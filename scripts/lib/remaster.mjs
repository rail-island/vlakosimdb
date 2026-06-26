// JS port of remaster_atlas.py — fully automated (no manual content_box review).
//
// One gpt-image-2 call per model: build a PADDED atlas (black gutters), pre-upscale
// it to the API size, sharpen it, then auto-detect each tile's cell in the AI result
// and crop it back out. The NEW save format writes one 4x file PER SOURCE TEXTURE
// (model/x_both_4x.png, ...) next to the originals; atlas_4x is then a derived
// artifact built independently from those files (scripts/lib/atlas4x.mjs).
//
// Debug artifacts mirror the Python pipeline under generated/<slug>/.
import fs from "node:fs";
import path from "node:path";
import { parseModel } from "./geometry.mjs";
import { buildAtlas, resolveImage } from "./atlas.mjs";
import { buildAtlas4x, fourxName } from "./atlas4x.mjs";
import {
  loadImageRGBA, solidImage, blit, encodePNG, decodePNG,
  resizeLanczos, cropImage, blendImages,
} from "./images.mjs";

const SCALE = 4;
const OPENAI_MODEL = "gpt-image-2";
const SIZE_LANDSCAPE = "1536x1024";
const SIZE_PORTRAIT = "1024x1536";
const SIZE_SQUARE = "1024x1024";

// Side textures, in preference order, used as the high-res thumbnail source.
const THUMBNAIL_SIDE_REFS = ["pack:z_both.bmp", "pack:z_dec.bmp", "pack:z_inc.bmp"];

// Pricing (USD per million tokens; approximate — verify against OpenAI).
const PRICE_TEXT_INPUT_PER_M = 5.0;
const PRICE_IMAGE_INPUT_PER_M = 10.0;
const PRICE_IMAGE_OUTPUT_PER_M = 40.0;

const ATLAS_PROMPT =
  "This is a flat texture atlas for an old 3D RAILROAD VEHICLE (a locomotive, " +
  "railcar or coach) — several small low-resolution textures packed onto one sheet " +
  "with black gaps between them, already upscaled and slightly blurry. Return a " +
  "sharper, cleaner, higher-detail version. IMPORTANT: keep every element in EXACTLY " +
  "the same position, size and layout — do not move, add, remove, rearrange, crop or " +
  "recolour anything, and keep the black gaps between the tiles. Keep all logos, text, " +
  "numbers and colours exactly as they are. LOGOS/EMBLEMS (e.g. the operator emblem): " +
  "reproduce every logo with its correct, original orientation and shape — NEVER mirror, " +
  "flip, rotate or reverse a logo, and make sure EVERY instance of the same logo on the " +
  "sheet looks identical (same direction, not a mirror image of another). Render the " +
  "vehicle's WHEELS as clean, perfectly ROUND circles (not ovals, not polygons). Only " +
  "sharpen edges and add fine, plausible detail to what is already present.";

function atlasPromptFor(modelDir, repoRoot) {
  const rel = path.relative(repoRoot, modelDir);
  const ident = path.basename(modelDir);
  return (
    ATLAS_PROMPT +
    ` Context — this vehicle's catalogue identifier is '${ident}' (path '${rel}'). ` +
    "This identifier is a FILENAME, not the number painted on the vehicle: NEVER " +
    "render it verbatim on the loco. Read it as 'operator_class' — the letters before " +
    "the underscore are the railway operator (here 'cd' = the Czech operator České " +
    "dráhy, written as the 'ČD' emblem) and the digits are the locomotive CLASS. Keep " +
    "whatever running number is already painted on the vehicle exactly as-is; only use " +
    "this identifier to resolve the CLASS digits when the painted number is too blurry " +
    "to read for certain. Do not invent a different class than the one implied by the " +
    "identifier."
  );
}

export function modelSlug(modelDir, repoRoot) {
  return path.relative(repoRoot, modelDir).split(path.sep).join("__").replace(/ /g, "_");
}

function safeRefName(ref) {
  return ref.replace(/:/g, "_").replace(/\//g, "_");
}

function pickSize(w, h) {
  const ratio = h ? w / h : 1.0;
  if (ratio > 1.3) return SIZE_LANDSCAPE;
  if (ratio < 0.77) return SIZE_PORTRAIT;
  return SIZE_SQUARE;
}

// ---------------------------------------------------------------------------
// Padded atlas (port of atlas_padded.py): shelf-pack with black gutters.
// ---------------------------------------------------------------------------

export function buildPaddedAtlas(textureSources, materialsDir, pad = 16) {
  const keys = [...textureSources.keys()].sort();
  if (keys.length === 0) return null;
  const images = new Map();
  for (const key of keys) images.set(key, resolveImage(textureSources.get(key), materialsDir));

  const sortedKeys = keys.slice().sort((a, b) => images.get(b).height - images.get(a).height);
  const all = [...images.values()];
  const maxW = Math.max(...all.map((i) => i.width));
  const totalW = all.reduce((s, i) => s + i.width + pad, 0);
  const maxH = Math.max(...all.map((i) => i.height));
  const targetW = Math.max(maxW, Math.floor(Math.sqrt(totalW * maxH)));

  const placements = new Map();
  let x = pad, y = pad, rowH = 0;
  for (const key of sortedKeys) {
    const im = images.get(key);
    if (x > pad && x + im.width + pad > targetW + pad) {
      y += rowH + pad; // wrap to next shelf
      x = pad;
      rowH = 0;
    }
    placements.set(key, [x, y]);
    x += im.width + pad;
    rowH = Math.max(rowH, im.height);
  }

  let atlasW = 0, atlasH = 0;
  for (const key of keys) {
    const [px, py] = placements.get(key);
    const im = images.get(key);
    atlasW = Math.max(atlasW, px + im.width);
    atlasH = Math.max(atlasH, py + im.height);
  }
  atlasW += pad;
  atlasH += pad;

  const atlas = solidImage(atlasW, atlasH, [0, 0, 0, 255]);
  const map = new Map();
  for (const key of keys) {
    const [px, py] = placements.get(key);
    const im = images.get(key);
    blit(atlas, im, px, py);
    map.set(key, [px / atlasW, py / atlasH, im.width / atlasW, im.height / atlasH]);
  }
  return { image: atlas, map };
}

// ---------------------------------------------------------------------------
// Tile detection (port of detect_tile_boxes / _runs from remaster_atlas.py).
// ---------------------------------------------------------------------------

function luminanceMask(img, threshold) {
  const { width, height, data } = img;
  const mask = new Uint8Array(width * height);
  for (let i = 0, p = 0; i < mask.length; i++, p += 4) {
    const lum = 0.299 * data[p] + 0.587 * data[p + 1] + 0.114 * data[p + 2];
    mask[i] = lum > threshold ? 1 : 0;
  }
  return mask;
}

function runs(has, minGap) {
  const n = has.length;
  const out = [];
  let i = 0;
  while (i < n) {
    if (!has[i]) { i++; continue; }
    let j = i + 1;
    while (j < n) {
      if (has[j]) { j++; continue; }
      let k = j;
      while (k < n && !has[k]) k++;
      if (k < n && k - j < minGap) { j = k; } else { break; }
    }
    out.push([i, j]);
    i = j;
  }
  return out;
}

export function detectTileBoxes(ai, paddedMap, threshold = 12) {
  const { width: W, height: H } = ai;
  const mask = luminanceMask(ai, threshold);

  // Content rect of the whole AI image.
  let cx0 = W, cy0 = H, cx1 = 0, cy1 = 0, any = false;
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      if (mask[y * W + x]) {
        any = true;
        if (x < cx0) cx0 = x;
        if (x > cx1) cx1 = x;
        if (y < cy0) cy0 = y;
        if (y > cy1) cy1 = y;
      }
    }
  }
  if (!any) { cx0 = 0; cy0 = 0; cx1 = W - 1; cy1 = H - 1; }
  cx1 += 1; cy1 += 1; // exclusive
  const crect = [cx0, cy0, cx1, cy1];
  const cw = cx1 - cx0, ch = cy1 - cy0;

  const packRefs = [...paddedMap.keys()].filter((r) => r.startsWith("pack:"));
  const minGap = Math.max(8, Math.floor(cw / 25));

  // Row runs over the content rect.
  const rowsHas = new Uint8Array(ch);
  for (let y = 0; y < ch; y++) {
    let h = 0;
    const base = (cy0 + y) * W;
    for (let x = cx0; x < cx1; x++) { if (mask[base + x]) { h = 1; break; } }
    rowsHas[y] = h;
  }

  const detected = [];
  for (const [ry0, ry1] of runs(rowsHas, minGap)) {
    const ay0 = cy0 + ry0, ay1 = cy0 + ry1;
    const colsHas = new Uint8Array(cw);
    for (let x = 0; x < cw; x++) {
      let h = 0;
      const ax = cx0 + x;
      for (let yy = ay0; yy < ay1; yy++) { if (mask[yy * W + ax]) { h = 1; break; } }
      colsHas[x] = h;
    }
    for (const [rx0, rx1] of runs(colsHas, minGap)) {
      detected.push([cx0 + rx0, ay0, cx0 + rx1, ay1]);
    }
  }

  const mappedCell = (ref) => {
    const [u, v, uw, vh] = paddedMap.get(ref);
    return [
      Math.round(cx0 + u * cw), Math.round(cy0 + v * ch),
      Math.round(cx0 + (u + uw) * cw), Math.round(cy0 + (v + vh) * ch),
    ];
  };
  const aspect = (b) => (b[2] - b[0]) / Math.max(1, b[3] - b[1]);

  const out = { _contentRect: crect };
  if (detected.length === packRefs.length) {
    const refsOrder = packRefs.slice().sort((a, b) => {
      const A = paddedMap.get(a), B = paddedMap.get(b);
      return A[1] - B[1] || A[0] - B[0];
    });
    detected.sort((a, b) => a[1] - b[1] || a[0] - b[0]);
    refsOrder.forEach((ref, idx) => {
      const box = detected[idx];
      const mc = mappedCell(ref);
      const ratio = aspect(box) / Math.max(1e-6, aspect(mc));
      let cell, conf;
      if (ratio >= 0.8 && ratio <= 1.25) { cell = box; conf = true; }
      else if (ratio >= 0.5 && ratio <= 2.0) { cell = box; conf = false; }
      else { cell = mc; conf = false; }
      out[ref] = { cell, content: cell, fillsFrame: conf };
    });
  } else {
    for (const ref of packRefs) {
      const cell = mappedCell(ref);
      out[ref] = { cell, content: cell, fillsFrame: true };
    }
  }
  // Solid-colour / other refs: position by mapping (tiny, no detail).
  for (const ref of paddedMap.keys()) {
    if (!(ref in out)) out[ref] = { cell: mappedCell(ref), content: mappedCell(ref), fillsFrame: false };
  }
  return out;
}

// ---------------------------------------------------------------------------
// OpenAI image edit (port of call_openai_edit) via native fetch.
// ---------------------------------------------------------------------------

function extractUsage(json) {
  const u = json.usage;
  if (!u) return {};
  const d = u.input_tokens_details || {};
  return {
    input_tokens: u.input_tokens || 0,
    output_tokens: u.output_tokens || 0,
    input_text_tokens: d.text_tokens || 0,
    input_image_tokens: d.image_tokens || 0,
  };
}

function computeCost(usage) {
  if (!usage || !Object.keys(usage).length) return 0.0;
  const cost =
    (usage.input_text_tokens || 0) / 1e6 * PRICE_TEXT_INPUT_PER_M +
    (usage.input_image_tokens || 0) / 1e6 * PRICE_IMAGE_INPUT_PER_M +
    (usage.output_tokens || 0) / 1e6 * PRICE_IMAGE_OUTPUT_PER_M;
  return Math.round(cost * 1e6) / 1e6;
}

async function editImage(pngBytes, size, quality, prompt, apiKey) {
  const form = new FormData();
  form.append("model", OPENAI_MODEL);
  form.append("image", new Blob([pngBytes], { type: "image/png" }), "atlas.png");
  form.append("prompt", prompt);
  form.append("size", size);
  form.append("quality", quality);
  const resp = await fetch("https://api.openai.com/v1/images/edits", {
    method: "POST",
    headers: { Authorization: `Bearer ${apiKey}` },
    body: form,
  });
  if (!resp.ok) {
    throw new Error(`OpenAI images.edit ${resp.status}: ${await resp.text()}`);
  }
  const json = await resp.json();
  const png = Buffer.from(json.data[0].b64_json, "base64");
  return { image: decodePNG(png), usage: extractUsage(json) };
}

// ---------------------------------------------------------------------------
// Orchestration
// ---------------------------------------------------------------------------

/**
 * Remaster one model's textures 4x.
 * @param {string} modelDir   absolute path to the model folder (contains model/index.xml)
 * @param {object} opts { repoRoot, materialsDir, quality, pad, dryRun, apiKey, onLog }
 * @returns {Promise<object>} a recompact summary
 */
export async function remasterModel(modelDir, opts = {}) {
  const {
    repoRoot,
    materialsDir,
    quality = "high",
    pad = 16,
    dryRun = false,
    apiKey = process.env.OPENAI_API_KEY,
    onLog = () => {},
  } = opts;

  if (!dryRun && !apiKey) throw new Error("OPENAI_API_KEY not set (or use dryRun)");

  const modelSub = path.join(modelDir, "model");
  const modelXml = path.join(modelSub, "index.xml");
  if (!fs.existsSync(modelXml)) throw new Error(`no model/index.xml in ${modelDir}`);

  // Root-only textures (matches the Python pipeline; inline sub-models get a
  // nearest-upscale fallback in buildAtlas4x).
  const { textureSources } = parseModel(modelSub, { includeInlines: false });
  const { pngBuffer: compactPng, atlasMap, width: cw, height: ch } = buildAtlas(textureSources, materialsDir);
  if (!cw || !ch) throw new Error("no textures / empty atlas");

  const slug = modelSlug(modelDir, repoRoot);
  const outDir = path.join(repoRoot, "generated", slug);
  fs.mkdirSync(path.join(outDir, "tiles"), { recursive: true });

  // Compact atlas + classical 4x baseline (for inspection).
  fs.writeFileSync(path.join(outDir, "atlas.png"), compactPng);
  const compactImg = decodePNG(compactPng);
  const raw4x = await resizeLanczos(compactImg, cw * SCALE, ch * SCALE);
  fs.writeFileSync(path.join(outDir, "atlas_4x_raw.png"), encodePNG(raw4x));

  // Padded atlas -> pre-upscale to the API size -> sharpen.
  onLog(`building padded atlas (pad=${pad})`);
  const padded = buildPaddedAtlas(textureSources, materialsDir, pad);
  const size = pickSize(padded.image.width * SCALE, padded.image.height * SCALE);
  const [aw, ah] = size.split("x").map(Number);
  const rawForAi = await resizeLanczos(padded.image, aw, ah);
  fs.writeFileSync(path.join(outDir, "atlas_padded_4x_raw.png"), encodePNG(rawForAi));

  let ai, usage = {}, cost = 0;
  if (dryRun) {
    onLog("dry-run: skipping the gpt-image-2 call (using classical upscale)");
    ai = rawForAi;
  } else {
    onLog(`calling gpt-image-2 (size=${size}, quality=${quality}) …`);
    const res = await editImage(encodePNG(rawForAi), size, quality, atlasPromptFor(modelDir, repoRoot), apiKey);
    ai = res.image;
    usage = res.usage;
    cost = computeCost(usage);
    onLog(`gpt-image-2 done (cost ~$${cost.toFixed(4)})`);
  }
  fs.writeFileSync(path.join(outDir, "atlas_padded_4x.png"), encodePNG(ai));

  // Detect each tile's cell in the AI result and crop per-texture 4x files.
  const det = detectTileBoxes(ai, padded.map);
  delete det._contentRect;

  const tiles = [];
  const upByRef = new Map();
  for (const ref of Object.keys(det).sort()) {
    if (!ref.startsWith("pack:")) continue;
    const src = textureSources.get(ref);
    if (!src || src.type !== "pack") continue;
    const cell = det[ref].cell;
    const tileImg = cropImage(ai, cell);
    fs.writeFileSync(path.join(outDir, "tiles", `${safeRefName(ref)}.png`), encodePNG(tileImg));

    let ow, oh;
    try {
      const orig = loadImageRGBA(path.join(src.dir, src.name));
      ow = orig.width; oh = orig.height;
    } catch {
      const m = atlasMap.get(ref);
      ow = Math.round(m[2] * cw); oh = Math.round(m[3] * ch);
    }
    const up = await resizeLanczos(tileImg, ow * SCALE, oh * SCALE);
    upByRef.set(ref, up);

    const outName = fourxName(src.name);
    fs.writeFileSync(path.join(src.dir, outName), encodePNG(up));
    onLog(`wrote model/${outName} (${ow * SCALE}×${oh * SCALE})`);
    tiles.push({ ref, cell, content: det[ref].content, fills_frame: det[ref].fillsFrame, out: outName });
  }

  // High-res thumbnail from a side texture (no extra AI call).
  const imageBmp = path.join(modelDir, "image.bmp");
  let imageOut = null;
  const sideRef = THUMBNAIL_SIDE_REFS.find((r) => upByRef.has(r));
  if (sideRef) {
    let thumb = upByRef.get(sideRef);
    if (fs.existsSync(imageBmp)) {
      try {
        const src = loadImageRGBA(imageBmp);
        thumb = await resizeLanczos(thumb, src.width * SCALE, src.height * SCALE);
      } catch { /* aspect match is best-effort */ }
    }
    fs.writeFileSync(path.join(modelDir, "image_4x.png"), encodePNG(thumb));
    imageOut = "image_4x.png";
    onLog("wrote image_4x.png thumbnail");
  }

  // Derived atlas_4x (independent step) + overlay, for inspection only.
  const { pngBuffer: atlas4xPng, hires } = buildAtlas4x(atlasMap, cw, ch, textureSources, materialsDir);
  fs.writeFileSync(path.join(outDir, "atlas_4x.png"), atlas4xPng);
  fs.writeFileSync(path.join(outDir, "atlas_4x_overlay.png"), encodePNG(blendImages(raw4x, decodePNG(atlas4xPng), 0.5)));

  const recompact = {
    model: path.relative(repoRoot, modelDir),
    slug,
    compact_size: [cw * SCALE, ch * SCALE],
    ai_image_size: [ai.width, ai.height],
    model_used: OPENAI_MODEL,
    quality,
    pad,
    usage,
    cost_usd: cost,
    status: dryRun ? "dry-run" : "remastered",
    image_4x: imageOut,
    tiles,
  };
  fs.writeFileSync(path.join(outDir, "recompact.json"), JSON.stringify(recompact, null, 2) + "\n");

  onLog(`done: ${tiles.length} textures upscaled`);
  return { ...recompact, hires, outDir, atlas4xPng };
}
