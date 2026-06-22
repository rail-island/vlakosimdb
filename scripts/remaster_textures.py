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
from PIL import Image, ImageDraw, ImageFilter

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
MODEL = "gpt-image-2"  # newest/best OpenAI image model available on this account

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
    ("x", "the flat END (front/rear cab) face of the locomotive viewed straight-on: "
          "cab windows, headlights/marker lights, buffers and coupling, end door, and "
          "at the bottom the wheels/bogie seen end-on. The locomotive RESTS ON THE "
          "GROUND: the bottom of the wheels and the lowest part of the end must sit "
          "EXACTLY on the bottom edge of the texture sheet (just like the side sheets) "
          "— the train must not float, there must be no empty gap below it, and no "
          "rail, track or ground line drawn beneath it"),
    ("y", "the ROOF / top-down view of the locomotive (roof panels and seams, "
          "vents, fan grilles, pantograph mounts, insulators, cable runs)"),
    ("z", "the long SIDE elevation of the locomotive body (side windows, doors, "
          "louvres/grilles, body panel seams, numbering and markings along the side, "
          "and a row of wheels along the bottom). The wheels must be perfectly "
          "CIRCULAR in outline (a round rim, not oval or elliptical) and of equal size, "
          "evenly spaced; keep each wheel's internal structure — hub, axle, spokes/rays "
          "and rim — as in the source, do NOT fill them in as plain solid discs. The "
          "bottom of each wheel must sit EXACTLY on the bottom edge of the texture sheet, so the "
          "lowest pixels of the wheels are the very bottom row of the image with no gap "
          "beneath. Do NOT draw any rail, track, sleeper, ground, ground line, baseline, "
          "platform, shadow or any strip below the wheels — there is nothing under the "
          "wheels, the image simply ends at the bottom of the wheels"),
    ("image", "a small preview thumbnail showing the whole locomotive"),
]
DEFAULT_SUBJECT = "a flat UV texture sheet for one part of a locomotive"

# Flat UV sheets (sides, ends, roof, gear): the #1 failure mode is the model
# re-rendering them as a 3D perspective picture of a train. The prompt hammers
# FLAT / ORTHOGRAPHIC / HEAD-ON and forbids perspective, camera angle and added
# lighting so the output stays usable as a texture.
FLAT_TEMPLATE = (
    "This is a single flat 2D UV texture sheet for an old low-poly 3D train model — "
    "a flat painted decal, like a game sprite sheet or a technical orthographic "
    "elevation drawing. It shows {subject}. "
    "Render it crisp and clean at {scale}x resolution, perfectly FLAT and "
    "ORTHOGRAPHIC, viewed exactly straight-on and axis-aligned. ABSOLUTELY NO "
    "perspective, NO camera angle, NO 3/4 view, NO foreshortening, NO added scene "
    "lighting, NO drop shadows, NO reflections, NO 3D rendering, NO background — it "
    "must stay a flat head-on sheet. "
    "Make it look like a clean high-resolution texture: smooth gradients and edges, "
    "recover plausible fine detail (panel seams, rivets, window frames, paint, "
    "metal surfaces), and produce NO pixelation, blockiness or jagged/aliased edges. "
    "Do NOT add new parts, new views, extra vehicles or imagined detail. Preserve "
    "the original layout, proportions, orientation and framing EXACTLY. Reproduce "
    "the input's framing one-to-one: the input may be LETTERBOXED with solid black "
    "bars/margins — KEEP those black bars exactly where they are, do NOT draw into "
    "them, and do NOT enlarge, zoom, shrink, crop or re-centre the train to fill "
    "them; keep the train at its current size and position within its band. Do not "
    "stretch or squash the content: aspect ratios must be preserved, so round shapes "
    "(wheels, buffers, lamps) stay perfectly round and not oval. Keep every marking, "
    "logo, number and text string in its original position and shape. The result "
    "must read as the SAME flat texture sheet at higher resolution, never a 3D "
    "picture of a train."
)

# The loco thumbnail (image.bmp) is genuinely a small 3D preview render, not a flat
# texture — so here we DO allow a richer rendered look, just sharper and cleaner.
THUMBNAIL_TEMPLATE = (
    "This is a small low-resolution preview render of a complete locomotive. "
    "Remaster it to a crisp {scale}x high-resolution version of the SAME image: "
    "keep the exact same locomotive, livery, colours, camera angle, pose and "
    "composition. Sharpen edges, clean up aliasing and JPEG/pixel artefacts, and "
    "recover fine detail (panel lines, windows, glass, metal, markings, lights). "
    "Render the side windows, doors and body markings to match THIS locomotive's "
    "actual side texture sheets and the reference facts below (same window count and "
    "shape for the side that is shown), not a generic locomotive. "
    "Do NOT change the design, repaint it, move the camera, add scenery or invent "
    "new text — just a faithful, higher-resolution, cleaner version of the same "
    "preview. Keep the background as in the source."
)


def texture_subject(stem: str) -> str:
    """Return the subject description for a texture, matched by filename prefix."""
    s = stem.lower()
    for prefix, desc in TEXTURE_SUBJECTS:
        if s.startswith(prefix):
            return desc
    return DEFAULT_SUBJECT


# Genuinely-sparse sheets (mostly empty black UV space) where the black-mask is
# worth keeping to stop hallucination. The body sheets (x_/y_/z_) fill the frame,
# so they are NOT masked — masking them only produced blocky/"pixelated" edges.
SPARSE_PREFIXES = ("gearx", "ploughx", "downx", "pant", "ins")


def is_thumbnail(name: str) -> bool:
    """True for the loco preview render (image.bmp), which is not a flat texture."""
    return Path(name).stem.lower() == "image"


def is_sparse(stem: str) -> bool:
    """True for mostly-black sheets that still benefit from the (soft) black-mask."""
    return stem.lower().startswith(SPARSE_PREFIXES)


def build_prompt(stem: str, facts: str = "", palette: str = "") -> tuple[str, str]:
    """Return (prompt, prompt_hash) tailored to one texture.

    ``facts`` is the per-model text read from all of its sheets (Pass 1); when
    present it is appended so the model renders numbers/markings correctly.
    ``palette`` is the model's exact livery hex colours (same for every sheet) so
    shades stay consistent across sheets.
    """
    if is_thumbnail(stem):
        prompt = THUMBNAIL_TEMPLATE.format(scale=UPSCALE)
    else:
        prompt = FLAT_TEMPLATE.format(subject=texture_subject(stem), scale=UPSCALE)
    if facts:
        prompt += (
            " Reference facts read from all texture sheets of this same locomotive "
            f"(facts may be labelled by sheet filename):\n{facts}\nUse the facts that "
            "apply to THIS sheet to render its running number, lettering, markings AND "
            "its window layout/shape to match. The two sides of the locomotive can "
            "legitimately differ, so follow the facts for THIS specific sheet rather "
            "than copying another side. Do not invent different digits, text or window "
            "shapes. Reproduce any operator logo/emblem EXACTLY as it appears in the "
            "source image (identical shape, letters and any diacritics such as the caron "
            "in 'ČD') — do not redesign it or replace it with a generic or different logo."
        )
    if palette:
        prompt += (
            f" Livery colour palette — use these EXACT colours and no others for the "
            f"paintwork, identical on every sheet of this locomotive: {palette}. Match "
            "these hex values precisely (e.g. the same single yellow for the band and "
            "the same green for the body) so all sheets share one consistent livery."
        )
    return prompt, hashlib.sha256(prompt.encode("utf-8")).hexdigest()

UPSCALE = 4  # target output scale relative to the source

# gpt-image-1 only emits these fixed sizes.
SIZE_LANDSCAPE = "1536x1024"
SIZE_PORTRAIT = "1024x1536"
SIZE_SQUARE = "1024x1024"

# --- Pricing -------------------------------------------------------------
# These models are billed per token. The USD-per-million-token constants are
# split out so they can be corrected against current OpenAI pricing without
# touching the cost logic. Output-token price varies with quality. Treat the
# reported cost_usd as approximate; the raw token usage is always recorded.
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


def extract_palette(paths: list[Path], k: int = 5) -> str:
    """Deterministically sample the dominant livery colours of a model.

    Quantises the body sheets (sides/ends/roof — skips the mostly-black gear/pant
    sheets and the thumbnail), drops the near-black UV background, and returns the
    top ``k`` colours as a hex string. Injecting the SAME palette into every sheet's
    prompt keeps shades consistent across sheets (e.g. x_both's yellow == z_dec's).
    """
    bodies = [Image.open(p).convert("RGB") for p in paths
              if not is_sparse(p.stem) and not is_thumbnail(p.name)]
    if not bodies:
        return ""
    width = max(b.width for b in bodies)
    canvas = Image.new("RGB", (width, sum(b.height for b in bodies)), (0, 0, 0))
    y = 0
    for b in bodies:
        canvas.paste(b, (0, y))
        y += b.height
    quant = canvas.quantize(colors=16, method=Image.MEDIANCUT)
    palette = quant.getpalette()
    hexes: list[str] = []
    for _count, idx in sorted(quant.getcolors(), reverse=True):
        r, g, b = palette[idx * 3:idx * 3 + 3]
        if r < 24 and g < 24 and b < 24:
            continue  # skip the black UV background
        hexes.append(f"#{r:02X}{g:02X}{b:02X}")
        if len(hexes) >= k:
            break
    return ", ".join(hexes)


def pick_size(w: int, h: int) -> str:
    """Closest supported output size by aspect ratio."""
    ratio = w / h if h else 1.0
    if ratio > 1.3:
        return SIZE_LANDSCAPE
    if ratio < 0.77:
        return SIZE_PORTRAIT
    return SIZE_SQUARE


def letterbox_to_size(img: Image.Image, api_w: int, api_h: int
                      ) -> tuple[Image.Image, tuple[float, float, float, float]]:
    """Fit ``img`` into an (api_w, api_h) frame WITHOUT distortion (black bars).

    Returns the framed image plus the content box as fractions
    ``(left, top, right, bottom)`` of the frame. Preserving aspect this way keeps
    circles circular (round wheels): the content sub-rectangle has the source
    aspect, and after the model edit we crop that exact box back out and scale it
    uniformly to the 4x target — so no squish/stretch is ever introduced.
    """
    img = img.convert("RGB")
    w, h = img.size
    scale = min(api_w / w, api_h / h)
    cw, ch = max(1, round(w * scale)), max(1, round(h * scale))
    left, top = (api_w - cw) // 2, (api_h - ch) // 2
    canvas = Image.new("RGB", (api_w, api_h), (0, 0, 0))
    canvas.paste(img.resize((cw, ch), Image.LANCZOS), (left, top))
    box = (left / api_w, top / api_h, (left + cw) / api_w, (top + ch) / api_h)
    return canvas, box


def crop_fraction_resize(img: Image.Image, box: tuple[float, float, float, float],
                         tw: int, th: int) -> Image.Image:
    """Crop ``img`` to the fractional ``box`` then LANCZOS-resize to (tw, th)."""
    img = img.convert("RGB")
    w, h = img.size
    crop = (round(box[0] * w), round(box[1] * h), round(box[2] * w), round(box[3] * h))
    return img.crop(crop).resize((tw, th), Image.LANCZOS)


def apply_black_mask(final: Image.Image, source: Image.Image, threshold: int = 16) -> Image.Image:
    """Force the large empty (near-black) UV regions of ``source`` back to black.

    Used only for genuinely-sparse sheets (gear/pant/...), to stop the model
    hallucinating in empty space. The boundary is feathered so it does NOT
    reintroduce blocky/"pixelated" edges: the keep-region is built at source
    resolution, eroded 1px so content edges aren't clipped, upscaled with LANCZOS
    (not NEAREST), and Gaussian-blurred before compositing.
    """
    final = final.convert("RGB")
    src_l = source.convert("L")
    # Binary keep-mask at source resolution (white = has content).
    keep = src_l.point(lambda p: 255 if p >= threshold else 0)
    # Erode 1px (MinFilter shrinks the white region) so we never clip content edges.
    keep = keep.filter(ImageFilter.MinFilter(3))
    # Smooth upscale to target, then feather the boundary.
    keep = keep.resize(final.size, Image.LANCZOS)
    keep = keep.filter(ImageFilter.GaussianBlur(radius=max(1, final.size[0] // 256)))
    black = Image.new("RGB", final.size, (0, 0, 0))
    return Image.composite(final, black, keep)


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
    return {"flat_template": FLAT_TEMPLATE, "thumbnail_template": THUMBNAIL_TEMPLATE,
            "entries": {}}


def save_manifest(path: Path, manifest: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )


# ---------------------------------------------------------------------------
# Pass 1: read shared facts (numbers/markings) from all of a model's sheets
# ---------------------------------------------------------------------------

VISION_MODEL = "gpt-4o"  # better at reading tiny digits than -mini
VISION_UPSCALE_W = 512   # enlarge each tiny sheet before sending so text is legible

FACTS_PROMPT = (
    "These images are the low-resolution texture sheets of a single old 3D train "
    "model (different views of the SAME locomotive: side elevations, end faces, roof, "
    "gear, and a preview thumbnail). Read them together and report concise PLAIN-TEXT "
    "facts, LABELLED BY SHEET FILENAME where they differ. Cover:\n"
    "- every running/serial number and registration plate you can read, with the "
    "EXACT digits, and which sheet shows it most clearly\n"
    "- railway/operator lettering and any other markings\n"
    "- the operator LOGO/EMBLEM: describe it precisely — the exact letters (with any "
    "diacritics, e.g. 'ČD' has a caron over the C), its style/shape, colour and where "
    "it sits. State that it must be reproduced exactly, not redrawn as a generic logo\n"
    "- the livery colours and their arrangement, described precisely (e.g. which exact "
    "green, which exact yellow) and confirm the SAME shades are used on every sheet\n"
    "- the SIDE WINDOW layout of each side sheet: how many windows/openings, their "
    "shape (rectangular, rounded-corner, square or round), how they are grouped and "
    "roughly where they sit along the side. IMPORTANT: the two sides of this "
    "locomotive (e.g. z_dec vs z_inc) may GENUINELY DIFFER, so describe each side "
    "sheet separately and state which side the preview thumbnail shows.\n"
    "- the wheels along the bottom of the side sheets: how many, and confirm they are "
    "round and sit right at the very bottom edge of the sheet (there is NO rail, track "
    "or ground drawn beneath them).\n"
    "Be precise about digits. If something is partly illegible, give your best single "
    "reading. Keep it reasonably short."
)


def read_model_facts(textures: list[Path], client) -> str:
    """Pass 1: one vision call over all of a model's sheets -> shared text facts.

    Returns '' on any failure (the batch then proceeds without injected facts).
    """
    try:
        content: list[dict] = [{"type": "text", "text": FACTS_PROMPT}]
        for t in textures:
            img = Image.open(t).convert("RGB")
            # Upscale tiny sheets so the vision model can actually read the digits.
            if img.width < VISION_UPSCALE_W:
                scale = VISION_UPSCALE_W / img.width
                img = img.resize((VISION_UPSCALE_W, max(1, round(img.height * scale))), Image.LANCZOS)
            buf = io.BytesIO()
            img.save(buf, "PNG")
            b64 = base64.b64encode(buf.getvalue()).decode("ascii")
            content.append({"type": "text", "text": f"Sheet: {t.name}"})
            content.append({
                "type": "image_url",
                "image_url": {"url": f"data:image/png;base64,{b64}", "detail": "high"},
            })
        resp = client.chat.completions.create(
            model=VISION_MODEL,
            messages=[{"role": "user", "content": content}],
            max_tokens=400,
        )
        return (resp.choices[0].message.content or "").strip()
    except Exception as exc:  # noqa: BLE001 - facts are best-effort; never break the batch
        print(f"  [facts] WARNING: vision read failed ({type(exc).__name__}: {exc})")
        return ""


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
    quality: str, dry_run: bool, db_copy: bool, black_mask: bool,
    facts: str = "", palette: str = "",
) -> dict:
    """Remaster one texture. Returns its manifest entry."""
    stem = tex_path.stem
    thumb = is_thumbnail(stem)
    sparse = is_sparse(stem)
    masked = black_mask and sparse  # body sheets are never masked (avoids pixelation)
    src_hash = file_content_hash(tex_path)
    prompt, prompt_hash = build_prompt(stem, facts, palette)
    primary_png, (w, h) = bmp_to_png_bytes(tex_path)
    tw, th = w * UPSCALE, h * UPSCALE
    size = pick_size(w, h)

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
        "is_thumbnail": thumb,
        "masked": masked,
        "palette": palette,
        "prompt_hash": prompt_hash,
        "source_hash": src_hash,
        "size_requested": size,
        "usage": {},
        "cost_usd": 0.0,
        "status": "ok",
        "error": None,
    }

    original = Image.open(io.BytesIO(primary_png)).convert("RGB")
    classical = original.resize((tw, th), Image.LANCZOS)

    if dry_run:
        # No API call: placeholder AI raw from the classical upscale so layout/
        # contact-sheet/cost logic can be exercised for free. Never writes into
        # db/ (placeholder data must not pollute the tracked tree).
        ai_raw = classical.copy()
        final = classical.copy()
        out_dir.mkdir(parents=True, exist_ok=True)
        ai_raw.save(raw_path, "PNG")
        final.save(final_path, "PNG")
        build_contact_sheet(original, classical, ai_raw, final, contact_path)
        entry["status"] = "dry-run"
        return entry

    try:
        # Letterbox the source into the API frame WITHOUT distortion (black bars),
        # then crop the same region back out below. This keeps the scale uniform
        # end-to-end so circles stay circular (round wheels). Stretching the source
        # to fill the frame instead would squish wheels into ellipses whenever the
        # source aspect is far from the API's (e.g. wide z_* side sheets at 2.8:1
        # vs the API's max 1.5:1). The prompt tells the model to keep the bars black.
        api_w, api_h = (int(v) for v in size.split("x"))
        framed, content_box = letterbox_to_size(original, api_w, api_h)
        api_input = io.BytesIO()
        framed.save(api_input, "PNG")
        resp = call_openai_edit(client, api_input.getvalue(), prompt, size, quality)
        b64 = resp.data[0].b64_json
        raw_bytes = base64.b64decode(b64)
        ai_raw = Image.open(io.BytesIO(raw_bytes)).convert("RGB")

        out_dir.mkdir(parents=True, exist_ok=True)
        raw_path.write_bytes(raw_bytes)
        # Crop the content region back out (undoing the letterbox), then scale
        # uniformly to the exact 4x target — no aspect distortion, no squished wheels.
        final = crop_fraction_resize(ai_raw, content_box, tw, th)
        # Safety net for genuinely-sparse sheets only (gear/pant/...): force the
        # large empty UV regions back to black so the model can't leave invented
        # detail there. Body sheets and the thumbnail are never masked — masking
        # them only reintroduced blocky/pixelated edges.
        if masked:
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
    parser.add_argument("--quality", choices=["low", "medium", "high"], default="high")
    parser.add_argument("--no-db-copy", action="store_true",
                        help="Do not write the _4x.png result next to the source in db/")
    parser.add_argument("--no-black-mask", action="store_true",
                        help="Do not force originally-black source pixels back to black in the output")
    parser.add_argument("--no-context", action="store_true",
                        help="Skip the Pass-1 vision read of numbers/markings from sibling sheets")
    parser.add_argument("--refresh-facts", action="store_true",
                        help="Force a fresh Pass-1 vision read, ignoring any cached facts")
    parser.add_argument("--dry-run", action="store_true",
                        help="Skip the API call; still write source/classical/contact/manifest")
    parser.add_argument("--limit", type=int, default=0,
                        help="Cap number of textures processed this run (0 = no cap)")
    parser.add_argument("--only", default="",
                        help="Only process textures with these exact name(s)/stem(s), "
                             "comma-separated (e.g. --only z_dec,z_inc — matches z_dec.bmp "
                             "and z_inc.bmp but NOT gearx_both). Saves credits when debugging.")
    args = parser.parse_args()

    db_copy = not args.no_db_copy
    black_mask = not args.no_black_mask
    use_context = not args.no_context

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
    manifest.setdefault("models", {})  # per-model facts cache
    manifest["flat_template"] = FLAT_TEMPLATE
    manifest["thumbnail_template"] = THUMBNAIL_TEMPLATE

    processed = 0
    total_cost = 0.0
    counts = {"ok": 0, "cached": 0, "dry-run": 0, "error": 0}

    for mi, (_override, model_dir) in enumerate(model_dirs, 1):
        model_rel = str(model_dir.relative_to(_repo_root()))
        all_textures = collect_textures(model_dir)  # full set (always used for facts context)
        if args.only:
            wanted = {s.strip() for s in args.only.split(",") if s.strip()}
            textures = [t for t in all_textures if wanted & {t.stem, t.name}]
        else:
            textures = all_textures
        if not textures:
            print(f"[{mi}/{len(model_dirs)}] {model_rel}: no textures, skipping")
            continue

        slug = model_slug(model_dir)
        out_dir = out_root / slug
        print(f"[{mi}/{len(model_dirs)}] {model_rel} -> {slug} ({len(textures)} texture(s))")

        def _needs_work(tex: Path) -> bool:
            if args.dry_run:
                return True
            p = entries.get(f"{slug}/{tex.name}")
            fp = out_dir / f"{tex.stem}_4x.png"
            return not (p and p.get("status") in ("ok", "dry-run") and fp.exists()
                        and p.get("source_hash") == file_content_hash(tex))

        # Pass 1: read shared numbers/markings once per model (cached by source
        # hashes), but only when there is real work and the API is in play.
        facts = ""
        any_work = (not args.limit or processed < args.limit) and any(_needs_work(t) for t in textures)
        if use_context and any_work and not args.dry_run and client is not None:
            # Facts always use the FULL sheet set (so --only x_both still gets the
            # number read from its clearer sibling z_dec); key is stable too.
            facts_key = hashlib.sha256(
                "".join(sorted(file_content_hash(t) for t in all_textures)).encode()
            ).hexdigest()
            cached = manifest["models"].get(model_rel)
            # Reuse only a *successful* (non-empty) cached read; an empty result
            # was a failure and must be retried, not cached forever. --refresh-facts
            # bypasses the cache and forces a fresh read.
            if (not args.refresh_facts and cached
                    and cached.get("facts_key") == facts_key and cached.get("facts")):
                facts = cached["facts"]
                tag = "cached"
            else:
                print(f"  [facts] reading numbers/markings from {len(all_textures)} sheet(s)...")
                facts = read_model_facts(all_textures, client)
                if facts:  # never cache a failure
                    manifest["models"][model_rel] = {
                        "facts": facts, "vision_model": VISION_MODEL, "facts_key": facts_key,
                    }
                tag = "read"
            shown = facts.replace("\n", "\n    ") if facts else "(none)"
            print(f"  [facts] ({tag}) {model_rel}:\n    {shown}")

        # Deterministic livery palette (local, no API) shared by every sheet so
        # colours stay consistent across them (e.g. x_both's yellow == z_dec's).
        palette = extract_palette(all_textures) if (any_work and not args.dry_run) else ""
        if palette:
            print(f"  [palette] {palette}")

        for tex in textures:
            if args.limit and processed >= args.limit:
                print(f"  --limit {args.limit} reached, stopping.")
                break

            key = f"{slug}/{tex.name}"
            final_path = out_dir / f"{tex.stem}_4x.png"
            prev = entries.get(key)
            # Resume guard: a real successful result + unchanged source -> skip.
            # Only "ok" counts; a "dry-run" placeholder must never block a real run.
            if (not args.dry_run and prev and prev.get("status") == "ok"
                    and final_path.exists()
                    and prev.get("source_hash") == file_content_hash(tex)):
                print(f"  [cached] {tex.name}")
                counts["cached"] += 1
                continue

            entry = process_texture(
                tex, out_dir, slug, model_rel, client,
                args.quality, args.dry_run, db_copy, black_mask, facts, palette,
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
