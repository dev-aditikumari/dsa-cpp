import * as D from "../../src/algorithms/dynamic-programming";
import { makeRng, randInt } from "../helpers/rng";

describe("fibonacci", () => {
  it("memo and tabulation agree and handle edges", () => {
    expect(D.fibTab(0)).toBe(0n);
    expect(D.fibTab(1)).toBe(1n);
    expect(D.fibTab(10)).toBe(55n);
    expect(D.fibTab(90)).toBe(2880067194370816120n);
    for (let n = 0; n <= 200; n++) expect(D.fibMemo(n)).toBe(D.fibTab(n));
    expect(D.fibMemo(1000)).toBe(D.fibTab(1000));
  });

  it("reject invalid n", () => {
    expect(() => D.fibMemo(-1)).toThrow(RangeError);
    expect(() => D.fibTab(1.5)).toThrow(RangeError);
  });
});

describe("knapsack01", () => {
  it("solves a known case and edge cases", () => {
    expect(D.knapsack01([1, 3, 4, 5], [1, 4, 5, 7], 7)).toBe(9);
    expect(D.knapsack01([], [], 10)).toBe(0);
    expect(D.knapsack01([5], [10], 4)).toBe(0);
    expect(D.knapsack01([5], [10], 5)).toBe(10);
    expect(D.knapsack01([1, 2], [3, 4], 0)).toBe(0);
  });

  it("rejects bad input", () => {
    expect(() => D.knapsack01([1], [1, 2], 3)).toThrow();
    expect(() => D.knapsack01([1], [1], -1)).toThrow(RangeError);
    expect(() => D.knapsack01([-1], [1], 3)).toThrow(RangeError);
  });

  it("matches brute force over all subsets on random instances", () => {
    const rng = makeRng(131);
    for (let t = 0; t < 150; t++) {
      const n = randInt(rng, 0, 9);
      const w = Array.from({ length: n }, () => randInt(rng, 0, 8));
      const v = Array.from({ length: n }, () => randInt(rng, 0, 20));
      const cap = randInt(rng, 0, 20);
      let best = 0;
      for (let mask = 0; mask < 1 << n; mask++) {
        let tw = 0;
        let tv = 0;
        for (let i = 0; i < n; i++) if (mask & (1 << i)) {
          tw += w[i] as number;
          tv += v[i] as number;
        }
        if (tw <= cap) best = Math.max(best, tv);
      }
      expect(D.knapsack01(w, v, cap)).toBe(best);
    }
  });
});

describe("lcs", () => {
  it("returns a valid longest common subsequence", () => {
    expect(D.lcs("abcde", "ace")).toBe("ace");
    expect(D.lcs("", "abc")).toBe("");
    expect(D.lcs("abc", "xyz")).toBe("");
    expect(D.lcs("same", "same")).toBe("same");
  });

  it("is a subsequence of both and has maximal length (brute force) on random strings", () => {
    const isSub = (s: string, t: string) => {
      let i = 0;
      for (const ch of t) if (ch === s[i]) i++;
      return i === s.length;
    };
    const rng = makeRng(132);
    const rs = (n: number) => Array.from({ length: n }, () => "abc"[randInt(rng, 0, 2)]).join("");
    for (let t = 0; t < 150; t++) {
      const a = rs(randInt(rng, 0, 8));
      const b = rs(randInt(rng, 0, 8));
      const out = D.lcs(a, b);
      expect(isSub(out, a)).toBe(true);
      expect(isSub(out, b)).toBe(true);
      let best = 0;
      for (let mask = 0; mask < 1 << a.length; mask++) {
        const sub = [...a].filter((_, i) => mask & (1 << i)).join("");
        if (sub.length > best && isSub(sub, b)) best = sub.length;
      }
      expect(out.length).toBe(best);
    }
  });
});

describe("lisLength", () => {
  it("handles known cases", () => {
    expect(D.lisLength([10, 9, 2, 5, 3, 7, 101, 18])).toBe(4);
    expect(D.lisLength([])).toBe(0);
    expect(D.lisLength([5])).toBe(1);
    expect(D.lisLength([2, 2, 2])).toBe(1);
    expect(D.lisLength([5, 4, 3])).toBe(1);
  });

  it("matches the O(n^2) DP on random arrays", () => {
    const rng = makeRng(133);
    for (let t = 0; t < 200; t++) {
      const a = Array.from({ length: randInt(rng, 0, 40) }, () => randInt(rng, 0, 20));
      const dp = a.map(() => 1);
      for (let i = 0; i < a.length; i++)
        for (let j = 0; j < i; j++) if ((a[j] as number) < (a[i] as number)) dp[i] = Math.max(dp[i] as number, (dp[j] as number) + 1);
      expect(D.lisLength(a)).toBe(a.length === 0 ? 0 : Math.max(...dp));
    }
  });
});

describe("editDistance", () => {
  it("known values", () => {
    expect(D.editDistance("kitten", "sitting")).toBe(3);
    expect(D.editDistance("", "abc")).toBe(3);
    expect(D.editDistance("abc", "")).toBe(3);
    expect(D.editDistance("", "")).toBe(0);
    expect(D.editDistance("same", "same")).toBe(0);
  });

  it("is symmetric and obeys the triangle inequality on random strings", () => {
    const rng = makeRng(134);
    const rs = () => Array.from({ length: randInt(rng, 0, 7) }, () => "ab"[randInt(rng, 0, 1)]).join("");
    for (let t = 0; t < 200; t++) {
      const a = rs();
      const b = rs();
      const c = rs();
      expect(D.editDistance(a, b)).toBe(D.editDistance(b, a));
      expect(D.editDistance(a, c)).toBeLessThanOrEqual(D.editDistance(a, b) + D.editDistance(b, c));
      expect(D.editDistance(a, b)).toBeLessThanOrEqual(Math.max(a.length, b.length));
      expect(D.editDistance(a, b)).toBeGreaterThanOrEqual(Math.abs(a.length - b.length));
    }
  });
});

describe("coin change", () => {
  it("min coins", () => {
    expect(D.coinChangeMin([1, 2, 5], 11)).toBe(3);
    expect(D.coinChangeMin([2], 3)).toBe(-1);
    expect(D.coinChangeMin([1], 0)).toBe(0);
    expect(D.coinChangeMin([], 5)).toBe(-1);
    expect(D.coinChangeMin([186, 419, 83, 408], 6249)).toBe(20);
    expect(() => D.coinChangeMin([1], -1)).toThrow(RangeError);
  });

  it("number of ways", () => {
    expect(D.coinChangeWays([1, 2, 5], 5)).toBe(4);
    expect(D.coinChangeWays([2], 3)).toBe(0);
    expect(D.coinChangeWays([], 0)).toBe(1);
    expect(D.coinChangeWays([1, 2, 5], 0)).toBe(1);
    expect(D.coinChangeWays([0, 1], 3)).toBe(1);
    expect(() => D.coinChangeWays([1], 1.5)).toThrow(RangeError);
  });

  it("min coins matches brute-force recursion on random small cases", () => {
    const rng = makeRng(135);
    for (let t = 0; t < 150; t++) {
      const coins = Array.from({ length: randInt(rng, 1, 4) }, () => randInt(rng, 1, 7));
      const amount = randInt(rng, 0, 25);
      const brute = (rem: number): number =>
        rem === 0 ? 0 : Math.min(Infinity, ...coins.filter((c) => c <= rem).map((c) => brute(rem - c) + 1));
      const b = brute(amount);
      expect(D.coinChangeMin(coins, amount)).toBe(b === Infinity ? -1 : b);
    }
  });
});

describe("uniquePaths", () => {
  it("counts paths with and without obstacles", () => {
    expect(D.uniquePaths([[0, 0, 0], [0, 0, 0], [0, 0, 0]])).toBe(6);
    expect(D.uniquePaths([[0, 0, 0], [0, 1, 0], [0, 0, 0]])).toBe(2);
    expect(D.uniquePaths([[1]])).toBe(0);
    expect(D.uniquePaths([[0]])).toBe(1);
    expect(D.uniquePaths([])).toBe(0);
    expect(D.uniquePaths([[0, 1]])).toBe(0);
    expect(D.uniquePaths(Array.from({ length: 10 }, () => new Array<number>(10).fill(0)))).toBe(48620);
  });

  it("rejects ragged grids", () => {
    expect(() => D.uniquePaths([[0, 0], [0]])).toThrow();
  });
});

describe("matrixChainOrder", () => {
  it("known values", () => {
    expect(D.matrixChainOrder([10, 30, 5, 60])).toBe(4500);
    expect(D.matrixChainOrder([40, 20, 30, 10, 30])).toBe(26000);
    expect(D.matrixChainOrder([5, 10])).toBe(0);
    expect(D.matrixChainOrder([5])).toBe(0);
    expect(D.matrixChainOrder([])).toBe(0);
  });

  it("matches exhaustive parenthesisation on random chains", () => {
    const rng = makeRng(136);
    const brute = (d: number[], i: number, j: number): number => {
      if (i === j) return 0;
      let best = Infinity;
      for (let k = i; k < j; k++) {
        best = Math.min(best, brute(d, i, k) + brute(d, k + 1, j) + (d[i] as number) * (d[k + 1] as number) * (d[j + 1] as number));
      }
      return best;
    };
    for (let t = 0; t < 100; t++) {
      const d = Array.from({ length: randInt(rng, 2, 8) }, () => randInt(rng, 1, 12));
      expect(D.matrixChainOrder(d)).toBe(brute(d, 0, d.length - 2));
    }
  });
});
