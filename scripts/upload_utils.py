"""
Shared utilities for Roblox upload scripts (upload_loco.py, upload_building.py).

Provides:
  - Lockfile management
  - XML parsing (Windows-1250 tolerant)
  - BMP → PNG conversion
  - Texture atlas building (shelf-packing)
  - Model XML → GLB conversion (with or without atlas)
  - Roblox Open Cloud API upload helpers
"""

import hashlib
import io
import json
import math
import struct
import time
import xml.etree.ElementTree as ET
from pathlib import Path

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

# Built-in material colors for "material:xxx.bmp" texture references
MATERIAL_COLORS: dict[str, tuple[int, int, int, int]] = {
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


# ---------------------------------------------------------------------------
# Repository root
# ---------------------------------------------------------------------------

def _repo_root() -> Path:
    """Return the repository root (parent of the scripts/ directory)."""
    return Path(__file__).resolve().parent.parent


# ---------------------------------------------------------------------------
# Lockfile
# ---------------------------------------------------------------------------

def load_lockfile() -> dict:
    """Load the upload lockfile. Returns {lock_key: asset_id} mapping."""
    path = _repo_root() / LOCKFILE_NAME
    if path.exists():
        return json.loads(path.read_text(encoding="utf-8"))
    return {}


def save_lockfile(lockdata: dict) -> None:
    """Save the upload lockfile (sorted for stable diffs)."""
    path = _repo_root() / LOCKFILE_NAME
    path.write_text(
        json.dumps(lockdata, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )


# ---------------------------------------------------------------------------
# Hashing helpers
# ---------------------------------------------------------------------------

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


# ---------------------------------------------------------------------------
# XML parsing
# ---------------------------------------------------------------------------

def parse_xml_safe(path: Path) -> ET.Element:
    """Parse an XML file, gracefully handling Windows-1250 encoding issues."""
    try:
        return ET.parse(path).getroot()
    except ET.ParseError:
        # Fallback: decode as cp1250, strip the XML declaration and re-parse
        content = path.read_bytes().decode("cp1250", errors="replace")
        if content.startswith("<?xml"):
            end = content.find("?>")
            if end != -1:
                content = content[end + 2:]
        return ET.fromstring(content)


# ---------------------------------------------------------------------------
# Image helpers
# ---------------------------------------------------------------------------

def convert_bmp_to_png(bmp_path: Path) -> Path:
    """Convert BMP to PNG (Roblox doesn't accept BMP). Returns path to PNG."""
    png_path = bmp_path.with_suffix(".png")
    if not HAS_PIL:
        print(f"  [WARN] Pillow not installed, cannot convert {bmp_path.name}. "
              "Install with: pip install Pillow")
        return bmp_path
    img = Image.open(bmp_path)
    img.save(png_path, "PNG")
    return png_path


# ---------------------------------------------------------------------------
# Texture atlas
# ---------------------------------------------------------------------------

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
            mat_name = ref[9:].replace(".bmp", "")
            color = MATERIAL_COLORS.get(mat_name, (128, 128, 128, 255))
            images[ref] = Image.new("RGBA", (16, 16), color)
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


# ---------------------------------------------------------------------------
# GLB conversion
# ---------------------------------------------------------------------------

def convert_model_xml_to_glb(
    model_index_xml: Path,
    output_glb: Path,
    atlas_png: bytes,
    atlas_map: dict[str, tuple[float, float, float, float]],
) -> dict:
    """Convert a model XML ``<model>`` section to glTF Binary (.glb).

    Works with both loco-style roots and simplemodel roots — finds the
    ``<model>`` child element containing ``<polygon>/<vertex>`` data.
    UVs are remapped into the atlas. When ``atlas_png`` is empty a plain
    grey PBR material is used instead.

    Returns a stats dict: ``{vertices, faces, length}``.
    """
    root = parse_xml_safe(model_index_xml)

    model_el = root.find("model")
    if model_el is None:
        print("  [ERROR] No <model> element found in model/index.xml")
        return {"vertices": 0, "faces": 0, "length": 0}

    positions: list[float] = []
    normals: list[float] = []
    texcoords: list[float] = []
    indices: list[int] = []
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
        for vt in poly_verts:
            positions.extend(vt[:3])
            normals.extend(vt[3:6])
            texcoords.extend(vt[6:8])
            vertex_count += 1

        for j in range(1, len(poly_verts) - 1):
            indices.extend([base_idx, base_idx + j, base_idx + j + 1])
            triangle_count += 1

    if vertex_count == 0:
        print("  [WARN] No vertices found in <model>, skipping GLB")
        return {"vertices": 0, "faces": 0, "length": 0}

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

    # Bounding box (required by glTF spec for POSITION accessor)
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
        textures_gltf = [{"source": 0, "sampler": 0}]
        images_gltf = [{"bufferView": 4, "mimeType": "image/png"}]
        samplers_gltf = [{
            "magFilter": 9729,   # LINEAR
            "minFilter": 9987,   # LINEAR_MIPMAP_LINEAR
            "wrapS": 33071,      # CLAMP_TO_EDGE
            "wrapT": 33071,
        }]
        buffer_views = [
            {"buffer": 0, "byteOffset": pos_offset,  "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": norm_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": uv_offset,   "byteLength": vertex_count * 8,  "target": 34962},
            {"buffer": 0, "byteOffset": idx_offset,  "byteLength": idx_byte_len,       "target": 34963},
            {"buffer": 0, "byteOffset": img_offset,  "byteLength": len(atlas_png)},
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
        textures_gltf = []
        images_gltf = []
        samplers_gltf = []
        buffer_views = [
            {"buffer": 0, "byteOffset": pos_offset,  "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": norm_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": uv_offset,   "byteLength": vertex_count * 8,  "target": 34962},
            {"buffer": 0, "byteOffset": idx_offset,  "byteLength": idx_byte_len,       "target": 34963},
        ]

    gltf: dict = {
        "asset": {"version": "2.0", "generator": "vlakosimdb"},
        "scene": 0,
        "scenes": [{"nodes": [0]}],
        "nodes": [{"mesh": 0}],
        "meshes": [{"primitives": [{
            "attributes": {"POSITION": 0, "NORMAL": 1, "TEXCOORD_0": 2},
            "indices": 3,
            "material": 0,
        }]}],
        "materials": materials,
        "accessors": [
            {"bufferView": 0, "componentType": 5126, "count": vertex_count,
             "type": "VEC3", "min": min_pos, "max": max_pos},
            {"bufferView": 1, "componentType": 5126, "count": vertex_count, "type": "VEC3"},
            {"bufferView": 2, "componentType": 5126, "count": vertex_count, "type": "VEC2"},
            {"bufferView": 3, "componentType": idx_component_type,
             "count": len(indices), "type": "SCALAR"},
        ],
        "bufferViews": buffer_views,
        "buffers": [{"byteLength": len(bin_data)}],
    }
    if textures_gltf:
        gltf["textures"] = textures_gltf
    if images_gltf:
        gltf["images"] = images_gltf
    if samplers_gltf:
        gltf["samplers"] = samplers_gltf

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
    print(f"  GLB written: {output_glb} "
          f"({stats['vertices']} vertices, {stats['faces']} triangles, length={stats['length']})")
    return stats


# ---------------------------------------------------------------------------
# Roblox API uploads
# ---------------------------------------------------------------------------

def upload_mesh_to_roblox(
    glb_path: Path, api_key: str, creator_id: str,
    display_name: str, description: str,
) -> str | None:
    """Upload a GLB mesh to Roblox via Open Cloud Assets API. Returns asset ID."""
    if not HAS_REQUESTS:
        print("  [ERROR] 'requests' library required. Install with: pip install requests")
        return None

    request_body = {
        "assetType": "Model",
        "displayName": display_name[:50],
        "description": description[:1000],
        "creationContext": {"creator": {"userId": creator_id}},
    }

    with open(glb_path, "rb") as f:
        mesh_data = f.read()

    resp = requests.post(
        ROBLOX_ASSETS_API,
        headers={"x-api-key": api_key},
        files={
            "request": (None, json.dumps(request_body), "application/json"),
            "fileContent": (glb_path.name, mesh_data, "model/gltf-binary"),
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


def upload_image_to_roblox(
    file_path: Path, api_key: str, creator_id: str,
    display_name: str, description: str,
) -> str | None:
    """Upload an image to Roblox via Open Cloud Assets API. Returns asset ID."""
    if not HAS_REQUESTS:
        print("  [ERROR] 'requests' library required. Install with: pip install requests")
        return None

    png_path = convert_bmp_to_png(file_path) if file_path.suffix.lower() == ".bmp" else file_path

    request_body = {
        "assetType": "Image",
        "displayName": display_name[:50],
        "description": description[:1000],
        "creationContext": {"creator": {"userId": creator_id}},
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
    """Poll a Roblox long-running operation until it completes. Returns asset ID."""
    if not operation_path.startswith("assets/"):
        url = f"https://apis.roblox.com/assets/v1/{operation_path}"
    else:
        url = f"https://apis.roblox.com/{operation_path}"
    for attempt in range(max_attempts):
        time.sleep(3)
        resp = requests.get(url, headers={"x-api-key": api_key}, timeout=30)
        if resp.status_code != 200:
            print(f"  [POLL] Attempt {attempt + 1}/{max_attempts}: status {resp.status_code}")
            continue
        data = resp.json()
        if data.get("done"):
            return data.get("response", {}).get("assetId")
        print(f"  [POLL] Attempt {attempt + 1}/{max_attempts}: not done yet...")
    print("  [WARN] Operation did not complete in time")
    return None
