# VLAKosimDB — 3D Train Model Browser

A web app for browsing and inspecting the 3D train models stored in this
repository. It converts the repo's proprietary XML + BMP models into glTF
(`.glb`) at build time and renders them in the browser with Three.js.

## Quick start (localhost)

Requires Node.js 18+.

```bash
npm install
npm run dev
```

Then open the printed `http://localhost:5173` URL. The dev command first runs the
model converter (`build:models`) to generate `public/models/*.glb`,
`public/thumbs/*.png` and `public/manifest.json`, then starts Vite.

## Scripts

| Command | What it does |
| --- | --- |
| `npm run build:models` | Convert `db/train/**` models → GLB + thumbnails + `manifest.json` (output in `public/`). |
| `npm run dev` | Build models, then start the Vite dev server. |
| `npm run build` | Build models, then produce a static site in `dist/`. |
| `npm run preview` | Serve the built `dist/` locally. |

## Deploying to Netlify

`netlify.toml` is included. Netlify runs `npm run build` and publishes `dist/`.
Because the models are regenerated from the committed `db/` sources during the
build, none of the generated assets need to be committed.

## How it works

The conversion pipeline is pure Node (no Python required):

- `scripts/build-models.mjs` — walks `db/train/**`, and for each model emits a
  `.glb`, a thumbnail, and a manifest entry.
- `scripts/lib/` — the converter, ported from the repo's Python tooling
  (`scripts/upload_utils.py`, `scripts/atlas.py`):
  - `xml.mjs` — Windows-1250 tolerant XML parsing.
  - `geometry.mjs` — parses the `<model>` polygon/vertex data and merges
    `<inline>` sub-models (e.g. pantographs) with their transforms.
  - `atlas.mjs` — shelf-packs textures into one atlas, preferring AI-remastered
    `*_4x.png` over the original `.bmp`, with material/solid-color fallbacks.
  - `glb.mjs` — writes a glTF 2.0 binary with the atlas embedded.
- `src/` — a React + Three.js (`@react-three/fiber` + `drei`) frontend: a
  filterable thumbnail gallery and an on-demand 3D viewer.

Generated output (`public/models/`, `public/thumbs/`, `public/manifest.json`,
`dist/`, `node_modules/`) is git-ignored.
