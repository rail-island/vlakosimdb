"""
Small helpers shared by the Python texture tools (atlas.py, atlas_padded.py,
remaster_textures.py, remaster_atlas.py): the repo root, the Windows-1250
tolerant XML reader, the built-in material colours and the INPUT_REMASTER list
reader.

The model converter itself is TypeScript (scripts/models/); these Python tools
only prepare and assemble AI-remastered textures.
"""

import sys
import xml.etree.ElementTree as ET
from pathlib import Path

# Built-in colours for "material:xxx.bmp" texture references. The repo ships no
# material bitmaps, so every material reference resolves to one of these (or grey).
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


def _repo_root() -> Path:
    """Return the repository root (parent of the scripts/ directory)."""
    return Path(__file__).resolve().parent.parent


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


def resolve_input_dirs(input_file: Path) -> list[tuple[str | None, Path]]:
    """Read a model list file (INPUT_REMASTER) and resolve paths, expanding wildcards.

    Lines may start with !PREFIX: (a path override, returned alongside the dir).
    Paths ending with ``/*`` expand one directory level, ``/*/*`` two levels.
    ``SKIP:path`` excludes a directory; lines starting with # are comments.
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

        if line.upper().startswith("SKIP:"):
            skip_dirs.add((root / line[5:]).resolve())
            continue

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
