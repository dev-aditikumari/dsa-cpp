import * as K from "../../src/algorithms/backtracking";

const factorial = (n: number): number => (n <= 1 ? 1 : n * factorial(n - 1));

describe("permutations", () => {
  it("generates n! distinct arrangements", () => {
    const p = K.permutations([1, 2, 3]);
    expect(p).toHaveLength(6);
    expect(new Set(p.map((x) => x.join(","))).size).toBe(6);
    expect(K.permutations([])).toEqual([[]]);
    expect(K.permutations(["x"])).toEqual([["x"]]);
    expect(K.permutations([1, 2, 3, 4, 5, 6])).toHaveLength(factorial(6));
  });
});

describe("subsets and combinations", () => {
  it("subsets includes empty and full set", () => {
    const s = K.subsets([1, 2, 3]);
    expect(s).toHaveLength(8);
    expect(s).toContainEqual([]);
    expect(s).toContainEqual([1, 2, 3]);
    expect(K.subsets([])).toEqual([[]]);
  });

  it("subsets skips duplicates", () => {
    const s = K.subsets([2, 1, 2]).map((x) => x.join(","));
    expect(s.sort()).toEqual(["", "1", "1,2", "1,2,2", "2", "2,2"]);
  });

  it("combinations of 1..n choose k", () => {
    expect(K.combinations(4, 2)).toEqual([[1, 2], [1, 3], [1, 4], [2, 3], [2, 4], [3, 4]]);
    expect(K.combinations(3, 0)).toEqual([[]]);
    expect(K.combinations(2, 3)).toEqual([]);
    expect(K.combinations(10, 5)).toHaveLength(252);
    expect(() => K.combinations(-1, 1)).toThrow(RangeError);
  });
});

describe("solveNQueens", () => {
  it.each([[0, 1], [1, 1], [2, 0], [3, 0], [4, 2], [5, 10], [6, 4], [8, 92]])("n=%i has %i solutions", (n, count) => {
    expect(K.solveNQueens(n)).toHaveLength(count);
  });

  it("every n=8 solution is valid", () => {
    for (const sol of K.solveNQueens(8)) {
      for (let i = 0; i < 8; i++)
        for (let j = i + 1; j < 8; j++) {
          const a = sol[i] as number;
          const b = sol[j] as number;
          expect(a !== b && Math.abs(a - b) !== j - i).toBe(true);
        }
    }
  });

  it("rejects invalid n", () => {
    expect(() => K.solveNQueens(-1)).toThrow(RangeError);
  });
});

describe("solveSudoku", () => {
  const puzzle = [
    "530070000", "600195000", "098000060", "800060003", "400803001", "700020006", "060000280", "000419005", "000080079",
  ].map((r) => [...r].map(Number));

  it("solves a standard puzzle with valid rows, columns and boxes", () => {
    const board = puzzle.map((r) => [...r]);
    expect(K.solveSudoku(board)).toBe(true);
    const full = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    for (let i = 0; i < 9; i++) {
      expect([...(board[i] as number[])].sort()).toEqual(full);
      expect(board.map((r) => r[i] as number).sort()).toEqual(full);
      const br = Math.floor(i / 3) * 3;
      const bc = (i % 3) * 3;
      const box: number[] = [];
      for (let r = 0; r < 3; r++) for (let c = 0; c < 3; c++) box.push((board[br + r] as number[])[bc + c] as number);
      expect(box.sort()).toEqual(full);
    }
    for (let r = 0; r < 9; r++)
      for (let c = 0; c < 9; c++) if ((puzzle[r] as number[])[c] !== 0) expect((board[r] as number[])[c]).toBe((puzzle[r] as number[])[c]);
  });

  it("returns false for an unsolvable board and throws on a bad shape", () => {
    const bad = puzzle.map((r) => [...r]);
    (bad[0] as number[])[2] = 5;
    (bad[0] as number[])[1] = 0;
    (bad[0] as number[])[0] = 5;
    expect(K.solveSudoku(bad)).toBe(false);
    expect(() => K.solveSudoku([[1]])).toThrow("9x9");
  });
});
