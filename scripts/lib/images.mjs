// Image helpers: decode BMP/PNG to raw RGBA, encode RGBA to PNG, and composite.
// BMP decoding uses bmp-js (handles the legacy 24-bit files in db/); PNG uses pngjs.
import fs from "node:fs";
import bmp from "bmp-js";
import { PNG } from "pngjs";
import sharp from "sharp";

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

/** Nearest-neighbour integer upscale of an RGBA image by `factor`. */
export function upscaleNearest(src, factor) {
  const width = src.width * factor;
  const height = src.height * factor;
  const data = Buffer.allocUnsafe(width * height * 4);
  for (let y = 0; y < height; y++) {
    const sy = (y / factor) | 0;
    for (let x = 0; x < width; x++) {
      const sx = (x / factor) | 0;
      const s = (sy * src.width + sx) * 4;
      const d = (y * width + x) * 4;
      data[d] = src.data[s];
      data[d + 1] = src.data[s + 1];
      data[d + 2] = src.data[s + 2];
      data[d + 3] = src.data[s + 3];
    }
  }
  return { width, height, data };
}

/** Encode a raw RGBA image ({width,height,data}) to PNG bytes (Buffer). */
export function encodePNG(img) {
  const png = new PNG({ width: img.width, height: img.height });
  img.data.copy(png.data);
  return PNG.sync.write(png);
}

/** Decode PNG bytes (Buffer) to raw RGBA ({width,height,data}). */
export function decodePNG(buf) {
  const png = PNG.sync.read(buf);
  return { width: png.width, height: png.height, data: png.data };
}

/** Per-channel linear blend of two same-size RGBA images: a*(1-t) + b*t. */
export function blendImages(a, b, t) {
  const data = Buffer.allocUnsafe(a.data.length);
  for (let i = 0; i < data.length; i++) {
    data[i] = Math.round(a.data[i] * (1 - t) + b.data[i] * t);
  }
  return { width: a.width, height: a.height, data };
}

/**
 * High-quality resample of a raw RGBA image to (dstW, dstH) using sharp's
 * libvips Lanczos3 kernel — the faithful equivalent of PIL's LANCZOS used by the
 * Python remaster pipeline. Async because sharp is async.
 * Returns { width, height, data } with a Buffer of length dstW*dstH*4.
 */
export async function resizeLanczos(img, dstW, dstH) {
  const data = await sharp(img.data, {
    raw: { width: img.width, height: img.height, channels: 4 },
  })
    .resize(dstW, dstH, { kernel: "lanczos3", fit: "fill" })
    .raw()
    .toBuffer();
  return { width: dstW, height: dstH, data };
}

/** Crop a raw RGBA image to box [x0,y0,x1,y1] (clamped). Returns a new image. */
export function cropImage(img, box) {
  const x0 = Math.max(0, Math.min(img.width, Math.round(box[0])));
  const y0 = Math.max(0, Math.min(img.height, Math.round(box[1])));
  const x1 = Math.max(x0, Math.min(img.width, Math.round(box[2])));
  const y1 = Math.max(y0, Math.min(img.height, Math.round(box[3])));
  const w = Math.max(1, x1 - x0);
  const h = Math.max(1, y1 - y0);
  const data = Buffer.allocUnsafe(w * h * 4);
  for (let y = 0; y < h; y++) {
    const srcRow = ((y0 + y) * img.width + x0) * 4;
    img.data.copy(data, y * w * 4, srcRow, srcRow + w * 4);
  }
  return { width: w, height: h, data };
}
