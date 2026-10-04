// Golden-geometry guard for the clean TypeScript model pipeline (Part A). Builds a
// couple of real bus models from db/motocar via the full procedural path (CSG clip
// solver, rotate solids, colorize) and asserts stable vertex/face/bbox values plus
// a structurally valid GLB. The CSG/colorize port is precision-sensitive, so these
// captured numbers catch regressions that would silently corrupt geometry/UVs.
import { describe, it, expect } from "vitest";
import path from "node:path";
import fs from "node:fs";
import { fileURLToPath } from "node:url";
import { buildModelGlb } from "../scripts/models/buildModelGlb.ts";

const HERE = path.dirname(fileURLToPath(import.meta.url));
// test -> repo root (db/ lives at the repo root).
const REPO_ROOT = path.resolve(HERE, "..");
const DB_MOTOCAR = path.join(REPO_ROOT, "db", "motocar");
const DB_TRAM = path.join(REPO_ROOT, "db", "tram");
const MATERIALS = path.join(REPO_ROOT, "materials");

const hasDb = fs.existsSync(DB_MOTOCAR);
const describeDb = hasDb ? describe : describe.skip;
const describeTram = fs.existsSync(DB_TRAM) ? describe : describe.skip;

describeDb("model pipeline (db/motocar)", () => {
  it("builds Karosa B731 with stable geometry", () => {
    const built = buildModelGlb(path.join(DB_MOTOCAR, "bus_kar_b731"), MATERIALS);
    expect(built).not.toBeNull();
    const { stats } = built!.glb;
    // Captured from the clean TS port; B731's catalogued length is 11.4 (moto_data).
    expect(stats.vertices).toBe(594);
    expect(stats.faces).toBe(326);
    expect(stats.length).toBeCloseTo(11.4, 1);
    // 3 verts per polygon-vertex, 3 indices per triangle.
    expect(built!.atlasMap.size).toBeGreaterThan(0);
  });

  it("emits a structurally valid GLB (header + JSON chunk + accessors)", () => {
    const built = buildModelGlb(path.join(DB_MOTOCAR, "bus_kar_b731"), MATERIALS);
    const buf = built!.glb.buffer;
    expect(buf.toString("ascii", 0, 4)).toBe("glTF");
    expect(buf.readUInt32LE(4)).toBe(2); // version
    expect(buf.readUInt32LE(8)).toBe(buf.length); // total length

    const jsonLen = buf.readUInt32LE(12);
    expect(buf.readUInt32LE(16)).toBe(0x4e4f534a); // "JSON"
    const json = JSON.parse(buf.toString("utf8", 20, 20 + jsonLen));
    expect(json.asset.version).toBe("2.0");
    expect(json.accessors).toHaveLength(4); // POSITION, NORMAL, TEXCOORD_0, indices
    expect(json.accessors[0].count).toBe(built!.glb.stats.vertices);
    expect(json.meshes[0].primitives[0].attributes.POSITION).toBe(0);
  });

  it("builds the minimal bus_1 model", () => {
    const built = buildModelGlb(path.join(DB_MOTOCAR, "bus_1"), MATERIALS);
    expect(built).not.toBeNull();
    expect(built!.glb.stats.vertices).toBe(54);
    expect(built!.glb.stats.faces).toBe(28);
  });
});

// The trams exercise the same CSG/colorize path on much heavier geometry, and
// two body styles the buses never had (tram3c, tram2v). Captured from the clean
// TS port; catalogued lengths are 15.7 (T3) and 28.2 (KT8) model units.
describeTram("model pipeline (db/tram)", () => {
  it("builds the rigid CKD T3 with stable geometry", () => {
    const built = buildModelGlb(path.join(DB_TRAM, "tram_ckd_t3"), MATERIALS);
    expect(built).not.toBeNull();
    expect(built!.articulation).toBeNull();
    const { stats } = built!.glb;
    expect(stats.vertices).toBe(730);
    expect(stats.faces).toBe(406);
    expect(stats.length).toBeCloseTo(15.7, 3);
    expect(stats.bbox.max[1]).toBeCloseTo(3.8, 3);
  });

  it("builds the three-section CKD KT8 with stable geometry", () => {
    const built = buildModelGlb(path.join(DB_TRAM, "tram_ckd_kt8"), MATERIALS);
    expect(built).not.toBeNull();
    expect(built!.articulation!.style).toBe("tram3c");
    const { stats } = built!.glb;
    // 1116 body verts + 2 bellows x 3 quads x 4 verts; 594 body faces + 12.
    expect(stats.vertices).toBe(1140);
    expect(stats.faces).toBe(606);
    expect(stats.length).toBeCloseTo(28.2, 3);
  });
});
