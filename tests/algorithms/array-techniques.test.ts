import * as A from "../../src/algorithms/array-techniques";
import { makeRng, randArray, randInt } from "../helpers/rng";

describe("two pointers", () => {
  it("twoSumSorted", () => {
    expect(A.twoSumSorted([1, 2, 4, 7, 11], 9)).toEqual([1, 3]);
    expect(A.twoSumSorted([1, 2, 3], 100)).toBeNull();
    expect(A.twoSumSorted([5], 10)).toBeNull();
    expect(A.twoSumSorted([], 0)).toBeNull();
    expect(A.twoSumSorted([3, 3], 6)).toEqual([0, 1]);
  });

  it("threeSum returns unique triplets and matches brute force", () => {
    expect(A.threeSum([-1, 0, 1, 2, -1, -4])).toEqual([[-1, -1, 2], [-1, 0, 1]]);
    expect(A.threeSum([0, 0, 0, 0])).toEqual([[0, 0, 0]]);
    expect(A.threeSum([])).toEqual([]);
    expect(A.threeSum([1, 2])).toEqual([]);
    const rng = makeRng(141);
    for (let t = 0; t < 150; t++) {
      const a = randArray(rng, 12, -5, 5);
      const expected = new Set<string>();
      for (let i = 0; i < a.length; i++)
        for (let j = i + 1; j < a.length; j++)
          for (let k = j + 1; k < a.length; k++)
            if ((a[i] as number) + (a[j] as number) + (a[k] as number) === 0)
              expected.add([a[i], a[j], a[k]].sort((x, y) => (x as number) - (y as number)).join(","));
      expect(new Set(A.threeSum(a).map((x) => x.join(",")))).toEqual(expected);
      expect(A.threeSum(a)).toHaveLength(expected.size);
    }
  });

  it("maxWater matches brute force", () => {
    expect(A.maxWater([1, 8, 6, 2, 5, 4, 8, 3, 7])).toBe(49);
    expect(A.maxWater([])).toBe(0);
    expect(A.maxWater([4])).toBe(0);
    const rng = makeRng(142);
    for (let t = 0; t < 150; t++) {
      const h = randArray(rng, 20, 0, 15);
      let best = 0;
      for (let i = 0; i < h.length; i++)
        for (let j = i + 1; j < h.length; j++) best = Math.max(best, Math.min(h[i] as number, h[j] as number) * (j - i));
      expect(A.maxWater(h)).toBe(best);
    }
  });
});

describe("sliding window", () => {
  it("maxSumWindow", () => {
    expect(A.maxSumWindow([2, 1, 5, 1, 3, 2], 3)).toBe(9);
    expect(A.maxSumWindow([4], 1)).toBe(4);
    expect(A.maxSumWindow([-3, -1, -2], 2)).toBe(-3);
    expect(() => A.maxSumWindow([1, 2], 3)).toThrow(RangeError);
    expect(() => A.maxSumWindow([1, 2], 0)).toThrow(RangeError);
  });

  it("maxSumWindow matches brute force", () => {
    const rng = makeRng(143);
    for (let t = 0; t < 150; t++) {
      const a = randArray(rng, 25).concat([1]);
      const k = randInt(rng, 1, a.length);
      let best = -Infinity;
      for (let i = 0; i + k <= a.length; i++) best = Math.max(best, a.slice(i, i + k).reduce((x, y) => x + y, 0));
      expect(A.maxSumWindow(a, k)).toBe(best);
    }
  });

  it("longestUniqueSubstring", () => {
    expect(A.longestUniqueSubstring("abcabcbb")).toBe(3);
    expect(A.longestUniqueSubstring("bbbbb")).toBe(1);
    expect(A.longestUniqueSubstring("pwwkew")).toBe(3);
    expect(A.longestUniqueSubstring("abba")).toBe(2);
    expect(A.longestUniqueSubstring("")).toBe(0);
  });

  it("longestUniqueSubstring matches brute force", () => {
    const rng = makeRng(144);
    for (let t = 0; t < 150; t++) {
      const s = Array.from({ length: randInt(rng, 0, 15) }, () => "abcd"[randInt(rng, 0, 3)]).join("");
      let best = 0;
      for (let i = 0; i < s.length; i++)
        for (let j = i + 1; j <= s.length; j++) if (new Set(s.slice(i, j)).size === j - i) best = Math.max(best, j - i);
      expect(A.longestUniqueSubstring(s)).toBe(best);
    }
  });

  it("minSubarrayLen", () => {
    expect(A.minSubarrayLen([2, 3, 1, 2, 4, 3], 7)).toBe(2);
    expect(A.minSubarrayLen([1, 1, 1], 100)).toBe(0);
    expect(A.minSubarrayLen([], 1)).toBe(0);
    expect(A.minSubarrayLen([9], 7)).toBe(1);
  });
});

describe("prefix sums", () => {
  it("PrefixSums answers range queries", () => {
    const p = new A.PrefixSums([1, 2, 3, 4]);
    expect(p.rangeSum(0, 3)).toBe(10);
    expect(p.rangeSum(1, 2)).toBe(5);
    expect(p.rangeSum(2, 2)).toBe(3);
    expect(() => p.rangeSum(2, 1)).toThrow(RangeError);
    expect(() => p.rangeSum(0, 4)).toThrow(RangeError);
    expect(() => new A.PrefixSums([]).rangeSum(0, 0)).toThrow(RangeError);
  });

  it("subarraySumEquals matches brute force, including zeros and negatives", () => {
    expect(A.subarraySumEquals([1, 1, 1], 2)).toBe(2);
    expect(A.subarraySumEquals([], 0)).toBe(0);
    expect(A.subarraySumEquals([0, 0, 0], 0)).toBe(6);
    const rng = makeRng(145);
    for (let t = 0; t < 150; t++) {
      const a = randArray(rng, 20, -3, 3);
      const k = randInt(rng, -4, 4);
      let count = 0;
      for (let i = 0; i < a.length; i++) {
        let s = 0;
        for (let j = i; j < a.length; j++) {
          s += a[j] as number;
          if (s === k) count++;
        }
      }
      expect(A.subarraySumEquals(a, k)).toBe(count);
    }
  });
});
