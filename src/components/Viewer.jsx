import { Suspense, useEffect, useState } from "react";
import { Canvas } from "@react-three/fiber";
import { OrbitControls, Bounds, useGLTF, Grid, Html } from "@react-three/drei";
import UpscalePanel from "./UpscalePanel.jsx";

function Model({ url }) {
  const { scene } = useGLTF(url);
  return <primitive object={scene} />;
}

function Loader() {
  return (
    <Html center>
      <div className="viewer-loading">Loading model…</div>
    </Html>
  );
}

export default function Viewer({ model, onClose }) {
  useEffect(() => {
    const onKey = (e) => e.key === "Escape" && onClose();
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [onClose]);

  // Cache-busting version so a freshly rebuilt GLB reloads after upscaling.
  const [version, setVersion] = useState(0);
  const url = version ? `${model.glb}?t=${version}` : model.glb;

  return (
    <div className="viewer-overlay" onClick={onClose}>
      <div className="viewer" onClick={(e) => e.stopPropagation()}>
        <div className="viewer-canvas">
          <Canvas camera={{ position: [12, 8, 14], fov: 45 }} dpr={[1, 2]}>
            <color attach="background" args={["#1a1d23"]} />
            <hemisphereLight args={["#ffffff", "#404048", 1.1]} />
            <directionalLight position={[10, 18, 8]} intensity={1.4} />
            <directionalLight position={[-8, 6, -10]} intensity={0.5} />
            <Suspense fallback={<Loader />}>
              <Bounds fit clip observe margin={1.25} key={url}>
                <Model url={url} />
              </Bounds>
            </Suspense>
            <Grid
              args={[60, 60]}
              cellColor="#2c313a"
              sectionColor="#3a4150"
              infiniteGrid
              fadeDistance={70}
              position={[0, 0, 0]}
            />
            <OrbitControls makeDefault enableDamping />
          </Canvas>
        </div>

        <aside className="viewer-info">
          <button className="close" onClick={onClose} aria-label="Close">
            ×
          </button>
          <h2>{model.title}</h2>
          <div className="info-sub">{model.name}</div>

          <dl className="specs">
            <Spec label="Category" value={[model.category, model.subcategory].filter(Boolean).join(" / ")} />
            <Spec label="Power" value={model.power ? `${model.power} kW` : null} />
            <Spec label="Max speed" value={model.maxspeed ? `${model.maxspeed} km/h` : null} />
            <Spec label="Weight" value={model.weight ? `${model.weight} t` : null} />
            <Spec label="Length" value={model.length ? `${model.length} m` : null} />
            <Spec label="Author" value={model.author} />
            <Spec label="Geometry" value={`${model.vertices} verts · ${model.faces} tris`} />
          </dl>

          {import.meta.env.DEV && (
            <UpscalePanel model={model} onDone={() => setVersion(Date.now())} />
          )}

          <p className="hint">Drag to orbit · scroll to zoom · Esc to close</p>
        </aside>
      </div>
    </div>
  );
}

function Spec({ label, value }) {
  if (!value) return null;
  return (
    <>
      <dt>{label}</dt>
      <dd>{value}</dd>
    </>
  );
}
