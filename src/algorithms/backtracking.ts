// O(n * n!) time, O(n) space besides output. All permutations of the items (duplicates in the input yield duplicate permutations).
export function permutations<T>(items: readonly T[]): T[][] {
  const out: T[][] = [];
  const used = new Array<boolean>(items.length).fill(false);
  const cur: T[] = [];
  const go = (): void => {
    if (cur.length === items.length) {
      out.push([...cur]);
      return;
    }
    for (let i = 0; i < items.length; i++) {
      if (used[i]) continue;
      used[i] = true;
      cur.push(items[i] as T);
      go();
      cur.pop();
      used[i] = false;
    }
  };
  go();
  return out;
}

// O(n * 2^n) time, O(n) space besides output. All subsets, skipping repeats when the input contains duplicates.
export function subsets(input: readonly number[]): number[][] {
  const a = [...input].sort((x, y) => x - y);
  const out: number[][] = [];
  const cur: number[] = [];
  const go = (start: number): void => {
    out.push([...cur]);
    for (let i = start; i < a.length; i++) {
      if (i > start && a[i] === a[i - 1]) continue;
      cur.push(a[i] as number);
      go(i + 1);
      cur.pop();
    }
  };
  go(0);
  return out;
}

// O(n * C(n, k)) time, O(k) space besides output. All k-element combinations of 1..n.
export function combinations(n: number, k: number): number[][] {
  if (!Number.isInteger(n) || !Number.isInteger(k) || n < 0 || k < 0) throw new RangeError("n and k must be non-negative integers");
  const out: number[][] = [];
  const cur: number[] = [];
  const go = (start: number): void => {
    if (cur.length === k) {
      out.push([...cur]);
      return;
    }
    for (let i = start; i <= n - (k - cur.length) + 1; i++) {
      cur.push(i);
      go(i + 1);
      cur.pop();
    }
  };
  go(1);
  return out;
}

// O(n!) time worst case, O(n) space. All solutions to n-queens; each solution lists the column of the queen in each row.
export function solveNQueens(n: number): number[][] {
  if (!Number.isInteger(n) || n < 0) throw new RangeError("n must be a non-negative integer");
  const out: number[][] = [];
  const cols = new Set<number>();
  const diag1 = new Set<number>();
  const diag2 = new Set<number>();
  const placement: number[] = [];
  const go = (row: number): void => {
    if (row === n) {
      out.push([...placement]);
      return;
    }
    for (let c = 0; c < n; c++) {
      if (cols.has(c) || diag1.has(row - c) || diag2.has(row + c)) continue;
      cols.add(c);
      diag1.add(row - c);
      diag2.add(row + c);
      placement.push(c);
      go(row + 1);
      placement.pop();
      cols.delete(c);
      diag1.delete(row - c);
      diag2.delete(row + c);
    }
  };
  go(0);
  return out;
}

// Exponential worst case, O(81) space. Solves a 9x9 sudoku in place (0 marks an empty cell); returns false if unsolvable.
export function solveSudoku(board: number[][]): boolean {
  if (board.length !== 9 || board.some((r) => r.length !== 9)) throw new Error("Board must be 9x9");
  const ok = (r: number, c: number, v: number): boolean => {
    for (let i = 0; i < 9; i++) {
      if ((board[r] as number[])[i] === v || (board[i] as number[])[c] === v) return false;
    }
    const br = r - (r % 3);
    const bc = c - (c % 3);
    for (let i = 0; i < 3; i++) {
      for (let j = 0; j < 3; j++) if ((board[br + i] as number[])[bc + j] === v) return false;
    }
    return true;
  };
  const solve = (): boolean => {
    for (let r = 0; r < 9; r++) {
      for (let c = 0; c < 9; c++) {
        const row = board[r] as number[];
        if (row[c] !== 0) continue;
        for (let v = 1; v <= 9; v++) {
          if (!ok(r, c, v)) continue;
          row[c] = v;
          if (solve()) return true;
          row[c] = 0;
        }
        return false;
      }
    }
    return true;
  };
  return solve();
}
