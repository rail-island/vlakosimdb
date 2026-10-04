// Steam locomotive chimney smoke: the anchor index.xml declares, read into model
// units by parseSmokeAnchor and exported as `smoke` in train.meta.json (Rail
// Island's smoke effect is gated on its presence).

import { describe, it, expect } from "vitest";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { parseMetadata, parseSmokeAnchor } from "../scripts/models/metadata.ts";

const REPO = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");

describe("smoke anchor parsing", () => {
  it("reads root smoke_x/smoke_y in model units (attr * 12.5)", () => {
    expect(parseSmokeAnchor({ "@_smoke_x": "0.12", "@_smoke_y": "0.35" })).toEqual({
      x: 1.5,
      y: 4.375,
    });
  });

  it("treats the empty-string placeholders as no smoke", () => {
    // 265 of the 276 models that declare the attributes leave them empty.
    expect(parseSmokeAnchor({ "@_smoke_x": "", "@_smoke_y": "", "@_smoke": "" })).toBeNull();
    expect(parseSmokeAnchor({ "@_name": "CD_B" })).toBeNull();
    expect(parseSmokeAnchor(undefined)).toBeNull();
  });

  it("prefers the <smoke> child's retuned x0/y0 over stale root attributes", () => {
    // csd_498_1: the root attrs are copy-pasted from the 433 template, the child is
    // the value that actually matches its funnel.
    const el = {
      "@_smoke_x": "0.12",
      "@_smoke_y": "0.35",
      smoke: { "@_x0": "0.2", "@_y0": "0.4" },
    };
    expect(parseSmokeAnchor(el)).toEqual({ x: 2.5, y: 5 });
  });

  it("falls back to the root when a <smoke> child omits x0/y0", () => {
    const el = { "@_smoke_x": "0.12", "@_smoke_y": "0.35", smoke: { "@_count": "500" } };
    expect(parseSmokeAnchor(el)).toEqual({ x: 1.5, y: 4.375 });
  });

  it("does not confuse the root `smoke` attribute with the `smoke` child element", () => {
    // `smoke="../smoke1"` is a dangling path to sprite assets that were never
    // ported; it must not be mistaken for a particle spec carrying x0/y0.
    const el = { "@_smoke": "../smoke1", "@_smoke_x": "0.2", "@_smoke_y": "0.4" };
    expect(parseSmokeAnchor(el)).toEqual({ x: 2.5, y: 5 });
  });

  it("resolves the real db/ locomotives, and nothing else", () => {
    const at = (rel: string) => parseMetadata(path.join(REPO, rel, "index.xml")).smoke;
    // anchors that came tuned in the source data
    expect(at("db/train/loco/steam/csd_433_0")).toEqual({ x: 1.5, y: 4.375 });
    expect(at("db/train/loco/steam/csd_386")).toEqual({ x: 3.125, y: 5 });
    expect(at("db/train/760_mm/csd_u46")).toEqual({ x: 1.25, y: 3.125 });
    // root and <smoke> child now agree here (the root used to be stale)
    expect(at("db/train/loco/steam/csd_498_1")).toEqual({ x: 2.5, y: 5 });
    // anchors corrected from the copy-pasted 433 template onto each loco's own
    // chimney (the `k1` shape in its <alongz> side profile)
    expect(at("db/train/loco/steam/csd_456_0")).toEqual({ x: 2.4, y: 4 });
    expect(at("db/train/loco/steam/csd_475_179#l")).toEqual({ x: 2.9, y: 4.1 });
    // ordinary stock does not smoke
    expect(at("db/train/coach/cd_b")).toBeNull();
    expect(at("db/train/default_loco")).toBeNull();
  });

  it("puts every steam loco's plume at its own chimney, not a shared template", () => {
    // The 433 template's 0.12/0.35 was copy-pasted across locos of very different
    // lengths; each should now carry its own measured anchor.
    const at = (rel: string) => parseMetadata(path.join(REPO, rel, "index.xml")).smoke!;
    const retuned = [
      "db/train/loco/steam/csd_456_0",
      "db/train/loco/steam/csd_475_179#l",
      "db/train/loco/steam/csd_498_1",
    ].map(at);
    for (const a of retuned) expect(a).not.toEqual({ x: 1.5, y: 4.375 });
    // and they are distinct from one another, not one new shared value
    expect(new Set(retuned.map((a) => `${a.x},${a.y}`)).size).toBe(retuned.length);
  });
});
