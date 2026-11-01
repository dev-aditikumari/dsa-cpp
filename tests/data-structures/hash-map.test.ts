import { HashMap } from "../../src/data-structures/hash-map";
import { makeRng, randInt } from "../helpers/rng";

describe("HashMap", () => {
  it("sets, gets, overwrites and deletes", () => {
    const m = new HashMap<string, number>();
    m.set("a", 1);
    m.set("b", 2);
    m.set("a", 3);
    expect(m.size).toBe(2);
    expect(m.get("a")).toBe(3);
    expect(m.has("b")).toBe(true);
    expect(m.get("zzz")).toBeUndefined();
    expect(m.delete("a")).toBe(true);
    expect(m.delete("a")).toBe(false);
    expect(m.size).toBe(1);
  });

  it("distinguishes 1 from '1' and treats NaN and -0 like Map", () => {
    const m = new HashMap<unknown, string>();
    m.set(1, "number");
    m.set("1", "string");
    m.set(NaN, "nan");
    m.set(0, "zero");
    expect(m.get(1)).toBe("number");
    expect(m.get("1")).toBe("string");
    expect(m.get(NaN)).toBe("nan");
    expect(m.get(-0)).toBe("zero");
  });

  it("handles collisions with a constant hash", () => {
    const m = new HashMap<number, number>(4, () => 7);
    for (let i = 0; i < 50; i++) m.set(i, i * 2);
    for (let i = 0; i < 50; i++) expect(m.get(i)).toBe(i * 2);
    expect(m.delete(25)).toBe(true);
    expect(m.delete(0)).toBe(true);
    expect(m.delete(49)).toBe(true);
    expect(m.size).toBe(47);
    expect(m.get(25)).toBeUndefined();
  });

  it("resizes when load factor is exceeded", () => {
    const m = new HashMap<number, number>(2);
    for (let i = 0; i < 100; i++) m.set(i, i);
    expect(m.capacity).toBeGreaterThan(100 / 0.75 - 1);
    expect(m.keys().sort((a, b) => a - b)).toEqual(Array.from({ length: 100 }, (_, i) => i));
  });

  it("rejects invalid capacity", () => {
    expect(() => new HashMap(0)).toThrow(RangeError);
    expect(() => new HashMap(1.5)).toThrow(RangeError);
  });

  it("matches Map under 20000 random operations", () => {
    const rng = makeRng(41);
    const m = new HashMap<number, number>();
    const ref = new Map<number, number>();
    for (let i = 0; i < 20000; i++) {
      const k = randInt(rng, 0, 300);
      const op = randInt(rng, 0, 2);
      if (op === 0) {
        m.set(k, i);
        ref.set(k, i);
      } else if (op === 1) {
        expect(m.delete(k)).toBe(ref.delete(k));
      } else {
        expect(m.get(k)).toBe(ref.get(k));
        expect(m.has(k)).toBe(ref.has(k));
      }
      expect(m.size).toBe(ref.size);
    }
    expect(new Map(m)).toEqual(ref);
  });
});
