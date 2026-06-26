import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import { modelOptionsPlugin } from "./scripts/vite-model-options.mjs";

// Vite serves /public as static root, so the generated manifest.json, models/
// and thumbs/ are available at the site root. Build output goes to dist/.
// modelOptionsPlugin adds a dev-only /api/model-options endpoint for live editing
// of per-model texture-flip options.json.
export default defineConfig({
  plugins: [react(), modelOptionsPlugin()],
  build: { outDir: "dist", chunkSizeWarningLimit: 1500 },
});
