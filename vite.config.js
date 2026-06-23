import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// Vite serves /public as static root, so the generated manifest.json, models/
// and thumbs/ are available at the site root. Build output goes to dist/.
export default defineConfig({
  plugins: [react()],
  build: { outDir: "dist", chunkSizeWarningLimit: 1500 },
});
