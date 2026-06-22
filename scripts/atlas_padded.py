"""
POC: padded texture-atlas builder.

Packs a model's textures into a grid with generous black PADDING (gutters) around
every sub-texture. The hypothesis: clearly-separated, bounded cells survive a
generative upscale better — the image model is less likely to merge neighbours,
and the gutters let us re-detect/re-snap each tile to its exact grid cell
afterwards (so UV mapping stays correct).

This is a standalone experiment used only by remaster_textures.py --padded. It
does NOT touch upload_utils.build_texture_atlas / the upload pipeline. If the POC
works, the upload model's atlas/UVs would be adjusted to match.
"""

import io
import math
from pathlib import Path

from upload_utils import MATERIAL_COLORS, MATERIALS_DIR, _repo_root, parse_xml_safe

try:
    from PIL import Image
    HAS_PIL = True
except ImportError:
    HAS_PIL = False


def _collect_images(model_dir: Path, model_xml_path: Path) -> tuple[dict, list[str]]:
    """Load the model's referenced textures as RGB images (same refs as the real atlas)."""
    root = parse_xml_safe(model_xml_path)
    refs: set[str] = set()
    model_el = root.find("model")
    if model_el is not None:
        for poly in model_el.findall("polygon"):
            t = poly.get("texture", "")
            if t:
                refs.add(t)
    colorizes = root.find("colorizes")
    if colorizes is not None:
        for c in colorizes.findall("colorize"):
            t = c.get("texture", "")
            if t:
                refs.add(t)
    if not refs:
        return {}, []

    materials_dir = _repo_root() / MATERIALS_DIR
    images: dict[str, "Image.Image"] = {}
    for ref in sorted(refs):
        if ref.startswith("pack:"):
            p = model_dir / ref[5:]
            images[ref] = (Image.open(p).convert("RGB") if p.exists()
                           else Image.new("RGB", (16, 16), (255, 0, 255)))
        elif ref.startswith("material:"):
            fn = ref[9:]
            name = fn.replace(".bmp", "").lower()
            mp = materials_dir / fn
            if not mp.exists():
                mp = materials_dir / fn.upper()
            if mp.exists():
                images[ref] = Image.open(mp).convert("RGB")
            else:
                col = MATERIAL_COLORS.get(name, (128, 128, 128, 255))
                images[ref] = Image.new("RGB", (64, 64), col[:3])
        else:
            images[ref] = Image.new("RGB", (16, 16), (255, 0, 255))
    return images, sorted(refs)


def build_padded_atlas(
    model_dir: Path, model_xml_path: Path, pad: int = 8
) -> tuple[bytes, dict, list[str]]:
    """Build a black-background atlas with ``pad`` px gutters around every tile.

    Returns (png_bytes, atlas_map, texture_refs) where atlas_map maps each ref to
    the normalized inner content rect (u_off, v_off, u_scale, v_scale).
    """
    if not HAS_PIL:
        print("  [ERROR] Pillow required for padded atlas.")
        return b"", {}, []

    images, refs = _collect_images(model_dir, model_xml_path)
    if not images:
        return b"", {}, []

    # Shelf packing, tallest first, with a `pad` gutter between tiles and a border.
    sorted_refs = sorted(images, key=lambda r: images[r].height, reverse=True)
    max_w = max(im.width for im in images.values())
    total_w = sum(im.width + pad for im in images.values())
    max_h = max(im.height for im in images.values())
    target_w = max(max_w, int(math.sqrt(total_w * max_h)))

    placements: dict[str, tuple[int, int]] = {}
    x, y, row_h = pad, pad, 0
    for ref in sorted_refs:
        im = images[ref]
        if x > pad and x + im.width + pad > target_w + pad:
            y += row_h + pad   # wrap to next shelf
            x, row_h = pad, 0
        placements[ref] = (x, y)
        x += im.width + pad
        row_h = max(row_h, im.height)

    atlas_w = max(placements[r][0] + images[r].width for r in images) + pad
    atlas_h = max(placements[r][1] + images[r].height for r in images) + pad

    atlas = Image.new("RGB", (atlas_w, atlas_h), (0, 0, 0))
    atlas_map: dict[str, tuple[float, float, float, float]] = {}
    for ref, (px, py) in placements.items():
        im = images[ref]
        atlas.paste(im, (px, py))
        atlas_map[ref] = (px / atlas_w, py / atlas_h, im.width / atlas_w, im.height / atlas_h)

    buf = io.BytesIO()
    atlas.save(buf, "PNG")
    png_bytes = buf.getvalue()
    print(f"  Padded atlas: {atlas_w}x{atlas_h}, {len(images)} tiles, pad={pad}")
    return png_bytes, atlas_map, refs
