// Image helpers: decode BMP/PNG to raw RGBA, encode RGBA to PNG, and composite.
// BMP decoding uses bmp-js (handles the legacy 24-bit files in db/); PNG uses pngjs.
import fs from "node:fs";
import bmp from "bmp-js";
import { PNG } from "pngjs";

/**
 * Load an image file as raw RGBA.
 * Returns { width, height, data } where data is a Buffer of length w*h*4.
 * bmp-js decodes to ABGR with the alpha byte left at 0, so we remap to RGBA
 * and force alpha to 255 (BMPs have no real alpha channel).
 */
export function loadImageRGBA(path) {
  const ext = path.toLowerCase().slice(path.lastIndexOf("."));
  const buf = fs.readFileSync(path);
  if (ext === ".png") {
    const png = PNG.sync.read(buf);
    return { width: png.width, height: png.height, data: png.data };
  }
  // .bmp (and anything else we try via bmp-js)
  const d = bmp.decode(buf);
  const out = Buffer.allocUnsafe(d.width * d.height * 4);
  const src = d.data; // [A, B, G, R] per pixel
  for (let i = 0; i < out.length; i += 4) {
    out[i] = src[i + 3]; // R
    out[i + 1] = src[i + 2]; // G
    out[i + 2] = src[i + 1]; // B
    out[i + 3] = 255; // A (opaque)
  }
  return { width: d.width, height: d.height, data: out };
}

/** Create a solid-color RGBA image. color = [r,g,b,a]. */
export function solidImage(width, height, color) {
  const data = Buffer.allocUnsafe(width * height * 4);
  for (let i = 0; i < data.length; i += 4) {
    data[i] = color[0];
    data[i + 1] = color[1];
    data[i + 2] = color[2];
    data[i + 3] = color[3] ?? 255;
  }
  return { width, height, data };
}

/** Copy a source RGBA image into dst at (dx, dy). */
export function blit(dst, src, dx, dy) {
  for (let y = 0; y < src.height; y++) {
    const srcRow = y * src.width * 4;
    const dstRow = ((dy + y) * dst.width + dx) * 4;
    src.data.copy(dst.data, dstRow, srcRow, srcRow + src.width * 4);
  }
}

/** Encode a raw RGBA image ({width,height,data}) to PNG bytes (Buffer). */
export function encodePNG(img) {
  const png = new PNG({ width: img.width, height: img.height });
  img.data.copy(png.data);
  return PNG.sync.write(png);
}
