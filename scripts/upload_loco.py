#!/usr/bin/env python3
"""
Upload locomotive assets to Roblox and generate a Lua metadata file.

Usage:
    python upload_loco.py <loco_dir> [--dry-run]

Configure ROBLOX_API_KEY and ROBLOX_CREATOR_ID in a .env file at the project root,
or pass them via --api-key / --creator-id flags (flags override .env values).

Example:
    python upload_loco.py db/loco/loco/electric/cd_163
    python upload_loco.py db/loco/loco/electric/cd_163 --dry-run
"""

import argparse
import hashlib
import json
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
TEXTURE_LOCKFILE = "roblox_assets.lock.json"
MESH_LOCKFILE = "roblox_meshes.lock.json"


def _repo_root() -> Path:
    return Path(__file__).resolve().parent.parent


def load_lockfile(name: str) -> dict:
    """Load a lockfile by name. Returns {content_hash: asset_id} mapping."""
    path = _repo_root() / name
    if path.exists():
        return json.loads(path.read_text(encoding="utf-8"))
    return {}


def save_lockfile(name: str, lockdata: dict):
    """Save a lockfile (sorted for stable diffs)."""
    path = _repo_root() / name
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


def convert_bmp_to_png(bmp_path: Path) -> Path:
    """Convert BMP to PNG (Roblox doesn't accept BMP). Returns path to PNG."""
    png_path = bmp_path.with_suffix(".png")
    if not HAS_PIL:
        print(f"  [WARN] Pillow not installed, cannot convert {bmp_path.name}. Install with: pip install Pillow")
        return bmp_path
    img = Image.open(bmp_path)
    img.save(png_path, "PNG")
    return png_path


def parse_loco_metadata(index_xml_path: Path) -> dict:
    """Parse the loco index.xml and extract all metadata attributes."""
    tree = ET.parse(index_xml_path)
    root = tree.getroot()
    # Extract all attributes from the root <loco> element
    metadata = {}
    attr_map = {
        "name": "name",
        "title": "title",
        "author": "author",
        "email": "email",
        "maxspeed": "maxspeed",
        "weight": "weight",
        "power": "power",
        "path": "path",
        "rep.name": "repName",
        "smoke": "smoke",
        "smoke_x": "smokeX",
        "smoke_y": "smokeY",
        "default": "default",
    }
    for xml_attr, lua_key in attr_map.items():
        val = root.get(xml_attr)
        if val is not None:
            metadata[lua_key] = val
    return metadata


def collect_textures(model_dir: Path) -> list[dict]:
    """Collect all BMP texture files from the model directory (recursive)."""
    textures = []
    for bmp_file in sorted(model_dir.rglob("*.bmp")):
        rel = bmp_file.relative_to(model_dir)
        textures.append({
            "path": str(bmp_file),
            "relativePath": str(rel),
            "name": bmp_file.stem,
        })
    return textures


def convert_model_xml_to_glb(model_index_xml: Path, output_glb: Path) -> dict:
    """Convert the simplemodel XML <model> section to glTF Binary (.glb).

    Returns stats dict with vertex/triangle counts.
    """
    tree = ET.parse(model_index_xml)
    root = tree.getroot()

    model_el = root.find("model")
    if model_el is None:
        print("  [ERROR] No <model> element found in model/index.xml")
        return {"vertices": 0, "faces": 0}

    # Collect vertex data and triangulate
    positions = []   # flat floats: x, y, z, x, y, z, ...
    normals = []     # flat floats: nx, ny, nz, ...
    texcoords = []   # flat floats: u, v, ...
    indices = []     # triangle indices
    vertex_count = 0
    triangle_count = 0

    for polygon in model_el.findall("polygon"):
        nx = float(polygon.get("nx", "0"))
        ny = float(polygon.get("ny", "0"))
        nz = float(polygon.get("nz", "0"))

        poly_verts = []
        for vert in polygon.findall("vertex"):
            x = float(vert.get("x", "0"))
            y = float(vert.get("y", "0"))
            z = float(vert.get("z", "0"))
            s = float(vert.get("s", "0"))
            t = float(vert.get("t", "0"))
            poly_verts.append((x, y, z, nx, ny, nz, s, 1.0 - t))

        if len(poly_verts) < 3:
            continue

        # Add vertices
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
        print("  [ERROR] No vertices found in model")
        return {"vertices": 0, "faces": 0}

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

    # Pad each section to 4-byte alignment
    def pad4(data):
        remainder = len(data) % 4
        return data + b'\x00' * (4 - remainder) if remainder else data

    pos_data = pad4(pos_data)
    norm_data = pad4(norm_data)
    uv_data = pad4(uv_data)
    idx_data = pad4(idx_data)

    # Buffer layout offsets
    pos_offset = 0
    norm_offset = pos_offset + len(pos_data)
    uv_offset = norm_offset + len(norm_data)
    idx_offset = uv_offset + len(uv_data)
    bin_data = pos_data + norm_data + uv_data + idx_data

    # Bounding box for positions accessor (required by glTF spec)
    min_pos = [float('inf')] * 3
    max_pos = [float('-inf')] * 3
    for i in range(vertex_count):
        for c in range(3):
            val = positions[i * 3 + c]
            min_pos[c] = min(min_pos[c], val)
            max_pos[c] = max(max_pos[c], val)

    # Build glTF JSON descriptor
    gltf = {
        "asset": {"version": "2.0", "generator": "vlakosimdb upload_loco.py"},
        "scene": 0,
        "scenes": [{"nodes": [0]}],
        "nodes": [{"mesh": 0}],
        "meshes": [{
            "primitives": [{
                "attributes": {
                    "POSITION": 0,
                    "NORMAL": 1,
                    "TEXCOORD_0": 2,
                },
                "indices": 3,
            }]
        }],
        "accessors": [
            {
                "bufferView": 0, "componentType": 5126, "count": vertex_count,
                "type": "VEC3", "min": min_pos, "max": max_pos,
            },
            {
                "bufferView": 1, "componentType": 5126, "count": vertex_count,
                "type": "VEC3",
            },
            {
                "bufferView": 2, "componentType": 5126, "count": vertex_count,
                "type": "VEC2",
            },
            {
                "bufferView": 3, "componentType": idx_component_type,
                "count": len(indices), "type": "SCALAR",
            },
        ],
        "bufferViews": [
            {"buffer": 0, "byteOffset": pos_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": norm_offset, "byteLength": vertex_count * 12, "target": 34962},
            {"buffer": 0, "byteOffset": uv_offset, "byteLength": vertex_count * 8, "target": 34962},
            {"buffer": 0, "byteOffset": idx_offset, "byteLength": idx_byte_len, "target": 34963},
        ],
        "buffers": [{"byteLength": len(bin_data)}],
    }

    json_str = json.dumps(gltf, separators=(",", ":"))
    # Pad JSON chunk to 4-byte alignment with spaces (glTF spec)
    while len(json_str) % 4 != 0:
        json_str += " "
    json_data = json_str.encode("utf-8")

    # Write GLB: header + JSON chunk + BIN chunk
    total_length = 12 + 8 + len(json_data) + 8 + len(bin_data)
    with open(output_glb, "wb") as f:
        # GLB header
        f.write(struct.pack("<I", 0x46546C67))  # magic "glTF"
        f.write(struct.pack("<I", 2))            # version 2
        f.write(struct.pack("<I", total_length))
        # JSON chunk
        f.write(struct.pack("<I", len(json_data)))
        f.write(struct.pack("<I", 0x4E4F534A))   # chunk type "JSON"
        f.write(json_data)
        # BIN chunk
        f.write(struct.pack("<I", len(bin_data)))
        f.write(struct.pack("<I", 0x004E4942))    # chunk type "BIN\0"
        f.write(bin_data)

    stats = {"vertices": vertex_count, "faces": triangle_count}
    print(f"  GLB written: {output_glb} ({stats['vertices']} vertices, {stats['faces']} triangles)")
    return stats


def upload_mesh_to_roblox(obj_path: Path, api_key: str, creator_id: str,
                          display_name: str, description: str) -> str | None:
    """Upload an OBJ mesh to Roblox via Open Cloud Assets API. Returns asset ID."""
    if not HAS_REQUESTS:
        print("  [ERROR] 'requests' library required. Install with: pip install requests")
        return None

    request_body = {
        "assetType": "Model",
        "displayName": display_name[:50],
        "description": description[:1000],
        "creationContext": {
            "creator": {
                "userId": creator_id,
            }
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
        "assetType": "Decal",
        "displayName": display_name[:50],  # Roblox limit
        "description": description[:1000],
        "creationContext": {
            "creator": {
                "userId": creator_id,
            }
        },
    }

    with open(png_path, "rb") as f:
        image_data = f.read()

    resp = requests.post(
        ROBLOX_ASSETS_API,
        headers={
            "x-api-key": api_key,
        },
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
    # The API returns an operation; poll for completion
    operation_path = result.get("path")
    if operation_path and not result.get("done"):
        asset_id = poll_operation(operation_path, api_key)
        return asset_id

    # If done immediately
    if result.get("done") and result.get("response"):
        return result["response"].get("assetId")

    return result.get("assetId")


def poll_operation(operation_path: str, api_key: str, max_attempts: int = 20) -> str | None:
    """Poll a Roblox operation until completion."""
    # operation_path is e.g. "operations/xxx" — needs the /assets/v1/ prefix
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


def generate_lua(metadata: dict, thumbnail_asset_id: str,
                 texture_assets: list[dict], mesh_asset_id: str,
                 output_path: Path):
    """Generate a Lua metadata file."""
    lines = []
    lines.append("-- Auto-generated locomotive metadata")
    lines.append(f"-- Source: {metadata.get('name', 'unknown')}")
    lines.append("")
    lines.append("local LocoData = {")

    # Metadata
    lines.append("\t-- Metadata from index.xml")
    lines.append(f'\tname = "{metadata.get("name", "")}",') 
    lines.append(f'\ttitle = "{metadata.get("title", "")}",') 
    lines.append(f'\tauthor = "{metadata.get("author", "")}",') 
    lines.append(f'\temail = "{metadata.get("email", "")}",') 

    # Numeric fields
    for key in ("maxspeed", "weight", "power"):
        val = metadata.get(key, "0")
        lines.append(f"\t{key} = {val},")

    lines.append(f'\tpath = "{metadata.get("path", "")}",') 
    lines.append("")

    # Thumbnail
    lines.append("\t-- Thumbnail image asset")
    lines.append(f'\tthumbnailAssetId = "{thumbnail_asset_id}",') 
    lines.append("")

    # 3D Mesh
    lines.append("\t-- 3D mesh asset")
    lines.append(f'\tmeshAssetId = "{mesh_asset_id}",') 

    # Textures
    lines.append("\t-- Model textures")
    lines.append("\ttextures = {")
    for tex in texture_assets:
        asset_id = tex.get("assetId", "PENDING")
        rel_path = tex.get("relativePath", "")
        lines.append(f'\t\t{{ name = "{tex["name"]}", path = "{rel_path}", assetId = "{asset_id}" }},')
    lines.append("\t},")

    lines.append("}")
    lines.append("")
    lines.append("return LocoData")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"Lua file written to: {output_path}")


def main():
    # Load .env from project root (two levels up from scripts/)
    env_path = Path(__file__).resolve().parent.parent / ".env"
    load_dotenv(env_path)

    parser = argparse.ArgumentParser(description="Upload loco assets to Roblox and generate Lua metadata")
    parser.add_argument("loco_dir", help="Path to locomotive directory (e.g. db/loco/loco/electric/cd_163)")
    parser.add_argument("--api-key", help="Roblox Open Cloud API key (overrides ROBLOX_API_KEY from .env)")
    parser.add_argument("--creator-id", help="Roblox user/creator ID (overrides ROBLOX_CREATOR_ID from .env)")
    parser.add_argument("--dry-run", action="store_true", help="Skip uploads, generate Lua with placeholder IDs")
    parser.add_argument("--output", help="Output Lua file path (default: <loco_dir>/loco_data.lua)")
    args = parser.parse_args()

    # Resolve credentials: CLI flags > .env > missing
    args.api_key = args.api_key or os.environ.get("ROBLOX_API_KEY")
    args.creator_id = args.creator_id or os.environ.get("ROBLOX_CREATOR_ID")

    loco_dir = Path(args.loco_dir).resolve()
    if not loco_dir.is_dir():
        print(f"Error: {loco_dir} is not a directory")
        sys.exit(1)

    index_xml = loco_dir / "index.xml"
    if not index_xml.exists():
        print(f"Error: {index_xml} not found")
        sys.exit(1)

    thumbnail = loco_dir / "image.bmp"
    model_dir = loco_dir / "model"

    # 1. Parse metadata
    print(f"Parsing metadata from {index_xml}...")
    metadata = parse_loco_metadata(index_xml)
    print(f"  Name: {metadata.get('name')}  Title: {metadata.get('title')}")
    print(f"  Author: {metadata.get('author')}  Max Speed: {metadata.get('maxspeed')} km/h")

    # 2. Collect textures
    textures = []
    if model_dir.is_dir():
        textures = collect_textures(model_dir)
        print(f"  Found {len(textures)} texture(s) in model/")

    # 3. Convert model XML to OBJ mesh
    model_xml = model_dir / "index.xml" if model_dir.is_dir() else None
    glb_path = None
    if model_xml and model_xml.exists():
        glb_path = loco_dir / "model.glb"
        print(f"\nConverting 3D model to GLB...")
        convert_model_xml_to_glb(model_xml, glb_path)
    else:
        print("  [WARN] model/index.xml not found, skipping mesh conversion")

    # 4. Load lockfiles
    tex_lockdata = load_lockfile(TEXTURE_LOCKFILE)
    mesh_lockdata = load_lockfile(MESH_LOCKFILE)
    uploads_made = 0
    cache_hits = 0

    # 5. Upload or use placeholders
    thumbnail_asset_id = "PENDING_UPLOAD"
    mesh_asset_id = "PENDING_UPLOAD"
    texture_assets = [{"name": t["name"], "relativePath": t["relativePath"], "assetId": "PENDING_UPLOAD"}
                      for t in textures]

    if args.dry_run:
        print("\n[DRY RUN] Skipping uploads, using placeholder asset IDs.")
    else:
        if not args.api_key or not args.creator_id:
            print("\nError: --api-key and --creator-id are required for upload (or use --dry-run)")
            sys.exit(1)

        # Upload thumbnail
        if thumbnail.exists():
            thumb_hash = file_content_hash(thumbnail)
            if thumb_hash in tex_lockdata:
                thumbnail_asset_id = tex_lockdata[thumb_hash]
                cache_hits += 1
                print(f"\nThumbnail: {thumbnail.name} [CACHED] assetId = {thumbnail_asset_id}")
            else:
                print(f"\nUploading thumbnail: {thumbnail.name}...")
                asset_id = upload_image_to_roblox(
                    thumbnail, args.api_key, args.creator_id,
                    display_name=f"{metadata.get('title', 'Loco')} Thumbnail",
                    description=f"Thumbnail for {metadata.get('title', '')} by {metadata.get('author', '')}",
                )
                if asset_id:
                    thumbnail_asset_id = asset_id
                    tex_lockdata[thumb_hash] = asset_id
                    save_lockfile(TEXTURE_LOCKFILE, tex_lockdata)
                    uploads_made += 1
                    print(f"  Uploaded: assetId = {asset_id}")
        else:
            print(f"  [WARN] Thumbnail not found: {thumbnail}")

        # Upload textures
        for i, tex in enumerate(textures):
            tex_path = Path(tex["path"])
            tex_hash = file_content_hash(tex_path)
            if tex_hash in tex_lockdata:
                texture_assets[i]["assetId"] = tex_lockdata[tex_hash]
                cache_hits += 1
                print(f"\nTexture [{i+1}/{len(textures)}]: {tex['relativePath']} [CACHED] assetId = {tex_lockdata[tex_hash]}")
            else:
                print(f"\nUploading texture [{i+1}/{len(textures)}]: {tex['relativePath']}...")
                asset_id = upload_image_to_roblox(
                    tex_path, args.api_key, args.creator_id,
                    display_name=f"{metadata.get('name', 'Loco')}_{tex['name']}",
                    description=f"Texture {tex['relativePath']} for {metadata.get('title', '')}",
                )
                if asset_id:
                    texture_assets[i]["assetId"] = asset_id
                    tex_lockdata[tex_hash] = asset_id
                    save_lockfile(TEXTURE_LOCKFILE, tex_lockdata)
                    uploads_made += 1
                    print(f"  Uploaded: assetId = {asset_id}")

        # Upload 3D mesh
        if glb_path and glb_path.exists():
            mesh_hash = file_content_hash(model_xml)
            if mesh_hash in mesh_lockdata:
                mesh_asset_id = mesh_lockdata[mesh_hash]
                cache_hits += 1
                print(f"\nMesh: model.glb [CACHED] assetId = {mesh_asset_id}")
            else:
                print(f"\nUploading 3D mesh: model.glb...")
                asset_id = upload_mesh_to_roblox(
                    glb_path, args.api_key, args.creator_id,
                    display_name=f"{metadata.get('title', 'Loco')} Mesh",
                    description=f"3D mesh for {metadata.get('title', '')} by {metadata.get('author', '')}",
                )
                if asset_id:
                    mesh_asset_id = asset_id
                    mesh_lockdata[mesh_hash] = asset_id
                    save_lockfile(MESH_LOCKFILE, mesh_lockdata)
                    uploads_made += 1
                    print(f"  Uploaded: assetId = {asset_id}")

        print(f"\nSummary: {uploads_made} uploaded, {cache_hits} cached")

    # 6. Generate Lua
    output_path = Path(args.output) if args.output else loco_dir / "loco_data.lua"
    print(f"\nGenerating Lua metadata file...")
    generate_lua(metadata, thumbnail_asset_id, texture_assets, mesh_asset_id, output_path.resolve())
    print("Done!")


if __name__ == "__main__":
    main()
