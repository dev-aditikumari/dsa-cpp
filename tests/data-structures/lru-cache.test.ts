import { LRUCache } from "../../src/data-structures/lru-cache";
import { makeRng, randInt } from "../helpers/rng";

describe("LRUCache", () => {
  it("evicts the least recently used entry", () => {
    const c = new LRUCache<string, number>(2);
    c.put("a", 1);
    c.put("b", 2);
    expect(c.get("a")).toBe(1);
    c.put("c", 3);
    expect(c.has("b")).toBe(false);
    expect(c.keys()).toEqual(["c", "a"]);
  });

  it("updating an existing key refreshes it without growing", () => {
    const c = new LRUCache<string, number>(2);
    c.put("a", 1);
    c.put("b", 2);
    c.put("a", 10);
    c.put("c", 3);
    expect(c.get("a")).toBe(10);
    expect(c.get("b")).toBeUndefined();
    expect(c.size).toBe(2);
  });

  it("works with capacity 1", () => {
    const c = new LRUCache<number, number>(1);
    c.put(1, 1);
    c.put(2, 2);
    expect(c.get(1)).toBeUndefined();
    expect(c.get(2)).toBe(2);
  });

  it("rejects invalid capacity", () => {
    expect(() => new LRUCache(0)).toThrow(RangeError);
    expect(() => new LRUCache(2.5)).toThrow(RangeError);
  });

  it("matches a Map-based reference under random operations", () => {
    const rng = makeRng(71);
    const cap = 7;
    const c = new LRUCache<number, number>(cap);
    const ref = new Map<number, number>(); // insertion order = recency, oldest first
    for (let i = 0; i < 10000; i++) {
      const k = randInt(rng, 0, 20);
      if (randInt(rng, 0, 1) === 0) {
        c.put(k, i);
        ref.delete(k);
        ref.set(k, i);
        if (ref.size > cap) ref.delete(ref.keys().next().value as number);
      } else {
        const expected = ref.get(k);
        expect(c.get(k)).toBe(expected);
        if (expected !== undefined) {
          ref.delete(k);
          ref.set(k, expected);
        }
      }
      expect(c.keys()).toEqual([...ref.keys()].reverse());
    }
  });
});
