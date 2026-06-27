export default function ModelCard({ model, categoryLabels, onSelect, checked, onCheckedChange }) {
  const label = categoryLabels[model.category] || model.category;
  return (
    <article className={`card ${checked ? "checked" : ""}`}>
      <label className="card-check">
        <input
          type="checkbox"
          checked={checked}
          aria-label={`Check ${model.title}`}
          onChange={(e) => onCheckedChange(model.id, e.target.checked)}
        />
      </label>
      <button className="card-open" onClick={() => onSelect(model)} title={model.title}>
        <div className="thumb">
          {model.thumb ? (
            <img src={model.thumb} alt={model.title} loading="lazy" />
          ) : (
            <div className="thumb-placeholder">no preview</div>
          )}
          <span className="badge">{model.subcategory || label}</span>
          {model.hires && <span className="badge badge-hires">HiRes</span>}
        </div>
        <div className="card-body">
          <div className="card-title">{model.title}</div>
          <div className="card-specs">
            {model.power ? <span>{model.power} kW</span> : null}
            {model.maxspeed ? <span>{model.maxspeed} km/h</span> : null}
            {model.length ? <span>{model.length} m</span> : null}
          </div>
        </div>
      </button>
    </article>
  );
}
