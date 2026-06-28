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
  - crop each tile's content_box from atlas_padded_4x and resize it to 4x the
    SOURCE texture's size, writing one PER-TEXTURE file next to the source
    (model/<name>_4x.png, e.g. z_both_4x.png),
  - write a high-res thumbnail (image_4x.png) and report the model's AI cost.

atlas_4x is NOT produced here — it is derived later from the *_4x.png files by
the JS build/upload step (scripts/lib/atlas4x.mjs). Originals are never modified.

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
# Roots searched when resolving a bare model name. Trains live under db/train,
# motocars (buses/trams) under db/motocar.
DB_SEARCH_ROOTS = ("db/train", "db/motocar")

ATLAS_PROMPT = (
    "This is a flat texture atlas for an old 3D RAILROAD VEHICLE (a locomotive, "
    "railcar or coach) — several small low-resolution textures packed onto one sheet "
    "with black gaps between them, already upscaled and slightly blurry. Return a "
    "sharper, cleaner, higher-detail version. IMPORTANT: keep every element in EXACTLY "
    "the same position, size and layout — do not move, add, remove, rearrange, crop or "
    "recolour anything, and keep the black gaps between the tiles. Keep all logos, text, "
    "numbers and colours exactly as they are. LOGOS/EMBLEMS (e.g. the operator emblem): "
    "reproduce every logo with its correct, original orientation and shape — NEVER mirror, "
    "flip, rotate or reverse a logo, and make sure EVERY instance of the same logo on the "
    "sheet looks identical (same direction, not a mirror image of another). Render the "
    "vehicle's WHEELS as clean, perfectly ROUND circles (not ovals, not polygons). "
    "DE-PIXELATE: the source is a low-resolution pixel image, so every diagonal or curved "
    "feature — pantograph arms and frames, window and door frames, handrails, roof and body "
    "lining stripes, gear/bogie/wheel outlines, lamps — currently looks jagged and "
    "stair-stepped. Redraw these as SMOOTH, clean, continuous, anti-aliased lines and curves: "
    "remove the blocky pixel staircase entirely while keeping each feature's shape, position "
    "and colour. Straight horizontal/vertical edges stay crisp; only the jagged diagonals and "
    "curves get smoothed. Add fine, plausible detail to what is already present; do not invent "
    "new features."
)


MOTOCAR_ATLAS_PROMPT = (
    "This is a flat texture atlas for an old 3D ROAD/CITY VEHICLE (a bus, trolleybus "
    "or tram) — several small low-resolution textures packed onto one sheet with black "
    "gaps between them, already upscaled and slightly blurry. Return a sharper, cleaner, "
    "higher-detail version. IMPORTANT: keep every element in EXACTLY the same position, "
    "size and layout — do not move, add, remove, rearrange, crop or recolour anything, and "
    "keep the black gaps between the tiles. Keep all logos, text, line/route numbers, "
    "destination signs and colours exactly as they are. LOGOS/EMBLEMS (operator or "
    "manufacturer marks): reproduce every logo with its correct, original orientation and "
    "shape — NEVER mirror, flip, rotate or reverse a logo, and make sure EVERY instance of "
    "the same logo on the sheet looks identical (same direction, not a mirror image of "
    "another). Render the vehicle's WHEELS and headlights as clean, perfectly ROUND circles "
    "(not ovals, not polygons). DE-PIXELATE: the source is a low-resolution pixel image, so "
    "every diagonal or curved feature — window and door frames, body and roof lining stripes, "
    "bumpers, mirrors, wheel arches and outlines, lamps, and (on a tram) the pantograph or "
    "trolley arms — currently looks jagged and stair-stepped. Redraw these as SMOOTH, clean, "
    "continuous, anti-aliased lines and curves: remove the blocky pixel staircase entirely "
    "while keeping each feature's shape, position and colour. Straight horizontal/vertical "
    "edges stay crisp; only the jagged diagonals and curves get smoothed. Add fine, plausible "
    "detail to what is already present; do not invent new features."
)


def is_motocar(model_dir: Path) -> bool:
    """True if the model lives under db/motocar (a bus/tram), not db/train."""
    return "motocar" in model_dir.relative_to(_repo_root()).parts


def motocar_prompt_for(model_dir: Path) -> str:
    """Motocar atlas prompt plus the model's catalogue identifier as context. The
    identifier (e.g. 'bus_kar_b731' = Karosa B 731 bus, 'tram_ckd_t3' = ČKD Tatra T3)
    is a FILENAME describing the vehicle type/manufacturer/model — never paint it on
    the vehicle. Any destination sign or line/route number already painted stays as-is."""
    rel = model_dir.relative_to(_repo_root())
    ident = model_dir.name
    return MOTOCAR_ATLAS_PROMPT + (
        f" Context — this vehicle's catalogue identifier is '{ident}' (path '{rel}'). "
        "This identifier is a FILENAME describing the vehicle's type, manufacturer and "
        "model (e.g. 'bus_kar_b731' = a Karosa B 731 city bus; 'tram_ckd_t3' = a ČKD "
        "Tatra T3 tram), NOT text painted on the vehicle: NEVER render the identifier "
        "verbatim anywhere on it. Keep whatever destination sign, line/route number or "
        "operator name is already painted exactly as-is; use the identifier only to "
        "recognise the vehicle's shape and proportions when the source is too blurry. Do "
        "not invent a different vehicle than the one implied by the identifier."
    )


def atlas_prompt_for(model_dir: Path) -> str:
    """Atlas prompt plus the model's catalogue identifier as context, so the model
    can resolve a blurry running number to the correct class.

    The identifier is 'operator_class' (e.g. 'cd_111' = operator ČD, class 111) — it is
    NOT the running number and must never be painted on the vehicle verbatim. Czech
    running numbers are 'CLASS SERIAL-CHECK' (e.g. '111 022-5' / '111.022'), with the
    operator (ČD) shown only as a separate emblem, never glued to the class digits.

    Motocars (buses/trams under db/motocar) use a different, road-vehicle prompt."""
    if is_motocar(model_dir):
        return motocar_prompt_for(model_dir)
    rel = model_dir.relative_to(_repo_root())
    ident = model_dir.name
    return ATLAS_PROMPT + (
        f" Context — this vehicle's catalogue identifier is '{ident}' (path '{rel}'). "
        "This identifier is a FILENAME, not the number painted on the vehicle: NEVER "
        "render it verbatim on the loco. Read it as 'operator_class' — the letters "
        "before the underscore are the railway operator (here 'cd' = the Czech operator "
        "České dráhy, written as the 'ČD' emblem) and the digits are the locomotive "
        "CLASS (so 'cd_111' = ČD class 111). A real Czech running number is written as "
        "'CLASS SERIAL-CHECK', e.g. '111 022-5' or '111.022' — the class digits, then a "
        "SEPARATE serial, never fused to the operator letters. So do NOT paint 'cd_111', "
        "'CD111', 'cd 111' or similar as the number. Keep whatever running number is "
        "already painted on the vehicle exactly as-is; only use this identifier to "
        "resolve the CLASS digits when the painted number is too blurry to read for "
        "certain (e.g. an unreadable number on 'cd_111' is a '111 ...'-series number, "
        "not 151). The 'ČD' operator emblem may still appear separately as the operator "
        "mark. Do not invent a different class than the one implied by the identifier."
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
        d for search_root in DB_SEARCH_ROOTS
        for d in sorted((root / search_root).rglob(name_or_path))
        if d.is_dir() and (d / "model" / "index.xml").exists()
    ]
    if not matches:
        print(f"Error: no model named '{name_or_path}' found under "
              f"{', '.join(r + '/' for r in DB_SEARCH_ROOTS)} (needs model/index.xml).")
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
            # The detected cell is gutter-bounded, so its POSITION is accurate even when
            # the AI re-proportioned the tile; assemble re-tightens content within it.
            # Only a GROSS aspect mismatch means real mis-segmentation (e.g. gear/end
            # tiles merged into one run) — there fall back to the aspect-correct mapped
            # cell. A moderate mismatch (AI redrew one tile wider/taller, like a cab
            # front) keeps the detected cell but is flagged for review.
            ratio = aspect(box) / max(1e-6, aspect(mc))
            if 0.8 <= ratio <= 1.25:
                cell, conf = box, True
            elif 0.5 <= ratio <= 2.0:
                cell, conf = box, False   # single redrawn tile — trust detected position
            else:
                cell, conf = mc, False    # likely merged/over-split — use mapped cell
            out[ref] = {"cell": cell, "content": cell, "fills_frame": conf}
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


SPARSE_FRAC = 0.20      # source with <20% bright pixels => use the bbox metric
SPARSE_SCALE = 3.0      # rescale the bbox metric onto the profile metric's threshold


def alignment_score(ai_tile: Image.Image, src: Image.Image, threshold: int = 24) -> float:
    """Position-sensitive, detail-robust mismatch between an AI tile and its source.
    Lower = better aligned; compare against ``ALIGN_FAIL`` (see ``check``).

    Two regimes — the profile metric is unreliable on near-black tiles (normalising an
    almost-flat profile amplifies noise, so an aligned sparse tile scores as high as a
    broken one):

    * DENSE source (>=20% bright px): compare the NORMALIZED row/column brightness
      PROFILES (where the dark/bright bands sit), not raw pixels — so the AI re-drawing a
      cab face or running number scores LOW, but a vertically squashed/shifted tile (the
      classic x_* cab drift) scores HIGH because its bands move.
    * SPARSE source (<20% bright px, e.g. a gear/end tile that is one small blob on black):
      compare the normalized CONTENT BBOX corners — does the blob land where the source's
      blob is? Rescaled (×3) so the same threshold applies as the dense metric.
    """
    import numpy as np

    s = np.asarray(src.convert("L"), float)
    a = np.asarray(ai_tile.resize(src.size).convert("L"), float)

    if (s > threshold).mean() < SPARSE_FRAC:
        def bbox(arr):
            m = arr > threshold
            ys, xs = m.nonzero()
            if len(xs) == 0:
                return None
            h, w = arr.shape
            return np.array([xs.min() / w, ys.min() / h,
                             (xs.max() + 1) / w, (ys.max() + 1) / h])

        bs, ba = bbox(s), bbox(a)
        if bs is None or ba is None:
            return 0.0 if bs is ba else 1.0
        return float(np.abs(bs - ba).mean()) * SPARSE_SCALE

    def prof(g):
        col, row = g.mean(axis=0), g.mean(axis=1)

        def nz(v):
            v = v - v.min()
            m = v.max()
            return v / m if m > 0 else v

        return nz(col), nz(row)

    cs, rs = prof(s)
    ca, ra = prof(a)
    return float(np.abs(cs - ca).mean() + np.abs(rs - ra).mean())


def refine_content_box(ai: Image.Image, box, src_path: Path) -> list[int]:
    """Locally nudge a detected ``content_box`` so the cropped+resized tile best matches
    its SOURCE texture, correcting the small per-tile drift gpt-image leaves — most often
    the ~15-25px vertical squash on near-full cab tiles (``x_dec``/``x_inc``) that the
    gutter detector under-sizes. Coarse-to-fine search of each edge; keeps the source as
    the ground truth for POSITION (the AI changes detail, not the silhouette). No-op when
    the source is missing.
    """
    try:
        src = Image.open(src_path).convert("L")
    except Exception:  # noqa: BLE001
        return list(box)
    import numpy as np

    sarr = np.asarray(src, float)
    iw, ih = ai.size

    def score(b):
        b = (max(0, b[0]), max(0, b[1]), min(iw, b[2]), min(ih, b[3]))
        if b[2] - b[0] < 2 or b[3] - b[1] < 2:
            return 1e9
        c = np.asarray(ai.crop(b).convert("L").resize(src.size), float)
        return float(np.abs(c - sarr).mean())

    x0, y0, x1, y1 = (int(v) for v in box)
    bw, bh = x1 - x0, y1 - y0
    cur = [x0, y0, x1, y1]
    # amplitude covers the observed cab drift; coarse pass then a tight refine
    ax, ay = max(3, round(bw * 0.12)), max(3, round(bh * 0.14))
    for amp_x, amp_y, step in [(ax, ay, max(2, ax // 3)), (3, 3, 1)]:
        bx0, by0, bx1, by1 = cur
        best = (score(cur), list(cur))
        for d0 in range(-amp_x, amp_x + 1, step):
            for e0 in range(-amp_y, amp_y + 1, step):
                for d1 in range(-amp_x, amp_x + 1, step):
                    for e1 in range(-amp_y, amp_y + 1, step):
                        b = [bx0 + d0, by0 + e0, bx1 + d1, by1 + e1]
                        if b[2] - b[0] < bw * 0.6 or b[3] - b[1] < bh * 0.6:
                            continue
                        sc = score(b)
                        if sc < best[0]:
                            best = (sc, b)
        cur = best[1]
    return [max(0, cur[0]), max(0, cur[1]), min(iw, cur[2]), min(ih, cur[3])]


def safe_ref_name(ref: str) -> str:
    """Filesystem-safe name for a texture ref (pack:z_dec.bmp -> pack_z_dec.bmp)."""
    return ref.replace(":", "_").replace("/", "_")


def fourx_name(name: str) -> str:
    """Per-texture 4x output name (z_both.bmp -> z_both_4x.png)."""
    stem = name.rsplit(".", 1)[0] if "." in name else name
    return f"{stem}_4x.png"


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

    # ... and the padded atlas (sent to the AI). Pre-upscale it LOCALLY to the exact API
    # output size so the model does no upscaling itself. Use NEAREST, not LANCZOS: the
    # sources are tiny pixel images and a LANCZOS pre-upscale BLURS fine detail (a 6px
    # 'ČD' emblem, window mullions, gear) into mush BEFORE the model ever sees it — the
    # model then hallucinates (e.g. 'ČD' -> two round 'eyes'). NEAREST preserves the
    # exact pixel blocks, so the model sees the true shapes and can both keep them correct
    # AND smooth their jagged edges (see the de-pixelate clause in ATLAS_PROMPT). Saved as
    # atlas_padded_4x_raw.png.
    padded_png, padded_map, _ = build_padded_atlas(model_sub, model_xml, pad)
    padded = Image.open(io.BytesIO(padded_png)).convert("RGB")
    pw, ph = padded.size
    size = pick_size(pw * UPSCALE, ph * UPSCALE)
    aw, ah = (int(v) for v in size.split("x"))
    raw_for_ai = padded.resize((aw, ah), Image.NEAREST)
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
        # Auto-fit the content_box to the source (fixes the cab-tile vertical drift the
        # gutter detector under-sizes), so the per-texture 4x lands in the right place
        # without manual editing. Only for real pack textures with a source file.
        if ref.startswith("pack:"):
            src_path = model_sub / ref[len("pack:"):]
            refined = refine_content_box(ai, content, src_path)
            if refined != list(content):
                content = refined
                fills = False  # nudged — worth a glance in check
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
    model_sub = model_dir / "model"

    # Crop each tile's content_box from the AI result and resize it to 4x the SOURCE
    # texture's own size — one independent PER-TEXTURE file (z_both.bmp -> z_both_4x.png).
    # The render atlas_4x is derived later by the JS build from these files; we also write
    # an inspection-only atlas_4x.png in generated/ below (build_inspection_atlas).
    written: list[str] = []
    upscaled: dict[str, Image.Image] = {}
    for t in rc["tiles"]:
        ref = t["ref"]
        if not ref.startswith("pack:"):
            continue
        name = ref[len("pack:"):]
        src_path = model_sub / name
        try:
            ow, oh = Image.open(src_path).size
        except Exception:  # noqa: BLE001 - fall back to the compact box dims
            cb = t["compact_box"]
            ow = max(1, (cb[2] - cb[0]) // UPSCALE)
            oh = max(1, (cb[3] - cb[1]) // UPSCALE)
        tile = ai.crop(t["content_box"]).resize((ow * UPSCALE, oh * UPSCALE), Image.LANCZOS)
        out_name = fourx_name(name)
        tile.save(out_dir / out_name)            # debug copy in generated/
        if db_copy:
            tile.save(model_sub / out_name)       # deliverable next to the source
        upscaled[ref] = tile
        written.append(out_name)

    # High-res thumbnail (image_4x.png) from the AI side texture — no extra AI call.
    image_4x_path = build_thumbnail(upscaled, model_dir, out_dir, db_copy)

    # Rebuild the combined inspection atlas (NOT used for rendering — the JS build derives
    # the real one from the per-texture files; this is the human-analysis copy, kept fresh
    # so it reflects THIS assemble instead of a stale earlier run).
    build_inspection_atlas(rc, upscaled, out_dir)

    cost = rc.get("cost_usd", 0.0)
    rc["status"] = "assembled"
    rc["textures_4x"] = written
    rc["db_copy"] = db_copy
    rc["image_4x_path"] = image_4x_path
    rc_path.write_text(json.dumps(rc, indent=2) + "\n", encoding="utf-8")

    print(f"  wrote {len(written)} per-texture 4x file(s): {', '.join(written)}")
    if db_copy:
        print(f"  saved next to the sources in {model_sub.relative_to(_repo_root())}/")
    if image_4x_path:
        print(f"  thumbnail: {image_4x_path}")
    print(f"  inspection atlas: {(out_dir / 'atlas_4x.png').relative_to(_repo_root())} (+ _overlay)")
    print(f"  AI upscaling cost for this model: ${cost:.4f}")
    return rc


def build_inspection_atlas(rc: dict, upscaled: dict[str, Image.Image], out_dir: Path) -> None:
    """Repack the per-texture *_4x tiles onto the compact layout at 4x -> atlas_4x.png,
    and blend it 50/50 with the classical atlas_4x_raw.png -> atlas_4x_overlay.png.

    For human inspection only (mirrors the JS `buildAtlas4x`). Tiles are placed at their
    `compact_box` origin — the exact 4x compact positions the renderer's UVs expect — so
    the overlay reveals any drift between the classical and AI upscales.
    """
    cw4, ch4 = rc["compact_size"]
    atlas = Image.new("RGB", (cw4, ch4), (0, 0, 0))
    for t in rc["tiles"]:
        tile = upscaled.get(t["ref"])
        if tile is None:
            continue
        x0, y0, x1, y1 = t["compact_box"]
        if tile.size != (x1 - x0, y1 - y0):  # guard rounding: fit to the slot
            tile = tile.resize((max(1, x1 - x0), max(1, y1 - y0)), Image.LANCZOS)
        if x0 + tile.width <= cw4 and y0 + tile.height <= ch4:
            atlas.paste(tile, (x0, y0))
    atlas.save(out_dir / "atlas_4x.png")

    raw_path = out_dir / "atlas_4x_raw.png"
    if raw_path.exists():
        raw = Image.open(raw_path).convert("RGB")
        if raw.size != atlas.size:
            raw = raw.resize(atlas.size, Image.LANCZOS)
        Image.blend(raw, atlas, 0.5).save(out_dir / "atlas_4x_overlay.png")


# Side textures, in preference order, used as the high-res thumbnail source.
THUMBNAIL_SIDE_REFS = ("pack:z_both.bmp", "pack:z_dec.bmp", "pack:z_inc.bmp")


def build_thumbnail(upscaled: dict[str, "Image.Image"], model_dir: Path,
                    out_dir: Path, db_copy: bool) -> str | None:
    """Build image_4x.png from an already-upscaled side texture (no extra AI call).

    Picks the first available side tile and, when a source image.bmp exists, resizes
    it to image.bmp's size x4 so it is a drop-in higher-res thumbnail.
    """
    # Match by SUFFIX, not exact ref: texture files are often prefixed (em475_z_both.bmp),
    # so an exact "pack:z_both.bmp" lookup would miss them and skip the thumbnail entirely.
    side = None
    for suffix in THUMBNAIL_SIDE_REFS:
        bare = suffix[len("pack:"):]
        side = next((img for ref, img in upscaled.items()
                     if ref.startswith("pack:") and ref[len("pack:"):].endswith(bare)), None)
        if side is not None:
            break
    if side is None:
        return None
    thumb = side
    image_bmp = model_dir / "image.bmp"
    if image_bmp.exists():
        try:
            w, h = Image.open(image_bmp).size
            thumb = side.resize((w * UPSCALE, h * UPSCALE), Image.LANCZOS)
        except Exception:  # noqa: BLE001 - aspect match is best-effort
            pass

    thumb.save(out_dir / "image_4x.png")
    if db_copy:
        dst = model_dir / "image_4x.png"
        thumb.save(dst)
        return str(dst.relative_to(_repo_root()))
    return str((out_dir / "image_4x.png").relative_to(_repo_root()))


def check(model_dir: Path, out_root: Path) -> dict:
    """Build a per-texture comparison of each source texture's classical 4x upscale
    vs the AI <name>_4x.png so each can be verified to show the SAME content in the
    SAME place. Writes textures_4x_check.png and prints a per-texture mismatch score."""
    from PIL import ImageDraw

    slug = model_slug(model_dir)
    out_dir = out_root / slug
    rc = json.loads((out_dir / RECOMPACT_NAME).read_text(encoding="utf-8"))
    model_sub = model_dir / "model"

    import numpy as np
    rows, row_h, pad = [], 120, 6
    # align score above this => the tile is shifted/squashed, not merely re-detailed.
    # Tuned so an aligned (even heavily re-drawn / de-dithered) tile passes — dense cab
    # ~0.14-0.21, sparse blob ~0 — while a ~20px vertical drift (~0.27+) or a misplaced
    # blob (~0.38) fails. See alignment_score for the dense/sparse regimes.
    ALIGN_FAIL = 0.24
    flagged: list[str] = []
    print("  per-texture mismatch (lower=better). align catches shift/squash; review=ALIGN: re-fit content_box:")
    for t in rc["tiles"]:
        ref = t["ref"]
        if not ref.startswith("pack:"):
            continue
        name = ref[len("pack:"):]
        fourx = out_dir / fourx_name(name)
        if not fourx.exists():
            continue
        at = Image.open(fourx).convert("RGB")
        src = model_sub / name
        rt = (Image.open(src).convert("RGB").resize(at.size, Image.LANCZOS)
              if src.exists() else at)
        # mismatch: downsized abs-diff — coarse, also rises with the AI's added detail.
        small = (32, 32)
        a = np.asarray(rt.resize(small).convert("L")).astype(float)
        b = np.asarray(at.resize(small).convert("L")).astype(float)
        a -= a.mean(); b -= b.mean()
        score = round(float(np.abs(a - b).mean()), 1)
        # align: position-sensitive but detail-robust (profile-based). This is the one
        # that reliably catches the x_* cab tiles drifting vertically — read it FIRST.
        align = round(alignment_score(at, Image.open(src).convert("L")), 3) if src.exists() else 0.0
        bad = align > ALIGN_FAIL
        if bad:
            flagged.append(name)
        w = max(1, round(at.width * row_h / at.height))
        strip = Image.new("RGB", (w * 2 + pad * 3, row_h + 18), (32, 32, 32))
        strip.paste(rt.resize((w, row_h)), (pad, 16))
        strip.paste(at.resize((w, row_h)), (w + pad * 2, 16))
        tag = "  <<< REVIEW: MISALIGNED" if bad else ""
        ImageDraw.Draw(strip).text((pad, 3),
                                   f"{name}  raw | AI   mismatch={score} align={align}{tag}",
                                   fill=(255, 90, 90) if bad else (230, 230, 230))
        if bad:  # red frame so a misaligned tile is obvious at a glance in the sheet
            ImageDraw.Draw(strip).rectangle([0, 0, strip.width - 1, strip.height - 1],
                                            outline=(255, 60, 60), width=3)
        rows.append(strip)
        print(f"    {name:22} mismatch={score}  align={align}"
              f"{'   <<< REVIEW: MISALIGNED (re-fit content_box)' if bad else ''}")

    if not rows:
        print("  [warn] no *_4x.png found in generated/ — run `assemble` first")
        return rc
    if flagged:
        print(f"  [REVIEW] {len(flagged)} tile(s) look misaligned (align>{ALIGN_FAIL}): "
              f"{', '.join(flagged)} — re-fit their content_box, re-assemble, re-check.")
    else:
        print("  [ok] all tiles within alignment tolerance.")
    width = max(r.width for r in rows)
    sheet = Image.new("RGB", (width, sum(r.height for r in rows)), (20, 20, 20))
    y = 0
    for r in rows:
        sheet.paste(r, (0, y))
        y += r.height
    sheet.save(out_dir / "textures_4x_check.png")
    print(f"  wrote {out_dir / 'textures_4x_check.png'} — read it: each texture's raw and "
          "AI halves must show the SAME thing; a high mismatch means fix that content_box.")
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
