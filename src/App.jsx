import { useCallback, useEffect, useMemo, useRef, useState } from "react";
import Filters from "./components/Filters.jsx";
import Gallery from "./components/Gallery.jsx";
import Viewer from "./components/Viewer.jsx";

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

export default function App() {
  const [manifest, setManifest] = useState(null);
  const [error, setError] = useState(null);
  const [search, setSearch] = useState("");
  const [category, setCategory] = useState("all");
  const [resolution, setResolution] = useState("all");
  const [selectedId, setSelectedId] = useState(selectedIdFromHash);
  const galleryScroll = useRef({ x: 0, y: 0 });

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

  const filtered = useMemo(() => {
    if (!manifest) return [];
    const q = search.trim().toLowerCase();
    return manifest.models.filter((m) => {
      if (category !== "all" && m.category !== category) return false;
      if (resolution === "hires" && !m.hires) return false;
      if (resolution === "lores" && m.hires) return false;
      if (!q) return true;
      return (
        m.title.toLowerCase().includes(q) ||
        m.name.toLowerCase().includes(q) ||
        (m.author && m.author.toLowerCase().includes(q))
      );
    });
  }, [manifest, search, category, resolution]);

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
            onCategory={setCategory}
            resolution={resolution}
            onResolution={setResolution}
            resolutionCounts={resolutionCounts}
            resultCount={filtered.length}
          />
          <Gallery
            models={filtered}
            categoryLabels={CATEGORY_LABELS}
            onSelect={(m) => select(m.id)}
          />
        </>
      )}

      {selected && <Viewer model={selected} onClose={() => select(null)} />}
    </div>
  );
}
