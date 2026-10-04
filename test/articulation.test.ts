// Articulated vehicles (style="bus2c") must build as jointed sections plus a
// bellows, not as one rigid body: before the port, the six articulated buses in
// db/motocar were fused into a single mesh and drove around with the joint gap as
// a visible hole. See scripts/models/articulate.ts.
import { describe, it, expect } from "vitest";
import path from "node:path";
import fs from "node:fs";
import { fileURLToPath } from "node:url";
import { parseModel } from "../scripts/models/geometry.ts";
import { parseMetadata } from "../scripts/models/metadata.ts";
import { planArticulation, sectionRanges } from "../scripts/models/articulate.ts";
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

function planIn(base: string, model: string) {
  const dir = path.join(base, model);
  const { polygons } = parseModel(path.join(dir, "model"), { includeInlines: true });
  const meta = parseMetadata(path.join(dir, "index.xml"));
  return { polygons, meta, plan: planArticulation(polygons, meta) };
}

function plan(model: string) {
  return planIn(DB_MOTOCAR, model);
}

function glbJson(buffer: Buffer): Record<string, any> {
  const jsonLen = buffer.readUInt32LE(12);
  return JSON.parse(buffer.toString("utf8", 20, 20 + jsonLen));
}

describeDb("articulated vehicles (db/motocar)", () => {
  it("splits every bus2c model into exactly two sections, and no other model", () => {
    const dirs = fs.readdirSync(DB_MOTOCAR).sort()
      .filter((d) => fs.existsSync(path.join(DB_MOTOCAR, d, "model", "index.xml")));
    expect(dirs.length).toBeGreaterThan(10);
    let articulated = 0;
    for (const d of dirs) {
      const { meta, polygons, plan: p } = plan(d);
      const ranges = sectionRanges(polygons);
      if (meta.style === "bus2c") {
        articulated++;
        expect(ranges, `${d} geometry`).toHaveLength(2);
        expect(p, `${d} plan`).not.toBeNull();
        expect(p!.parts.map((part) => part.name)).toEqual(["section0", "section1", "bellows0"]);
      } else {
        expect(ranges, `${d} geometry`).toHaveLength(1);
        expect(p, `${d} plan`).toBeNull();
      }
    }
    // b741, b941, b941e, b961, karrencity18m, mb_o345
    expect(articulated).toBe(6);
  });

  it("puts the Karosa B741's sections either side of the joint gap", () => {
    const { plan: p } = plan("bus_kar_b741");
    expect(p).not.toBeNull();
    const [front, rear] = p!.articulation.sections;
    expect(front!.min[0]).toBeCloseTo(0.2, 3);
    expect(front!.max[0]).toBeCloseTo(9.7, 3);
    expect(rear!.min[0]).toBeCloseTo(11.6, 3);
    expect(rear!.max[0]).toBeCloseTo(17.9, 3);
    // Every polygon lands in exactly one section; none straddles the gap.
    const [s0, s1] = p!.parts;
    for (const poly of s0!.polygons) for (const v of poly.verts) expect(v.x).toBeLessThanOrEqual(9.7 + 1e-6);
    for (const poly of s1!.polygons) for (const v of poly.verts) expect(v.x).toBeGreaterThanOrEqual(11.6 - 1e-6);
    expect(p!.articulation.bellows[0]!.splitX).toBeCloseTo(10.65, 3);
  });

  it("bridges the gap with three textured quads that continue the body's UVs", () => {
    const { plan: p } = plan("bus_kar_b741");
    const bellows = p!.parts[2]!.polygons;
    expect(bellows).toHaveLength(3);
    expect(bellows.map((q) => q.textureKey).sort())
      .toEqual(["pack:y_dec.bmp", "pack:z_dec.bmp", "pack:z_inc.bmp"]);
    for (const quad of bellows) {
      expect(quad.verts).toHaveLength(4);
      // spans exactly the gap, nothing more
      expect(Math.min(...quad.verts.map((v) => v.x))).toBeCloseTo(9.7, 3);
      expect(Math.max(...quad.verts.map((v) => v.x))).toBeCloseTo(11.6, 3);
    }
    // The side textures map s = x / 18 and t = y / 4 on the body (colorize
    // texx 0..18, texy 0..4); the bellows has to use the same mapping or the
    // accordion painted into the texture would not line up with the body.
    const side = bellows.find((q) => q.textureKey === "pack:z_dec.bmp")!;
    for (const v of side.verts) {
      expect(v.s).toBeCloseTo(v.x / 18, 6);
      expect(v.t).toBeCloseTo(v.y / 4, 6);
    }
    // Roof: s = x / 18, t = (z + 1.5) / 3 (colorize texy -1.5..1.5).
    const roof = bellows.find((q) => q.textureKey === "pack:y_dec.bmp")!;
    for (const v of roof.verts) {
      expect(v.s).toBeCloseTo(v.x / 18, 6);
      expect(v.t).toBeCloseTo((v.z + 1.5) / 3, 6);
    }
    // The bellows box is inset within the body, per pas_wi / pas_y1 / pas_y2.
    expect(Math.max(...bellows.flatMap((q) => q.verts.map((v) => v.z)))).toBeCloseTo(1.2, 6);
    expect(Math.max(...bellows.flatMap((q) => q.verts.map((v) => v.y)))).toBeCloseTo(2.9, 6);
    expect(Math.min(...bellows.flatMap((q) => q.verts.map((v) => v.y)))).toBeCloseTo(0.3, 6);
  });

  it("builds the B741 GLB as named nodes without moving the model", () => {
    const built = buildModelGlb(path.join(DB_MOTOCAR, "bus_kar_b741"), MATERIALS)!;
    expect(built.articulation).not.toBeNull();
    const json = glbJson(built.glb.buffer);
    expect(json.nodes.map((n: { name?: string }) => n.name))
      .toEqual(["root", "section0", "section1", "bellows0"]);
    expect(json.nodes[0].children).toEqual([1, 2, 3]);
    expect(json.meshes).toHaveLength(3);
    // Every mesh keeps its own POSITION/NORMAL/TEXCOORD_0/indices accessor set.
    expect(json.accessors).toHaveLength(12);
    for (const mesh of json.meshes) {
      expect(json.accessors[mesh.primitives[0].attributes.POSITION].min).toHaveLength(3);
    }
    // Splitting must not resize the vehicle: the sim reads `length` from here.
    expect(built.glb.stats.length).toBeCloseTo(17.7, 3);
    expect(built.glb.stats.bbox.min[0]).toBeCloseTo(0.2, 3);
    expect(built.glb.stats.bbox.max[0]).toBeCloseTo(17.9, 3);
    // Only the 3 bellows quads (4 verts, 2 triangles each) are new geometry.
    expect(built.glb.stats.vertices).toBe(856 + 12);
    expect(built.glb.stats.faces).toBe(466 + 6);
  });

  it("leaves a rigid model as a single unnamed node", () => {
    const built = buildModelGlb(path.join(DB_MOTOCAR, "bus_kar_b731"), MATERIALS)!;
    expect(built.articulation).toBeNull();
    const json = glbJson(built.glb.buffer);
    expect(json.nodes).toEqual([{ mesh: 0 }]);
    expect(json.meshes).toHaveLength(1);
    expect(json.accessors).toHaveLength(4);
  });

  it("falls back to one solid when the geometry does not match the style", () => {
    const { polygons } = plan("bus_kar_b731"); // one continuous body
    const warnings: string[] = [];
    const p = planArticulation(polygons, { style: "bus2c", bellows: null }, (m) => warnings.push(m));
    expect(p).toBeNull();
    expect(warnings).toHaveLength(1);
  });
});

// Trams add three jt7 styles the bus port never needed: a second two-section
// style (tram2c), a THREE-section one with two bellows (tram3c), and a coupled
// pair that is not jointed at all (tram2v). See scripts/models/articulate.ts.
// The T3/T3M/T6 "Double" pairs — the same solo car baked twice — are gone from
// db/tram: a tram is a train-style CONSIST now, so a player couples two solos
// in the board instead. The GT4 stays tram2v (it has no solo variant).
describeTram("tram body styles (db/tram)", () => {
  it("gives every tram the section/bellows layout its style promises", () => {
    const dirs = fs.readdirSync(DB_TRAM).sort()
      .filter((d) => fs.existsSync(path.join(DB_TRAM, d, "model", "index.xml")));
    expect(dirs).toHaveLength(9);
    const byStyle: Record<string, number> = {};
    for (const d of dirs) {
      const { meta, plan: p } = planIn(DB_TRAM, d);
      byStyle[meta.style] = (byStyle[meta.style] ?? 0) + 1;
      if (meta.style === "solid" || meta.style === "") {
        expect(p, `${d} plan`).toBeNull();
        continue;
      }
      expect(p, `${d} plan`).not.toBeNull();
      const { sections, bellows } = p!.articulation;
      const wanted = meta.style === "tram3c" ? 3 : 2;
      expect(sections, `${d} sections`).toHaveLength(wanted);
      // A tram2v is two whole cars hitched together, so nothing bridges them.
      expect(bellows, `${d} bellows`).toHaveLength(meta.style === "tram2v" ? 0 : wanted - 1);
      expect(p!.parts.map((part) => part.name)).toEqual([
        ...sections.map((s) => s.node), ...bellows.map((b) => b.node),
      ]);
    }
    // 4 solo T3/T3M/T3X/T6, the coupled GT4, the KT4, 3 KT8s.
    expect(byStyle).toEqual({ solid: 4, tram2v: 1, tram2c: 1, tram3c: 3 });
  });

  it("splits the KT4 at its single joint", () => {
    const { plan: p } = planIn(DB_TRAM, "tram_ckd_kt4");
    const [front, rear] = p!.articulation.sections;
    expect(front!.min[0]).toBeCloseTo(0.5, 3);
    expect(front!.max[0]).toBeCloseTo(10.6, 3);
    expect(rear!.min[0]).toBeCloseTo(11.5, 3);
    expect(rear!.max[0]).toBeCloseTo(21.6, 3);
    expect(p!.articulation.bellows[0]!.splitX).toBeCloseTo(11.05, 3);
  });

  it("splits the KT8 into three sections joined by two bellows", () => {
    const { plan: p } = planIn(DB_TRAM, "tram_ckd_kt8");
    const bounds = p!.articulation.sections.map((s) => [s.min[0], s.max[0]]);
    expect(bounds[0]![0]).toBeCloseTo(0, 3);
    expect(bounds[0]![1]).toBeCloseTo(10.5, 3);
    expect(bounds[1]![0]).toBeCloseTo(11.2, 3);
    expect(bounds[1]![1]).toBeCloseTo(17.1, 3);
    expect(bounds[2]![0]).toBeCloseTo(17.8, 3);
    expect(bounds[2]![1]).toBeCloseTo(28.2, 3);

    const [b0, b1] = p!.articulation.bellows;
    expect(b0!.sections).toEqual([0, 1]);
    expect(b1!.sections).toEqual([1, 2]);
    expect(b0!.splitX).toBeCloseTo(10.85, 3);
    expect(b1!.splitX).toBeCloseTo(17.45, 3);

    // The KT8 declares ONE pas_* block; jt7 loads it once per gap, so each
    // bellows spans its own gap and nothing else (imotocar.cpp MT_Tram3c).
    const quadsFor = (node: string) => p!.parts.find((part) => part.name === node)!.polygons;
    for (const [node, [a, b]] of [["bellows0", [10.5, 11.2]], ["bellows1", [17.1, 17.8]]] as const) {
      const quads = quadsFor(node);
      expect(quads, node).toHaveLength(3);
      for (const q of quads) {
        expect(Math.min(...q.verts.map((v) => v.x))).toBeCloseTo(a, 3);
        expect(Math.max(...q.verts.map((v) => v.x))).toBeCloseTo(b, 3);
      }
    }
    // Two bellows continuing one body texture must not share UVs: with *_rel=1
    // each maps from its own model x, so bellows1 sits further along the bitmap.
    const sAt = (node: string) => Math.min(...quadsFor(node).flatMap((q) => q.verts.map((v) => v.s)));
    expect(sAt("bellows1")).toBeGreaterThan(sAt("bellows0"));
  });

  it("couples a tram2v to a second copy of itself at len + tramspace", () => {
    // GT4: body 0.4..14.3, tramspace 0.05 game units = 0.625 model units.
    const { meta, plan: p } = planIn(DB_TRAM, "tram_kvag_gt4");
    expect(meta.tramspace).toBe(0.05);
    const [first, second] = p!.articulation.sections;
    expect(first!.min[0]).toBeCloseTo(0.4, 3);
    expect(first!.max[0]).toBeCloseTo(14.3, 3);
    expect(second!.min[0]).toBeCloseTo(14.925, 3);
    expect(second!.max[0]).toBeCloseTo(28.825, 3);
    // Same car twice: identical polygon count, identical UVs, shifted geometry.
    const [s0, s1] = p!.parts;
    expect(s1!.polygons).toHaveLength(s0!.polygons.length);
    expect(s1!.polygons[0]!.verts.map((v) => [v.s, v.t]))
      .toEqual(s0!.polygons[0]!.verts.map((v) => [v.s, v.t]));
  });

  it("builds the KT8 GLB as five named nodes covering the whole vehicle", () => {
    const built = buildModelGlb(path.join(DB_TRAM, "tram_ckd_kt8"), MATERIALS)!;
    const json = glbJson(built.glb.buffer);
    expect(json.nodes.map((n: { name?: string }) => n.name))
      .toEqual(["root", "section0", "section1", "section2", "bellows0", "bellows1"]);
    expect(json.nodes[0].children).toEqual([1, 2, 3, 4, 5]);
    expect(json.meshes).toHaveLength(5);
    // Splitting must not resize the vehicle: the sim reads `length` from here.
    expect(built.glb.stats.length).toBeCloseTo(28.2, 3);
    expect(built.glb.stats.bbox.min[0]).toBeCloseTo(0, 3);
    expect(built.glb.stats.bbox.max[0]).toBeCloseTo(28.2, 3);
  });

  it("reports the coupled pair's full length, both cars included", () => {
    // GT4: one car 0.4..14.3, the baked copy tramspace (0.625 model units)
    // further back — the GLB's length is the whole road space the pair takes.
    const pair = buildModelGlb(path.join(DB_TRAM, "tram_kvag_gt4"), MATERIALS)!;
    expect(pair.articulation!.style).toBe("tram2v");
    expect(pair.glb.stats.length).toBeCloseTo(28.425, 3);
    const [first, second] = pair.articulation!.sections;
    expect(pair.glb.stats.vertices % 2).toBe(0); // the same car twice
    expect(second!.min[0] - first!.max[0]).toBeCloseTo(0.625, 3);
  });

  it("falls back to one solid when a tram's geometry does not match its style", () => {
    const { polygons, meta } = planIn(DB_TRAM, "tram_ckd_t3"); // one continuous body
    const warn = (style: string, extra: Record<string, unknown> = {}) => {
      const warnings: string[] = [];
      const p = planArticulation(polygons, { ...meta, style, ...extra }, (m) => warnings.push(m));
      expect(p, style).toBeNull();
      expect(warnings, style).toHaveLength(1);
    };
    warn("tram3c", { bellows: planIn(DB_TRAM, "tram_ckd_kt8").meta.bellows }); // 1 range, wants 3
    warn("tram2c", { bellows: planIn(DB_TRAM, "tram_ckd_kt4").meta.bellows }); // 1 range, wants 2
    // The KT8 is three bodies, so it cannot be a coupled pair.
    const kt8 = planIn(DB_TRAM, "tram_ckd_kt8");
    const warnings: string[] = [];
    expect(planArticulation(kt8.polygons, { ...kt8.meta, style: "tram2v" }, (m) => warnings.push(m))).toBeNull();
    expect(warnings).toHaveLength(1);
  });
});
