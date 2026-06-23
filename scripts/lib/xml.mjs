// Windows-1250 tolerant XML parsing (port of parse_xml_safe from upload_utils.py).
import fs from "node:fs";
import iconv from "iconv-lite";
import { XMLParser } from "fast-xml-parser";

const parser = new XMLParser({
  ignoreAttributes: false,
  attributeNamePrefix: "@_",
  // Keep elements that appear once as objects; arrays handled by helpers below.
  isArray: () => false,
});

/**
 * Parse an XML file that is encoded in Windows-1250 (cp1250) and may carry a
 * mismatched `<?xml encoding=...?>` declaration. Returns the parsed object tree
 * (fast-xml-parser shape: attributes prefixed with `@_`).
 */
export function parseXmlSafe(path) {
  const bytes = fs.readFileSync(path);
  let content = iconv.decode(bytes, "cp1250");
  if (content.charCodeAt(0) === 0xfeff) content = content.slice(1);
  // Drop the declaration so its (wrong) encoding hint can't confuse anything.
  if (content.startsWith("<?xml")) {
    const end = content.indexOf("?>");
    if (end !== -1) content = content.slice(end + 2);
  }
  return parser.parse(content);
}

/** Normalize a child accessor to always return an array. */
export function asArray(node) {
  if (node === undefined || node === null) return [];
  return Array.isArray(node) ? node : [node];
}

/** Read a numeric attribute (`@_name`) with a default. */
export function attrNum(obj, name, dflt = 0) {
  const v = obj?.["@_" + name];
  if (v === undefined || v === null || v === "") return dflt;
  const n = Number(v);
  return Number.isFinite(n) ? n : dflt;
}

/** Read a string attribute (`@_name`) with a default. */
export function attrStr(obj, name, dflt = "") {
  const v = obj?.["@_" + name];
  return v === undefined || v === null ? dflt : String(v);
}
