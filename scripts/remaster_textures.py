#!/usr/bin/env python3
"""
Atlas-based texture remastering.

For each model listed in INPUT_REMASTER, this processes a SINGLE file — the model's
combined texture atlas — instead of each UV sheet separately:

    1. Build atlas.png for the model (shelf-packed UV textures, via atlas.py).
    2. Upscale it 4x locally with PIL (LANCZOS)        -> atlas_4x_raw.png
    3. Send atlas_4x_raw.png to the OpenAI image model with a short generic prompt
       ("keep every element in the same position, only sharpen and add detail").
    4. Save the result resized to exactly 4x the atlas size -> atlas_4x.png

No per-texture heuristics and no analysis pass — the model just sharpens the packed
sheet in place. Originals are never modified; a git-tracked atlas_4x.png copy is
written next to the model.

Usage:
    python remaster_textures.py [--dry-run] [--limit N] [--quality high]

Configure OPENAI_API_KEY in a .env file at the project root (or the environment).
"""

import argparse
import base64
import hashlib
import io
import json
import os
import sys
from pathlib import Path

from dotenv import load_dotenv
from PIL import Image

from atlas import build_texture_atlas
from atlas_padded import build_padded_atlas
from upload_loco import resolve_input_dirs
from upload_utils import _repo_root

try:
    from openai import OpenAI
    HAS_OPENAI = True
except ImportError:
    HAS_OPENAI = False


INPUT_FILE = "INPUT_REMASTER"
OUT_DIR = "generated"
MANIFEST_NAME = "manifest.json"
MODEL = "gpt-image-2"
UPSCALE = 4  # target output scale relative to the atlas

# gpt-image only emits these fixed sizes; we pick the closest by aspect ratio.
SIZE_LANDSCAPE = "1536x1024"
SIZE_PORTRAIT = "1024x1536"
SIZE_SQUARE = "1024x1024"

PROMPT = (
    "This is a texture atlas — several small low-resolution game textures packed "
    "onto one sheet, upscaled and slightly blurry. Return a sharper, higher-"
    "resolution version. IMPORTANT: keep every element in EXACTLY the same position, "
    "size and layout — do not move, add, remove, rearrange, crop or recolour "
    "anything, and keep all logos, text, numbers and colours exactly as they are. "
    "Only sharpen edges and add fine, plausible detail to what is already present."
)
PROMPT_HASH = hashlib.sha256(PROMPT.encode("utf-8")).hexdigest()

# --- Pricing (USD per million tokens; approximate — verify against OpenAI) ----
PRICE_TEXT_INPUT_PER_M = 5.0
PRICE_IMAGE_INPUT_PER_M = 10.0
PRICE_IMAGE_OUTPUT_PER_M = {"low": 40.0, "medium": 40.0, "high": 40.0}


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def pick_size(w: int, h: int) -> str:
    """Closest supported output size by aspect ratio."""
    ratio = w / h if h else 1.0
    if ratio > 1.3:
        return SIZE_LANDSCAPE
    if ratio < 0.77:
        return SIZE_PORTRAIT
    return SIZE_SQUARE


def model_slug(model_dir: Path) -> str:
    """Flat output-folder name: repo-relative path with '/' -> '__'."""
    return str(model_dir.relative_to(_repo_root())).replace("/", "__").replace(" ", "_")


def call_openai_edit(client, png_bytes: bytes, size: str, quality: str, prompt: str = PROMPT):
    """Sharpen a single image via images.edit (optionally with a custom prompt)."""
    return client.images.edit(
        model=MODEL,
        image=("atlas.png", io.BytesIO(png_bytes), "image/png"),
        prompt=prompt,
        size=size,
        quality=quality,
    )


def extract_usage(resp) -> dict:
    usage = getattr(resp, "usage", None)
    if usage is None:
        return {}
    details = getattr(usage, "input_tokens_details", None)
    return {
        "input_tokens": getattr(usage, "input_tokens", 0),
        "output_tokens": getattr(usage, "output_tokens", 0),
        "input_text_tokens": getattr(details, "text_tokens", 0) if details else 0,
        "input_image_tokens": getattr(details, "image_tokens", 0) if details else 0,
    }


def compute_cost(usage: dict, quality: str) -> float:
    if not usage:
        return 0.0
    out_price = PRICE_IMAGE_OUTPUT_PER_M.get(quality, 40.0)
    return round(
        usage.get("input_text_tokens", 0) / 1e6 * PRICE_TEXT_INPUT_PER_M
        + usage.get("input_image_tokens", 0) / 1e6 * PRICE_IMAGE_INPUT_PER_M
        + usage.get("output_tokens", 0) / 1e6 * out_price,
        6,
    )


def build_contact_sheet(atlas: Image.Image, raw4x: Image.Image, final: Image.Image,
                        out_path: Path) -> None:
    """Write a 3-panel before/after comparison (atlas | classical 4x | AI 4x)."""
    labels = ["atlas", "classical 4x", "AI 4x"]
    panel_h, pad = 256, 8
    scaled = []
    for p in (atlas, raw4x, final):
        p = p.convert("RGB")
        w = max(1, round(p.width * panel_h / p.height))
        scaled.append(p.resize((w, panel_h), Image.NEAREST))
    total_w = sum(p.width for p in scaled) + pad * (len(scaled) + 1)
    sheet = Image.new("RGB", (total_w, panel_h + pad * 2 + 16), (32, 32, 32))
    from PIL import ImageDraw
    draw = ImageDraw.Draw(sheet)
    x = pad
    for label, p in zip(labels, scaled):
        sheet.paste(p, (x, pad + 16))
        draw.text((x, pad // 2), label, fill=(230, 230, 230))
        x += p.width + pad
    out_path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(out_path, "PNG")


def load_manifest(path: Path) -> dict:
    if path.exists():
        try:
            return json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            pass
    return {"prompt": PROMPT, "prompt_hash": PROMPT_HASH, "entries": {}}


def save_manifest(path: Path, manifest: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(manifest, indent=2, ensure_ascii=False) + "\n",
                    encoding="utf-8")


# ---------------------------------------------------------------------------
# Core per-model processing
# ---------------------------------------------------------------------------

def process_model(model_dir: Path, atlas_png: bytes, atlas_hash: str, out_root: Path,
                  slug: str, client, quality: str, dry_run: bool, db_copy: bool) -> dict:
    """Remaster one model's (already built) atlas. Returns its manifest entry."""
    atlas = Image.open(io.BytesIO(atlas_png)).convert("RGB")
    w, h = atlas.size
    tw, th = w * UPSCALE, h * UPSCALE

    out_dir = out_root / slug
    atlas_path = out_dir / "atlas.png"
    raw_path = out_dir / "atlas_4x_raw.png"
    final_path = out_dir / "atlas_4x.png"
    contact_path = out_dir / "atlas_contact.png"
    db_path = model_dir / "atlas_4x.png"  # next to the model (git-tracked deliverable)

    entry = {
        "model": str(model_dir.relative_to(_repo_root())),
        "atlas_size": [w, h],
        "final_size": [tw, th],
        "size_requested": pick_size(tw, th),
        "atlas_path": str(atlas_path.relative_to(_repo_root())),
        "atlas_4x_raw_path": str(raw_path.relative_to(_repo_root())),
        "atlas_4x_path": str(final_path.relative_to(_repo_root())),
        "db_copy_path": None,
        "model_used": MODEL,
        "quality": quality,
        "prompt_hash": PROMPT_HASH,
        "atlas_hash": atlas_hash,
        "usage": {},
        "cost_usd": 0.0,
        "status": "ok",
        "error": None,
    }

    out_dir.mkdir(parents=True, exist_ok=True)
    atlas.save(atlas_path, "PNG")
    raw4x = atlas.resize((tw, th), Image.LANCZOS)
    raw4x.save(raw_path, "PNG")

    if dry_run:
        # No API call: the classical upscale stands in for the AI result so the
        # layout/contact/manifest logic can be checked for free. No db/ write.
        raw4x.save(final_path, "PNG")
        build_contact_sheet(atlas, raw4x, raw4x, contact_path)
        entry["status"] = "dry-run"
        return entry

    try:
        buf = io.BytesIO()
        raw4x.save(buf, "PNG")
        resp = call_openai_edit(client, buf.getvalue(), entry["size_requested"], quality)
        out_bytes = base64.b64decode(resp.data[0].b64_json)
        # Resize the (fixed-size) model output back to the exact 4x atlas dims so
        # the packed sub-textures stay aligned with the atlas UV map.
        final = Image.open(io.BytesIO(out_bytes)).convert("RGB").resize((tw, th), Image.LANCZOS)
        final.save(final_path, "PNG")
        build_contact_sheet(atlas, raw4x, final, contact_path)

        if db_copy:
            final.save(db_path, "PNG")
            entry["db_copy_path"] = str(db_path.relative_to(_repo_root()))

        entry["usage"] = extract_usage(resp)
        entry["cost_usd"] = compute_cost(entry["usage"], quality)
    except Exception as exc:  # noqa: BLE001 - record and continue the batch
        entry["status"] = "error"
        entry["error"] = f"{type(exc).__name__}: {exc}"

    return entry


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    load_dotenv(_repo_root() / ".env")

    parser = argparse.ArgumentParser(description="Remaster model texture atlases with gpt-image-2.")
    parser.add_argument("--api-key", help="OpenAI API key (overrides .env / env)")
    parser.add_argument("--input", default=INPUT_FILE, help=f"Input file (default: {INPUT_FILE})")
    parser.add_argument("--out-dir", default=OUT_DIR, help=f"Output dir (default: {OUT_DIR})")
    parser.add_argument("--quality", choices=["low", "medium", "high"], default="high")
    parser.add_argument("--no-db-copy", action="store_true",
                        help="Do not write atlas_4x.png next to the model in db/")
    parser.add_argument("--dry-run", action="store_true",
                        help="Skip the API call; still write atlas/raw/contact/manifest")
    parser.add_argument("--limit", type=int, default=0,
                        help="Cap number of models processed this run (0 = no cap)")
    parser.add_argument("--padded", action="store_true",
                        help="POC: use the padded-grid atlas (atlas_padded.py) instead of the "
                             "real packed atlas, to test whether sub-textures keep their positions")
    parser.add_argument("--pad", type=int, default=8,
                        help="Gutter padding (source px) for --padded (default 8)")
    args = parser.parse_args()

    db_copy = not args.no_db_copy

    client = None
    if not args.dry_run:
        if not HAS_OPENAI:
            print("Error: the 'openai' package is not installed. Run: pip install openai")
            sys.exit(1)
        api_key = args.api_key or os.environ.get("OPENAI_API_KEY")
        if not api_key:
            print("Error: OPENAI_API_KEY not set (add it to .env or pass --api-key, or use --dry-run)")
            sys.exit(1)
        client = OpenAI(api_key=api_key)

    input_file = _repo_root() / args.input
    print(f"Reading input from: {input_file}")
    model_dirs = resolve_input_dirs(input_file)
    if not model_dirs:
        print("No model directories found.")
        sys.exit(1)
    print(f"Found {len(model_dirs)} model(s) to process.\n")

    out_root = _repo_root() / args.out_dir
    manifest_path = out_root / MANIFEST_NAME
    manifest = load_manifest(manifest_path)
    manifest["prompt"] = PROMPT
    manifest["prompt_hash"] = PROMPT_HASH
    entries: dict = manifest["entries"]

    processed = 0
    total_cost = 0.0
    counts = {"ok": 0, "cached": 0, "dry-run": 0, "error": 0, "skip": 0}

    for mi, (_override, model_dir) in enumerate(model_dirs, 1):
        if args.limit and processed >= args.limit:
            break
        slug = model_slug(model_dir)
        model_rel = str(model_dir.relative_to(_repo_root()))
        print(f"[{mi}/{len(model_dirs)}] {model_rel} -> {slug}")

        # Build the atlas first (cheap, local) so we can key the resume guard on it.
        model_sub = model_dir / "model"
        model_xml = model_sub / "index.xml"
        if not model_xml.exists():
            print("  [skip] no model/index.xml")
            counts["skip"] += 1
            continue
        if args.padded:
            atlas_png, _m, _r = build_padded_atlas(model_sub, model_xml, args.pad)
        else:
            atlas_png, _m, _r = build_texture_atlas(model_sub, model_xml)
        if not atlas_png:
            print("  [skip] no textures in atlas")
            counts["skip"] += 1
            continue
        atlas_hash = hashlib.sha256(atlas_png).hexdigest()

        final_path = out_root / slug / "atlas_4x.png"
        prev = entries.get(slug)
        if (not args.dry_run and prev and prev.get("status") == "ok"
                and final_path.exists() and prev.get("atlas_hash") == atlas_hash):
            print("  [cached] atlas unchanged")
            counts["cached"] += 1
            continue

        entry = process_model(model_dir, atlas_png, atlas_hash, out_root, slug, client,
                              args.quality, args.dry_run, db_copy)
        entries[slug] = entry
        save_manifest(manifest_path, manifest)
        status = entry["status"]
        counts[status] = counts.get(status, 0) + 1
        total_cost += entry.get("cost_usd", 0.0)
        processed += 1
        if status == "error":
            print(f"  [error] {entry['error']}")
        else:
            print(f"  [{status}] atlas_4x.png "
                  f"({entry['final_size'][0]}x{entry['final_size'][1]}, ${entry['cost_usd']:.4f})")

    save_manifest(manifest_path, manifest)
    print(f"\nDone. {counts['ok']} ok, {counts['cached']} cached, {counts['dry-run']} dry-run, "
          f"{counts['error']} error, {counts['skip']} skipped. Total cost: ${total_cost:.4f}")
    print(f"Manifest: {manifest_path}")


if __name__ == "__main__":
    main()
