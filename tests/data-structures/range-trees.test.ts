import { FenwickTree } from "../../src/data-structures/fenwick-tree";
import { SegmentTree } from "../../src/data-structures/segment-tree";
import { makeRng, randArray, randInt } from "../helpers/rng";

const naiveSum = (a: number[], l: number, r: number) => a.slice(l, r + 1).reduce((x, y) => x + y, 0);

describe("FenwickTree", () => {
  it("answers prefix and range sums", () => {
    const f = new FenwickTree([1, 2, 3, 4, 5]);
    expect(f.prefixSum(0)).toBe(1);
    expect(f.prefixSum(4)).toBe(15);
    expect(f.prefixSum(-1)).toBe(0);
    expect(f.rangeSum(1, 3)).toBe(9);
    f.add(2, 10);
    expect(f.rangeSum(2, 2)).toBe(13);
  });

  it("rejects bad indices and empty input is size 0", () => {
    const f = new FenwickTree([1]);
    expect(() => f.prefixSum(1)).toThrow(RangeError);
    expect(() => f.add(-2, 1)).toThrow(RangeError);
    expect(() => f.rangeSum(1, 0)).toThrow(RangeError);
    expect(new FenwickTree([]).size).toBe(0);
  });

  it("matches naive sums under random updates", () => {
    const rng = makeRng(81);
    const arr = randArray(rng, 80).concat([1]);
    const f = new FenwickTree(arr);
    for (let i = 0; i < 1000; i++) {
      const idx = randInt(rng, 0, arr.length - 1);
      const d = randInt(rng, -20, 20);
      arr[idx] = (arr[idx] as number) + d;
      f.add(idx, d);
      const l = randInt(rng, 0, arr.length - 1);
      const r = randInt(rng, l, arr.length - 1);
      expect(f.rangeSum(l, r)).toBe(naiveSum(arr, l, r));
    }
  });
});

describe("SegmentTree", () => {
  it("supports min queries and point updates", () => {
    const s = new SegmentTree([5, 2, 8, 1, 9], Math.min, Infinity);
    expect(s.query(0, 4)).toBe(1);
    expect(s.query(0, 2)).toBe(2);
    s.set(3, 7);
    expect(s.query(2, 4)).toBe(7);
    expect(s.size).toBe(5);
  });

  it("preserves order for non-commutative operations", () => {
    const s = new SegmentTree(["a", "b", "c", "d", "e"], (x, y) => x + y, "");
    expect(s.query(1, 3)).toBe("bcd");
    expect(s.query(0, 4)).toBe("abcde");
    s.set(2, "X");
    expect(s.query(0, 4)).toBe("abXde");
  });

  it("rejects bad ranges", () => {
    const s = new SegmentTree([1, 2], (a, b) => a + b, 0);
    expect(() => s.query(1, 0)).toThrow(RangeError);
    expect(() => s.query(0, 2)).toThrow(RangeError);
    expect(() => s.set(-1, 0)).toThrow(RangeError);
  });

  it("matches naive max and sum for random arrays of odd and even size", () => {
    const rng = makeRng(82);
    for (let t = 0; t < 100; t++) {
      const arr = randArray(rng, 40).concat([0]);
      const mx = new SegmentTree(arr, Math.max, -Infinity);
      const sm = new SegmentTree(arr, (a, b) => a + b, 0);
      for (let q = 0; q < 20; q++) {
        const i = randInt(rng, 0, arr.length - 1);
        const v = randInt(rng, -50, 50);
        arr[i] = v;
        mx.set(i, v);
        sm.set(i, v);
        const l = randInt(rng, 0, arr.length - 1);
        const r = randInt(rng, l, arr.length - 1);
        expect(mx.query(l, r)).toBe(Math.max(...arr.slice(l, r + 1)));
        expect(sm.query(l, r)).toBe(naiveSum(arr, l, r));
      }
    }
  });
});
