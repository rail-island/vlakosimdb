"""
Texture-atlas builder, extracted from upload_utils.

Parses a model XML and shelf-packs its referenced textures into a single combined
PNG atlas. Used by the Roblox upload scripts (upload_loco.py, upload_building.py)
and by the texture remastering pipeline (remaster_textures.py).

Imports its small helpers one-way from ``upload_utils`` (which must NOT import this
module back, to avoid a circular import).
"""

import hashlib
import io
import math
from pathlib import Path

from upload_utils import (
    MATERIAL_COLORS,
    MATERIALS_DIR,
    _repo_root,
    parse_xml_safe,
)

try:
    from PIL import Image
    HAS_PIL = True
except ImportError:
    HAS_PIL = False


def build_texture_atlas(
    model_dir: Path, model_xml_path: Path
) -> tuple[bytes, dict, list[str]]:
    """Parse a model XML and build a combined texture atlas (shelf-packed PNG).

    Collects texture references from:
      - ``<model><polygon texture="...">`` (pre-computed mesh data)
      - ``<colorizes><colorize texture="...">`` (simplemodel face descriptors)

    Returns:
        (atlas_png_bytes, atlas_map, texture_refs)
        - atlas_png_bytes: PNG bytes of the atlas (empty bytes if no textures)
        - atlas_map: ``{tex_ref: (u_offset, v_offset, u_scale, v_scale)}``
        - texture_refs: sorted list of unique texture reference strings
    """
    if not HAS_PIL:
        print("  [ERROR] Pillow required for atlas. Install with: pip install Pillow")
        return b"", {}, []

    root = parse_xml_safe(model_xml_path)

    texture_refs_set: set[str] = set()

    model_el = root.find("model")
    if model_el is not None:
        for polygon in model_el.findall("polygon"):
            tex = polygon.get("texture", "")
            if tex:
                texture_refs_set.add(tex)

    colorizes_el = root.find("colorizes")
    if colorizes_el is not None:
        for colorize in colorizes_el.findall("colorize"):
            tex = colorize.get("texture", "")
            if tex:
                texture_refs_set.add(tex)

    if not texture_refs_set:
        return b"", {}, []

    materials_dir = _repo_root() / MATERIALS_DIR
    images: dict[str, "Image.Image"] = {}
    for ref in sorted(texture_refs_set):
        if ref.startswith("pack:"):
            tex_path = model_dir / ref[5:]
            if tex_path.exists():
                images[ref] = Image.open(tex_path).convert("RGBA")
            else:
                print(f"  [WARN] Texture not found: {tex_path}")
                images[ref] = Image.new("RGBA", (16, 16), (255, 0, 255, 255))
        elif ref.startswith("material:"):
            mat_filename = ref[9:]  # e.g. "metal052.bmp"
            mat_name = mat_filename.replace(".bmp", "").lower()
            # Prefer a real texture file from the materials/ folder
            mat_path = materials_dir / mat_filename
            if not mat_path.exists():
                mat_path = materials_dir / mat_filename.upper()
            if mat_path.exists():
                images[ref] = Image.open(mat_path).convert("RGBA")
                print(f"  [MAT] Using texture file: {mat_path.name}")
            else:
                color = MATERIAL_COLORS.get(mat_name, (128, 128, 128, 255))
                images[ref] = Image.new("RGBA", (64, 64), color)
        else:
            images[ref] = Image.new("RGBA", (16, 16), (255, 0, 255, 255))

    # Shelf packing — tallest images first
    padding = 1
    sorted_refs = sorted(images.keys(), key=lambda r: images[r].height, reverse=True)

    max_w = max(img.width for img in images.values())
    total_w = sum(img.width + padding for img in images.values())
    max_h = max(img.height for img in images.values())
    target_width = max(max_w + padding, int(math.sqrt(total_w * max_h)))

    shelves: list[tuple[int, int, list[tuple[str, int]]]] = []
    for ref in sorted_refs:
        img = images[ref]
        placed = False
        for shelf in shelves:
            sy, sh, items = shelf
            used_w = items[-1][1] + images[items[-1][0]].width + padding if items else 0
            if used_w + img.width <= target_width and img.height <= sh:
                items.append((ref, used_w))
                placed = True
                break
        if not placed:
            y = (shelves[-1][0] + shelves[-1][1] + padding) if shelves else 0
            shelves.append((y, img.height, [(ref, 0)]))

    atlas_w = max(items[-1][1] + images[items[-1][0]].width for _, _, items in shelves)
    atlas_h = shelves[-1][0] + shelves[-1][1]

    atlas = Image.new("RGBA", (atlas_w, atlas_h), (0, 0, 0, 0))
    atlas_map: dict[str, tuple[float, float, float, float]] = {}

    for sy, _sh, items in shelves:
        for ref, x in items:
            img = images[ref]
            atlas.paste(img, (x, sy))
            atlas_map[ref] = (
                x / atlas_w,
                sy / atlas_h,
                img.width / atlas_w,
                img.height / atlas_h,
            )

    buf = io.BytesIO()
    atlas.save(buf, "PNG")
    png_bytes = buf.getvalue()

    print(f"  Atlas: {atlas_w}x{atlas_h}, {len(images)} textures, {len(png_bytes)} bytes")
    for ref, (uo, vo, us, vs) in sorted(atlas_map.items()):
        print(f"    {ref} -> u={uo:.3f} v={vo:.3f} w={us:.3f} h={vs:.3f}")

    return png_bytes, atlas_map, sorted(texture_refs_set)


def compute_atlas_source_hash(model_dir: Path, texture_refs: list[str]) -> str:
    """Compute a combined hash of all source pack: textures for lockfile keying."""
    h = hashlib.sha256()
    for ref in sorted(texture_refs):
        h.update(ref.encode("utf-8"))
        if ref.startswith("pack:"):
            tex_path = model_dir / ref[5:]
            if tex_path.exists():
                with open(tex_path, "rb") as f:
                    for chunk in iter(lambda: f.read(8192), b""):
                        h.update(chunk)
    return h.hexdigest()
