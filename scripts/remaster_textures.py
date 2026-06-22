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

# Per-texture-type subject descriptions, keyed by filename. The model is told
# exactly what each UV sheet depicts so it sharpens the right thing instead of
# inventing a whole vehicle in empty space (e.g. z_inc = side, y_dec = roof).
# Matched in order: first key that is a prefix of the lowercased stem wins, so
# more specific prefixes (gearx, ploughx, ...) must come before the bare axis.
TEXTURE_SUBJECTS: list[tuple[str, str]] = [
    ("gearx", "the running gear / underframe at one END of the vehicle (bogies, "
              "wheels, buffers, coupler, brake pipes) sitting in a mostly-empty "
              "black sheet — most of this sheet is unused black space with detail "
              "only in a small band"),
    ("ploughx", "the snow plough / pilot at the END of the vehicle, on an otherwise "
                "mostly-black sheet"),
    ("downx", "the underframe / underbody detail at the vehicle END, on a mostly-black sheet"),
    ("pant", "a roof-mounted pantograph (current collector): thin metal arms, "
             "springs and contact strip, on a black sheet"),
    ("ins", "an insulator / roof electrical fitting, on a black sheet"),
    ("x", "the flat END face of the locomotive (front or rear cab end): cab "
          "windows, headlights/marker lights, buffers and coupling, end door"),
    ("y", "the ROOF / top-down view of the locomotive (roof panels and seams, "
          "vents, fan grilles, pantograph mounts, insulators, cable runs)"),
    ("z", "the long SIDE elevation of the locomotive body (side windows, doors, "
          "louvres/grilles, body panel seams, numbering and markings along the side)"),
    ("image", "a small preview thumbnail showing the whole locomotive"),
]
DEFAULT_SUBJECT = "a flat UV texture sheet for one part of a locomotive"

PROMPT_TEMPLATE = (
    "This is a single low-resolution UV texture sheet for an old 3D train model. "
    "It shows {subject}. "
    "Upscale it to a clean {scale}x high-resolution version. Sharpen and de-alias "
    "ONLY the detail that is already present, and recover fine texture (panel seams, "
    "rivets, glass, metal) only where the source already shows it. "
    "Do NOT invent or add anything that is not in the source: no new parts, no new "
    "views, no extra vehicles, no imagined scenery. Preserve the original layout, "
    "proportions, orientation and framing EXACTLY — do not move, crop, rescale or "
    "re-centre the content. Large flat black areas are UNUSED UV space: keep them "
    "solid black and never fill them with invented detail. Keep every marking, logo, "
    "number and text string in its original position and shape; if unsure, preserve "
    "the original shape rather than inventing new text. The result must read as the "
    "same texture sheet at higher resolution, not a re-drawing. Output one standalone "
    "texture sheet with the identical layout to the input."
)


def texture_subject(stem: str) -> str:
    """Return the subject description for a texture, matched by filename prefix."""
    s = stem.lower()
    for prefix, desc in TEXTURE_SUBJECTS:
        if s.startswith(prefix):
            return desc
    return DEFAULT_SUBJECT


def build_prompt(stem: str) -> tuple[str, str]:
    """Return (prompt, prompt_hash) tailored to one texture."""
    prompt = PROMPT_TEMPLATE.format(subject=texture_subject(stem), scale=UPSCALE)
    return prompt, hashlib.sha256(prompt.encode("utf-8")).hexdigest()

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


def apply_black_mask(final: Image.Image, source: Image.Image, threshold: int = 16) -> Image.Image:
    """Force pixels that are near-black in the source back to black in ``final``.

    The source is upscaled (NEAREST, blocky) to the final size to build a mask;
    wherever the source luminance is below ``threshold`` the output is set to
    black. This keeps unused/empty UV space empty so the model cannot leave
    hallucinated detail there, while detailed regions pass through untouched.
    """
    final = final.convert("RGB")
    mask_src = source.convert("L").resize(final.size, Image.NEAREST)
    black = Image.new("RGB", final.size, (0, 0, 0))
    # keep_mask: white (255) where source has content -> use final; else black.
    keep_mask = mask_src.point(lambda p: 255 if p >= threshold else 0)
    return Image.composite(final, black, keep_mask)


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
    return {"prompt_template": PROMPT_TEMPLATE, "entries": {}}


def save_manifest(path: Path, manifest: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )


# ---------------------------------------------------------------------------
# Core per-texture processing
# ---------------------------------------------------------------------------

def call_openai_edit(client, primary_png: bytes, prompt: str, size: str, quality: str):
    """Remaster a single texture via images.edit.

    Only the target image is sent. gpt-image-1's images.edit *composites* every
    image passed in the ``image[]`` array, so sending sibling textures as
    "references" merges them into the output sheet — never do that here.
    """
    return client.images.edit(
        model=MODEL,
        image=("primary.png", io.BytesIO(primary_png), "image/png"),
        prompt=prompt,
        size=size,
        quality=quality,
    )


def process_texture(
    tex_path: Path, out_dir: Path, slug: str, model_rel: str, client,
    quality: str, fit: str, dry_run: bool, db_copy: bool, black_mask: bool,
) -> dict:
    """Remaster one texture. Returns its manifest entry."""
    stem = tex_path.stem
    src_hash = file_content_hash(tex_path)
    prompt, prompt_hash = build_prompt(stem)
    primary_png, (w, h) = bmp_to_png_bytes(tex_path)
    tw, th = w * UPSCALE, h * UPSCALE

    raw_path = out_dir / f"{stem}_ai_raw.png"
    final_path = out_dir / f"{stem}_4x.png"
    contact_path = out_dir / f"{stem}_contact.png"
    # Result copy written next to the source in db/ (git-tracked deliverable).
    db_path = tex_path.with_name(f"{stem}_4x.png")

    entry = {
        "model": model_rel,
        "texture": tex_path.name,
        "source_path": str(tex_path.relative_to(_repo_root())),
        "source_size": [w, h],
        "ai_raw_path": str(raw_path.relative_to(_repo_root())),
        "final_path": str(final_path.relative_to(_repo_root())),
        "db_copy_path": None,
        "final_size": [tw, th],
        "model_used": MODEL,
        "quality": quality,
        "subject": texture_subject(stem),
        "prompt_hash": prompt_hash,
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
        # so layout/contact-sheet/cost logic can be exercised for free. Never
        # writes into db/ (placeholder data must not pollute the tracked tree).
        ai_raw = classical.copy()
        final = fit_to_exact(ai_raw, tw, th, fit)
        out_dir.mkdir(parents=True, exist_ok=True)
        ai_raw.save(raw_path, "PNG")
        final.save(final_path, "PNG")
        build_contact_sheet(original, classical, ai_raw, final, contact_path)
        entry["status"] = "dry-run"
        return entry

    try:
        resp = call_openai_edit(client, primary_png, prompt, entry["size_requested"], quality)
        b64 = resp.data[0].b64_json
        raw_bytes = base64.b64decode(b64)
        ai_raw = Image.open(io.BytesIO(raw_bytes)).convert("RGB")

        out_dir.mkdir(parents=True, exist_ok=True)
        raw_path.write_bytes(raw_bytes)
        final = fit_to_exact(ai_raw, tw, th, fit)
        # Safety net: force originally-black UV space back to black so the model
        # cannot leave invented detail in regions that are empty in the source.
        if black_mask:
            final = apply_black_mask(final, original)
        final.save(final_path, "PNG")
        build_contact_sheet(original, classical, ai_raw, final, contact_path)

        # Git-tracked deliverable next to the source — real successful runs only.
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

    parser = argparse.ArgumentParser(description="Remaster train textures with gpt-image-1.")
    parser.add_argument("--api-key", help="OpenAI API key (overrides .env / env)")
    parser.add_argument("--input", default=INPUT_FILE, help=f"Input file (default: {INPUT_FILE})")
    parser.add_argument("--out-dir", default=OUT_DIR, help=f"Output dir (default: {OUT_DIR})")
    parser.add_argument("--quality", choices=["low", "medium", "high"], default="medium")
    parser.add_argument("--fit", choices=["crop", "pad"], default="crop",
                        help="Exact-4x post-process mode (default: crop)")
    parser.add_argument("--no-db-copy", action="store_true",
                        help="Do not write the _4x.png result next to the source in db/")
    parser.add_argument("--no-black-mask", action="store_true",
                        help="Do not force originally-black source pixels back to black in the output")
    parser.add_argument("--dry-run", action="store_true",
                        help="Skip the API call; still write source/classical/contact/manifest")
    parser.add_argument("--limit", type=int, default=0,
                        help="Cap number of textures processed this run (0 = no cap)")
    args = parser.parse_args()

    db_copy = not args.no_db_copy
    black_mask = not args.no_black_mask

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
    manifest["prompt_template"] = PROMPT_TEMPLATE

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

            entry = process_texture(
                tex, out_dir, slug, model_rel, client,
                args.quality, args.fit, args.dry_run, db_copy, black_mask,
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
