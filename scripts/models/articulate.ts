// Multi-body vehicles: split a model's polygon soup into sections and synthesize
// the bellows (the accordion) that bridges each gap between them.
//
// Port of jt7's MT_Bus2c / MT_Tram2c / MT_Tram3c / MT_Tram2v
// (jt7_src/jt7/imotocar.cpp). A model whose index.xml declares one of those
// styles is not one rigid body:
//
//   bus2c, tram2c  two sections, one bellows  (Karosa B741, Tatra KT4)
//   tram3c         three sections, TWO bellows (Tatra KT8)
//   tram2v         the same rigid car coupled to itself, no bellows (T3 Double)
//
// For the jointed styles the geometry itself has the holes in it, and the
// original renders each body as a separate solid posed on the trajectory, filling
// every gap with three textured quads (roof + both sides) whose front edge is
// transformed by the section ahead and rear edge by the section behind. Note that
// a tram3c's index.xml carries only ONE pas_* block: jt7 loads it twice, once per
// gap (MT_Tram3c::dopasload -> pas1.load(end1,beg2), pas2.load(end2,beg3)), and
// the *_rel UVs then place each bellows against its own stretch of the body
// texture. We do the same by reusing one BellowsSpec at two spans.
//
// A tram2v is not jointed at all: MT_Tram2v::render draws the one solid twice,
// the second copy `len + tramspace` further back. We bake that second copy in
// here as a second section, so the model's bbox, length and thumbnail all cover
// the real coupled pair and the render layer poses the two cars as a chain.
//
// We do the split here, at export time: each section becomes its own named node in
// the GLB (see glb.ts) and the bellows quads are baked in the straight pose, so a
// static render (thumbnail, vehicle browser) is unchanged. Rail Island's render
// layer poses the sections and re-skins the bellows every frame (its
// src/models/ArticulatedBus.ts), reading this metadata from <tree>.meta.json.
//
// Model axes here are the parsed ones (db/motocar convention): +X = length with
// the nose at min X, +Y = height, +Z = width.
import type { ModelPolygon, ModelVertex } from "./geometry.ts";
import type { BellowsFace, BellowsSpec } from "./metadata.ts";

/** One named group of polygons; a null name means "the whole model, unsplit". */
export interface ModelPart {
  name: string | null;
  polygons: ModelPolygon[];
}

// How many disjoint bodies each jointed style promises. A style not listed here
// is either rigid or handled separately (tram2v duplicates one body instead of
// splitting it).
const JOINTED_SECTIONS: Record<string, number> = { bus2c: 2, tram2c: 2, tram3c: 3 };

// jt7's carsizefactor: the scale from native model units to game units
// (imotocar.cpp, `float CSF=0.08`). Everything here works in model units, but
// `tramspace` is authored in game units, so it is the one value that needs it.
const CSF = 0.08;

/** A rigid section of an articulated vehicle, as a node in the built GLB. */
export interface ArticulationSection {
  node: string;
  min: [number, number, number];
  max: [number, number, number];
}

/** The deformable joint between two sections. */
export interface ArticulationBellows {
  node: string;
  /** Indices into `sections`: [front, rear]. */
  sections: [number, number];
  /** Model x at or below which a bellows vertex rides the front section. */
  splitX: number;
}

/** Articulation metadata carried in <tree>.meta.json for the render layer. */
export interface Articulation {
  style: string;
  sections: ArticulationSection[];
  bellows: ArticulationBellows[];
}

export interface ArticulationPlan {
  parts: ModelPart[];
  articulation: Articulation;
  /** Bare pack texture names the bellows needs in the atlas (e.g. "z_dec.bmp"). */
  textureFiles: string[];
}

// Two polygon x-ranges closer than this count as one section. The body is a
// continuous chain of overlapping polygons, so anything above it is a real gap.
const GAP_EPS = 1e-6;

interface Range {
  a: number;
  b: number;
}

/**
 * Merge every polygon's x-extent into disjoint ranges, low to high (the port of
 * jt7's addinterval + the scan in MT_PasCommon::load). A rigid model yields one
 * range; an articulated one yields a range per section.
 */
export function sectionRanges(polygons: ModelPolygon[]): Range[] {
  const raw: Range[] = [];
  for (const p of polygons) {
    if (p.verts.length === 0) continue;
    let a = Infinity;
    let b = -Infinity;
    for (const v of p.verts) {
      if (v.x < a) a = v.x;
      if (v.x > b) b = v.x;
    }
    if (a <= b) raw.push({ a, b });
  }
  raw.sort((x, y) => x.a - y.a);
  const merged: Range[] = [];
  for (const r of raw) {
    const last = merged[merged.length - 1];
    if (last && r.a <= last.b + GAP_EPS) last.b = Math.max(last.b, r.b);
    else merged.push({ ...r });
  }
  return merged;
}

function boundsOf(polygons: ModelPolygon[]): { min: [number, number, number]; max: [number, number, number] } {
  const min: [number, number, number] = [Infinity, Infinity, Infinity];
  const max: [number, number, number] = [-Infinity, -Infinity, -Infinity];
  for (const p of polygons) {
    for (const v of p.verts) {
      const c = [v.x, v.y, v.z];
      for (let i = 0; i < 3; i++) {
        if (c[i]! < min[i]!) min[i] = c[i]!;
        if (c[i]! > max[i]!) max[i] = c[i]!;
      }
    }
  }
  return { min, max };
}

// UV for one bellows corner. `along` is the model x, `across` the second axis
// (z for the roof, y for the sides). With rel=1 the XML's t1/t2/s1/s2 are model
// coordinates that normalize the corner into the texture -- exactly the mapping
// ColorAlg gives the body faces, so the accordion painted into the texture lines
// up with the body either side of it. With rel=0 they are literal UVs, applied
// across the quad's own extent.
//
// The original mirrors the roof's across axis (MD_PasData::load swaps +/-pas_wi/2
// into yd_s1/yd_s2); we deliberately do not, because our parsed z is the mirror of
// jt7's lateral axis and the body faces here map (z + s1) / (s2 - s1) unmirrored.
function faceUV(
  face: BellowsFace, along: number, across: number,
  alongRange: Range, acrossRange: Range,
): { s: number; t: number } {
  if (face.rel) {
    const ts = face.t2 - face.t1;
    const ss = face.s2 - face.s1;
    return {
      s: ts ? (along - face.t1) / ts : 0,
      t: ss ? (across - face.s1) / ss : 0,
    };
  }
  const alongSpan = alongRange.b - alongRange.a;
  const acrossSpan = acrossRange.b - acrossRange.a;
  return {
    s: face.t1 + (alongSpan ? (along - alongRange.a) / alongSpan : 0) * (face.t2 - face.t1),
    t: face.s1 + (acrossSpan ? (across - acrossRange.a) / acrossSpan : 0) * (face.s2 - face.s1),
  };
}

/** Bare file name of a bellows texture ("model/z_dec.bmp" -> "z_dec.bmp"). */
export function bellowsTextureName(file: string): string {
  const parts = file.split(/[\\/]/);
  return parts[parts.length - 1] ?? file;
}

// One bellows quad: four corners given as [along (x), across (z or y)] pairs in
// the winding the neighbouring body faces use, so the quad faces outward.
function bellowsQuad(
  face: BellowsFace, corners: [number, number][], toVertex: (along: number, across: number) => { x: number; y: number; z: number },
  normal: [number, number, number], alongRange: Range, acrossRange: Range,
): ModelPolygon {
  const verts: ModelVertex[] = corners.map(([along, across]) => {
    const p = toVertex(along, across);
    const uv = faceUV(face, along, across, alongRange, acrossRange);
    return { x: p.x, y: p.y, z: p.z, s: uv.s, t: uv.t };
  });
  return {
    textureKey: `pack:${bellowsTextureName(face.file)}`,
    normal,
    color: [1, 1, 1],
    transparent: false,
    verts,
  };
}

/**
 * Build the bellows for the gap `begX`..`endX`: roof plus both sides, in the
 * straight (undeformed) pose. Exported for tests.
 */
export function bellowsPolygons(spec: BellowsSpec, begX: number, endX: number): ModelPolygon[] {
  const hw = spec.width / 2;
  const { y1, y2 } = spec;
  const alongRange: Range = { a: begX, b: endX };
  const heightRange: Range = { a: y1, b: y2 };
  const widthRange: Range = { a: -hw, b: hw };

  // The db/motocar sides are mirrored against their own texture names: the faces
  // textured z_dec sit at +Z, z_inc at -Z (verified across every bus2c model).
  // The bellows follows the geometry, not the name, so each side continues the
  // body panel it touches.
  return [
    // roof: across = z, in the (x, z) winding the body's top faces use
    bellowsQuad(spec.ydec, [[endX, hw], [endX, -hw], [begX, -hw], [begX, hw]],
      (along, across) => ({ x: along, y: y2, z: across }), [0, -1, 0], alongRange, widthRange),
    // +Z side: across = y
    bellowsQuad(spec.zdec, [[begX, y2], [begX, y1], [endX, y1], [endX, y2]],
      (along, across) => ({ x: along, y: across, z: hw }), [0, 0, -1], alongRange, heightRange),
    // -Z side: across = y (mirrored winding)
    bellowsQuad(spec.zinc, [[endX, y2], [endX, y1], [begX, y1], [begX, y2]],
      (along, across) => ({ x: along, y: across, z: -hw }), [0, 0, 1], alongRange, heightRange),
  ];
}

/** Shift a polygon along +X, keeping its UVs (the copy is the same car). */
function translatedX(p: ModelPolygon, dx: number): ModelPolygon {
  return { ...p, verts: p.verts.map((v) => ({ ...v, x: v.x + dx })) };
}

/**
 * A `tram2v` (jt7 MT_Tram2v): one rigid car drawn twice, the second copy
 * `len + tramspace` further back. Both copies become sections, and there is no
 * bellows between them -- they are coupled vehicles, not a jointed body.
 */
function planCoupledPair(
  polygons: ModelPolygon[],
  meta: { style: string; tramspace: number | null },
  onWarn: (message: string) => void,
): ArticulationPlan | null {
  const ranges = sectionRanges(polygons);
  if (ranges.length !== 1) {
    onWarn(`style="${meta.style}" expects 1 solid body, found ${ranges.length}; building as one solid`);
    return null;
  }
  const body = ranges[0]!;
  // jt7 offsets the copy by solid->len + space, where both are already in game
  // units; here everything is native model units, so the gap converts back.
  const offset = (body.b - body.a) + (meta.tramspace ?? 0) / CSF;
  const parts: ModelPart[] = [
    { name: "section0", polygons },
    { name: "section1", polygons: polygons.map((p) => translatedX(p, offset)) },
  ];
  return {
    parts,
    articulation: {
      style: meta.style,
      sections: parts.map((part) => ({ node: part.name!, ...boundsOf(part.polygons) })),
      bellows: [],
    },
    textureFiles: [],
  };
}

/**
 * Split `polygons` into sections (plus a bellows per joint), or return null when
 * the model is rigid (no multi-body style) or its geometry does not match what
 * the style promises -- the caller then builds it as one solid, like jt7's
 * jtMotocar::load falling back to MT_Solid.
 */
export function planArticulation(
  polygons: ModelPolygon[],
  meta: { style: string; bellows: BellowsSpec | null; tramspace?: number | null },
  onWarn: (message: string) => void = () => {},
): ArticulationPlan | null {
  if (meta.style === "tram2v") {
    return planCoupledPair(polygons, { style: meta.style, tramspace: meta.tramspace ?? null }, onWarn);
  }
  const wanted = JOINTED_SECTIONS[meta.style];
  if (wanted === undefined) return null;
  if (!meta.bellows) {
    onWarn(`style="${meta.style}" but no bellows (pas_*) attributes; building as one solid`);
    return null;
  }
  const ranges = sectionRanges(polygons);
  if (ranges.length !== wanted) {
    onWarn(`style="${meta.style}" expects ${wanted} disjoint sections, found ${ranges.length}; building as one solid`);
    return null;
  }

  const parts: ModelPart[] = ranges.map((_, i) => ({ name: `section${i}`, polygons: [] as ModelPolygon[] }));
  for (const p of polygons) {
    let maxX = -Infinity;
    for (const v of p.verts) if (v.x > maxX) maxX = v.x;
    // The first section whose range this polygon ends inside owns it; anything
    // past the last boundary falls to the rear section.
    let si = ranges.findIndex((r) => maxX <= r.b + GAP_EPS);
    if (si < 0) si = ranges.length - 1;
    parts[si]!.polygons.push(p);
  }

  const sections: ArticulationSection[] = parts.map((part) => ({
    node: part.name!,
    ...boundsOf(part.polygons),
  }));

  // One bellows per gap. A tram3c declares a single pas_* block and jt7 loads it
  // once per gap, so the same spec is reused at each span -- with *_rel=1 the UVs
  // are computed from real model x, which is what makes the two agree with the
  // body panels either side of them.
  const bellows: ArticulationBellows[] = [];
  for (let i = 0; i + 1 < ranges.length; i++) {
    const front = ranges[i]!;
    const rear = ranges[i + 1]!;
    const node = `bellows${i}`;
    parts.push({ name: node, polygons: bellowsPolygons(meta.bellows, front.b, rear.a) });
    bellows.push({ node, sections: [i, i + 1], splitX: (front.b + rear.a) / 2 });
  }

  return {
    parts,
    articulation: { style: meta.style, sections, bellows },
    textureFiles: [meta.bellows.ydec, meta.bellows.zdec, meta.bellows.zinc].map((f) => bellowsTextureName(f.file)),
  };
}
