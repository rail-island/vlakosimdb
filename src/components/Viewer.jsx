import { Suspense, useCallback, useEffect, useMemo, useState } from "react";
import { Canvas } from "@react-three/fiber";
import { OrbitControls, Bounds, useGLTF, Grid, Html } from "@react-three/drei";
import * as THREE from "three";

const RAILROAD_KEY = "vlakosimdb:railroad";

// Texture-flip sides (must match scripts/lib/simplemodel/coloralg.mjs DIR_KEYS).
const FLIP_SIDES = ["x+", "x-", "y+", "y-", "z+", "z-"];
const IS_DEV = import.meta.env.DEV;

function Model({ url, onMeasure }) {
  const { scene } = useGLTF(url);
  useEffect(() => {
    const box = new THREE.Box3().setFromObject(scene);
    onMeasure({
      size: box.getSize(new THREE.Vector3()),
      center: box.getCenter(new THREE.Vector3()),
      min: box.min.clone(),
    });
  }, [scene, onMeasure]);
  return <primitive object={scene} />;
}

// Procedural railroad (rails + sleepers + gravel ballast) on a grass ground,
// sized from the loaded model's bounding box so it scales to any GLB. Models
// sit on y=0, so the rail head is placed at the box's min y (where wheels rest).
function Railroad({ bbox }) {
  const built = useMemo(() => {
    if (!bbox) return null;
    const { size, center, min } = bbox;
    const alongZ = size.z >= size.x;
    const longAxis = Math.max(size.x, size.z);
    const crossAxis = Math.min(size.x, size.z) || longAxis;
    const trackLen = longAxis * 3;
    const halfGauge = crossAxis * 0.3;
    // Vertical stack, top-down, with the rail head at the group origin (y=0 =
    // model min.y) so the loco sits exactly on the rails: rails on top of
    // sleepers, sleepers on the ballast.
    const railH = Math.max(crossAxis * 0.06, 0.06);
    const railW = halfGauge * 0.14;
    const sleeperH = Math.max(crossAxis * 0.05, 0.05);
    const ballastH = Math.max(crossAxis * 0.06, 0.05);
    const railY = -railH / 2;
    const sleeperY = -railH - sleeperH / 2;
    const ballastY = -railH - sleeperH - ballastH / 2;
    const groundY = -railH - sleeperH - ballastH - 0.02;
    const sleeperW = halfGauge * 2.2;
    const sleeperLen = halfGauge * 0.45;
    const spacing = sleeperLen * 2.2;
    const count = Math.min(Math.round(trackLen / spacing), 80);
    const sleepers = Array.from({ length: count }, (_, i) => -trackLen / 2 + (i + 0.5) * (trackLen / count));
    return {
      center, min, alongZ, trackLen, halfGauge,
      railH, railW, sleeperH, ballastH, railY, sleeperY, ballastY, groundY,
      sleeperW, sleeperLen, sleepers,
    };
  }, [bbox]);

  if (!built) return null;
  const {
    center, min, alongZ, trackLen, halfGauge,
    railH, railW, sleeperH, ballastH, railY, sleeperY, ballastY, groundY,
    sleeperW, sleeperLen, sleepers,
  } = built;
  const ground = trackLen * 1.5;

  return (
    <group position={[center.x, min.y, center.z]} rotation={[0, alongZ ? Math.PI / 2 : 0, 0]}>
      {/* Grass ground */}
      <mesh rotation={[-Math.PI / 2, 0, 0]} position={[0, groundY, 0]}>
        <planeGeometry args={[ground, ground]} />
        <meshStandardMaterial color="#3f5f3a" roughness={1} />
      </mesh>

      {/* Gravel ballast */}
      <mesh position={[0, ballastY, 0]}>
        <boxGeometry args={[trackLen, ballastH, sleeperW * 1.1]} />
        <meshStandardMaterial color="#6f6a60" roughness={1} />
      </mesh>

      {/* Sleepers */}
      {sleepers.map((x, i) => (
        <mesh key={i} position={[x, sleeperY, 0]}>
          <boxGeometry args={[sleeperLen, sleeperH, sleeperW]} />
          <meshStandardMaterial color="#4f3a28" roughness={0.9} />
        </mesh>
      ))}

      {/* Rails (on top of the sleepers; rail head at y=0 so the model sits on them) */}
      {[-halfGauge, halfGauge].map((z) => (
        <mesh key={z} position={[0, railY, z]}>
          <boxGeometry args={[trackLen, railH, railW]} />
          <meshStandardMaterial color="#9a9a9a" metalness={0.8} roughness={0.4} />
        </mesh>
      ))}
    </group>
  );
}

function Loader() {
  return (
    <Html center>
      <div className="viewer-loading">Loading model…</div>
    </Html>
  );
}

export default function Viewer({ model, onClose, checked, onCheckedChange, folderName }) {
  useEffect(() => {
    const onKey = (e) => e.key === "Escape" && onClose();
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [onClose]);

  const [showRailroad, setShowRailroad] = useState(
    () => localStorage.getItem(RAILROAD_KEY) === "1"
  );
  useEffect(() => {
    localStorage.setItem(RAILROAD_KEY, showRailroad ? "1" : "0");
  }, [showRailroad]);

  // Per-side texture flips (dev only). `ver` busts the GLB cache after a rebuild.
  const [flip, setFlip] = useState(() => model.flip || {});
  const [ver, setVer] = useState(0);
  const [saving, setSaving] = useState(false);
  useEffect(() => {
    setFlip(model.flip || {});
    setVer(0);
  }, [model.id]);

  const glbUrl = useMemo(() => (ver ? `${model.glb}?v=${ver}` : model.glb), [model.glb, ver]);

  const toggleFlip = useCallback(
    async (key, checked) => {
      const prev = flip;
      const next = { ...flip };
      if (checked) next[key] = true;
      else delete next[key];
      setFlip(next);
      setSaving(true);
      try {
        const r = await fetch("/api/model-options", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ id: model.id, flipS: next }),
        });
        if (!r.ok) throw new Error((await r.json().catch(() => ({}))).error || r.statusText);
        useGLTF.clear(glbUrl);
        setVer(Date.now()); // reload the rebuilt GLB
      } catch (e) {
        setFlip(prev); // revert on failure
        // eslint-disable-next-line no-alert
        alert("Saving flip failed: " + e.message);
      } finally {
        setSaving(false);
      }
    },
    [flip, model.id, glbUrl]
  );

  const [bbox, setBbox] = useState(null);
  const onMeasure = useCallback((b) => setBbox(b), []);
  // Re-measure when the displayed GLB changes (model switch or rebuild).
  useEffect(() => setBbox(null), [glbUrl]);

  return (
    <div className="viewer-overlay" onClick={onClose}>
      <div className="viewer" onClick={(e) => e.stopPropagation()}>
        <button className="close" onClick={onClose} aria-label="Close">
          ×
        </button>

        <div className="viewer-canvas">
          <Canvas camera={{ position: [12, 8, 14], fov: 45 }} dpr={[1, 2]}>
            <color attach="background" args={["#1a1d23"]} />
            <hemisphereLight args={["#ffffff", "#404048", 1.1]} />
            <directionalLight position={[10, 18, 8]} intensity={1.4} />
            <directionalLight position={[-8, 6, -10]} intensity={0.5} />
            <Suspense fallback={<Loader />}>
              <Bounds fit clip observe margin={1.25} key={glbUrl}>
                <Model url={glbUrl} onMeasure={onMeasure} />
              </Bounds>
            </Suspense>
            {showRailroad ? (
              <Railroad bbox={bbox} />
            ) : (
              <Grid
                args={[60, 60]}
                cellColor="#2c313a"
                sectionColor="#3a4150"
                infiniteGrid
                fadeDistance={70}
                position={[0, 0, 0]}
              />
            )}
            <OrbitControls makeDefault enableDamping />
          </Canvas>
        </div>

        <aside className="viewer-info">
          <div className="viewer-title">
            <input
              type="checkbox"
              checked={checked}
              aria-label={`Check ${model.title}`}
              onChange={(e) => onCheckedChange(e.target.checked)}
            />
            <h2>{model.title}</h2>
          </div>
          <div className="info-sub">{folderName}</div>

          <dl className="specs">
            <Spec label="Category" value={[model.category, model.subcategory].filter(Boolean).join(" / ")} />
            <Spec label="Power" value={model.power ? `${model.power} kW` : null} />
            <Spec label="Max speed" value={model.maxspeed ? `${model.maxspeed} km/h` : null} />
            <Spec label="Weight" value={model.weight ? `${model.weight} t` : null} />
            <Spec label="Length" value={model.length ? `${model.length} m` : null} />
            <Spec label="Author" value={model.author} />
            <Spec label="Geometry" value={`${model.vertices} verts · ${model.faces} tris`} />
          </dl>

          <label className="toggle">
            <input
              type="checkbox"
              checked={showRailroad}
              onChange={(e) => setShowRailroad(e.target.checked)}
            />
            Show railroad &amp; ground
          </label>

          {IS_DEV && (
            <fieldset className="flip-panel">
              <legend>Flip texture (dev){saving ? " · saving…" : ""}</legend>
              <div className="flip-grid">
                {FLIP_SIDES.map((side) => (
                  <label key={side} className="toggle flip-side">
                    <input
                      type="checkbox"
                      checked={!!flip[side]}
                      disabled={saving}
                      onChange={(e) => toggleFlip(side, e.target.checked)}
                    />
                    {side}
                  </label>
                ))}
              </div>
              <p className="hint">Mirrors that side's lettering; saved to options.json.</p>
            </fieldset>
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
