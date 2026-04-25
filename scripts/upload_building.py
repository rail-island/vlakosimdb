#!/usr/bin/env python3
"""
Upload building assets to Roblox and generate a combined Lua metadata file.

Usage:
    python upload_building.py [--dry-run]

Reads INPUT_BUILDING file from the project root for the list of building directories.
Each line is a path relative to the repo root, e.g.:
    db/building/house1
    db/building/*

Lines ending with /* will expand to all sub-directories.
Lines ending with /*/* will expand two directory levels deep.

Configure ROBLOX_API_KEY and ROBLOX_CREATOR_ID in a .env file at the project root,
or pass them via --api-key / --creator-id flags (flags override .env values).
"""

import argparse
import hashlib
import io
import json
import math
import os
import struct
import sys
import time
import xml.etree.ElementTree as ET
from pathlib import Path

from dotenv import load_dotenv

try:
    from PIL import Image
    HAS_PIL = True
except ImportError:
    HAS_PIL = False

try:
    import requests
    HAS_REQUESTS = True
except ImportError:
    HAS_REQUESTS = False


ROBLOX_ASSETS_API = "https://apis.roblox.com/assets/v1/assets"
LOCKFILE_NAME = "roblox_uploads.lock.json"
INPUT_FILE = "INPUT_BUILDING"
OUTPUT_LUA = "building_data.lua"

# Built-in material colors for "material:xxx.bmp" texture references
MATERIAL_COLORS = {
    "black": (0, 0, 0, 255),
    "white": (255, 255, 255, 255),
    "grey": (128, 128, 128, 255),
    "gray": (128, 128, 128, 255),
    "red": (255, 0, 0, 255),
    "green": (0, 128, 0, 255),
    "blue": (0, 0, 255, 255),
    "wall": (180, 150, 120, 255),
    "roof": (100, 80, 60, 255),
    "glass": (200, 220, 255, 200),
}


def _repo_root() -> Path:
    return Path(__file__).resolve().parent.parent


def load_lockfile() -> dict:
    """Load the upload lockfile. Returns {lock_key: asset_id} mapping."""
    path = _repo_root() / LOCKFILE_NAME
    if path.exists():
        return json.loads(path.read_text(encoding="utf-8"))
    return {}


def save_lockfile(lockdata: dict):
    """Save the upload lockfile (sorted for stable diffs)."""
    path = _repo_root() / LOCKFILE_NAME
    path.write_text(
        json.dumps(lockdata, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )


def file_content_hash(file_path: Path) -> str:
    """Compute SHA-256 hash of file contents."""
    h = hashlib.sha256()
    with open(file_path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()


def make_lock_key(rel_path: str, content_hash: str) -> str:
    """Build a lockfile key: relative_path:content_hash."""
    return f"{rel_path}:{content_hash}"


def parse_xml_safe(path: Path) -> ET.Element:
    """Parse an XML file, gracefully handling Windows-1250 encoding issues."""
    try:
        return ET.parse(path).getroot()
    except ET.ParseError:
        # Fallback: read bytes, decode with cp1250, strip XML declaration
        content = path.read_bytes().decode("cp1250", errors="replace")
        if content.startswith("<?xml"):
            end = content.find("?>")
            if end != -1:
                content = content[end + 2:]
        return ET.fromstring(content)


def convert_bmp_to_png(bmp_path: Path) -> Path:
    """Convert BMP to PNG (Roblox doesn't accept BMP). Returns path to PNG."""
    png_path = bmp_path.with_suffix(".png")
    if not HAS_PIL:
        print(f"  [WARN] Pillow not installed, cannot convert {bmp_path.name}. Install with: pip install Pillow")
        return bmp_path
    img = Image.open(bmp_path)
    img.save(png_path, "PNG")
    return png_path


def parse_building_metadata(index_xml_path: Path) -> dict:
    """Parse the building index.xml and extract metadata attributes."""
    root = parse_xml_safe(index_xml_path)
    metadata = {}
    attr_map = {
        "name": "name",
        "title": "title",
        "author": "author",
        "email": "email",
        "w": "w",
        "h": "h",
        "rep.name": "repName",
    }
    for xml_attr, lua_key in attr_map.items():
        val = root.get(xml_attr)
        if val is not None:
            metadata[lua_key] = val

    # Strip $ locale markers from title strings (e.g. "$Railway-station $building 2")
    if "title" in metadata:
        metadata["title"] = metadata["title"].replace("$", "")

    return metadata


def build_texture_atlas(model_dir: Path, model_xml_path: Path) -> tuple[bytes, dict, list[str]]:
    """Parse model XML for texture references and build a combined atlas.

    Supports both loco-style <model><polygon texture="pack:..."> and
    building simplemodel-style <simplemodel><colorizes><colorize texture="pack:...">.

    Returns:
        (atlas_png_bytes, atlas_map, texture_refs)
        - atlas_png_bytes: PNG data of the combined atlas
        - atlas_map: {texture_ref: (u_offset, v_offset, u_scale, v_scale)}
        - texture_refs: sorted unique texture references
    """
    if not HAS_PIL:
        print("  [ERROR] Pillow required for atlas. Install with: pip install Pillow")
        return b"", {}, []

    root = parse_xml_safe(model_xml_path)

    # Collect unique texture references from:
    # 1. <model><polygon texture="..."> (pre-computed mesh in simplemodel)
    # 2. <colorizes><colorize texture="..."> (simplemodel face descriptors)
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

    # Load image for each reference
    images: dict[str, Image.Image] = {}
    for ref in sorted(texture_refs_set):
        if ref.startswith("pack:"):
            path = model_dir / ref[5:]
            if path.exists():
                images[ref] = Image.open(path).convert("RGBA")
            else:
                print(f"  [WARN] Texture not found: {path}")
                images[ref] = Image.new("RGBA", (16, 16), (255, 0, 255, 255))
        elif ref.startswith("material:"):
            mat_name = ref[9:].replace(".bmp", "")
            color = MATERIAL_COLORS.get(mat_name, (128, 128, 128, 255))
            images[ref] = Image.new("RGBA", (16, 16), color)
        else:
            images[ref] = Image.new("RGBA", (16, 16), (255, 0, 255, 255))

    # Shelf packing — sort by height descending for better fit
    padding = 1
    sorted_refs = sorted(images.keys(), key=lambda r: images[r].height, reverse=True)

    max_w = max(img.width for img in images.values())
    total_w = sum(img.width + padding for img in images.values())
    max_h = max(img.height for img in images.values())
    target_width = max(max_w + padding, int(math.sqrt(total_w * max_h)))

    shelves: list[tuple[int, int, list[tuple[str, int]]]] = []  # (y, height, [(ref, x)])
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

    for sy, sh, items in shelves:
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
    """Compute combined hash of all source textures for lockfile keying."""
    h = hashlib.sha256()
    for ref in sorted(texture_refs):
        h.update(ref.encode("utf-8"))
        if ref.startswith("pack:"):
            path = model_dir / ref[5:]
            if path.exists():
                with open(path, "rb") as f:
                    for chunk in iter(lambda: f.read(8192), b""):
                        h.update(chunk)
    return h.hexdigest()


def convert_model_xml_to_glb(
    model_index_xml: Path,
    output_glb: Path,
    atlas_png: bytes,
    atlas_map: dict[str, tuple[float, float, float, float]],
) -> dict:
    """Convert the model XML <model> section to glTF Binary (.glb).

    Works with both loco-style roots and simplemodel roots — finds the
    <model> child element containing <polygon>/<vertex> data and converts
    it to a textured GLB with remapped UVs into the atlas.

    Returns stats dict with vertex/triangle/length counts.
    """
    root = parse_xml_safe(model_index_xml)

    model_el = root.find("model")
    if model_el is None:
        print("  [ERROR] No <model> element found in model/index.xml")
        return {"vertices": 0, "faces": 0, "length": 0}

    # Collect vertex data and triangulate polygons
    positions = []
    normals = []
    texcoords = []
    indices = []
    vertex_count = 0
    triangle_count = 0

    for polygon in model_el.findall("polygon"):
        nx = float(polygon.get("nx", "0"))
        ny = float(polygon.get("ny", "0"))
        nz = float(polygon.get("nz", "0"))

        tex_ref = polygon.get("texture", "")
        mapping = atlas_map.get(tex_ref)

        poly_verts = []
        for vert in polygon.findall("vertex"):
            x = float(vert.get("x", "0"))
            y = float(vert.get("y", "0"))
            z = float(vert.get("z", "0"))
            s = float(vert.get("s", "0"))
            t = float(vert.get("t", "0"))

            # Remap UVs from per-texture space into atlas space
            if mapping:
                u_off, v_off, u_sc, v_sc = mapping
                u = u_off + s * u_sc
                v = v_off + (1.0 - t) * v_sc
            else:
                u = s
                v = 1.0 - t

            poly_verts.append((x, y, z, nx, ny, nz, u, v))

        if len(poly_verts) < 3:
            continue

        base_idx = vertex_count
        for v in poly_verts:
            positions.extend(v[:3])
            normals.extend(v[3:6])
            texcoords.extend(v[6:8])
            vertex_count += 1

        # Fan triangulation from first vertex
        for j in range(1, len(poly_verts) - 1):
            indices.extend([base_idx, base_idx + j, base_idx + j + 1])
            triangle_count += 1

    if vertex_count == 0:
        print("  [WARN] No vertices found in <model>, skipping GLB")
        return {"vertices": 0, "faces": 0, "length": 0}

    # Build binary buffer sections
    use_uint32 = vertex_count > 65535

    pos_data = struct.pack(f"<{len(positions)}f", *positions)
    norm_data = struct.pack(f"<{len(normals)}f", *normals)
    uv_data = struct.pack(f"<{len(texcoords)}f", *texcoords)

    if use_uint32:
        idx_data = struct.pack(f"<{len(indices)}I", *indices)
        idx_component_type = 5125  # UNSIGNED_INT
        idx_byte_len = len(indices) * 4
    else:
        idx_data = struct.pack(f"<{len(indices)}H", *indices)
        idx_component_type = 5123  # UNSIGNED_SHORT
        idx_byte_len = len(indices) * 2

    def pad4(data: bytes) -> bytes:
        remainder = len(data) % 4
        return data + b"\x00" * (4 - remainder) if remainder else data

    pos_data = pad4(pos_data)
    norm_data = pad4(norm_data)
    uv_data = pad4(uv_data)
    idx_data = pad4(idx_data)

    # Bounding box
    min_pos = [float("inf")] * 3
    max_pos = [float("-inf")] * 3
    for i in range(vertex_count):
        for c in range(3):
            val = positions[i * 3 + c]
            min_pos[c] = min(min_pos[c], val)
            max_pos[c] = max(max_pos[c], val)

    has_texture = bool(atlas_png)

    if has_texture:
        img_data = pad4(atlas_png)
        pos_offset = 0
        norm_offset = pos_offset + len(pos_data)
        uv_offset = norm_offset + len(norm_data)
        idx_offset = uv_offset + len(uv_data)
        img_offset = idx_offset + len(idx_data)
        bin_data = pos_data + norm_data + uv_data + idx_data + img_data

        materials = [{
            "pbrMetallicRoughness": {
                "baseColorTexture": {"index": 0},
                "metallicFactor": 0.0,
                "roughnessFactor": 0.8,
            },
        }]
        textures = [{"source": 0, "sampler": 0}]
        images_gltf = [{"bufferView": 4, "mimeType": "image/png"}]
        samplers = [{
            "magFilter": 9729,
            "minFilter": 9987,
            "wrapS": 33071,
            "wrapT": 33071,
        }]
        buffer_views = [
            {"buffer": 0, "byteOffset": pos_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": norm_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": uv_offset, "byteLength": vertex_count * 8, "target": 34962},
            {"buffer": 0, "byteOffset": idx_offset, "byteLength": idx_byte_len, "target": 34963},
            {"buffer": 0, "byteOffset": img_offset, "byteLength": len(atlas_png)},
        ]
    else:
        bin_data = pos_data + norm_data + uv_data + idx_data
        pos_offset = 0
        norm_offset = pos_offset + len(pos_data)
        uv_offset = norm_offset + len(norm_data)
        idx_offset = uv_offset + len(uv_data)

        materials = [{
            "pbrMetallicRoughness": {
                "baseColorFactor": [0.7, 0.7, 0.7, 1.0],
                "metallicFactor": 0.0,
                "roughnessFactor": 0.8,
            },
        }]
        textures = []
        images_gltf = []
        samplers = []
        buffer_views = [
            {"buffer": 0, "byteOffset": pos_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": norm_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": uv_offset, "byteLength": vertex_count * 8, "target": 34962},
            {"buffer": 0, "byteOffset": idx_offset, "byteLength": idx_byte_len, "target": 34963},
        ]

    primitive = {
        "attributes": {"POSITION": 0, "NORMAL": 1, "TEXCOORD_0": 2},
        "indices": 3,
        "material": 0,
    }

    gltf: dict = {
        "asset": {"version": "2.0", "generator": "vlakosimdb upload_building.py"},
        "scene": 0,
        "scenes": [{"nodes": [0]}],
        "nodes": [{"mesh": 0}],
        "meshes": [{"primitives": [primitive]}],
        "materials": materials,
        "accessors": [
            {
                "bufferView": 0, "componentType": 5126, "count": vertex_count,
                "type": "VEC3", "min": min_pos, "max": max_pos,
            },
            {"bufferView": 1, "componentType": 5126, "count": vertex_count, "type": "VEC3"},
            {"bufferView": 2, "componentType": 5126, "count": vertex_count, "type": "VEC2"},
            {"bufferView": 3, "componentType": idx_component_type, "count": len(indices), "type": "SCALAR"},
        ],
        "bufferViews": buffer_views,
        "buffers": [{"byteLength": len(bin_data)}],
    }
    if textures:
        gltf["textures"] = textures
    if images_gltf:
        gltf["images"] = images_gltf
    if samplers:
        gltf["samplers"] = samplers

    json_str = json.dumps(gltf, separators=(",", ":"))
    while len(json_str) % 4 != 0:
        json_str += " "
    json_data = json_str.encode("utf-8")

    total_length = 12 + 8 + len(json_data) + 8 + len(bin_data)
    with open(output_glb, "wb") as f:
        f.write(struct.pack("<I", 0x46546C67))  # magic "glTF"
        f.write(struct.pack("<I", 2))            # version 2
        f.write(struct.pack("<I", total_length))
        f.write(struct.pack("<I", len(json_data)))
        f.write(struct.pack("<I", 0x4E4F534A))   # chunk type "JSON"
        f.write(json_data)
        f.write(struct.pack("<I", len(bin_data)))
        f.write(struct.pack("<I", 0x004E4942))   # chunk type "BIN\0"
        f.write(bin_data)

    stats = {
        "vertices": vertex_count,
        "faces": triangle_count,
        "length": round(max_pos[0] - min_pos[0], 3),
    }
    print(f"  GLB written: {output_glb} ({stats['vertices']} vertices, {stats['faces']} triangles)")
    return stats


def upload_mesh_to_roblox(obj_path: Path, api_key: str, creator_id: str,
                          display_name: str, description: str) -> str | None:
    """Upload a GLB mesh to Roblox via Open Cloud Assets API. Returns asset ID."""
    if not HAS_REQUESTS:
        print("  [ERROR] 'requests' library required. Install with: pip install requests")
        return None

    request_body = {
        "assetType": "Model",
        "displayName": display_name[:50],
        "description": description[:1000],
        "creationContext": {
            "creator": {"userId": creator_id},
        },
    }

    with open(obj_path, "rb") as f:
        mesh_data = f.read()

    resp = requests.post(
        ROBLOX_ASSETS_API,
        headers={"x-api-key": api_key},
        files={
            "request": (None, json.dumps(request_body), "application/json"),
            "fileContent": (obj_path.name, mesh_data, "model/gltf-binary"),
        },
        timeout=120,
    )

    if resp.status_code not in (200, 201):
        print(f"  [ERROR] Mesh upload failed ({resp.status_code}): {resp.text}")
        return None

    result = resp.json()
    operation_path = result.get("path")
    if operation_path and not result.get("done"):
        return poll_operation(operation_path, api_key)
    if result.get("done") and result.get("response"):
        return result["response"].get("assetId")
    return result.get("assetId")


def upload_image_to_roblox(file_path: Path, api_key: str, creator_id: str,
                           display_name: str, description: str) -> str | None:
    """Upload an image to Roblox via Open Cloud Assets API. Returns asset ID."""
    if not HAS_REQUESTS:
        print("  [ERROR] 'requests' library required. Install with: pip install requests")
        return None

    png_path = convert_bmp_to_png(file_path) if file_path.suffix.lower() == ".bmp" else file_path

    request_body = {
        "assetType": "Image",
        "displayName": display_name[:50],
        "description": description[:1000],
        "creationContext": {
            "creator": {"userId": creator_id},
        },
    }

    with open(png_path, "rb") as f:
        image_data = f.read()

    resp = requests.post(
        ROBLOX_ASSETS_API,
        headers={"x-api-key": api_key},
        files={
            "request": (None, json.dumps(request_body), "application/json"),
            "fileContent": (png_path.name, image_data, "image/png"),
        },
        timeout=60,
    )

    if resp.status_code not in (200, 201):
        print(f"  [ERROR] Upload failed ({resp.status_code}): {resp.text}")
        return None

    result = resp.json()
    operation_path = result.get("path")
    if operation_path and not result.get("done"):
        return poll_operation(operation_path, api_key)
    if result.get("done") and result.get("response"):
        return result["response"].get("assetId")
    return result.get("assetId")


def poll_operation(operation_path: str, api_key: str, max_attempts: int = 20) -> str | None:
    """Poll a Roblox operation until completion."""
    if not operation_path.startswith("assets/"):
        url = f"https://apis.roblox.com/assets/v1/{operation_path}"
    else:
        url = f"https://apis.roblox.com/{operation_path}"
    for attempt in range(max_attempts):
        time.sleep(3)
        resp = requests.get(url, headers={"x-api-key": api_key}, timeout=30)
        if resp.status_code != 200:
            print(f"  [POLL] Attempt {attempt+1}/{max_attempts}: status {resp.status_code}")
            continue
        data = resp.json()
        if data.get("done"):
            response = data.get("response", {})
            return response.get("assetId")
        print(f"  [POLL] Attempt {attempt+1}/{max_attempts}: not done yet...")
    print("  [WARN] Operation did not complete in time")
    return None


def find_primary_model_dir(building_dir: Path) -> Path | None:
    """Find the primary model directory for a building.

    Prefers the 'model' subdirectory; falls back to the first model name
    referenced in the building's index.xml <models> section.
    """
    model_dir = building_dir / "model"
    if model_dir.is_dir() and (model_dir / "index.xml").exists():
        return model_dir

    index_xml = building_dir / "index.xml"
    if not index_xml.exists():
        return None

    try:
        root = parse_xml_safe(index_xml)
        for model_el in root.findall("models/model"):
            model_name = model_el.get("model", "")
            if model_name:
                candidate = building_dir / model_name
                if candidate.is_dir() and (candidate / "index.xml").exists():
                    return candidate
    except Exception:
        pass

    return None


def resolve_input_dirs(input_file: Path) -> list[Path]:
    """Read INPUT_BUILDING and resolve paths, expanding wildcards.

    Lines ending with /* expand one directory level.
    Lines ending with /*/* expand two levels.
    Lines starting with # are comments.
    """
    if not input_file.exists():
        print(f"Error: {input_file} not found")
        sys.exit(1)

    root = _repo_root()
    dirs: list[Path] = []

    for raw_line in input_file.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        if line.endswith("/*/*"):
            parent = root / line[:-4]
            if not parent.is_dir():
                print(f"  [WARN] Directory not found: {parent}")
                continue
            for child in sorted(parent.iterdir()):
                if child.is_dir():
                    for grandchild in sorted(child.iterdir()):
                        if grandchild.is_dir() and (grandchild / "index.xml").exists():
                            dirs.append(grandchild)
        elif line.endswith("/*"):
            parent = root / line[:-2]
            if not parent.is_dir():
                print(f"  [WARN] Directory not found: {parent}")
                continue
            for child in sorted(parent.iterdir()):
                if child.is_dir() and (child / "index.xml").exists():
                    dirs.append(child)
        else:
            d = root / line
            if d.is_dir() and (d / "index.xml").exists():
                dirs.append(d)
            else:
                print(f"  [WARN] Directory not found or missing index.xml: {d}")

    return dirs


def process_building(building_dir: Path, lockdata: dict, api_key: str | None,
                     creator_id: str | None, dry_run: bool) -> dict:
    """Process a single building: parse metadata, build atlas, write GLB, upload.

    Returns a building result dict suitable for generate_building_lua.
    """
    index_xml = building_dir / "index.xml"
    thumbnail = building_dir / "image.bmp"
    building_rel = str(building_dir.relative_to(_repo_root()))

    # 1. Parse building metadata
    metadata = parse_building_metadata(index_xml)
    print(f"  Name: {metadata.get('name')}  Title: {metadata.get('title')}  "
          f"Size: {metadata.get('w', '?')}x{metadata.get('h', '?')}")

    # 2. Locate primary model directory
    model_dir = find_primary_model_dir(building_dir)
    model_xml = (model_dir / "index.xml") if model_dir else None

    # 3. Build texture atlas and GLB
    atlas_png: bytes = b""
    atlas_map: dict = {}
    tex_refs: list[str] = []
    glb_path: Path | None = None
    glb_stats: dict = {"vertices": 0, "faces": 0, "length": 0}

    if model_xml and model_xml.exists():
        atlas_png, atlas_map, tex_refs = build_texture_atlas(model_dir, model_xml)
        if atlas_png:
            atlas_path = building_dir / "atlas.png"
            atlas_path.write_bytes(atlas_png)

        glb_path = building_dir / "model.glb"
        glb_stats = convert_model_xml_to_glb(model_xml, glb_path, atlas_png, atlas_map)
        if glb_stats.get("vertices", 0) == 0:
            glb_path = None  # Don't upload an empty GLB
    else:
        print(f"  [WARN] No model/index.xml found, skipping GLB")

    # 4. Upload assets or use placeholder IDs
    thumbnail_asset_id = "PENDING_UPLOAD"
    mesh_asset_id = "PENDING_UPLOAD"
    uploads_made = 0
    cache_hits = 0

    if not dry_run:
        # Thumbnail
        if thumbnail.exists():
            thumb_hash = file_content_hash(thumbnail)
            thumb_key = make_lock_key(f"{building_rel}/image.bmp", thumb_hash)
            if thumb_key in lockdata:
                thumbnail_asset_id = lockdata[thumb_key]
                cache_hits += 1
            else:
                asset_id = upload_image_to_roblox(
                    thumbnail, api_key, creator_id,
                    display_name=f"{metadata.get('title', 'Building')} Thumbnail",
                    description=f"Thumbnail for {metadata.get('title', '')} by {metadata.get('author', '')}",
                )
                if asset_id:
                    thumbnail_asset_id = asset_id
                    lockdata[thumb_key] = asset_id
                    save_lockfile(lockdata)
                    uploads_made += 1

        # Mesh
        if glb_path and glb_path.exists():
            model_hash = file_content_hash(model_xml)
            atlas_hash = compute_atlas_source_hash(model_dir, tex_refs)
            combined_hash = hashlib.sha256((model_hash + atlas_hash).encode()).hexdigest()
            mesh_key = make_lock_key(f"{building_rel}/model.glb", combined_hash)
            if mesh_key in lockdata:
                mesh_asset_id = lockdata[mesh_key]
                cache_hits += 1
            else:
                asset_id = upload_mesh_to_roblox(
                    glb_path, api_key, creator_id,
                    display_name=f"{metadata.get('title', 'Building')} Mesh",
                    description=f"3D mesh for {metadata.get('title', '')} by {metadata.get('author', '')}",
                )
                if asset_id:
                    mesh_asset_id = asset_id
                    lockdata[mesh_key] = asset_id
                    save_lockfile(lockdata)
                    uploads_made += 1

        print(f"  Uploads: {uploads_made} new, {cache_hits} cached")

    return {
        "metadata": metadata,
        "thumbnailAssetId": thumbnail_asset_id,
        "modelAssetId": mesh_asset_id,
    }


def generate_building_lua(all_buildings: list[dict], output_path: Path):
    """Generate a single Lua file with all building data."""
    lines = []
    lines.append("-- Auto-generated building database")
    lines.append(f"-- {len(all_buildings)} building(s)")
    lines.append("")
    lines.append("local BuildingDatabase = {")

    for building in all_buildings:
        meta = building["metadata"]
        key = meta.get("name", "unknown")
        lines.append(f'\t["{key}"] = {{')

        lines.append(f'\t\tname = "{meta.get("name", "")}",')
        lines.append(f'\t\ttitle = "{meta.get("title", "")}",')
        lines.append(f'\t\tauthor = "{meta.get("author", "")}",')
        lines.append(f'\t\temail = "{meta.get("email", "")}",')

        try:
            w = int(meta.get("w", 1))
        except (ValueError, TypeError):
            w = 1
        try:
            h = int(meta.get("h", 1))
        except (ValueError, TypeError):
            h = 1
        lines.append(f"\t\tw = {w},")
        lines.append(f"\t\th = {h},")

        if meta.get("repName"):
            lines.append(f'\t\trepName = "{meta["repName"]}",')

        lines.append(f'\t\tthumbnailAssetId = "{building["thumbnailAssetId"]}",')
        lines.append(f'\t\tmodelAssetId = "{building["modelAssetId"]}",')
        lines.append("\t},")

    lines.append("}")
    lines.append("")
    lines.append("return BuildingDatabase")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"\nLua file written to: {output_path}")


def main():
    env_path = _repo_root() / ".env"
    load_dotenv(env_path)

    parser = argparse.ArgumentParser(
        description="Upload building assets to Roblox, reading directories from INPUT_BUILDING"
    )
    parser.add_argument("--api-key", help="Roblox Open Cloud API key (overrides .env)")
    parser.add_argument("--creator-id", help="Roblox user/creator ID (overrides .env)")
    parser.add_argument("--dry-run", action="store_true", help="Skip uploads, use placeholder IDs")
    parser.add_argument("--input", help=f"Input file (default: {INPUT_FILE})")
    parser.add_argument("--output", help=f"Output Lua file (default: {OUTPUT_LUA})")
    args = parser.parse_args()

    api_key = args.api_key or os.environ.get("ROBLOX_API_KEY")
    creator_id = args.creator_id or os.environ.get("ROBLOX_CREATOR_ID")

    if not args.dry_run and (not api_key or not creator_id):
        print("Error: --api-key and --creator-id are required (or use --dry-run)")
        sys.exit(1)

    input_file = _repo_root() / (args.input or INPUT_FILE)
    print(f"Reading input from: {input_file}")
    building_dirs = resolve_input_dirs(input_file)

    if not building_dirs:
        print("No building directories found.")
        sys.exit(1)

    print(f"Found {len(building_dirs)} building(s) to process.\n")

    lockdata = load_lockfile()
    all_buildings: list[dict] = []

    for i, building_dir in enumerate(building_dirs, 1):
        building_rel = building_dir.relative_to(_repo_root())
        print(f"[{i}/{len(building_dirs)}] Processing {building_rel}...")

        if not (building_dir / "index.xml").exists():
            print(f"  [SKIP] No index.xml found")
            continue

        result = process_building(building_dir, lockdata, api_key, creator_id, args.dry_run)
        all_buildings.append(result)

    output_path = _repo_root() / (args.output or OUTPUT_LUA)
    generate_building_lua(all_buildings, output_path)
    print(f"\nDone! Processed {len(all_buildings)} building(s).")


if __name__ == "__main__":
    main()
