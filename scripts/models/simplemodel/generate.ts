// Port of recount() (jt7 modellib/simpleobj) — regenerate a simplemodel's <model>
// polygon soup from its source description (clips, shapes, colorizes, stamps).
// Returns a synthetic <model> node in fast-xml-parser shape ({ polygon: [...] })
// so geometry.ts's walk() consumes it like a parsed <model>.
//
// Inlines are intentionally NOT expanded here — geometry.ts handles them via its
// own <inlines> recursion. <crook>/<noise> are unused across the DB and skipped.

import { asArray, attrStr, attrNum, type XmlNode } from "../xml.ts";
import { PedrAlg } from "./pedralg.ts";
import { polyedrToPolygons, type PolyedrLike } from "./polyedr.ts";
import { ColorAlg, StampAlg, type XmlPolygon } from "./coloralg.ts";
import { generateRotateSolid, loadCylindricColorize } from "./rotatesolid.ts";

// findalong: resolve an axis section, following a `sameas` redirect.
function findAlong(sm: XmlNode, name: string): XmlNode | undefined {
  const along = sm["along" + name] as XmlNode | undefined;
  if (!along) return undefined;
  const sameas = attrStr(along, "sameas");
  if (sameas) return sm["along" + sameas] as XmlNode | undefined;
  return along;
}

// makeclips: feed every clip polygon whose `apply` list names this shape.
function makeClips(name: string, along: XmlNode | undefined, pedr: PedrAlg, dir: number): boolean {
  let res = false;
  if (!along) return res;
  const clip = along.clip as XmlNode | undefined;
  for (const poly of asArray(clip?.polygon as XmlNode | XmlNode[] | undefined)) {
    const names = attrStr(poly, "apply").split(";");
    if (names.includes(name)) {
      const points = asArray(poly.point as XmlNode | XmlNode[] | undefined).map((p) => ({
        x: attrNum(p, "x"),
        y: attrNum(p, "y"),
        faceid: attrStr(p, "faceid"),
      }));
      pedr.addclip(dir, points);
      res = true;
    }
  }
  return res;
}

/**
 * Regenerate <model> polygons from source. Returns a fast-xml-parser-shaped node
 * `{ polygon: [...] }`, or `null` when there is no source to regenerate from.
 */
export function generateModel(sm: XmlNode, { flipS = {} }: { flipS?: Record<string, boolean> } = {}): { polygon: XmlPolygon[] } | null {
  const shapes = asArray((sm.shapes as XmlNode | undefined)?.shape as XmlNode | XmlNode[] | undefined);
  if (shapes.length === 0) return null;
  const alongx = findAlong(sm, "x");
  const alongy = findAlong(sm, "y");
  const alongz = findAlong(sm, "z");
  if (!alongx && !alongy && !alongz) return null;

  const model: XmlPolygon[] = [];

  for (const shape of shapes) {
    const name = attrStr(shape, "name");
    const pedr = new PedrAlg();
    let areclips = false;
    if (makeClips(name, alongx, pedr, 0)) areclips = true;
    if (makeClips(name, alongy, pedr, 1)) areclips = true;
    if (makeClips(name, alongz, pedr, 2)) areclips = true;

    const type = attrStr(shape, "type");
    let mod: PolyedrLike | null = null;
    if (type === "clip") {
      pedr.initalg();
      pedr.run();
      mod = pedr.toPolyedr();
    } else if (type === "rotate") {
      const vertEls = asArray((shape.verteces as XmlNode | undefined)?.vertex as XmlNode | XmlNode[] | undefined);
      const pts = vertEls.map((v) => ({ x: attrNum(v, "x"), y: attrNum(v, "y"), z: attrNum(v, "z") }));
      const faceids: string[] = [];
      for (let i = 0; i < pts.length - 1; i++) faceids.push(attrStr(vertEls[i], "faceid"));
      const cize = loadCylindricColorize(asArray((shape.colorizes as XmlNode | undefined)?.colorize as XmlNode | XmlNode[] | undefined));
      const cnt = attrNum(shape, "rotateverteces");
      const rotateOut = generateRotateSolid(pts, cnt, cize, faceids);
      if (attrStr(shape, "cullsafe") === "1") for (const f of rotateOut.faces) f.culling = 0;
      if (areclips) {
        pedr.loadInput(rotateOut);
        pedr.run();
        mod = pedr.toPolyedr();
      } else {
        mod = rotateOut;
      }
    } else {
      continue;
    }

    for (const polygon of polyedrToPolygons(mod)) model.push(polygon);
  }

  // Build colorize sides (non-stamp items only).
  const calg = new ColorAlg();
  calg.flipS = flipS;
  for (const cize of asArray((sm.colorizes as XmlNode | undefined)?.colorize as XmlNode | XmlNode[] | undefined)) {
    if (attrStr(cize, "stamp") === "1") continue;
    const material = attrStr(cize, "material") === "1";
    const dirs = attrStr(cize, "dirs").split(";");
    const rk = attrStr(cize, "rkoef") || "1";
    const gk = attrStr(cize, "gkoef") || "1";
    const bk = attrStr(cize, "bkoef") || "1";
    const denylight = attrStr(cize, "denylight") === "1";
    const transparent = attrStr(cize, "transparent") === "1";
    const cond = attrStr(cize, "cond");
    const texture = attrStr(cize, "texture");
    for (let dir = 0; dir < dirs.length; dir++) {
      if (dirs[dir] !== "1") continue;
      if (material) {
        calg.addMaterialSide(dir, attrNum(cize, "dx"), attrNum(cize, "dy"), attrNum(cize, "xscale"),
          attrNum(cize, "yscale"), texture, transparent, rk, gk, bk, attrStr(cize, "transp") === "1", cond, denylight);
      } else {
        calg.addSide(dir, attrNum(cize, "texx1"), attrNum(cize, "texy1"), attrNum(cize, "texx2"),
          attrNum(cize, "texy2"), texture, transparent, cond, rk, gk, bk, denylight);
      }
    }
  }

  for (const polygon of model) calg.run(polygon);

  // Stamp decals (colorizes with stamp="1").
  const newpolys: XmlPolygon[] = [];
  for (const stamp of asArray((sm.colorizes as XmlNode | undefined)?.colorize as XmlNode | XmlNode[] | undefined)) {
    if (attrStr(stamp, "stamp") !== "1") continue;
    let xcnt = attrNum(stamp, "stampcntx");
    let ycnt = attrNum(stamp, "stampcnty");
    const xsp = attrNum(stamp, "stampspacex");
    const ysp = attrNum(stamp, "stampspacey");
    if (xcnt < 1) xcnt = 1;
    if (ycnt < 1) ycnt = 1;
    const tx1 = attrNum(stamp, "texx1");
    const ty1 = attrNum(stamp, "texy1");
    const tx2 = attrNum(stamp, "texx2");
    const ty2 = attrNum(stamp, "texy2");
    const elemwi = (tx2 - tx1 - (xcnt - 1) * xsp) / xcnt;
    const elemhi = (ty2 - ty1 - (ycnt - 1) * ysp) / ycnt;

    const stalg = new StampAlg();
    stalg.texture = attrStr(stamp, "texture");
    stalg.material = attrStr(stamp, "material") === "1";
    stalg.xscale = attrNum(stamp, "xscale");
    stalg.yscale = attrNum(stamp, "yscale");
    stalg.setCondition(attrStr(stamp, "cond"));
    stalg.dist = attrNum(stamp, "stampdist");
    if (attrStr(stamp, "rkoef")) stalg.rkoef = attrStr(stamp, "rkoef");
    if (attrStr(stamp, "gkoef")) stalg.gkoef = attrStr(stamp, "gkoef");
    if (attrStr(stamp, "bkoef")) stalg.bkoef = attrStr(stamp, "bkoef");

    const dirs = attrStr(stamp, "dirs").split(";");
    for (let x = 0; x < xcnt; x++) {
      for (let y = 0; y < ycnt; y++) {
        stalg.x1 = tx1 + (elemwi + xsp) * x;
        stalg.y1 = ty1 + (elemhi + ysp) * y;
        stalg.x2 = stalg.x1 + elemwi;
        stalg.y2 = stalg.y1 + elemhi;
        for (let dir = 0; dir < dirs.length; dir++) {
          if (dirs[dir] !== "1") continue;
          stalg.dir = dir;
          for (const polygon of model) stalg.run(polygon, newpolys);
        }
      }
    }
  }
  for (const p of newpolys) model.push(p);

  // deletefaces: drop polygons whose faceid is listed.
  const erase = attrStr(sm, "deletefaces").split(";").filter((s) => s !== "");
  const result = erase.length
    ? model.filter((p) => !erase.includes(attrStr(p as XmlNode, "faceid")))
    : model;

  return { polygon: result };
}
