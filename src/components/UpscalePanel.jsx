import { useEffect, useRef, useState } from "react";

// Localhost-only 4x upscale control. Streams logs from the dev server's
// /api/upscale (Server-Sent Events) and, when done, shows before/after previews
// and asks the viewer to reload the rebuilt GLB. Rendered only when
// import.meta.env.DEV is true, so it never appears on the deployed site.
export default function UpscalePanel({ model, onDone }) {
  const [status, setStatus] = useState("idle"); // idle | running | done | error
  const [logs, setLogs] = useState([]);
  const [result, setResult] = useState(null);
  const [error, setError] = useState(null);
  const [dryRun, setDryRun] = useState(false);
  const esRef = useRef(null);
  const logEndRef = useRef(null);

  useEffect(() => () => esRef.current?.close(), []);
  useEffect(() => { logEndRef.current?.scrollIntoView({ block: "end" }); }, [logs]);

  const start = () => {
    setStatus("running");
    setLogs([]);
    setResult(null);
    setError(null);
    const es = new EventSource(`/api/upscale?id=${encodeURIComponent(model.id)}${dryRun ? "&dry=1" : ""}`);
    esRef.current = es;
    es.addEventListener("log", (e) => {
      const { message } = JSON.parse(e.data);
      setLogs((l) => [...l, message]);
    });
    es.addEventListener("done", (e) => {
      const data = JSON.parse(e.data);
      setResult(data);
      setStatus("done");
      es.close();
      onDone?.(data);
    });
    es.addEventListener("error", (e) => {
      const msg = e.data ? JSON.parse(e.data).message : "connection lost";
      setError(msg);
      setStatus("error");
      es.close();
    });
  };

  return (
    <div className="upscale">
      <div className="upscale-head">
        <button className="upscale-btn" onClick={start} disabled={status === "running"}>
          {status === "running" ? "Upscaling…" : "Upscale 4×"}
        </button>
        <label className="upscale-dry">
          <input type="checkbox" checked={dryRun} disabled={status === "running"}
            onChange={(e) => setDryRun(e.target.checked)} />
          dry run
        </label>
      </div>

      {(status === "running" || logs.length > 0) && (
        <pre className="upscale-log">
          {logs.map((l, i) => <div key={i}>{l}</div>)}
          <div ref={logEndRef} />
        </pre>
      )}

      {error && <div className="upscale-error">Error: {error}</div>}

      {result && (
        <div className="upscale-result">
          <div className="upscale-meta">
            {result.tiles} textures{result.cost ? ` · ~$${result.cost.toFixed(4)}` : ""}
            {result.hires ? " · HiRes" : ""}
          </div>
          <div className="upscale-previews">
            <figure><img src={result.before} alt="before" /><figcaption>raw 4×</figcaption></figure>
            <figure><img src={result.after} alt="after" /><figcaption>AI 4×</figcaption></figure>
          </div>
        </div>
      )}
    </div>
  );
}
