export default function ModelCard({ model, categoryLabels, onSelect }) {
  const label = categoryLabels[model.category] || model.category;
  return (
    <button className="card" onClick={() => onSelect(model)} title={model.title}>
      <div className="thumb">
        {model.thumb ? (
          <img src={model.thumb} alt={model.title} loading="lazy" />
        ) : (
          <div className="thumb-placeholder">no preview</div>
        )}
        <span className="badge">{model.subcategory || label}</span>
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
  );
}
