import ModelCard from "./ModelCard.jsx";

export default function Gallery({ models, categoryLabels, onSelect, checkedIds, onCheckedChange }) {
  if (models.length === 0) {
    return <div className="empty">No models match your filters.</div>;
  }
  return (
    <div className="gallery">
      {models.map((m) => (
        <ModelCard
          key={m.id}
          model={m}
          categoryLabels={categoryLabels}
          onSelect={onSelect}
          checked={checkedIds.has(m.id)}
          onCheckedChange={onCheckedChange}
        />
      ))}
    </div>
  );
}
