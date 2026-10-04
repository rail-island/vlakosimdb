// Windows-1250 tolerant XML parsing for the legacy model database.
//
// The model XML files in db/ are encoded in Windows-1250 (cp1250) and often carry
// a mismatched `<?xml encoding=...?>` declaration. We decode the bytes explicitly
// and strip the declaration so its (wrong) hint cannot confuse the parser.
import fs from "node:fs";
import iconv from "iconv-lite";
import { XMLParser } from "fast-xml-parser";

// A parsed XML node in fast-xml-parser shape: attributes are prefixed with `@_`,
// child elements are keyed by tag name (object when unique, array when repeated).
export type XmlNode = Record<string, unknown>;

const parser = new XMLParser({
  ignoreAttributes: false,
  attributeNamePrefix: "@_",
  // Keep single elements as objects; the asArray() helper normalizes repeats.
  isArray: () => false,
});

/** Parse a cp1250-encoded XML file and return the fast-xml-parser object tree. */
export function parseXmlSafe(filePath: string): XmlNode {
  const bytes = fs.readFileSync(filePath);
  let content = iconv.decode(bytes, "cp1250");
  if (content.charCodeAt(0) === 0xfeff) content = content.slice(1);
  if (content.startsWith("<?xml")) {
    const end = content.indexOf("?>");
    if (end !== -1) content = content.slice(end + 2);
  }
  return parser.parse(content) as XmlNode;
}

/** Normalize a child accessor to always return an array. */
export function asArray<T = XmlNode>(node: T | T[] | undefined | null): T[] {
  if (node === undefined || node === null) return [];
  return Array.isArray(node) ? node : [node];
}

/** Read a numeric attribute (`@_name`) with a default. */
export function attrNum(obj: XmlNode | undefined, name: string, dflt = 0): number {
  const v = obj?.["@_" + name];
  if (v === undefined || v === null || v === "") return dflt;
  const n = Number(v);
  return Number.isFinite(n) ? n : dflt;
}

/** Read a string attribute (`@_name`) with a default. */
export function attrStr(obj: XmlNode | undefined, name: string, dflt = ""): string {
  const v = obj?.["@_" + name];
  return v === undefined || v === null ? dflt : String(v);
}
