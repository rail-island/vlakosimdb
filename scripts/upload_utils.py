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

# Directory (relative to repo root) where material texture files can be placed.
# If a file matching the material reference (e.g. metal052.bmp) exists there it
# will be used as the atlas tile instead of the solid-colour fallback.
MATERIALS_DIR = "materials"

# Built-in material colours for "material:xxx.bmp" texture references
# (used when no matching file is found in MATERIALS_DIR)
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

# PBR (metallic/roughness) properties inferred from material name substrings.
# Each entry: name_substring -> (metallic_factor, roughness_factor)
# Used by detect_material_pbr() and passed on to the GLB builder.
MATERIAL_PBR: dict[str, tuple[float, float]] = {
    "metal": (1.0, 0.35),
    "chrome": (1.0, 0.10),
    "steel": (1.0, 0.40),
    "iron": (0.9, 0.50),
    "copper": (1.0, 0.40),
    "gold": (1.0, 0.30),
    "silver": (1.0, 0.20),
    "aluminum": (1.0, 0.30),
    "aluminium": (1.0, 0.30),
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
# NOTE: build_texture_atlas() and compute_atlas_source_hash() were extracted into
# scripts/atlas.py. Import them from there (`from atlas import ...`). They are not
# re-exported here to keep the dependency one-way (atlas.py imports upload_utils).


def detect_material_pbr(texture_refs: list[str]) -> tuple[float, float]:
    """Infer dominant PBR metallic/roughness from a model's texture references.

    Scans ``material:`` references and pattern-matches against ``MATERIAL_PBR``.
    Pack textures (``pack:``) are treated as non-metallic and dilute the result.

    Returns ``(metallic_factor, roughness_factor)``; defaults to ``(0.0, 0.8)``
    when no metallic materials are detected.
    """
    metallic_sum = 0.0
    roughness_sum = 0.0
    matched = 0

    for ref in texture_refs:
        if ref.startswith("material:"):
            mat_name = ref[9:].replace(".bmp", "").lower()
            for pattern, (mf, rf) in MATERIAL_PBR.items():
                if pattern in mat_name:
                    metallic_sum += mf
                    roughness_sum += rf
                    matched += 1
                    break

    if matched == 0:
        return 0.0, 0.8

    # Dilute by pack textures — assume they carry non-metallic surface colour
    pack_count = sum(1 for ref in texture_refs if ref.startswith("pack:"))
    total = matched + pack_count
    return metallic_sum / total, (roughness_sum + pack_count * 0.8) / total


# ---------------------------------------------------------------------------
# GLB conversion
# ---------------------------------------------------------------------------

def convert_model_xml_to_glb(
    model_index_xml: Path,
    output_glb: Path,
    atlas_png: bytes,
    atlas_map: dict[str, tuple[float, float, float, float]],
    metallic_factor: float = 0.0,
    roughness_factor: float = 0.8,
) -> dict:
    """Convert a model XML ``<model>`` section to glTF Binary (.glb).

    Works with both loco-style roots and simplemodel roots — finds the
    ``<model>`` child element containing ``<polygon>/<vertex>`` data.
    UVs are remapped into the atlas. When ``atlas_png`` is empty a plain
    grey PBR material is used instead.

    ``metallic_factor`` and ``roughness_factor`` control the PBR material
    properties; use ``detect_material_pbr()`` to derive sensible values.

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
                "metallicFactor": metallic_factor,
                "roughnessFactor": roughness_factor,
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
                "metallicFactor": metallic_factor,
                "roughnessFactor": roughness_factor,
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
