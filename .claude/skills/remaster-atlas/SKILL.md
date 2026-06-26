---
name: remaster-atlas
description: Remaster a train/building model's textures to 4x with AI (gpt-image-2) using ONE call per model, writing one per-texture file next to each source (model/z_both_4x.png, etc.) — NOT a combined atlas. Use when the user asks to "upscale"/"remaster"/"4x" a specific model's textures by name, e.g. "Upscale cd_163 loco", "remaster the cd_a coach textures". Drives scripts/remaster_atlas.py (prepare/assemble) and uses your own vision to crop each AI-enhanced tile back into its own 4x file.
user-invocable: true
---

# remaster-atlas

Upscale one model's textures 4x with AI using a SINGLE call per model, writing one per-texture
file next to each source (`model/z_both_4x.png`, etc.). The user gives a model **name**
(e.g. `cd_163`); you do everything. A combined `atlas_4x.png` is NOT produced here — the JS
build/upload step derives it later from these `*_4x.png` files.

Why one padded call + your vision: sending each texture separately would be many AI calls, and
sending the whole atlas unpadded rearranges/rescales the tiles. So we pack all the model's textures
into one *padded* atlas (gutters keep tiles apart), send it in a single call, then **you read the AI
result and crop each tile back into its own 4x file** at its exact rectangle — your vision corrects
the small per-tile drift the model introduces, especially on mostly-black gear/end tiles where an
automatic crop is unreliable.

All commands run from the repo root. The model name comes from the user's request.

## Workflow

1. **Prepare** (builds padded atlas, makes ONE AI call, writes everything needed):
   ```
   python3 scripts/remaster_atlas.py prepare --model <NAME>
   ```
   - If it exits with "Ambiguous name", show the user the matches and ask which, then re-run with
     the full path as `--model`.
   - Outputs land in `generated/<slug>/`: `atlas_padded_4x.png` (the AI result),
     `tiles/<ref>.png` (each tile's padded-cell crop), `atlas.png` + `atlas_4x_raw.png`
     (compact non-AI reference for manual comparison), and `recompact.json` (the tile boxes —
     this is what you edit).

2. **Review the AI result (usually no edits needed).** `prepare` already auto-detects each tile's
   box by finding the AI's content region and segmenting it on the black gutters
   (`detect_tile_boxes`), so `recompact.json` `content_box`es are normally correct. First just run
   `assemble` (step 3) and look at the overlay (step 4). Only if a tile is misaligned there, open
   `generated/<slug>/atlas_padded_4x.png` (and `tiles/<ref>.png`) and fix that tile's `content_box`
   — the absolute `[x0,y0,x1,y1]` rectangle in `atlas_padded_4x.png` that bounds the AI-drawn tile
   (keep its aspect ratio close to the source so it isn't stretched). `fills_frame:false` marks
   tiles the detector wasn't confident about (sparse gear/end) — those are the likeliest to need a
   nudge. Edit `recompact.json` and re-run `assemble`.

3. **Assemble** the per-texture 4x files:
   ```
   python3 scripts/remaster_atlas.py assemble --model <NAME>
   ```
   For each source texture it crops the tile's `content_box` from the AI result, resizes it to 4x
   the source's own size, and writes **one file per texture next to the source**:
   `db/.../model/<name>_4x.png` (e.g. `z_both_4x.png`, `x_both_4x.png`) — plus debug copies in
   `generated/<slug>/`. It also writes a high-res thumbnail `db/.../image_4x.png` from the AI side
   texture (z_both/z_dec/z_inc, no extra AI call; sized to `image.bmp`×4), and prints the model's AI
   upscaling cost. **No `atlas_4x.png` is written** — the JS build/upload derives the 4x atlas from
   these `*_4x.png` files. `upload_loco.py` uses `image_4x.png` instead of `image.bmp` when present.

4. **Verify positions — REQUIRED.** Run:
   ```
   python3 scripts/remaster_atlas.py check --model <NAME>
   ```
   This prints a per-texture mismatch score and writes `generated/<slug>/textures_4x_check.png` — a
   per-texture **`raw | AI`** side-by-side (`raw` is the source texture upscaled classically; `AI`
   is the `<name>_4x.png` you produced). **Read `textures_4x_check.png`.** For every texture the two
   halves must show the SAME part in the SAME position/shape (the AI half just sharper, and its
   number/markings may differ — that is fine). A texture is WRONG only if its AI half shows a
   different part, is shifted, cropped, or stretched.
   - A **high mismatch score alone is not a failure** — the AI re-renders content, so sharp tiles
     score high. Use the score only to decide which tiles to look at; judge by eye.
   - If a tile is genuinely wrong, find its real rectangle in `atlas_padded_4x.png`, fix that tile's
     `content_box` in `recompact.json` (keep the source aspect ratio), re-run `assemble`, then
     `check` again. Iterate until every tile matches.

5. **Report** to the user: the per-texture `<name>_4x.png` files written next to the sources in
   `db/.../model/` (list them), that `image_4x.png` was written, and — REQUIRED — **the price of
   the upscaling** (the AI cost in USD, printed by `assemble` as "AI upscaling cost for this model";
   it is also in `recompact.json` → `cost_usd`).

## Notes
- One AI call per model (the `prepare` step); steps 2–4 are local/free, just your vision + crops.
- `--dry-run` on `prepare` skips the AI call (uses the classical upscale) to test the pipeline free
  (cost is then $0.00).
- `--pad N` controls the padded gutter (default 16 source px) — increase it if the AI is merging
  neighbouring tiles.
- The per-texture `*_4x.png` are 4x the source's own size; the JS build/upload derives the 4x atlas
  from them (`scripts/lib/atlas4x.mjs`), so no manual atlas/UV step is needed here.
- Originals are never modified; only new `<name>_4x.png` and `image_4x.png` files are written.
