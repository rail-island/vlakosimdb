// Dev-only Vite plugin: POST /api/model-options { id, flipS } writes a model's
// committed options.json (per-side texture flips), rebuilds just that model's GLB,
// and updates public/manifest.json. Only registered for `vite` (serve), so it is
// reachable only on the local dev server.
import fs from "node:fs";
import path from "node:path";
import { ROOT, OUT, buildOneModel } from "./build-models.mjs";

function readBody(req) {
  return new Promise((resolve, reject) => {
    let body = "";
    req.on("data", (c) => {
      body += c;
      if (body.length > 1e6) req.destroy();
    });
    req.on("end", () => resolve(body));
    req.on("error", reject);
  });
}

export function modelOptionsPlugin() {
  return {
    name: "model-options",
    apply: "serve",
    configureServer(server) {
      server.middlewares.use("/api/model-options", async (req, res) => {
        const json = (code, obj) => {
          res.statusCode = code;
          res.setHeader("Content-Type", "application/json");
          res.end(JSON.stringify(obj));
        };
        if (req.method !== "POST") return json(405, { error: "POST only" });
        try {
          const { id, flipS } = JSON.parse((await readBody(req)) || "{}");
          const manifestPath = path.join(OUT, "manifest.json");
          const manifest = JSON.parse(fs.readFileSync(manifestPath, "utf8"));
          const idx = manifest.models.findIndex((m) => m.id === id);
          if (idx < 0) return json(404, { error: "unknown model id" });

          const dir = path.resolve(ROOT, manifest.models[idx].dir);
          const dbRoot = path.join(ROOT, "db");
          if (!dir.startsWith(dbRoot + path.sep) || !fs.existsSync(path.join(dir, "model", "index.xml"))) {
            return json(400, { error: "invalid model dir" });
          }

          // Persist sparse flipS (only true keys) to the committed options.json.
          const clean = {};
          for (const [k, v] of Object.entries(flipS || {})) if (v) clean[k] = true;
          fs.writeFileSync(path.join(dir, "model", "options.json"), JSON.stringify({ flipS: clean }, null, 2) + "\n");

          const rebuilt = buildOneModel(dir, { id });
          if (!rebuilt) return json(400, { error: "model produced no geometry" });
          manifest.models[idx] = rebuilt.entry;
          fs.writeFileSync(manifestPath, JSON.stringify(manifest));

          return json(200, { ok: true, glb: rebuilt.entry.glb, flip: rebuilt.entry.flip });
        } catch (e) {
          return json(500, { error: String((e && e.message) || e) });
        }
      });
    },
  };
}
