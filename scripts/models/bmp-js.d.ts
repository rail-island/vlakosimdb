// Minimal ambient types for bmp-js (no published @types package).
declare module "bmp-js" {
  interface BmpDecoded {
    width: number;
    height: number;
    data: Buffer; // ABGR per pixel
  }
  export function decode(buffer: Buffer): BmpDecoded;
  const _default: { decode: typeof decode };
  export default _default;
}
