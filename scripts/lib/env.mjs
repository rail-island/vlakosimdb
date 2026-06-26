// Minimal .env loader (replaces Python's python-dotenv load_dotenv). Reads the
// repo-root .env and populates process.env without overwriting existing values.
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

export function repoRoot() {
  return path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..", "..");
}

export function loadEnv() {
  const envPath = path.join(repoRoot(), ".env");
  let text;
  try {
    text = fs.readFileSync(envPath, "utf-8");
  } catch {
    return; // no .env — rely on the ambient environment
  }
  for (const raw of text.split(/\r?\n/)) {
    const line = raw.trim();
    if (!line || line.startsWith("#")) continue;
    const eq = line.indexOf("=");
    if (eq === -1) continue;
    const key = line.slice(0, eq).trim();
    let val = line.slice(eq + 1).trim();
    // Strip matching surrounding quotes.
    if (
      (val.startsWith('"') && val.endsWith('"')) ||
      (val.startsWith("'") && val.endsWith("'"))
    ) {
      val = val.slice(1, -1);
    }
    if (key && process.env[key] === undefined) process.env[key] = val;
  }
}
