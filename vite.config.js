import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import { upscalePlugin } from "./scripts/lib/vite-upscale-plugin.mjs";

// Vite serves /public as static root, so the generated manifest.json, models/
// and thumbs/ are available at the site root. Build output goes to dist/.
// upscalePlugin adds a localhost-only /api/upscale endpoint (dev server only).
export default defineConfig({
  plugins: [react(), upscalePlugin()],
  build: { outDir: "dist", chunkSizeWarningLimit: 1500 },
});
