#!/usr/bin/env python3
"""
Atlas remastering in two phases, driven by a model name. Designed to be run by the
`remaster-atlas` skill so the user can just say "Upscale cd_163 loco".

Phase `prepare`:
  - build the model's PADDED atlas (black gutters around each tile),
  - upscale it 4x locally and send it once to gpt-image-2,
  - also build the real COMPACT atlas (build_texture_atlas) for the target layout,
  - write atlas_padded_4x.png, per-tile crops, atlas_4x_classical.png and
    recompact.json (each tile's padded box, an auto content box, and its exact
    destination in the compact atlas).

Then Claude reads atlas_padded_4x.png and corrects each tile's `content_box` in
recompact.json where the AI drifted (esp. mostly-black gear/end tiles).

Phase `assemble`:
  - crop each tile's content_box from atlas_padded_4x, resize to its compact box,
    paste into a black canvas of the compact size -> compact atlas_4x.png,
  - write the git-tracked db copy and an overlay for position checking.

Usage:
  python remaster_atlas.py prepare  --model cd_163
  python remaster_atlas.py assemble --model cd_163
"""

import argparse
import base64
import io
import json
import sys
from pathlib import Path

from dotenv import load_dotenv
from PIL import Image

from atlas import build_texture_atlas
from atlas_padded import build_padded_atlas
from remaster_textures import (
    MODEL,
    OUT_DIR,
    call_openai_edit,
    compute_cost,
    extract_usage,
    model_slug,
    pick_size,
)
from upload_utils import _repo_root

try:
    from openai import OpenAI
    HAS_OPENAI = True
except ImportError:
    HAS_OPENAI = False

import os

UPSCALE = 4
RECOMPACT_NAME = "recompact.json"
DB_SEARCH_ROOT = "db/train"

ATLAS_PROMPT = (
    "This is a flat texture atlas for an old 3D RAILROAD VEHICLE (a locomotive, "
    "railcar or coach) — several small low-resolution textures packed onto one sheet "
    "with black gaps between them, already upscaled and slightly blurry. Return a "
    "sharper, cleaner, higher-detail version. IMPORTANT: keep every element in EXACTLY "
    "the same position, size and layout — do not move, add, remove, rearrange, crop or "
    "recolour anything, and keep the black gaps between the tiles. Keep all logos, text, "
    "numbers and colours exactly as they are. Render the vehicle's WHEELS as clean, "
    "perfectly ROUND circles (not ovals, not polygons). Only sharpen edges and add fine, "
    "plausible detail to what is already present."
)


def atlas_prompt_for(model_dir: Path) -> str:
    """Atlas prompt plus the model's catalogue identifier as context, so the model
    can resolve a blurry running number to the correct class (e.g. cd_163 -> 163)."""
    rel = model_dir.relative_to(_repo_root())
    return ATLAS_PROMPT + (
        f" Context — this vehicle's catalogue identifier is '{model_dir.name}' "
        f"(path '{rel}'). Use it to resolve any text that is too blurry to read for "
        "certain: in particular, the running/serial number painted on the vehicle must "
        "be consistent with this identifier's class/series digits (for example a name "
        "like 'cd_163' means the number is a 163-series number, not 151). Do not invent "
        "a different class number than the one implied by the identifier."
    )


# ---------------------------------------------------------------------------
# Model resolution
# ---------------------------------------------------------------------------

def resolve_model(name_or_path: str) -> Path:
    """Resolve a model NAME (e.g. 'cd_163') or repo path to its directory.

    A valid model dir contains model/index.xml. On an ambiguous name, prints the
    matches and exits(2) so the caller can disambiguate.
    """
    root = _repo_root()
    p = root / name_or_path
    if (p / "model" / "index.xml").exists():
        return p

    matches = [
        d for d in sorted((root / DB_SEARCH_ROOT).rglob(name_or_path))
        if d.is_dir() and (d / "model" / "index.xml").exists()
    ]
    if not matches:
        print(f"Error: no model named '{name_or_path}' found under {DB_SEARCH_ROOT}/ "
              "(needs model/index.xml).")
        sys.exit(2)
    if len(matches) > 1:
        print(f"Ambiguous name '{name_or_path}' — {len(matches)} matches:")
        for m in matches:
            print(f"  {m.relative_to(root)}")
        sys.exit(2)
    return matches[0]


# ---------------------------------------------------------------------------
# Geometry helpers
# ---------------------------------------------------------------------------

def map_to_boxes(atlas_map: dict, w4: int, h4: int) -> dict:
    """Convert a normalized atlas_map to integer pixel boxes in a (w4,h4) atlas."""
    boxes = {}
    for ref, (u, v, uw, vh) in atlas_map.items():
        x0, y0 = round(u * w4), round(v * h4)
        x1, y1 = round((u + uw) * w4), round((v + vh) * h4)
        boxes[ref] = [x0, y0, x1, y1]
    return boxes


def _runs(has, min_gap: int) -> list[tuple[int, int]]:
    """Contiguous True-runs in a bool array, merging runs split by gaps < min_gap.

    (So real gutters split tiles, but small internal black gaps inside a tile —
    e.g. between wheels — do not.)
    """
    n = len(has)
    runs: list[tuple[int, int]] = []
    i = 0
    while i < n:
        if not has[i]:
            i += 1
            continue
        j = i + 1
        while j < n:
            if has[j]:
                j += 1
            else:
                k = j
                while k < n and not has[k]:
                    k += 1
                if k < n and (k - j) < min_gap:   # small gap inside a tile -> merge
                    j = k
                else:
                    break
        runs.append((i, j))
        i = j
    return runs


def detect_tile_boxes(ai: Image.Image, padded_map: dict, threshold: int = 12) -> dict:
    """Find where each PACK tile actually sits in the AI result by detecting gutters.

    gpt-image-2 insets the padded atlas and drifts tiles non-uniformly, so a fixed
    map is offset. Instead we segment the AI content the way the padded atlas was
    built: split into shelf ROWS on the wide black gutters, then split each row into
    tile cells on the vertical gutters. Each detected cell is the tile's true rect
    (gutter-bounded -> keeps the tile's own black margins, so aspect is preserved).
    Cells are matched to refs by reading order (the shelf packer's order).

    Falls back to a content-rect mapping when segmentation does not yield exactly
    the expected number of pack tiles (e.g. odd cases) so it never crashes.

    Returns ``{ref: {"cell":[...], "content":[...], "fills_frame":bool}}`` plus
    ``"_content_rect"``.
    """
    import numpy as np

    m = np.asarray(ai.convert("L")) > threshold
    ys, xs = np.where(m)
    if len(xs) == 0:
        crect = (0, 0, ai.width, ai.height)
    else:
        crect = (int(xs.min()), int(ys.min()), int(xs.max()) + 1, int(ys.max()) + 1)
    cx0, cy0, cx1, cy1 = crect
    sub = m[cy0:cy1, cx0:cx1]

    pack_refs = [r for r in padded_map if r.startswith("pack:")]
    # Gutter size in AI px ≈ a fraction of the content; use a robust min-gap.
    min_gap = max(8, (cx1 - cx0) // 25)

    detected: list[list[int]] = []
    for (ry0, ry1) in _runs(sub.any(axis=1), min_gap):
        band = sub[ry0:ry1]
        for (rx0, rx1) in _runs(band.any(axis=0), min_gap):
            # tight vertical extent of this column run within the band
            # Use the ROW band's full y-extent (tiles in a shelf row are top-aligned
            # at the same height), not each blob's tight y — otherwise a mostly-black
            # tile (e.g. the gear strip) collapses to just its visible detail.
            detected.append([cx0 + rx0, cy0 + ry0, cx0 + rx1, cy0 + ry1])

    out: dict = {"_content_rect": list(crect)}
    cw, ch = cx1 - cx0, cy1 - cy0

    def mapped_cell(ref):
        u, v, uw, vh = padded_map[ref]
        return [round(cx0 + u * cw), round(cy0 + v * ch),
                round(cx0 + (u + uw) * cw), round(cy0 + (v + vh) * ch)]

    def aspect(b):
        return (b[2] - b[0]) / max(1, b[3] - b[1])

    if len(detected) == len(pack_refs):
        # Match detected cells (reading order) to refs (padded reading order).
        refs_order = sorted(pack_refs, key=lambda r: (padded_map[r][1], padded_map[r][0]))
        detected.sort(key=lambda b: (b[1], b[0]))
        for ref, box in zip(refs_order, detected):
            mc = mapped_cell(ref)
            # Trust the detected cell only if its aspect matches the tile's expected
            # aspect (mapped cell). Mis-segmented small tiles (e.g. gear/end packed
            # together in one row) fail this -> fall back to the aspect-correct cell.
            ok = 0.8 <= aspect(box) / max(1e-6, aspect(mc)) <= 1.25
            cell = box if ok else mc
            out[ref] = {"cell": cell, "content": cell, "fills_frame": ok}
    else:
        # Fallback: content-rect mapping (aspect-correct, fixes the global offset).
        for ref in pack_refs:
            cell = mapped_cell(ref)
            out[ref] = {"cell": cell, "content": cell, "fills_frame": True}

    # material:/other solid-colour refs: position by mapping (tiny, no detail).
    for ref in padded_map:
        if ref not in out and ref != "_content_rect":
            cell = mapped_cell(ref)
            out[ref] = {"cell": cell, "content": cell, "fills_frame": False}
    return out


def content_bbox(img: Image.Image, box, threshold: int = 14) -> list[int] | None:
    """Absolute bbox of non-black content inside ``box`` (None if all black)."""
    crop = img.crop(box).convert("L").point(lambda p: 255 if p > threshold else 0)
    bb = crop.getbbox()
    if bb is None:
        return None
    return [box[0] + bb[0], box[1] + bb[1], box[0] + bb[2], box[1] + bb[3]]


def safe_ref_name(ref: str) -> str:
    """Filesystem-safe name for a texture ref (pack:z_dec.bmp -> pack_z_dec.bmp)."""
    return ref.replace(":", "_").replace("/", "_")


# ---------------------------------------------------------------------------
# Phase: prepare
# ---------------------------------------------------------------------------

def prepare(model_dir: Path, out_root: Path, client, quality: str, pad: int,
            dry_run: bool) -> dict:
    model_sub = model_dir / "model"
    model_xml = model_sub / "index.xml"
    slug = model_slug(model_dir)
    out_dir = out_root / slug
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "tiles").mkdir(exist_ok=True)

    # Compact atlas (the real upload layout = target) ...
    compact_png, compact_map, _ = build_texture_atlas(model_sub, model_xml)
    if not compact_png:
        print("  [error] no textures / could not build compact atlas")
        sys.exit(1)
    compact = Image.open(io.BytesIO(compact_png)).convert("RGB")
    cw, ch = compact.size
    cw4, ch4 = cw * UPSCALE, ch * UPSCALE
    compact.save(out_dir / "atlas.png")
    # Plain LANCZOS 4x of the compact atlas — the non-AI baseline, kept for manual
    # comparison against the AI-assembled atlas_4x.png.
    compact.resize((cw4, ch4), Image.LANCZOS).save(out_dir / "atlas_4x_raw.png")
    compact_boxes = map_to_boxes(compact_map, cw4, ch4)

    # ... and the padded atlas (sent to the AI). Pre-upscale it LOCALLY (LANCZOS) to
    # the exact API output size, so the model only has to sharpen at native target
    # resolution — it does no upscaling itself, and tiles carry more pixels for detail
    # (rounder wheels, less pixelation). Saved as atlas_padded_4x_raw.png.
    padded_png, padded_map, _ = build_padded_atlas(model_sub, model_xml, pad)
    padded = Image.open(io.BytesIO(padded_png)).convert("RGB")
    pw, ph = padded.size
    size = pick_size(pw * UPSCALE, ph * UPSCALE)
    aw, ah = (int(v) for v in size.split("x"))
    raw_for_ai = padded.resize((aw, ah), Image.LANCZOS)
    raw_for_ai.save(out_dir / "atlas_padded_4x_raw.png")

    usage, cost = {}, 0.0
    if dry_run:
        ai = raw_for_ai.copy()  # stand-in
    else:
        buf = io.BytesIO()
        raw_for_ai.save(buf, "PNG")
        resp = call_openai_edit(client, buf.getvalue(), size, quality,
                                prompt=atlas_prompt_for(model_dir))
        ai = Image.open(io.BytesIO(base64.b64decode(resp.data[0].b64_json))).convert("RGB")
        usage = extract_usage(resp)
        cost = compute_cost(usage, quality)
    ai.save(out_dir / "atlas_padded_4x.png")

    # Detect where each tile actually sits in the AI result (the AI insets the
    # whole atlas with black margins, so a full-image mapping would be offset).
    det = detect_tile_boxes(ai, padded_map)
    content_rect = det.pop("_content_rect")

    tiles = []
    for ref in sorted(det):
        if ref not in compact_boxes:
            continue  # material: refs exist in both; keep only shared refs
        cell = det[ref]["cell"]
        content = det[ref]["content"]
        fills = det[ref]["fills_frame"]
        cbox = compact_boxes[ref]
        ai.crop(cell).save(out_dir / "tiles" / f"{safe_ref_name(ref)}.png")
        tiles.append({"ref": ref, "padded_box": cell, "content_box": content,
                      "compact_box": cbox, "fills_frame": fills})

    recompact = {
        "model": str(model_dir.relative_to(_repo_root())),
        "slug": slug,
        "compact_size": [cw4, ch4],
        "ai_image_size": [ai.width, ai.height],
        "ai_content_rect": content_rect,
        "model_used": MODEL,
        "quality": quality,
        "pad": pad,
        "usage": usage,
        "cost_usd": cost,
        "status": "dry-run" if dry_run else "prepared",
        "tiles": tiles,
    }
    (out_dir / RECOMPACT_NAME).write_text(json.dumps(recompact, indent=2) + "\n", encoding="utf-8")

    print(f"  prepared: {len(tiles)} tiles | AI input/output {aw}x{ah} -> compact {cw4}x{ch4} "
          f"| ${cost:.4f}")
    print(f"  inspect: {out_dir / 'atlas_padded_4x.png'}  and  {out_dir / 'tiles'}/")
    print(f"  edit content_box if needed in: {out_dir / RECOMPACT_NAME}")
    return recompact


# ---------------------------------------------------------------------------
# Phase: assemble
# ---------------------------------------------------------------------------

def assemble(model_dir: Path, out_root: Path, db_copy: bool) -> dict:
    slug = model_slug(model_dir)
    out_dir = out_root / slug
    rc_path = out_dir / RECOMPACT_NAME
    if not rc_path.exists():
        print(f"  [error] {rc_path} not found — run `prepare` first")
        sys.exit(1)
    rc = json.loads(rc_path.read_text(encoding="utf-8"))

    ai = Image.open(out_dir / "atlas_padded_4x.png").convert("RGB")
    raw_ref = Image.open(out_dir / "atlas_4x_raw.png").convert("RGB")  # correct layout
    cw4, ch4 = rc["compact_size"]
    canvas = Image.new("RGB", (cw4, ch4), (0, 0, 0))
    for t in rc["tiles"]:
        cb = t["compact_box"]
        # Anchor to the RAW reference: place the AI tile's content into the exact
        # sub-rectangle the original texture occupies inside this cell. This matches
        # both the content's position (e.g. x_both's cab sits at the bottom with a
        # black top) AND its aspect — no centring, no stretch mismatch.
        raw_sub = content_bbox(raw_ref, cb)
        ai_sub = content_bbox(ai, t["content_box"])
        if raw_sub and ai_sub:
            tw, th = raw_sub[2] - raw_sub[0], raw_sub[3] - raw_sub[1]
            canvas.paste(ai.crop(ai_sub).resize((tw, th), Image.LANCZOS), (raw_sub[0], raw_sub[1]))
        else:  # solid/empty tile (e.g. material:black) — just fill the cell
            tile = ai.crop(t["content_box"]).resize((cb[2] - cb[0], cb[3] - cb[1]), Image.LANCZOS)
            canvas.paste(tile, (cb[0], cb[1]))

    final_path = out_dir / "atlas_4x.png"
    canvas.save(final_path)

    # Overlay vs the raw (classical) upscale for a position sanity-check.
    raw = Image.open(out_dir / "atlas_4x_raw.png").convert("RGB")
    Image.blend(raw, canvas, 0.5).save(out_dir / "atlas_4x_overlay.png")

    db_path = None
    if db_copy:
        dst = model_dir / "atlas_4x.png"
        canvas.save(dst)
        db_path = str(dst.relative_to(_repo_root()))

    rc["status"] = "assembled"
    rc["atlas_4x_path"] = str(final_path.relative_to(_repo_root()))
    rc["db_copy_path"] = db_path
    rc_path.write_text(json.dumps(rc, indent=2) + "\n", encoding="utf-8")

    print(f"  assembled compact atlas_4x.png {cw4}x{ch4} (no padding)")
    print(f"  overlay (check positions): {out_dir / 'atlas_4x_overlay.png'}")
    if db_path:
        print(f"  db copy: {db_path}")
    return rc


def check(model_dir: Path, out_root: Path) -> dict:
    """Build a per-tile comparison of atlas_4x_raw (reference layout) vs atlas_4x
    (the AI result) so each tile can be verified to show the SAME content in the
    SAME place. Writes atlas_4x_check.png and prints a per-tile mismatch score."""
    from PIL import ImageDraw

    slug = model_slug(model_dir)
    out_dir = out_root / slug
    rc = json.loads((out_dir / RECOMPACT_NAME).read_text(encoding="utf-8"))
    raw = Image.open(out_dir / "atlas_4x_raw.png").convert("RGB")
    ai = Image.open(out_dir / "atlas_4x.png").convert("RGB")

    import numpy as np
    rows, row_h, pad = [], 120, 6
    print("  per-tile mismatch (raw vs AI, lower = better aligned; high => fix content_box):")
    for t in rc["tiles"]:
        if not t["ref"].startswith("pack:"):
            continue
        cb = t["compact_box"]
        rt = raw.crop(cb)
        at = ai.crop(cb)
        # alignment score: edge-map correlation is robust to the AI's extra sharpness;
        # here a simple downsized abs-diff of normalized tiles flags gross mispositions.
        small = (32, 32)
        a = np.asarray(rt.resize(small).convert("L")).astype(float)
        b = np.asarray(at.resize(small).convert("L")).astype(float)
        a -= a.mean(); b -= b.mean()
        score = round(float(np.abs(a - b).mean()), 1)
        w = max(1, round((cb[2] - cb[0]) * row_h / (cb[3] - cb[1])))
        strip = Image.new("RGB", (w * 2 + pad * 3, row_h + 18), (32, 32, 32))
        strip.paste(rt.resize((w, row_h)), (pad, 16))
        strip.paste(at.resize((w, row_h)), (w + pad * 2, 16))
        ImageDraw.Draw(strip).text((pad, 3), f"{t['ref']}  raw | AI   mismatch={score}",
                                   fill=(230, 230, 230))
        rows.append(strip)
        print(f"    {t['ref']:22} mismatch={score}")

    width = max(r.width for r in rows)
    sheet = Image.new("RGB", (width, sum(r.height for r in rows)), (20, 20, 20))
    y = 0
    for r in rows:
        sheet.paste(r, (0, y))
        y += r.height
    sheet.save(out_dir / "atlas_4x_check.png")
    print(f"  wrote {out_dir / 'atlas_4x_check.png'} — read it: each tile's raw and AI "
          "halves must show the SAME thing; a high mismatch means fix that content_box.")
    return rc


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    load_dotenv(_repo_root() / ".env")
    parser = argparse.ArgumentParser(description="Atlas remaster (prepare/assemble) by model name.")
    parser.add_argument("phase", choices=["prepare", "assemble", "check"])
    parser.add_argument("--model", required=True, help="Model name (cd_163) or repo path")
    parser.add_argument("--out-dir", default=OUT_DIR)
    parser.add_argument("--quality", choices=["low", "medium", "high"], default="high")
    parser.add_argument("--pad", type=int, default=16, help="Padded-atlas gutter (source px)")
    parser.add_argument("--no-db-copy", action="store_true")
    parser.add_argument("--dry-run", action="store_true", help="prepare without the AI call")
    parser.add_argument("--api-key")
    args = parser.parse_args()

    model_dir = resolve_model(args.model)
    out_root = _repo_root() / args.out_dir
    print(f"Model: {model_dir.relative_to(_repo_root())}")

    if args.phase == "prepare":
        client = None
        if not args.dry_run:
            if not HAS_OPENAI:
                print("Error: 'openai' not installed (pip install openai)")
                sys.exit(1)
            key = args.api_key or os.environ.get("OPENAI_API_KEY")
            if not key:
                print("Error: OPENAI_API_KEY not set (or use --dry-run)")
                sys.exit(1)
            client = OpenAI(api_key=key)
        prepare(model_dir, out_root, client, args.quality, args.pad, args.dry_run)
    elif args.phase == "assemble":
        assemble(model_dir, out_root, not args.no_db_copy)
    else:
        check(model_dir, out_root)


if __name__ == "__main__":
    main()
