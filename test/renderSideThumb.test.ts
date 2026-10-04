// The export-time side thumbnail of an XML-built vehicle (scripts/models/
// renderSideThumb.ts), driven with a synthetic GLB: one textured quad.
//
// What it must share with Rail Island's upload thumbnail (its src/creator/
// glbThumb.ts) is pinned through scripts/models/thumbView.ts, a copy of the table
// Rail Island's renderer reads: the frame, the backdrop, the lights, the final
// sharpen. What only a software renderer can
// get wrong is pinned here: the texture is sampled per pixel, a back face is
// culled as three's FrontSide culls it, and the lighting lands where three's
// MeshStandardMaterial puts it.
//
// The look itself is tuned to the hand-drawn side views the XML packs shipped
// with (reported: the first renders were dull next to them): a flank facing the
// camera shows its paint at full colour, and metal is drawn as paint.

import { describe, expect, it } from "vitest";
import { renderSideThumb } from "../scripts/models/renderSideThumb.ts";
import { SIDE_THUMB, THUMB_BACKGROUND, sharpenRgba } from "../scripts/models/thumbView.ts";
import { PNG } from "pngjs";

/** A 2x1 texture: left half red, right half blue. */
function texture(): Buffer {
  const png = new PNG({ width: 2, height: 1 });
  png.data.set([255, 0, 0, 255, 0, 0, 255, 255]);
  return PNG.sync.write(png);
}

/**
 * A 4 x 1 quad in the XY plane at z = 0, facing +Z (where the side camera sits),
 * or facing away when `back`. Optional texture across it, left to right.
 */
function quad(opts: { back?: boolean; textured?: boolean; metallic?: number; depth?: number } = {}): Uint8Array {
  // `depth` makes a body: the front face at z = +depth/2 and a far wall (facing
  // away, so culled) at z = -depth/2, which puts the depth into the bounding box.
  const d = (opts.depth ?? 0) / 2;
  const face = [0, 0, d, 4, 0, d, 4, 1, d, 0, 1, d];
  const far = [0, 0, -d, 4, 0, -d, 4, 1, -d, 0, 1, -d];
  const pos = new Float32Array(d ? [...face, ...far] : face);
  const nz = opts.back ? -1 : 1;
  const n4 = (z: number) => [0, 0, z, 0, 0, z, 0, 0, z, 0, 0, z];
  const nrm = new Float32Array(d ? [...n4(nz), ...n4(-1)] : n4(nz));
  const uv = new Float32Array(d ? [0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0] : [0, 1, 1, 1, 1, 0, 0, 0]);
  // Counter-clockwise seen from +Z is the front; reversed, the quad faces -Z.
  const front = opts.back ? [0, 2, 1, 0, 3, 2] : [0, 1, 2, 0, 2, 3];
  const idx = new Uint16Array(d ? [...front, 4, 6, 5, 4, 7, 6] : front);
  const img = opts.textured ? texture() : Buffer.alloc(0);
  const parts = [Buffer.from(pos.buffer), Buffer.from(nrm.buffer), Buffer.from(uv.buffer), Buffer.from(idx.buffer), img];
  const views: { buffer: number; byteOffset: number; byteLength: number }[] = [];
  let offset = 0;
  const padded = parts.map((p) => {
    views.push({ buffer: 0, byteOffset: offset, byteLength: p.length });
    const pad = (4 - (p.length % 4)) % 4;
    offset += p.length + pad;
    return Buffer.concat([p, Buffer.alloc(pad)]);
  });
  const bin = Buffer.concat(padded);
  const pbr: Record<string, unknown> = { metallicFactor: opts.metallic ?? 0, roughnessFactor: 1 };
  if (opts.textured) pbr.baseColorTexture = { index: 0 };
  else pbr.baseColorFactor = [1, 1, 1, 1];
  const json = {
    asset: { version: "2.0" },
    scene: 0,
    scenes: [{ nodes: [0] }],
    nodes: [{ mesh: 0 }],
    meshes: [{ primitives: [{ attributes: { POSITION: 0, NORMAL: 1, TEXCOORD_0: 2 }, indices: 3, material: 0 }] }],
    materials: [{ pbrMetallicRoughness: pbr }],
    ...(opts.textured
      ? { textures: [{ source: 0 }], images: [{ bufferView: 4, mimeType: "image/png" }] }
      : {}),
    accessors: [
      { bufferView: 0, componentType: 5126, count: pos.length / 3, type: "VEC3", min: [0, 0, -d], max: [4, 1, d] },
      { bufferView: 1, componentType: 5126, count: pos.length / 3, type: "VEC3" },
      { bufferView: 2, componentType: 5126, count: pos.length / 3, type: "VEC2" },
      { bufferView: 3, componentType: 5123, count: idx.length, type: "SCALAR" },
    ],
    bufferViews: views,
    buffers: [{ byteLength: bin.length }],
  };
  const jsonBytes = Buffer.from(JSON.stringify(json));
  const jsonChunk = Buffer.concat([jsonBytes, Buffer.alloc((4 - (jsonBytes.length % 4)) % 4, 0x20)]);
  const header = Buffer.alloc(12);
  header.writeUInt32LE(0x46546c67, 0);
  header.writeUInt32LE(2, 4);
  header.writeUInt32LE(12 + 8 + jsonChunk.length + 8 + bin.length, 8);
  const chunk = (data: Buffer, type: number) => {
    const h = Buffer.alloc(8);
    h.writeUInt32LE(data.length, 0);
    h.writeUInt32LE(type, 4);
    return Buffer.concat([h, data]);
  };
  return new Uint8Array(Buffer.concat([header, chunk(jsonChunk, 0x4e4f534a), chunk(bin, 0x004e4942)]));
}

const pixel = (t: ReturnType<typeof renderSideThumb>, x: number, y: number) => {
  const o = (Math.round(y) * t.width + Math.round(x)) * 4;
  return [t.data[o]!, t.data[o + 1]!, t.data[o + 2]!];
};

const BACKGROUND = [0x23, 0x2d, 0x3d];

describe("renderSideThumb", () => {
  it("frames the upload's side view on the upload's backdrop", () => {
    const t = renderSideThumb(quad());
    expect([t.width, t.height]).toEqual([SIDE_THUMB.width, SIDE_THUMB.height]);
    expect(THUMB_BACKGROUND).toBe("#232d3d");
    expect(pixel(t, 0, 0)).toEqual(BACKGROUND);
    expect(pixel(t, t.width - 1, t.height - 1)).toEqual(BACKGROUND);
  });

  it("fits the silhouette: a 4:1 body fills nearly the whole width, centred", () => {
    const t = renderSideThumb(quad());
    const row = Math.round(t.height / 2);
    const covered = Array.from({ length: t.width }, (_, x) => pixel(t, x, row)).filter(
      (p) => p.join() !== BACKGROUND.join(),
    ).length;
    expect(covered / t.width).toBeGreaterThan(0.9);
    expect(covered / t.width).toBeLessThan(1);
    expect(pixel(t, t.width / 2, row)).not.toEqual(BACKGROUND);
  });

  it("keeps a wide body's ends inside the frame", () => {
    // Reported: with the tight frame a wide vehicle's ends were cut off -- the flank
    // facing the camera is half the body's width closer than the centre it was
    // fitted at, so it drew larger than the frame.
    const t = renderSideThumb(quad({ depth: 2 }));
    for (let y = 0; y < t.height; y++) {
      expect(pixel(t, 0, y)).toEqual(BACKGROUND);
      expect(pixel(t, t.width - 1, y)).toEqual(BACKGROUND);
    }
  });

  it("samples the texture per pixel, not once per triangle", () => {
    // Both triangles of the quad span both halves of the texture, so a per-triangle
    // sample would paint one flat purple-ish colour across the whole body.
    const t = renderSideThumb(quad({ textured: true }));
    const row = Math.round(t.height / 2);
    const [lr, , lb] = pixel(t, t.width * 0.3, row);
    const [rr, , rb] = pixel(t, t.width * 0.7, row);
    expect(lr - lb).toBeGreaterThan(100);
    expect(rb - rr).toBeGreaterThan(100);
  });

  it("culls a back face, as the game's FrontSide material does", () => {
    const t = renderSideThumb(quad({ back: true }));
    for (let x = 0; x < t.width; x += 8) expect(pixel(t, x, t.height / 2)).toEqual(BACKGROUND);
  });

  it("shows a flank facing the camera at its full paint colour, as the drawings did", () => {
    // Front light at (0.5,2,6): N.L = 6/sqrt(40.25), and (2.4 + 1.0 * N.L) / pi is
    // just over 1 in linear light -- white paint reads white, not grey.
    const nl = 6 / Math.sqrt(40.25);
    expect((2.4 + nl) / Math.PI).toBeGreaterThan(1);
    const [r, g, b] = pixel(renderSideThumb(quad()), 160, 60);
    expect(Math.min(r, g, b)).toBeGreaterThanOrEqual(250);
  });

  it("draws metal as paint, not as a dark mirror with nothing to reflect", () => {
    const dielectric = pixel(renderSideThumb(quad({ textured: true })), 100, 60);
    const metal = pixel(renderSideThumb(quad({ textured: true, metallic: 1 })), 100, 60);
    expect(metal).toEqual(dielectric);
  });

  it("sharpens: an edge gains contrast on both sides, a flat area is untouched", () => {
    // One row: 4 dark pixels then 4 light ones.
    const row = (v: number[]) => new Uint8Array(v.flatMap((x) => [x, x, x, 255]));
    const data = row([60, 60, 60, 60, 180, 180, 180, 180]);
    sharpenRgba(data, 8, 1, 0.25);
    const at = (i: number) => data[i * 4]!;
    expect(at(0)).toBe(60);
    expect(at(7)).toBe(180);
    expect(at(3)).toBeLessThan(60);
    expect(at(4)).toBeGreaterThan(180);
    expect(data[3]).toBe(255);
  });

  it("refuses a model with no geometry rather than drawing an empty frame", () => {
    const empty = quad();
    // Same file, but the scene names no node.
    const text = new TextDecoder().decode(empty.subarray(20, 20 + new DataView(empty.buffer).getUint32(12, true)));
    expect(() => renderSideThumb(rewriteJson(empty, text.replace('"nodes":[0]', '"nodes":[]')))).toThrow();
  });
});

function rewriteJson(glbBytes: Uint8Array, json: string): Uint8Array {
  const view = new DataView(glbBytes.buffer, glbBytes.byteOffset);
  const oldLen = view.getUint32(12, true);
  const rest = Buffer.from(glbBytes.subarray(20 + oldLen));
  const bytes = Buffer.from(json.trimEnd());
  const padded = Buffer.concat([bytes, Buffer.alloc((4 - (bytes.length % 4)) % 4, 0x20)]);
  const head = Buffer.alloc(20);
  head.writeUInt32LE(0x46546c67, 0);
  head.writeUInt32LE(2, 4);
  head.writeUInt32LE(20 + padded.length + rest.length, 8);
  head.writeUInt32LE(padded.length, 12);
  head.writeUInt32LE(0x4e4f534a, 16);
  return new Uint8Array(Buffer.concat([head, padded, rest]));
}
