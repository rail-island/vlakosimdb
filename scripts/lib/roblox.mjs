// Roblox Open Cloud Assets API helpers + upload lockfile (port of the Roblox
// pieces of upload_utils.py). Uses native fetch/FormData/Blob (Node 18+).
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import { repoRoot } from "./env.mjs";

const ROBLOX_ASSETS_API = "https://apis.roblox.com/assets/v1/assets";
const LOCKFILE_NAME = "roblox_uploads.lock.json";

// ---------------------------------------------------------------------------
// Lockfile + hashing
// ---------------------------------------------------------------------------

function lockfilePath() {
  return path.join(repoRoot(), LOCKFILE_NAME);
}

export function loadLockfile() {
  try {
    return JSON.parse(fs.readFileSync(lockfilePath(), "utf-8"));
  } catch {
    return {};
  }
}

export function saveLockfile(lockdata) {
  const sorted = {};
  for (const k of Object.keys(lockdata).sort()) sorted[k] = lockdata[k];
  fs.writeFileSync(lockfilePath(), JSON.stringify(sorted, null, 2) + "\n");
}

export function fileContentHash(filePath) {
  return crypto.createHash("sha256").update(fs.readFileSync(filePath)).digest("hex");
}

export function bufferHash(buf) {
  return crypto.createHash("sha256").update(buf).digest("hex");
}

export function makeLockKey(relPath, contentHash) {
  return `${relPath}:${contentHash}`;
}

// ---------------------------------------------------------------------------
// Uploads
// ---------------------------------------------------------------------------

async function postAsset(assetType, fileName, mime, bytes, apiKey, creatorId, displayName, description) {
  const request = {
    assetType,
    displayName: displayName.slice(0, 50),
    description: description.slice(0, 1000),
    creationContext: { creator: { userId: creatorId } },
  };
  const form = new FormData();
  form.append("request", JSON.stringify(request));
  form.append("fileContent", new Blob([bytes], { type: mime }), fileName);

  const resp = await fetch(ROBLOX_ASSETS_API, {
    method: "POST",
    headers: { "x-api-key": apiKey },
    body: form,
  });
  if (resp.status !== 200 && resp.status !== 201) {
    console.error(`  [ERROR] ${assetType} upload failed (${resp.status}): ${await resp.text()}`);
    return null;
  }
  const result = await resp.json();
  if (result.path && !result.done) return pollOperation(result.path, apiKey);
  if (result.done && result.response) return result.response.assetId;
  return result.assetId ?? null;
}

export async function uploadMeshToRoblox(glbBytes, fileName, apiKey, creatorId, displayName, description) {
  return postAsset("Model", fileName, "model/gltf-binary", glbBytes, apiKey, creatorId, displayName, description);
}

export async function uploadImageToRoblox(pngBytes, fileName, apiKey, creatorId, displayName, description) {
  return postAsset("Image", fileName, "image/png", pngBytes, apiKey, creatorId, displayName, description);
}

export async function pollOperation(operationPath, apiKey, maxAttempts = 20) {
  const url = operationPath.startsWith("assets/")
    ? `https://apis.roblox.com/${operationPath}`
    : `https://apis.roblox.com/assets/v1/${operationPath}`;
  for (let attempt = 0; attempt < maxAttempts; attempt++) {
    await new Promise((r) => setTimeout(r, 3000));
    const resp = await fetch(url, { headers: { "x-api-key": apiKey } });
    if (resp.status !== 200) {
      console.log(`  [POLL] ${attempt + 1}/${maxAttempts}: status ${resp.status}`);
      continue;
    }
    const data = await resp.json();
    if (data.done) return data.response?.assetId ?? null;
    console.log(`  [POLL] ${attempt + 1}/${maxAttempts}: not done yet…`);
  }
  console.log("  [WARN] Operation did not complete in time");
  return null;
}
