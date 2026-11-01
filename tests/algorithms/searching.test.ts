import * as B from "../../src/algorithms/searching";
import { makeRng, randArray, randInt } from "../helpers/rng";

describe("binarySearch", () => {
  it("finds present and absent values", () => {
    const a = [1, 3, 5, 7, 9];
    expect(B.binarySearch(a, 7)).toBe(3);
    expect(B.binarySearch(a, 1)).toBe(0);
    expect(B.binarySearch(a, 9)).toBe(4);
    expect(B.binarySearch(a, 4)).toBe(-1);
    expect(B.binarySearch([], 1)).toBe(-1);
    expect(B.binarySearch(["a", "c"], "c")).toBe(1);
  });
});

describe("bounds", () => {
  it("lower/upper bound with duplicates", () => {
    const a = [1, 2, 2, 2, 5];
    expect(B.lowerBound(a, 2)).toBe(1);
    expect(B.upperBound(a, 2)).toBe(4);
    expect(B.lowerBound(a, 0)).toBe(0);
    expect(B.upperBound(a, 9)).toBe(5);
    expect(B.lowerBound(a, 3)).toBe(4);
    expect(B.countOccurrences(a, 2)).toBe(3);
    expect(B.countOccurrences(a, 4)).toBe(0);
    expect(B.lowerBound([], 1)).toBe(0);
  });

  it("match a linear scan on 200 random sorted arrays", () => {
    const rng = makeRng(111);
    for (let t = 0; t < 200; t++) {
      const a = randArray(rng, 40, 0, 15).sort((x, y) => x - y);
      const target = randInt(rng, -1, 16);
      expect(B.lowerBound(a, target)).toBe(a.filter((x) => x < target).length);
      expect(B.upperBound(a, target)).toBe(a.filter((x) => x <= target).length);
      const idx = B.binarySearch(a, target);
      if (a.includes(target)) expect(a[idx]).toBe(target);
      else expect(idx).toBe(-1);
    }
  });
});

describe("rotated arrays", () => {
  it("searches and finds the rotation point", () => {
    const a = [4, 5, 6, 7, 0, 1, 2];
    expect(B.searchRotated(a, 0)).toBe(4);
    expect(B.searchRotated(a, 6)).toBe(2);
    expect(B.searchRotated(a, 3)).toBe(-1);
    expect(B.searchRotated([], 1)).toBe(-1);
    expect(B.searchRotated([1], 1)).toBe(0);
    expect(B.findRotationPoint(a)).toBe(4);
    expect(B.findRotationPoint([1, 2, 3])).toBe(0);
    expect(B.findRotationPoint([9])).toBe(0);
    expect(() => B.findRotationPoint([])).toThrow("empty");
  });

  it("searchRotated handles duplicates", () => {
    expect(B.searchRotated([1, 1, 1, 3, 1], 3)).toBe(3);
    expect(B.searchRotated([2, 2, 2, 0, 2], 0)).toBe(3);
    expect(B.searchRotated([1, 1, 1], 2)).toBe(-1);
  });

  it("agrees with indexOf on random rotations of distinct values", () => {
    const rng = makeRng(112);
    for (let t = 0; t < 200; t++) {
      const n = randInt(rng, 1, 30);
      const base = Array.from({ length: n }, (_, i) => i * 2);
      const r = randInt(rng, 0, n - 1);
      const a = [...base.slice(r), ...base.slice(0, r)];
      expect(B.findRotationPoint(a)).toBe(a.indexOf(0));
      for (let target = -1; target <= n * 2; target++) {
        expect(B.searchRotated(a, target)).toBe(a.indexOf(target));
      }
    }
  });
});

describe("search on answer", () => {
  it("finds the smallest satisfying integer", () => {
    expect(B.searchOnAnswer(0, 100, (x) => x * x >= 50)).toBe(8);
    expect(B.searchOnAnswer(0, 10, () => false)).toBe(11);
    expect(B.searchOnAnswer(0, 10, () => true)).toBe(0);
    expect(() => B.searchOnAnswer(0.5, 3, () => true)).toThrow(TypeError);
  });

  it("integerSqrt matches Math.floor(Math.sqrt) for 0..2000 and big values", () => {
    for (let x = 0; x <= 2000; x++) expect(B.integerSqrt(x)).toBe(Math.floor(Math.sqrt(x)));
    expect(B.integerSqrt(2 ** 52)).toBe(2 ** 26);
    expect(() => B.integerSqrt(-1)).toThrow(RangeError);
  });

  it("shipWithinDays matches brute force", () => {
    expect(B.shipWithinDays([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 5)).toBe(15);
    expect(B.shipWithinDays([], 3)).toBe(0);
    expect(() => B.shipWithinDays([1], 0)).toThrow(RangeError);
    const rng = makeRng(113);
    for (let t = 0; t < 100; t++) {
      const w = Array.from({ length: randInt(rng, 1, 8) }, () => randInt(rng, 1, 10));
      const days = randInt(rng, 1, w.length);
      let brute = Math.max(...w);
      const feasible = (cap: number) => {
        let used = 1;
        let load = 0;
        for (const x of w) {
          if (load + x > cap) {
            used++;
            load = 0;
          }
          load += x;
        }
        return used <= days;
      };
      while (!feasible(brute)) brute++;
      expect(B.shipWithinDays(w, days)).toBe(brute);
    }
  });
});
