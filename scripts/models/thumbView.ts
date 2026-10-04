// How a vehicle thumbnail is photographed -- the numbers, and nothing else.
//
// A COPY of Rail Island's src/models/thumbView.ts. Two renderers read that table:
//
//   - Rail Island's src/creator/glbThumb.ts renders an UPLOAD in the uploader's
//     browser, with three.js and WebGL;
//   - scripts/models/renderSideThumb.ts (here) renders every vehicle of this
//     database at `npm run export`, with a software rasterizer that reproduces
//     three's MeshStandardMaterial lighting.
//
// So a shipped train and an uploaded one sit side by side in a picker framed, lit
// and backed identically -- as long as the two copies agree. Change one, change
// the other, then re-export and re-import.

/** The quarter view's frame (buildings and the free-floating fleets). */
export const QUARTER_THUMB = { width: 256, height: 160 } as const;

/**
 * The side elevation's frame: what a vehicle on rails or a road is photographed
 * in, square on to its flank, wide and short.
 */
export const SIDE_THUMB = { width: 320, height: 120 } as const;

/** Vertical field of view, degrees: the quarter view. */
export const THUMB_FOV_DEG = 35;

/**
 * The side view's field of view. Narrow on purpose: from far away the elevation is
 * nearly flat, as the hand-drawn side views the XML packs shipped with were, so a
 * long coach does not show its end walls at both edges of the frame.
 */
export const SIDE_FOV_DEG = 10;

/** Camera distance, as a multiple of the distance that just fits the extent. */
export const SIDE_DIST_FACTOR = 1.04;
export const QUARTER_DIST_FACTOR = 1.6;

/** How far above the centre line the side camera sits, as a fraction of its distance. */
export const SIDE_ELEVATION = 0.08;

/** The frame's opaque backdrop, sRGB. */
export const THUMB_BACKGROUND = "#232d3d";

/** three.js light intensities (physically based units, three >= r155): the quarter view. */
export const THUMB_AMBIENT = 1.6;
export const THUMB_SUN = 1.9;
/** Where the sun sits; it shines toward the origin. */
export const THUMB_SUN_POSITION: readonly [number, number, number] = [3, 5, 4];

/**
 * The side view's lights. Bright and from the FRONT, a little above: the flank the
 * camera sees takes the paint at about its own colour, as the hand-drawn side views
 * did, while the roof and the ends fall off just enough to keep the shape.
 */
export const SIDE_AMBIENT = 2.4;
export const SIDE_SUN = 1.0;
export const SIDE_SUN_POSITION: readonly [number, number, number] = [0.5, 2, 6];

/**
 * The side view draws every material as paint (metalness 0). A metal with no
 * environment map to reflect renders nearly black in its diffuse, which on a
 * thumbnail reads as a dirty vehicle rather than a shiny one.
 */
export const SIDE_FLAT_METAL = true;

/** Strength of the side view's final sharpen (see sharpenRgba); 0 turns it off. */
export const SIDE_SHARPEN = 0.25;

/**
 * A 3x3 unsharp mask over an RGBA buffer, in place: each colour channel becomes
 * (1 + 4a) x itself - a x its four neighbours. Alpha is left alone and edges are
 * clamped. The same function runs in the browser (the upload) and at build time,
 * so both sides finish a side view identically.
 */
export function sharpenRgba(data: Uint8ClampedArray | Uint8Array, width: number, height: number, amount: number): void {
  if (amount <= 0) return;
  const src = data.slice();
  const at = (x: number, y: number, c: number) =>
    src[(Math.min(height - 1, Math.max(0, y)) * width + Math.min(width - 1, Math.max(0, x))) * 4 + c]!;
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      for (let c = 0; c < 3; c++) {
        const v = (1 + 4 * amount) * at(x, y, c) - amount * (at(x - 1, y, c) + at(x + 1, y, c) + at(x, y - 1, c) + at(x, y + 1, c));
        data[(y * width + x) * 4 + c] = Math.max(0, Math.min(255, Math.round(v)));
      }
    }
  }
}

/** WebP quality, 0..1 as canvas.toBlob takes it. */
export const THUMB_WEBP_QUALITY = 0.82;
