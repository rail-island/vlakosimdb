// Parse a model's top-level index.xml metadata (name, title, specs, author...)
// plus the articulation spec (`style` + the bellows description) that decides
// whether the model is built as one rigid body or as jointed sections.
import { parseXmlSafe, asArray, attrStr, attrNum, type XmlNode } from "./xml.ts";

// The original game stored the chimney anchor normalized by this divisor, so a
// stored 0.12 means model x = 1.5. Confirmed two ways: every model's side-view
// `image.bmp` is exactly 10 px per model unit (a 12x6 model ships a 120x60 bitmap),
// and feeding `attr * 12.5` back through the sprite silhouette lands on the funnel
// rim on every smoking loco. It is NOT a per-model fraction of xsize/ysize -- that
// reading puts the plume mid-boiler, and is impossible for csd_386 (0.4 * 30 = 12
// exceeds its 4.9 model height).
const SMOKE_UNIT = 12.5;

// One of the three textured faces of the bellows ("pas" in the original), as
// described by the `<zd|zi|yd>_*` attribute family on <motocar>. `t` runs along
// the vehicle, `s` across it (the original's naming, which is the transpose of
// our ModelVertex s/t -- see articulate.ts).
export interface BellowsFace {
  /** Texture file, as written in the XML (e.g. "model/z_dec.bmp"). */
  file: string;
  t1: number;
  t2: number;
  s1: number;
  s2: number;
  /** When set, t1/t2/s1/s2 are model coordinates to normalize, not raw UVs. */
  rel: boolean;
}

// The gap-filling accordion between two sections: a box of width `width`
// spanning `y1`..`y2` in height, textured on the roof and both sides.
export interface BellowsSpec {
  width: number;
  y1: number;
  y2: number;
  ydec: BellowsFace;
  zdec: BellowsFace;
  zinc: BellowsFace;
}

// Where a steam locomotive's plume leaves the chimney, in native model units
// (+X from the nose at min X, +Y up from rail level; Z is always the centerline,
// there is no smoke_z). Null for everything that does not smoke.
export interface SmokeAnchor {
  x: number;
  y: number;
}

export interface ModelMetadata {
  name: string;
  title: string;
  author: string;
  email: string;
  weight: number | null;
  power: number | null;
  maxspeed: number | null;
  path: string;
  /**
   * Render style, as jt7's jtMotocar::load dispatches it: "bus2c" / "tram2c" =
   * two jointed sections + a bellows, "tram3c" = three sections + two bellows,
   * "tram2v" = the same rigid car coupled to itself, "" / "solid" = rigid.
   */
  style: string;
  /** Bellows description; null when the model declares no textured joint. */
  bellows: BellowsSpec | null;
  /**
   * Gap between the two cars of a `tram2v`, in GAME units (jt7 reads `tramspace`
   * straight into `MT_Tram2v::space` and adds it to the already CSF-scaled body
   * length). Null when the model declares none. See articulate.ts for the
   * conversion back to the native model units everything else here uses.
   */
  tramspace: number | null;
  /** Chimney plume origin (native model units), or null when this model has none. */
  smoke: SmokeAnchor | null;
}

/**
 * Resolve the chimney anchor from a `<loco>`/`<motocar>` root node.
 *
 * Two places carry it, and they disagree on one model. The root `smoke_x`/`smoke_y`
 * attributes are present on 276 models but empty on 265 of them, and five of the
 * remaining share a copy-pasted `0.12 / 0.35` despite lengths of 12..26 units. Four
 * models additionally carry a `<smoke>` CHILD element with the original game's full
 * particle spec, whose `x0`/`y0` are the retuned values -- on `csd_498_1` the child
 * says 0.2/0.4 (which matches its funnel) while the root still says the stale
 * 0.12/0.35. So the child wins where it exists.
 *
 * Note `smoke` is BOTH a root attribute (`@_smoke`, a dangling "../smoke1" path to
 * sprite assets that were never ported) and a child element name. Attributes carry
 * the `@_` prefix, so `el.smoke` reads only the child and the two cannot collide.
 */
export function parseSmokeAnchor(el: XmlNode | undefined): SmokeAnchor | null {
  if (!el) return null;
  const child = asArray<XmlNode>(el["smoke"] as XmlNode | XmlNode[] | undefined)[0];
  // attrNum() returns its default for missing/empty attributes, so NaN marks
  // "not declared here" and lets the root fall through when the child omits x0/y0.
  const candidates: [number, number][] = [
    [attrNum(child, "x0", NaN), attrNum(child, "y0", NaN)],
    [attrNum(el, "smoke_x", NaN), attrNum(el, "smoke_y", NaN)],
  ];
  for (const [x, y] of candidates) {
    if (Number.isFinite(x) && Number.isFinite(y)) {
      // Round off binary-float dust (0.232 * 12.5 = 2.9000000000000004) so the
      // manifest carries clean numbers, as glb.ts does for `length`. 1e6 keeps the
      // deepest real value exact -- csd_636's 0.425 -> 5.3125 needs four decimals.
      return { x: roundUnit(x * SMOKE_UNIT), y: roundUnit(y * SMOKE_UNIT) };
    }
  }
  return null;
}

function roundUnit(v: number): number {
  return Math.round(v * 1e6) / 1e6;
}

function bellowsFace(el: XmlNode, prefix: string, fileAttr: string): BellowsFace | null {
  const file = attrStr(el, fileAttr);
  if (!file) return null;
  return {
    file,
    t1: attrNum(el, `${prefix}_t1`),
    t2: attrNum(el, `${prefix}_t2`),
    s1: attrNum(el, `${prefix}_s1`),
    s2: attrNum(el, `${prefix}_s2`),
    rel: attrNum(el, `${prefix}_rel`) === 1,
  };
}

function parseBellows(el: XmlNode): BellowsSpec | null {
  const width = attrNum(el, "pas_wi");
  const y1 = attrNum(el, "pas_y1");
  const y2 = attrNum(el, "pas_y2");
  if (!(width > 0) || !(y2 > y1)) return null;
  const ydec = bellowsFace(el, "yd", "ydecfile");
  const zdec = bellowsFace(el, "zd", "zdecfile");
  const zinc = bellowsFace(el, "zi", "zincfile");
  if (!ydec || !zdec || !zinc) return null;
  return { width, y1, y2, ydec, zdec, zinc };
}

export function parseMetadata(indexXmlPath: string): ModelMetadata {
  const root = parseXmlSafe(indexXmlPath);
  // Root element is usually <motocar ...> / <loco ...>; fall back to the first element.
  const el = (root.loco ?? root.motocar ?? root.building ??
    Object.values(root).find((v) => v && typeof v === "object")) as XmlNode ?? {};
  const num = (n: string): number | null => {
    const v = el["@_" + n];
    if (v === undefined || v === null || v === "") return null;
    const x = Number(v);
    return Number.isFinite(x) ? x : null;
  };
  return {
    name: attrStr(el, "name"),
    title: attrStr(el, "title"),
    author: attrStr(el, "author"),
    email: attrStr(el, "email"),
    weight: num("weight"),
    power: num("power"),
    maxspeed: num("maxspeed"),
    path: attrStr(el, "path"),
    style: attrStr(el, "style"),
    bellows: parseBellows(el),
    tramspace: num("tramspace"),
    smoke: parseSmokeAnchor(el),
  };
}
