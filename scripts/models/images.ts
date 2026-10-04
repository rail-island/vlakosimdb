// Image helpers: decode BMP/PNG to raw RGBA, encode RGBA to PNG, and composite.
// BMP decoding uses bmp-js (the legacy 24-bit files in db/); PNG uses pngjs.
/// <reference path="./bmp-js.d.ts" />
import fs from "node:fs";
import bmp from "bmp-js";
import { PNG } from "pngjs";

export interface RgbaImage {
  width: number;
  height: number;
  data: Buffer; // length width*height*4, RGBA
}

/**
 * Load an image file as raw RGBA.
 * bmp-js decodes to ABGR with alpha left at 0, so we remap to RGBA and force
 * alpha to 255 (the legacy BMPs have no real alpha channel).
 */
export function loadImageRGBA(filePath: string): RgbaImage {
  const ext = filePath.toLowerCase().slice(filePath.lastIndexOf("."));
  const buf = fs.readFileSync(filePath);
  if (ext === ".png") {
    const png = PNG.sync.read(buf);
    return { width: png.width, height: png.height, data: png.data };
  }
  const d = bmp.decode(buf);
  const out = Buffer.allocUnsafe(d.width * d.height * 4);
  const src = d.data; // [A, B, G, R] per pixel
  for (let i = 0; i < out.length; i += 4) {
    out[i] = src[i + 3]!; // R
    out[i + 1] = src[i + 2]!; // G
    out[i + 2] = src[i + 1]!; // B
    out[i + 3] = 255; // A (opaque)
  }
  return { width: d.width, height: d.height, data: out };
}

/** Create a solid-color RGBA image. color = [r,g,b,a]. */
export function solidImage(width: number, height: number, color: number[]): RgbaImage {
  const data = Buffer.allocUnsafe(width * height * 4);
  for (let i = 0; i < data.length; i += 4) {
    data[i] = color[0]!;
    data[i + 1] = color[1]!;
    data[i + 2] = color[2]!;
    data[i + 3] = color[3] ?? 255;
  }
  return { width, height, data };
}

/** Copy a source RGBA image into dst at (dx, dy). */
export function blit(dst: RgbaImage, src: RgbaImage, dx: number, dy: number): void {
  for (let y = 0; y < src.height; y++) {
    const srcRow = y * src.width * 4;
    const dstRow = ((dy + y) * dst.width + dx) * 4;
    src.data.copy(dst.data, dstRow, srcRow, srcRow + src.width * 4);
  }
}

/** Nearest-neighbour integer upscale of an RGBA image by `factor`. */
export function upscaleNearest(src: RgbaImage, factor: number): RgbaImage {
  const width = src.width * factor;
  const height = src.height * factor;
  const data = Buffer.allocUnsafe(width * height * 4);
  for (let y = 0; y < height; y++) {
    const sy = (y / factor) | 0;
    for (let x = 0; x < width; x++) {
      const sx = (x / factor) | 0;
      const s = (sy * src.width + sx) * 4;
      const d = (y * width + x) * 4;
      data[d] = src.data[s]!;
      data[d + 1] = src.data[s + 1]!;
      data[d + 2] = src.data[s + 2]!;
      data[d + 3] = src.data[s + 3]!;
    }
  }
  return { width, height, data };
}

/** Encode a raw RGBA image to PNG bytes. */
export function encodePNG(img: RgbaImage): Buffer {
  const png = new PNG({ width: img.width, height: img.height });
  img.data.copy(png.data);
  return PNG.sync.write(png);
}

/** Decode PNG bytes to raw RGBA. */
export function decodePNG(buf: Buffer): RgbaImage {
  const png = PNG.sync.read(buf);
  return { width: png.width, height: png.height, data: png.data };
}
