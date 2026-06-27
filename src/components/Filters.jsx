export default function Filters({
  search,
  onSearch,
  categories,
  category,
  onCategory,
  resolution,
  onResolution,
  resolutionCounts,
  checkedOnly,
  onCheckedOnly,
  checkedCount,
  locomotiveTypes,
  locomotiveType,
  onLocomotiveType,
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
        <button
          className={`chip ${checkedOnly ? "active" : ""}`}
          onClick={() => onCheckedOnly(!checkedOnly)}
        >
          Checked <span className="chip-count">{checkedCount}</span>
        </button>
      </div>
      <div className="chips">
        <button
          className={`chip ${locomotiveType === "all" ? "active" : ""}`}
          onClick={() => onLocomotiveType("all")}
        >
          All locomotive types
        </button>
        {locomotiveTypes.map((type) => (
          <button
            key={type.key}
            className={`chip ${locomotiveType === type.key ? "active" : ""}`}
            onClick={() => onLocomotiveType(type.key)}
          >
            {type.label} <span className="chip-count">{type.count}</span>
          </button>
        ))}
      </div>
      <div className="result-count">{resultCount} shown</div>
    </div>
  );
}
