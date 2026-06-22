---
name: remaster-atlas
description: Remaster a train/building model's texture atlas to 4x with AI (gpt-image-2) while keeping every packed sub-texture at its exact UV position and producing a COMPACT result (no padding, small Roblox texture). Use when the user asks to "upscale"/"remaster"/"4x" a specific model's textures/atlas by name, e.g. "Upscale cd_163 loco", "remaster the cd_a coach atlas". Drives scripts/remaster_atlas.py (prepare/assemble) and uses your own vision to crop each AI-enhanced tile back into the compact atlas.
user-invocable: true
---

# remaster-atlas

Upscale one model's texture atlas 4x with AI, keeping sub-texture positions exact and the result
compact. The user gives a model **name** (e.g. `cd_163`); you do everything.

Why two phases + your vision: sending the whole atlas to the image model rearranges/rescales the
packed tiles (breaks UVs). So we send a *padded* atlas (gutters keep tiles apart), then **you read
the AI result and crop each tile back into the real compact atlas** at its exact position — your
vision corrects the small per-tile drift the model introduces, especially on mostly-black gear/end
tiles where an automatic crop is unreliable.

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

3. **Assemble** the compact atlas:
   ```
   python3 scripts/remaster_atlas.py assemble --model <NAME>
   ```
   Writes the compact `generated/<slug>/atlas_4x.png` (size == `atlas.png`×4, no padding), an
   `atlas_4x_overlay.png`, the git-tracked `db/.../atlas_4x.png`, and a high-res thumbnail
   `db/.../image_4x.png` cropped from the AI side texture (z_both/z_dec/z_inc, no extra AI call;
   sized to `image.bmp`×4). `upload_loco.py` uses `image_4x.png` instead of `image.bmp` when present.

4. **Verify positions — REQUIRED.** Run:
   ```
   python3 scripts/remaster_atlas.py check --model <NAME>
   ```
   This prints a per-tile mismatch score and writes `generated/<slug>/atlas_4x_check.png` — a
   per-tile **`raw | AI`** side-by-side (`atlas_4x_raw` is the correct compact layout; `atlas_4x` is
   your assembled result). **Read `atlas_4x_check.png`.** For every tile the two halves must show
   the SAME part in the SAME position/shape (the AI half just sharper, and its number/markings may
   differ — that is fine). A tile is WRONG only if its AI half shows a different part, is shifted,
   cropped, or stretched. (Also glance at `atlas_4x_overlay.png`, the 50% blend, for gross drift.)
   - A **high mismatch score alone is not a failure** — the AI re-renders content, so sharp tiles
     score high. Use the score only to decide which tiles to look at; judge by eye.
   - If a tile is genuinely wrong, find its real rectangle in `atlas_padded_4x.png`, fix that tile's
     `content_box` in `recompact.json` (keep the source aspect ratio), re-run `assemble`, then
     `check` again. Iterate until every tile matches.

5. **Report** to the user: the compact `atlas_4x.png` (dimensions, confirms it matches the original
   `atlas.png` layout so it's a drop-in higher-res texture), that `db/.../atlas_4x.png` was written,
   and the AI cost.

## Notes
- One AI call per model (the `prepare` step); steps 2–4 are local/free, just your vision + crops.
- `--dry-run` on `prepare` skips the AI call (uses the classical upscale) to test the pipeline free.
- `--pad N` controls the padded gutter (default 16 source px) — increase it if the AI is merging
  neighbouring tiles.
- The compact result matches `build_texture_atlas`'s UV layout, so no upload/GLB changes are needed.
- Originals are never modified; only new `atlas_4x.png` files are written.
