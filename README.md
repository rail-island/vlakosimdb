# VLAKosimDB

The train, bus and tram model database of the JT7 simulator, and the converter that
turns it into the GLB vehicles [Rail Island](https://railisland.com) ships.

The models are JT7's own format: an `index.xml` with the vehicle's metadata, a
`model/index.xml` with its geometry (or the procedural `simplemodel` description the
JT7 editor generates it from) and BMP textures. This repository holds those sources
and builds them; Rail Island never reads them directly.

## Layout

| Path | What it is |
| --- | --- |
| `db/train/` | Trains: `loco/{electric,diesel,steam,muv}`, `coach`, `emu`, `dmu`, `truck`, `760_mm`, `metro`, `tram`, ... |
| `db/motocar/` | Buses. |
| `db/tram/` | Trams. |
| `scripts/models/` | The converter (TypeScript) and `export.ts`, the one command that runs it. |
| `scripts/*.py`, `INPUT_REMASTER` | AI texture remastering (see below). |
| `test/` | Vitest: golden geometry, articulation, metadata, thumbnail renderer, the id contract. |
| `jt7_src/` | The original JT7 C++ sources, kept as the reference for the format and the `simplemodel` port. |

Each model folder looks like `db/train/loco/electric/cd_163/`:

- `index.xml`: metadata (name, title, author, weight, power, maxspeed, body style, smoke anchor), Windows-1250.
- `image.bmp`: the hand-drawn side view (`image_4x.png` when remastered), used only when a thumbnail cannot be rendered.
- `model/index.xml`: geometry, plus its textures (`x_dec.bmp`, ... and the AI-remastered `*_4x.png` next to them) and an optional `options.json` of texture flips.

## Export

Requires Node.js 20+.

```bash
npm install
npm run export          # -> export/
npm test
npm run typecheck
```

`npm run export` converts every model and writes, per tree (`motocar`, `train`, `tram`):

| File | Content |
| --- | --- |
| `export/<tree>/glb/<id>.glb` | The vehicle: geometry, one embedded texture atlas (4x when remastered textures exist), articulated vehicles as named section nodes. Raw, unencoded. |
| `export/<tree>/thumbs/<id>.webp` | A 320x120 side view rendered from that GLB (a `.png` of the drawn side view only if the render fails). |
| `export/<tree>/<tree>.meta.json` | `{ category, vehicles: [...] }`, one row per vehicle: id, file names, name, title, subcategory, traction, author, email, weight, power, maxspeed, length, bbox, vertex/face counts, hires, source folder, texture flips, articulation and smoke anchor. |

`export/` is gitignored. A model that fails to convert is reported and makes the
command exit non-zero.

**Vehicle ids are a contract.** An id is the category (`moto`, `train`, `tram`) plus
the model's path below its tree, slugged: `db/train/loco/electric/cd_163` is
`train-loco-electric-cd-163`. Rail Island stores ids in saved maps, so
`test/export.test.ts` pins the full list in `test/fixtures/vehicle-ids.json`. Moving,
renaming or deleting a model folder fails that test. Regenerate the fixture only when
the change is intended, knowing the old id disappears from the game.

## Using it from Rail Island

Rail Island's `pnpm import:vehicles` expects this repository checked out next to its
own (`../vlakosimdb`). It runs `npm run export` here and then replaces its
`db/motocar`, `db/train` and `db/tram` with the three `export/` trees, which it
commits. Everything that depends on the game's runtime (LOD proxies, meshopt
encoding, hashed file names, archiving) happens on the Rail Island side.

The side-view camera, lights and backdrop live in `scripts/models/thumbView.ts`, a
copy of Rail Island's `src/models/thumbView.ts`. Keep the two in step so a shipped
vehicle and a player's upload are photographed alike.

## Remastering textures

AI 4x upscaling (gpt-image-2) writes a `*_4x.png` next to each source texture and an
`image_4x.png` next to `image.bmp`. The export prefers them automatically.

- `python3 scripts/remaster_atlas.py prepare|assemble --model <name>`: one model,
  one AI call. The `remaster-atlas` skill (`.claude/skills/`) drives it.
- `python3 scripts/remaster_textures.py`: the batch path over the models listed in
  `INPUT_REMASTER`.

Both need Python 3 with `pillow`, `python-dotenv` and `openai`, and an
`OPENAI_API_KEY` in `.env`. After remastering, run the export and the Rail Island
import to see the result in the game.
