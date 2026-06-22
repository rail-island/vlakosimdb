#!/usr/bin/env python3
"""
Batch-remaster old, low-resolution train model textures using the OpenAI image
editing API (gpt-image-1).

For each train model listed in INPUT_REMASTER, every BMP texture (the model/*.bmp
UV sheets plus the loco-level image.bmp thumbnail) is upscaled to a faithful
high-resolution version at exactly 4x the original dimensions. Originals are never
modified — all output goes under generated/.

Usage:
    python remaster_textures.py [--dry-run] [--limit N] [--quality medium]

Configure OPENAI_API_KEY in a .env file at the project root (or the environment),
or pass it with --api-key. INPUT_REMASTER uses the same syntax as INPUT_LOCO.

Pipeline per texture:
    1. BMP -> RGB PNG (in memory, recording exact source w,h)
    2. images.edit(gpt-image-1, primary + sibling references) at the nearest API size
    3. save raw output         -> generated/<slug>/<texture>_ai_raw.png
    4. crop/pad + resize to exact (w*4, h*4)
                               -> generated/<slug>/<texture>_4x.png
    5. 4-panel contact sheet   -> generated/<slug>/<texture>_contact.png
    6. record entry in           generated/manifest.json
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
from PIL import Image, ImageDraw

from upload_utils import _repo_root, file_content_hash  # noqa: F401
from upload_loco import resolve_input_dirs

try:
    from openai import OpenAI
    HAS_OPENAI = True
except ImportError:
    HAS_OPENAI = False


INPUT_FILE = "INPUT_REMASTER"
OUT_DIR = "generated"
MANIFEST_NAME = "manifest.json"
MODEL = "gpt-image-1"

# The thumbnail lives at the loco root; the UV sheets live in model/.
THUMBNAIL_NAME = "image.bmp"

PROMPT = (
    "Edit the primary train texture and remaster it as an aggressive 4x "
    "high-resolution AI texture. Preserve the original layout, proportions, "
    "orientation, black background, and UV sheet structure exactly. Do not "
    "redesign the train, move parts, crop the texture, add new views, or change "
    "the color scheme. This is a low-resolution train texture sheet, so infer "
    "plausible missing details: sharpen edges, clean aliasing, refine windows, "
    "doors, vents, roof details, panel seams, undercarriage details, lights, "
    "buffers, pantographs, pipes, rivets, shadows, glass reflections, and "
    "metallic surfaces. Keep markings, logos, numbers, and text as close to the "
    "source as possible; if uncertain, prefer preserving the original shape over "
    "inventing new text. The output should look like a faithful high-resolution "
    "remaster of the same old texture, not a pixelated enlargement. Output a "
    "single standalone texture sheet."
)
PROMPT_HASH = hashlib.sha256(PROMPT.encode("utf-8")).hexdigest()

UPSCALE = 4  # target output scale relative to the source

# gpt-image-1 only emits these fixed sizes.
SIZE_LANDSCAPE = "1536x1024"
SIZE_PORTRAIT = "1024x1536"
SIZE_SQUARE = "1024x1024"

# --- Pricing -------------------------------------------------------------
# gpt-image-1 is billed per token. These USD-per-million-token constants are
# split out so they can be corrected against current OpenAI pricing without
# touching the cost logic. Output-token price varies with quality.
PRICE_TEXT_INPUT_PER_M = 5.0
PRICE_IMAGE_INPUT_PER_M = 10.0
PRICE_IMAGE_OUTPUT_PER_M = {
    "low": 40.0,
    "medium": 40.0,
    "high": 40.0,
}


# ---------------------------------------------------------------------------
# Texture collection
# ---------------------------------------------------------------------------

def collect_textures(model_dir: Path) -> list[Path]:
    """Return the BMP textures to remaster for one model.

    All ``model/*.bmp`` UV sheets plus the loco ``image.bmp`` thumbnail (if any).
    """
    textures: list[Path] = []
    model_sub = model_dir / "model"
    if model_sub.is_dir():
        textures.extend(sorted(model_sub.glob("*.bmp")))
    thumb = model_dir / THUMBNAIL_NAME
    if thumb.exists():
        textures.append(thumb)
    return textures


def model_slug(model_dir: Path) -> str:
    """Flat output-folder name: repo-relative path with '/' -> '__'."""
    rel = model_dir.relative_to(_repo_root())
    return str(rel).replace("/", "__").replace(" ", "_")


# ---------------------------------------------------------------------------
# Image helpers
# ---------------------------------------------------------------------------

def bmp_to_png_bytes(bmp_path: Path) -> tuple[bytes, tuple[int, int]]:
    """Load a BMP, return (PNG bytes, (width, height)) without touching disk."""
    img = Image.open(bmp_path).convert("RGB")
    buf = io.BytesIO()
    img.save(buf, "PNG")
    return buf.getvalue(), img.size


def pick_size(w: int, h: int) -> str:
    """Closest gpt-image-1 output size by aspect ratio."""
    ratio = w / h if h else 1.0
    if ratio > 1.3:
        return SIZE_LANDSCAPE
    if ratio < 0.77:
        return SIZE_PORTRAIT
    return SIZE_SQUARE


def fit_to_exact(img: Image.Image, tw: int, th: int, mode: str) -> Image.Image:
    """Return ``img`` resized to exactly (tw, th).

    ``crop``: center-crop the source to the target aspect ratio first (fills the
    frame, no distortion). ``pad``: contain the whole source and pad with black.
    """
    img = img.convert("RGB")
    sw, sh = img.size
    target_ratio = tw / th
    src_ratio = sw / sh

    if mode == "pad":
        scale = min(tw / sw, th / sh)
        rw, rh = max(1, round(sw * scale)), max(1, round(sh * scale))
        resized = img.resize((rw, rh), Image.LANCZOS)
        canvas = Image.new("RGB", (tw, th), (0, 0, 0))
        canvas.paste(resized, ((tw - rw) // 2, (th - rh) // 2))
        return canvas

    # crop: trim the longer dimension to match the target aspect, then resize.
    if src_ratio > target_ratio:
        new_w = round(sh * target_ratio)
        left = (sw - new_w) // 2
        img = img.crop((left, 0, left + new_w, sh))
    elif src_ratio < target_ratio:
        new_h = round(sw / target_ratio)
        top = (sh - new_h) // 2
        img = img.crop((0, top, sw, top + new_h))
    return img.resize((tw, th), Image.LANCZOS)


def build_contact_sheet(
    original: Image.Image,
    classical: Image.Image,
    ai_raw: Image.Image,
    final: Image.Image,
    out_path: Path,
) -> None:
    """Write a labelled 4-panel comparison sheet."""
    labels = ["original", "classical 4x", "AI raw", "final 4x"]
    panels = [original, classical, ai_raw, final]

    panel_h = 256
    label_h = 18
    pad = 8

    scaled = []
    for p in panels:
        p = p.convert("RGB")
        w = max(1, round(p.width * panel_h / p.height))
        scaled.append(p.resize((w, panel_h), Image.NEAREST))

    total_w = sum(p.width for p in scaled) + pad * (len(scaled) + 1)
    total_h = panel_h + label_h + pad * 2
    sheet = Image.new("RGB", (total_w, total_h), (32, 32, 32))
    draw = ImageDraw.Draw(sheet)

    x = pad
    for label, p in zip(labels, scaled):
        sheet.paste(p, (x, pad + label_h))
        draw.text((x, pad // 2), label, fill=(230, 230, 230))
        x += p.width + pad

    out_path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(out_path, "PNG")


# ---------------------------------------------------------------------------
# Cost
# ---------------------------------------------------------------------------

def extract_usage(resp) -> dict:
    """Pull token usage out of an images response into a plain dict."""
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
    """USD cost for one call from token usage."""
    if not usage:
        return 0.0
    text_in = usage.get("input_text_tokens", 0)
    image_in = usage.get("input_image_tokens", 0)
    out = usage.get("output_tokens", 0)
    out_price = PRICE_IMAGE_OUTPUT_PER_M.get(quality, 40.0)
    return round(
        text_in / 1e6 * PRICE_TEXT_INPUT_PER_M
        + image_in / 1e6 * PRICE_IMAGE_INPUT_PER_M
        + out / 1e6 * out_price,
        6,
    )


# ---------------------------------------------------------------------------
# Manifest
# ---------------------------------------------------------------------------

def load_manifest(path: Path) -> dict:
    if path.exists():
        try:
            return json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            pass
    return {"prompt": PROMPT, "prompt_hash": PROMPT_HASH, "entries": {}}


def save_manifest(path: Path, manifest: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )


# ---------------------------------------------------------------------------
# Core per-texture processing
# ---------------------------------------------------------------------------

def call_openai_edit(
    client, primary_png: bytes, sibling_pngs: list[bytes],
    size: str, quality: str, primary_name: str,
):
    """Call images.edit with the primary first and siblings as references."""
    files = [("primary.png", primary_png)]
    files += [(f"ref_{i}.png", b) for i, b in enumerate(sibling_pngs)]
    image_args = [
        (name, io.BytesIO(data), "image/png") for name, data in files
    ]
    return client.images.edit(
        model=MODEL,
        image=image_args,
        prompt=PROMPT,
        size=size,
        quality=quality,
    )


def process_texture(
    tex_path: Path, sibling_paths: list[Path], out_dir: Path, slug: str,
    model_rel: str, client, quality: str, fit: str, dry_run: bool,
    use_references: bool,
) -> dict:
    """Remaster one texture. Returns its manifest entry."""
    stem = tex_path.stem
    src_hash = file_content_hash(tex_path)
    primary_png, (w, h) = bmp_to_png_bytes(tex_path)
    tw, th = w * UPSCALE, h * UPSCALE

    raw_path = out_dir / f"{stem}_ai_raw.png"
    final_path = out_dir / f"{stem}_4x.png"
    contact_path = out_dir / f"{stem}_contact.png"

    entry = {
        "model": model_rel,
        "texture": tex_path.name,
        "source_path": str(tex_path.relative_to(_repo_root())),
        "source_size": [w, h],
        "ai_raw_path": str(raw_path.relative_to(_repo_root())),
        "final_path": str(final_path.relative_to(_repo_root())),
        "final_size": [tw, th],
        "model_used": MODEL,
        "quality": quality,
        "prompt_hash": PROMPT_HASH,
        "source_hash": src_hash,
        "size_requested": pick_size(w, h),
        "usage": {},
        "cost_usd": 0.0,
        "status": "ok",
        "error": None,
    }

    original = Image.open(io.BytesIO(primary_png)).convert("RGB")
    classical = original.resize((tw, th), Image.LANCZOS)

    if dry_run:
        # No API call: synthesize a placeholder AI raw from the classical upscale
        # so layout/contact-sheet/cost logic can be exercised for free.
        ai_raw = classical.copy()
        final = fit_to_exact(ai_raw, tw, th, fit)
        out_dir.mkdir(parents=True, exist_ok=True)
        ai_raw.save(raw_path, "PNG")
        final.save(final_path, "PNG")
        build_contact_sheet(original, classical, ai_raw, final, contact_path)
        entry["status"] = "dry-run"
        return entry

    sibling_pngs: list[bytes] = []
    if use_references:
        for sib in sibling_paths:
            try:
                sibling_pngs.append(bmp_to_png_bytes(sib)[0])
            except Exception:  # noqa: BLE001 - a bad sibling shouldn't fail the primary
                pass

    try:
        resp = call_openai_edit(
            client, primary_png, sibling_pngs,
            entry["size_requested"], quality, tex_path.name,
        )
        b64 = resp.data[0].b64_json
        raw_bytes = base64.b64decode(b64)
        ai_raw = Image.open(io.BytesIO(raw_bytes)).convert("RGB")

        out_dir.mkdir(parents=True, exist_ok=True)
        raw_path.write_bytes(raw_bytes)
        final = fit_to_exact(ai_raw, tw, th, fit)
        final.save(final_path, "PNG")
        build_contact_sheet(original, classical, ai_raw, final, contact_path)

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

    parser = argparse.ArgumentParser(description="Remaster train textures with gpt-image-1.")
    parser.add_argument("--api-key", help="OpenAI API key (overrides .env / env)")
    parser.add_argument("--input", default=INPUT_FILE, help=f"Input file (default: {INPUT_FILE})")
    parser.add_argument("--out-dir", default=OUT_DIR, help=f"Output dir (default: {OUT_DIR})")
    parser.add_argument("--quality", choices=["low", "medium", "high"], default="medium")
    parser.add_argument("--fit", choices=["crop", "pad"], default="crop",
                        help="Exact-4x post-process mode (default: crop)")
    parser.add_argument("--no-references", action="store_true",
                        help="Do not send sibling textures as reference images")
    parser.add_argument("--dry-run", action="store_true",
                        help="Skip the API call; still write source/classical/contact/manifest")
    parser.add_argument("--limit", type=int, default=0,
                        help="Cap number of textures processed this run (0 = no cap)")
    args = parser.parse_args()

    use_references = not args.no_references

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
    entries: dict = manifest["entries"]
    manifest["prompt"] = PROMPT
    manifest["prompt_hash"] = PROMPT_HASH

    processed = 0
    total_cost = 0.0
    counts = {"ok": 0, "cached": 0, "dry-run": 0, "error": 0}

    for mi, (_override, model_dir) in enumerate(model_dirs, 1):
        model_rel = str(model_dir.relative_to(_repo_root()))
        textures = collect_textures(model_dir)
        if not textures:
            print(f"[{mi}/{len(model_dirs)}] {model_rel}: no textures, skipping")
            continue

        slug = model_slug(model_dir)
        out_dir = out_root / slug
        print(f"[{mi}/{len(model_dirs)}] {model_rel} -> {slug} ({len(textures)} texture(s))")

        for tex in textures:
            if args.limit and processed >= args.limit:
                print(f"  --limit {args.limit} reached, stopping.")
                break

            key = f"{slug}/{tex.name}"
            final_path = out_dir / f"{tex.stem}_4x.png"
            prev = entries.get(key)
            # Resume guard: completed + unchanged source -> skip.
            if (not args.dry_run and prev and prev.get("status") in ("ok", "dry-run")
                    and final_path.exists()
                    and prev.get("source_hash") == file_content_hash(tex)):
                print(f"  [cached] {tex.name}")
                counts["cached"] += 1
                continue

            siblings = [t for t in textures if t != tex]
            entry = process_texture(
                tex, siblings, out_dir, slug, model_rel, client,
                args.quality, args.fit, args.dry_run, use_references,
            )
            entries[key] = entry
            save_manifest(manifest_path, manifest)  # persist after each texture

            status = entry["status"]
            counts[status] = counts.get(status, 0) + 1
            total_cost += entry.get("cost_usd", 0.0)
            processed += 1
            if status == "error":
                print(f"  [error] {tex.name}: {entry['error']}")
            else:
                print(f"  [{status}] {tex.name} -> {tex.stem}_4x.png "
                      f"({entry['final_size'][0]}x{entry['final_size'][1]}, "
                      f"${entry['cost_usd']:.4f})")

        if args.limit and processed >= args.limit:
            break

    save_manifest(manifest_path, manifest)
    print(f"\nDone. {counts['ok']} ok, {counts['cached']} cached, "
          f"{counts['dry-run']} dry-run, {counts['error']} error. "
          f"Total cost: ${total_cost:.4f}")
    print(f"Manifest: {manifest_path}")


if __name__ == "__main__":
    main()
