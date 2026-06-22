#!/usr/bin/env python3
"""
Upload locomotive assets to Roblox and generate a combined Lua metadata file.

Usage:
    python upload_loco.py [--dry-run]

Reads INPUT_LOCO file from the project root for the list of loco directories.
Each line is a path relative to the repo root, e.g.:
    db/loco/loco/electric/cd_163
    db/loco/loco/electric/*

Lines ending with /* will expand to all sub-directories.

Configure ROBLOX_UPLOAD_ASSETS_KEY (falls back to ROBLOX_API_KEY) and
ROBLOX_CREATOR_ID in a .env file at the project root, or pass them via
--api-key / --creator-id flags (flags override .env values).
"""

import argparse
import hashlib
import os
import sys
from pathlib import Path

from dotenv import load_dotenv

from atlas import build_texture_atlas, compute_atlas_source_hash
from upload_utils import (
    _repo_root,
    convert_model_xml_to_glb,
    detect_material_pbr,
    file_content_hash,
    load_lockfile,
    make_lock_key,
    parse_xml_safe,
    save_lockfile,
    upload_image_to_roblox,
    upload_mesh_to_roblox,
)

INPUT_FILE = "INPUT_LOCO"
OUTPUT_LUA = "loco_data.lua"


def parse_loco_metadata(index_xml_path: Path) -> dict:
    """Parse the loco index.xml and extract all metadata attributes."""
    root = parse_xml_safe(index_xml_path)
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


def resolve_input_dirs(input_file: Path) -> list[tuple[str | None, Path]]:
    """Generate a single Lua file with all locomotive data."""
    lines = []
    lines.append("-- Auto-generated locomotive database")
    lines.append(f"-- {len(all_locos)} locomotive(s)")
    lines.append("")
    lines.append("local LocoDatabase = {")

    for loco in all_locos:
        meta = loco["metadata"]
        key = meta.get("name", "unknown")
        lines.append(f'\t["{key}"] = {{')

        lines.append(f'\t\tname = "{meta.get("name", "")}",')
        lines.append(f'\t\ttitle = "{meta.get("title", "")}",')
        lines.append(f'\t\tauthor = "{meta.get("author", "")}",')
        lines.append(f'\t\temail = "{meta.get("email", "")}",')

        for field in ("maxspeed", "weight", "power"):
            val = meta.get(field, "0")
            lines.append(f"\t\t{field} = {val},")

        lines.append(f'\t\tpath = "{meta.get("path", "")}",')
        lines.append(f'\t\tlength = {loco["length"]},')
        lines.append(f'\t\tthumbnailAssetId = "{loco["thumbnailAssetId"]}",')
        lines.append(f'\t\tmodelAssetId = "{loco["modelAssetId"]}",')
        lines.append("\t},")

    lines.append("}")
    lines.append("")
    lines.append("return LocoDatabase")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"\nLua file written to: {output_path}")


def resolve_input_dirs(input_file: Path) -> list[tuple[str | None, Path]]:
    """Read INPUT_LOCO file and resolve paths, expanding wildcards.

    Lines may start with !PREFIX: to override the 'path' field in the output,
    e.g. ``!EMU:db/train/emu/*/*`` sets path="EMU" for all matched dirs.
    Paths ending with ``/*/*`` expand two directory levels deep.
    """
    if not input_file.exists():
        print(f"Error: {input_file} not found")
        sys.exit(1)

    root = _repo_root()
    dirs: list[tuple[str | None, Path]] = []
    skip_dirs: set[Path] = set()

    for raw_line in input_file.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        # Handle SKIP:path — collect skip set first pass isn't needed;
        # we handle it inline and continue.
        if line.upper().startswith("SKIP:"):
            skip_dirs.add((root / line[5:]).resolve())
            continue

        # Handle !PREFIX:path syntax — strip ! and split on first :
        path_override: str | None = None
        if line.startswith("!"):
            prefix, _, rest = line[1:].partition(":")
            path_override = prefix
            line = rest

        if line.endswith("/*/*"):
            parent = root / line[:-4]
            if not parent.is_dir():
                print(f"  [WARN] Directory not found: {parent}")
                continue
            for child in sorted(parent.iterdir()):
                if child.is_dir():
                    for grandchild in sorted(child.iterdir()):
                        if grandchild.is_dir() and (grandchild / "index.xml").exists():
                            if grandchild.resolve() not in skip_dirs:
                                dirs.append((path_override, grandchild))
        elif line.endswith("/*"):
            parent = root / line[:-2]
            if not parent.is_dir():
                print(f"  [WARN] Directory not found: {parent}")
                continue
            for child in sorted(parent.iterdir()):
                if child.is_dir() and (child / "index.xml").exists():
                    if child.resolve() not in skip_dirs:
                        dirs.append((path_override, child))
        else:
            d = root / line
            if d.is_dir():
                if d.resolve() not in skip_dirs:
                    dirs.append((path_override, d))
            else:
                print(f"  [WARN] Directory not found: {d}")

    return dirs


def process_loco(loco_dir: Path, lockdata: dict, api_key: str | None,
                 creator_id: str | None, dry_run: bool) -> dict:
    """Process a single locomotive: atlas, GLB, upload. Returns loco result dict."""
    index_xml = loco_dir / "index.xml"
    thumbnail = loco_dir / "image.bmp"
    model_dir = loco_dir / "model"
    model_xml = model_dir / "index.xml" if model_dir.is_dir() else None
    loco_rel = str(loco_dir.relative_to(_repo_root()))

    # 1. Parse metadata
    metadata = parse_loco_metadata(index_xml)
    print(f"  Name: {metadata.get('name')}  Title: {metadata.get('title')}")

    # 2. Build texture atlas
    atlas_png = b""
    atlas_map = {}
    atlas_path = None
    glb_path = None
    tex_refs: list[str] = []

    atlas_4x_path = loco_dir / "atlas_4x.png"
    use_4x_atlas = False

    if model_xml and model_xml.exists():
        atlas_png, atlas_map, tex_refs = build_texture_atlas(model_dir, model_xml)
        if atlas_png:
            atlas_path = loco_dir / "atlas.png"
            atlas_path.write_bytes(atlas_png)

        # Prefer the AI-remastered 4x atlas when present. It is built to match the
        # compact build_texture_atlas layout exactly (just 4x resolution), so the
        # normalized UVs in atlas_map are unchanged — it is a drop-in higher-res
        # texture for the GLB.
        glb_atlas = atlas_png
        if atlas_png and atlas_4x_path.exists():
            glb_atlas = atlas_4x_path.read_bytes()
            use_4x_atlas = True
            print(f"  Using AI-remastered atlas: {atlas_4x_path.name}")

        glb_path = loco_dir / "model.glb"
        metallic, roughness = detect_material_pbr(tex_refs)
        glb_stats = convert_model_xml_to_glb(model_xml, glb_path, glb_atlas, atlas_map,
                                             metallic_factor=metallic, roughness_factor=roughness)

    # 3. Upload or placeholders
    thumbnail_asset_id = "PENDING_UPLOAD"
    mesh_asset_id = "PENDING_UPLOAD"
    length = glb_stats.get("length", 0) if model_xml and model_xml.exists() else 0
    uploads_made = 0
    cache_hits = 0

    if not dry_run:
        # Thumbnail
        if thumbnail.exists():
            thumb_hash = file_content_hash(thumbnail)
            thumb_key = make_lock_key(f"{loco_rel}/image.bmp", thumb_hash)
            if thumb_key in lockdata:
                thumbnail_asset_id = lockdata[thumb_key]
                cache_hits += 1
            else:
                asset_id = upload_image_to_roblox(
                    thumbnail, api_key, creator_id,
                    display_name=f"{metadata.get('title', 'Loco')} Thumbnail",
                    description=f"Thumbnail for {metadata.get('title', '')} by {metadata.get('author', '')}",
                )
                if asset_id:
                    thumbnail_asset_id = asset_id
                    lockdata[thumb_key] = asset_id
                    save_lockfile(lockdata)
                    uploads_made += 1

        # Mesh (model)
        if glb_path and glb_path.exists():
            model_hash = file_content_hash(model_xml)
            # When the 4x atlas is baked into the GLB, key the lockfile on that file
            # so editing/regenerating atlas_4x.png re-uploads the mesh. Otherwise key
            # on the source pack textures as before.
            if use_4x_atlas:
                atlas_hash_for_mesh = file_content_hash(atlas_4x_path)
            else:
                atlas_hash_for_mesh = compute_atlas_source_hash(model_dir, tex_refs)
            combined_hash = hashlib.sha256(
                (model_hash + atlas_hash_for_mesh).encode()
            ).hexdigest()
            mesh_key = make_lock_key(f"{loco_rel}/model.glb", combined_hash)
            if mesh_key in lockdata:
                mesh_asset_id = lockdata[mesh_key]
                cache_hits += 1
            else:
                asset_id = upload_mesh_to_roblox(
                    glb_path, api_key, creator_id,
                    display_name=f"{metadata.get('title', 'Loco')} Mesh",
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
        "length": length,
    }


def generate_combined_lua(all_locos: list[dict], output_path: Path):
    """Generate a single Lua file with all loco data."""
    lines = []
    lines.append("-- Auto-generated locomotive database")
    lines.append(f"-- {len(all_locos)} locomotive(s)")
    lines.append("")
    lines.append("local LocoDatabase = {")

    for loco in all_locos:
        meta = loco["metadata"]
        key = meta.get("name", "unknown")
        lines.append(f'\t["{key}"] = {{')

        lines.append(f'\t\tname = "{meta.get("name", "")}",')
        lines.append(f'\t\ttitle = "{meta.get("title", "")}",')
        lines.append(f'\t\tauthor = "{meta.get("author", "")}",')
        lines.append(f'\t\temail = "{meta.get("email", "")}",')

        for field in ("maxspeed", "weight", "power"):
            try:
                val = int(meta.get(field, 0))
            except (ValueError, TypeError):
                val = 0
            lines.append(f"\t\t{field} = {val},")

        lines.append(f'\t\tpath = "{meta.get("path", "")}",')
        lines.append(f'\t\tlength = {loco.get("length", 0)},')
        lines.append(f'\t\tthumbnailAssetId = "{loco["thumbnailAssetId"]}",')
        lines.append(f'\t\tmodelAssetId = "{loco["modelAssetId"]}",')
        lines.append("\t},")

    lines.append("}")
    lines.append("")
    lines.append("return LocoDatabase")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"\nLua file written to: {output_path}")


def main():
    env_path = _repo_root() / ".env"
    load_dotenv(env_path)

    parser = argparse.ArgumentParser(
        description="Upload loco assets to Roblox, reading directories from INPUT_LOCO"
    )
    parser.add_argument("--api-key", help="Roblox Open Cloud API key (overrides .env)")
    parser.add_argument("--creator-id", help="Roblox user/creator ID (overrides .env)")
    parser.add_argument("--dry-run", action="store_true", help="Skip uploads, use placeholder IDs")
    parser.add_argument("--input", help=f"Input file (default: {INPUT_FILE})")
    parser.add_argument("--output", help=f"Output Lua file (default: {OUTPUT_LUA})")
    args = parser.parse_args()

    api_key = (args.api_key or os.environ.get("ROBLOX_UPLOAD_ASSETS_KEY")
               or os.environ.get("ROBLOX_API_KEY"))
    creator_id = args.creator_id or os.environ.get("ROBLOX_CREATOR_ID")

    if not args.dry_run and (not api_key or not creator_id):
        print("Error: --api-key and --creator-id are required (or use --dry-run)")
        sys.exit(1)

    # Resolve input directories
    input_file = _repo_root() / (args.input or INPUT_FILE)
    print(f"Reading input from: {input_file}")
    loco_dirs = resolve_input_dirs(input_file)

    if not loco_dirs:
        print("No locomotive directories found.")
        sys.exit(1)

    print(f"Found {len(loco_dirs)} locomotive(s) to process.\n")

    # Load lockfile once
    lockdata = load_lockfile()
    all_locos: list[dict] = []

    for i, (path_override, loco_dir) in enumerate(loco_dirs, 1):
        loco_rel = loco_dir.relative_to(_repo_root())
        print(f"[{i}/{len(loco_dirs)}] Processing {loco_rel}...")

        if not (loco_dir / "index.xml").exists():
            print(f"  [SKIP] No index.xml found")
            continue

        result = process_loco(loco_dir, lockdata, api_key, creator_id, args.dry_run)
        if path_override is not None:
            result["metadata"]["path"] = path_override
        all_locos.append(result)

    # Generate combined Lua
    output_path = _repo_root() / (args.output or OUTPUT_LUA)
    generate_combined_lua(all_locos, output_path)
    print(f"\nDone! Processed {len(all_locos)} locomotive(s).")


if __name__ == "__main__":
    main()
