// Parse a model's top-level index.xml metadata (name, title, specs, author...).
import { parseXmlSafe, attrStr, attrNum } from "./xml.mjs";

export function parseMetadata(indexXmlPath) {
  const root = parseXmlSafe(indexXmlPath);
  // Root element is usually <loco ...>; fall back to the first element.
  const el = root.loco ?? root.building ?? Object.values(root).find((v) => v && typeof v === "object") ?? {};
  const num = (n) => {
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
  };
}
