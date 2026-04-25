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
import os
import sys
from pathlib import Path

from dotenv import load_dotenv

from upload_utils import (
    _repo_root,
    build_texture_atlas,
    compute_atlas_source_hash,
    convert_model_xml_to_glb,
    file_content_hash,
    load_lockfile,
    make_lock_key,
    parse_xml_safe,
    save_lockfile,
    upload_image_to_roblox,
    upload_mesh_to_roblox,
)

INPUT_FILE = "INPUT_BUILDING"
OUTPUT_LUA = "building_data.lua"


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
