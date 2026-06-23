import { useEffect, useMemo, useState } from "react";
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

export default function App() {
  const [manifest, setManifest] = useState(null);
  const [error, setError] = useState(null);
  const [search, setSearch] = useState("");
  const [category, setCategory] = useState("all");
  const [selectedId, setSelectedId] = useState(() => window.location.hash.slice(2) || null);

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
    const onHash = () => setSelectedId(window.location.hash.slice(2) || null);
    window.addEventListener("hashchange", onHash);
    return () => window.removeEventListener("hashchange", onHash);
  }, []);

  const select = (id) => {
    window.location.hash = id ? `/${id}` : "";
    setSelectedId(id);
  };

  const categories = useMemo(() => {
    if (!manifest) return [];
    const counts = new Map();
    for (const m of manifest.models) counts.set(m.category, (counts.get(m.category) || 0) + 1);
    return [...counts.entries()]
      .sort((a, b) => b[1] - a[1])
      .map(([key, count]) => ({ key, count, label: CATEGORY_LABELS[key] || key }));
  }, [manifest]);

  const filtered = useMemo(() => {
    if (!manifest) return [];
    const q = search.trim().toLowerCase();
    return manifest.models.filter((m) => {
      if (category !== "all" && m.category !== category) return false;
      if (!q) return true;
      return (
        m.title.toLowerCase().includes(q) ||
        m.name.toLowerCase().includes(q) ||
        (m.author && m.author.toLowerCase().includes(q))
      );
    });
  }, [manifest, search, category]);

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
