// Vite dev-server plugin: exposes /api/upscale (SSE) so the browser can trigger
// the Node remaster pipeline ON LOCALHOST ONLY. Because it is registered via
// configureServer, it exists only under `vite` dev — never in `vite build` /
// `preview` / the deployed Netlify site.
import fs from "node:fs";
import path from "node:path";
import { loadEnv, repoRoot } from "./env.mjs";
import { remasterModel, modelSlug } from "./remaster.mjs";
import { buildModelGlb } from "./model-build.mjs";
import { loadImageRGBA, encodePNG } from "./images.mjs";

const ROOT = repoRoot();
const MATERIALS = path.join(ROOT, "materials");
const PUBLIC = path.join(ROOT, "public");

function sse(res) {
  res.writeHead(200, {
    "Content-Type": "text/event-stream",
    "Cache-Control": "no-cache",
    Connection: "keep-alive",
  });
  return (event, data) => res.write(`event: ${event}\ndata: ${JSON.stringify(data)}\n\n`);
}

function readManifest() {
  return JSON.parse(fs.readFileSync(path.join(PUBLIC, "manifest.json"), "utf-8"));
}

async function handleUpscale(req, res, url) {
  const send = sse(res);
  try {
    const id = url.searchParams.get("id");
    const dryRun = url.searchParams.get("dry") === "1";
    const manifest = readManifest();
    const entry = manifest.models.find((m) => m.id === id);
    if (!entry || !entry.dir) {
      send("error", { message: `unknown model id: ${id}` });
      return res.end();
    }
    const modelDir = path.join(ROOT, entry.dir);
    send("log", { message: `remastering ${entry.dir}${dryRun ? " (dry-run)" : ""}…` });

    // 1. AI upscale -> per-texture model/*_4x.png + debug artifacts.
    const result = await remasterModel(modelDir, {
      repoRoot: ROOT,
      materialsDir: MATERIALS,
      dryRun,
      onLog: (message) => send("log", { message }),
    });

    // 2. Rebuild this model's GLB + thumbnail with the new hi-res atlas.
    send("log", { message: "rebuilding GLB with hi-res atlas…" });
    const built = buildModelGlb(modelDir, MATERIALS);
    if (built) {
      fs.writeFileSync(path.join(PUBLIC, "models", `${id}.glb`), built.glb.buffer);
      const img4x = path.join(modelDir, "image_4x.png");
      if (fs.existsSync(img4x)) {
        fs.writeFileSync(path.join(PUBLIC, "thumbs", `${id}.png`), encodePNG(loadImageRGBA(img4x)));
      }
      entry.hires = built.hires;
      fs.writeFileSync(path.join(PUBLIC, "manifest.json"), JSON.stringify(manifest));
    }

    const slug = modelSlug(modelDir, ROOT);
    send("done", {
      hires: built ? built.hires : entry.hires,
      cost: result.cost_usd,
      tiles: result.tiles.length,
      before: `/api/generated/${slug}/atlas_4x_raw.png`,
      after: `/api/generated/${slug}/atlas_4x.png`,
      thumb: `/api/generated/${slug}/image_4x.png`,
    });
  } catch (e) {
    send("error", { message: e.message || String(e) });
  }
  res.end();
}

function serveGenerated(req, res, url) {
  // /api/generated/<slug>/<file> -> generated/<slug>/<file> (preview images).
  const rel = decodeURIComponent(url.pathname.replace(/^\/api\/generated\//, ""));
  const file = path.join(ROOT, "generated", rel);
  // Contain to generated/.
  if (!file.startsWith(path.join(ROOT, "generated") + path.sep) || !fs.existsSync(file)) {
    res.statusCode = 404;
    return res.end("not found");
  }
  res.setHeader("Content-Type", "image/png");
  res.setHeader("Cache-Control", "no-store");
  fs.createReadStream(file).pipe(res);
}

export function upscalePlugin() {
  return {
    name: "vlakosimdb-upscale",
    apply: "serve", // dev only
    configureServer(server) {
      loadEnv();
      server.middlewares.use((req, res, next) => {
        if (!req.url || !req.url.startsWith("/api/")) return next();
        const url = new URL(req.url, "http://localhost");
        if (url.pathname === "/api/upscale") return void handleUpscale(req, res, url);
        if (url.pathname.startsWith("/api/generated/")) return void serveGenerated(req, res, url);
        return next();
      });
    },
  };
}
