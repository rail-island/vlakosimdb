import { useCallback, useEffect, useMemo, useRef, useState } from "react";
import Filters from "./components/Filters.jsx";
import Gallery from "./components/Gallery.jsx";
import Viewer from "./components/Viewer.jsx";

const CHECKED_TRAINS_KEY = "vlakosimdb:checkedTrains";

const CATEGORY_LABELS = {
  loco: "Locomotives",
  coach: "Coaches",
  emu: "EMU",
  dmu: "DMU",
  truck: "Freight",
  tram: "Trams",
  metro: "Metro",
  "760_mm": "Narrow gauge",
  other: "Other",
};

function selectedIdFromHash() {
  return window.location.hash.startsWith("#/") ? window.location.hash.slice(2) || null : null;
}

function formatLabel(value) {
  return value.replace(/[_-]+/g, " ").replace(/\b\w/g, (letter) => letter.toUpperCase());
}

function readCheckedTrains() {
  try {
    const stored = JSON.parse(localStorage.getItem(CHECKED_TRAINS_KEY) || "[]");
    return new Set(Array.isArray(stored) ? stored.filter((id) => typeof id === "string") : []);
  } catch {
    return new Set();
  }
}

export default function App() {
  const [manifest, setManifest] = useState(null);
  const [error, setError] = useState(null);
  const [search, setSearch] = useState("");
  const [category, setCategory] = useState("all");
  const [resolution, setResolution] = useState("all");
  const [checkedOnly, setCheckedOnly] = useState(false);
  const [locomotiveType, setLocomotiveType] = useState("all");
  const [selectedId, setSelectedId] = useState(selectedIdFromHash);
  const [checkedTrainIds, setCheckedTrainIds] = useState(readCheckedTrains);
  const [menuOpen, setMenuOpen] = useState(false);
  const [copyStatus, setCopyStatus] = useState("");
  const galleryScroll = useRef({ x: 0, y: 0 });

  useEffect(() => {
    localStorage.setItem(CHECKED_TRAINS_KEY, JSON.stringify([...checkedTrainIds]));
  }, [checkedTrainIds]);

  useEffect(() => {
    fetch("manifest.json")
      .then((r) => {
        if (!r.ok) throw new Error(`manifest.json ${r.status}`);
        return r.json();
      })
      .then(setManifest)
      .catch((e) => setError(e.message));
  }, []);

  // Keep selection in the URL hash for shareable deep links.
  useEffect(() => {
    const onHash = () => setSelectedId(selectedIdFromHash());
    window.addEventListener("hashchange", onHash);
    window.addEventListener("popstate", onHash);
    return () => {
      window.removeEventListener("hashchange", onHash);
      window.removeEventListener("popstate", onHash);
    };
  }, []);

  const select = useCallback((id) => {
    if (id) {
      galleryScroll.current = { x: window.scrollX, y: window.scrollY };
      window.history.pushState(null, "", `#/${id}`);
      setSelectedId(id);
      return;
    }

    const { x, y } = galleryScroll.current;
    window.history.pushState(null, "", `${window.location.pathname}${window.location.search}`);
    setSelectedId(id);
    requestAnimationFrame(() => window.scrollTo({ left: x, top: y, behavior: "auto" }));
  }, []);

  const setTrainChecked = useCallback((id, checked) => {
    setCheckedTrainIds((prev) => {
      const next = new Set(prev);
      if (checked) next.add(id);
      else next.delete(id);
      return next;
    });
  }, []);

  const handleCategory = useCallback((nextCategory) => {
    setCategory(nextCategory);
    if (nextCategory !== "all" && nextCategory !== "loco") setLocomotiveType("all");
  }, []);

  const handleLocomotiveType = useCallback((nextType) => {
    setLocomotiveType(nextType);
    if (nextType !== "all") setCategory("loco");
  }, []);

  const categories = useMemo(() => {
    if (!manifest) return [];
    const counts = new Map();
    for (const m of manifest.models) counts.set(m.category, (counts.get(m.category) || 0) + 1);
    return [...counts.entries()]
      .sort((a, b) => b[1] - a[1])
      .map(([key, count]) => ({ key, count, label: CATEGORY_LABELS[key] || key }));
  }, [manifest]);

  const resolutionCounts = useMemo(() => {
    if (!manifest) return { hires: 0, lores: 0 };
    let hires = 0;
    for (const m of manifest.models) if (m.hires) hires++;
    return { hires, lores: manifest.models.length - hires };
  }, [manifest]);

  const locomotiveTypes = useMemo(() => {
    if (!manifest) return [];
    const counts = new Map();
    for (const m of manifest.models) {
      if (m.category !== "loco" || !m.subcategory) continue;
      counts.set(m.subcategory, (counts.get(m.subcategory) || 0) + 1);
    }
    return [...counts.entries()]
      .sort((a, b) => b[1] - a[1])
      .map(([key, count]) => ({ key, count, label: formatLabel(key) }));
  }, [manifest]);

  const checkedTrainLines = useMemo(() => {
    if (!manifest) return [];
    return manifest.models.filter((m) => checkedTrainIds.has(m.id)).map((m) => m.id);
  }, [manifest, checkedTrainIds]);

  const copySelectedTrains = useCallback(async () => {
    const text = checkedTrainLines.join("\n");
    try {
      if (navigator.clipboard?.writeText) {
        await navigator.clipboard.writeText(text);
      } else {
        const textarea = document.createElement("textarea");
        textarea.value = text;
        textarea.setAttribute("readonly", "");
        textarea.style.position = "fixed";
        textarea.style.opacity = "0";
        document.body.appendChild(textarea);
        textarea.select();
        document.execCommand("copy");
        document.body.removeChild(textarea);
      }
      setCopyStatus(`Copied ${checkedTrainLines.length}`);
    } catch {
      setCopyStatus("Copy failed");
    } finally {
      setMenuOpen(false);
      window.setTimeout(() => setCopyStatus(""), 1800);
    }
  }, [checkedTrainLines]);

  const filtered = useMemo(() => {
    if (!manifest) return [];
    const q = search.trim().toLowerCase();
    return manifest.models.filter((m) => {
      if (category !== "all" && m.category !== category) return false;
      if (locomotiveType !== "all" && (m.category !== "loco" || m.subcategory !== locomotiveType)) return false;
      if (resolution === "hires" && !m.hires) return false;
      if (resolution === "lores" && m.hires) return false;
      if (checkedOnly && !checkedTrainIds.has(m.id)) return false;
      if (!q) return true;
      return (
        m.title.toLowerCase().includes(q) ||
        m.name.toLowerCase().includes(q) ||
        (m.author && m.author.toLowerCase().includes(q))
      );
    });
  }, [manifest, search, category, resolution, checkedOnly, checkedTrainIds, locomotiveType]);

  const selected = useMemo(
    () => (manifest && selectedId ? manifest.models.find((m) => m.id === selectedId) : null),
    [manifest, selectedId]
  );

  return (
    <div className="app">
      <header className="topbar">
        <h1>
          VLAKosim<span className="accent">DB</span>
        </h1>
        <p className="tagline">3D train model browser</p>
        {manifest && <span className="total">{manifest.count} models</span>}
        <div className="topbar-actions">
          {copyStatus && <span className="copy-status">{copyStatus}</span>}
          <div className="menu-wrap">
            <button
              className="menu-button"
              onClick={() => setMenuOpen((open) => !open)}
              aria-label="Open menu"
              aria-haspopup="menu"
              aria-expanded={menuOpen}
            >
              ...
            </button>
            {menuOpen && (
              <div className="menu" role="menu">
                <button
                  role="menuitem"
                  onClick={copySelectedTrains}
                  disabled={checkedTrainLines.length === 0}
                >
                  Copy selected trains
                </button>
              </div>
            )}
          </div>
        </div>
      </header>

      {error && <div className="error">Failed to load models: {error}</div>}

      {!manifest && !error && <div className="loading">Loading model database…</div>}

      {manifest && (
        <>
          <Filters
            search={search}
            onSearch={setSearch}
            categories={categories}
            category={category}
            onCategory={handleCategory}
            resolution={resolution}
            onResolution={setResolution}
            resolutionCounts={resolutionCounts}
            checkedOnly={checkedOnly}
            onCheckedOnly={setCheckedOnly}
            checkedCount={checkedTrainIds.size}
            locomotiveTypes={locomotiveTypes}
            locomotiveType={locomotiveType}
            onLocomotiveType={handleLocomotiveType}
            resultCount={filtered.length}
          />
          <Gallery
            models={filtered}
            categoryLabels={CATEGORY_LABELS}
            onSelect={(m) => select(m.id)}
            checkedIds={checkedTrainIds}
            onCheckedChange={setTrainChecked}
          />
        </>
      )}

      {selected && (
        <Viewer
          model={selected}
          onClose={() => select(null)}
          checked={checkedTrainIds.has(selected.id)}
          onCheckedChange={(checked) => setTrainChecked(selected.id, checked)}
        />
      )}
    </div>
  );
}
