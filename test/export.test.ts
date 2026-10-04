// The export's id contract. Rail Island stores these ids in every saved map, so a
// vehicle whose id changes vanishes from the islands it ran on. The full list is
// pinned in test/fixtures/vehicle-ids.json: moving, renaming or deleting a model
// folder fails here first. When that is really intended, regenerate the fixture
// and expect Rail Island to need a migration (or db/archived.json) for the ids
// that went.

import { describe, it, expect } from "vitest";
import fs from "node:fs";
import os from "node:os";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { allocateIds, modelId, TREES } from "../scripts/models/export.ts";

const REPO = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");
const PINNED = JSON.parse(fs.readFileSync(path.join(REPO, "test", "fixtures", "vehicle-ids.json"), "utf8")) as string[];

describe("vehicle ids", () => {
  const models = allocateIds();

  it("are exactly the pinned list, in export order", () => {
    expect(models.map((m) => m.id)).toEqual(PINNED);
  });

  it("are unique across the three trees", () => {
    expect(new Set(models.map((m) => m.id)).size).toBe(models.length);
  });

  it("carry the category of the tree they come from", () => {
    const count: Record<string, number> = {};
    for (const m of models) {
      expect(m.id.startsWith(`${m.category}-`), m.id).toBe(true);
      count[m.tree] = (count[m.tree] ?? 0) + 1;
    }
    expect(count).toEqual({ motocar: 18, train: 312, tram: 9 });
  });

  it("depend on the path below the tree only, not on where the tree sits", () => {
    // The trams moved from db/motocar/tram to db/tram without changing an id.
    const tree = TREES.find((t) => t.tree === "tram")!;
    expect(modelId("/a/db/tram", "/a/db/tram/tram_ckd_kt8#2", tree.category)).toBe("tram-tram-ckd-kt8-2");
    expect(modelId("/b/db/motocar/tram", "/b/db/motocar/tram/tram_ckd_kt8#2", tree.category)).toBe("tram-tram-ckd-kt8-2");
    expect(modelId("/r/db/train", "/r/db/train/loco/steam/csd_475_179#l", "train")).toBe("train-loco-steam-csd-475-179-l");
  });

  it("settles a slug clash with a -2 suffix, in tree order", () => {
    const root = fs.mkdtempSync(path.join(os.tmpdir(), "vlakosimdb-ids-"));
    try {
      for (const rel of ["train/a b/model", "train/a_b/model", "tram/x/model"]) {
        fs.mkdirSync(path.join(root, rel), { recursive: true });
        fs.writeFileSync(path.join(root, rel, "index.xml"), "<simplemodel/>");
      }
      expect(allocateIds(root).map((m) => m.id)).toEqual(["train-a-b", "train-a-b-2", "tram-x"]);
    } finally {
      fs.rmSync(root, { recursive: true, force: true });
    }
  });
});
