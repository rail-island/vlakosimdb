export default function Filters({
  search,
  onSearch,
  categories,
  category,
  onCategory,
  resolution,
  onResolution,
  resolutionCounts,
  resultCount,
}) {
  return (
    <div className="filters">
      <input
        className="search"
        type="search"
        placeholder="Search by name or author…"
        value={search}
        onChange={(e) => onSearch(e.target.value)}
      />
      <div className="chips">
        <button
          className={`chip ${category === "all" ? "active" : ""}`}
          onClick={() => onCategory("all")}
        >
          All
        </button>
        {categories.map((c) => (
          <button
            key={c.key}
            className={`chip ${category === c.key ? "active" : ""}`}
            onClick={() => onCategory(c.key)}
          >
            {c.label} <span className="chip-count">{c.count}</span>
          </button>
        ))}
      </div>
      <div className="chips">
        <button
          className={`chip ${resolution === "all" ? "active" : ""}`}
          onClick={() => onResolution("all")}
        >
          All textures
        </button>
        <button
          className={`chip ${resolution === "hires" ? "active" : ""}`}
          onClick={() => onResolution("hires")}
        >
          HiRes <span className="chip-count">{resolutionCounts.hires}</span>
        </button>
        <button
          className={`chip ${resolution === "lores" ? "active" : ""}`}
          onClick={() => onResolution("lores")}
        >
          LoRes <span className="chip-count">{resolutionCounts.lores}</span>
        </button>
      </div>
      <div className="result-count">{resultCount} shown</div>
    </div>
  );
}
