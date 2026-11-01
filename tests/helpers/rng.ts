// Small seeded generator (mulberry32) so random tests are reproducible.
export function makeRng(seed: number): () => number {
  let s = seed >>> 0;
  return () => {
    s = (s + 0x6d2b79f5) >>> 0;
    let t = s;
    t = Math.imul(t ^ (t >>> 15), t | 1);
    t ^= t + Math.imul(t ^ (t >>> 7), t | 61);
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

export function randInt(rng: () => number, lo: number, hi: number): number {
  return lo + Math.floor(rng() * (hi - lo + 1));
}

export function randArray(rng: () => number, maxLen: number, lo = -50, hi = 50): number[] {
  const n = randInt(rng, 0, maxLen);
  return Array.from({ length: n }, () => randInt(rng, lo, hi));
}
