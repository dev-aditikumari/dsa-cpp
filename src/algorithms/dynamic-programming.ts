// O(n) time, O(n) space (memo, recursion depth n so keep n below a few thousand). Uses bigint so results stay exact.
export function fibMemo(n: number, memo: Map<number, bigint> = new Map()): bigint {
  checkCount(n);
  if (n < 2) return BigInt(n);
  const cached = memo.get(n);
  if (cached !== undefined) return cached;
  const value = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
  memo.set(n, value);
  return value;
}

// O(n) time, O(1) space. Bottom-up Fibonacci with bigint.
export function fibTab(n: number): bigint {
  checkCount(n);
  let a = 0n;
  let b = 1n;
  for (let i = 0; i < n; i++) [a, b] = [b, a + b];
  return a;
}

function checkCount(n: number): void {
  if (!Number.isInteger(n) || n < 0) throw new RangeError("n must be a non-negative integer");
}

// O(n * W) time, O(W) space. Maximum value with total weight <= capacity, each item used at most once.
export function knapsack01(weights: readonly number[], values: readonly number[], capacity: number): number {
  if (weights.length !== values.length) throw new Error("weights and values must have the same length");
  if (!Number.isInteger(capacity) || capacity < 0) throw new RangeError("capacity must be a non-negative integer");
  const best = new Array<number>(capacity + 1).fill(0);
  weights.forEach((w, i) => {
    if (!Number.isInteger(w) || w < 0) throw new RangeError("weights must be non-negative integers");
    for (let c = capacity; c >= w; c--) {
      best[c] = Math.max(best[c] as number, (best[c - w] as number) + (values[i] as number));
    }
  });
  return best[capacity] as number;
}

// O(m * n) time, O(m * n) space. Longest common subsequence of two strings (one of them, if tied).
export function lcs(a: string, b: string): string {
  const dp: number[][] = Array.from({ length: a.length + 1 }, () => new Array<number>(b.length + 1).fill(0));
  for (let i = 1; i <= a.length; i++) {
    for (let j = 1; j <= b.length; j++) {
      (dp[i] as number[])[j] =
        a[i - 1] === b[j - 1]
          ? ((dp[i - 1] as number[])[j - 1] as number) + 1
          : Math.max((dp[i - 1] as number[])[j] as number, (dp[i] as number[])[j - 1] as number);
    }
  }
  let i = a.length;
  let j = b.length;
  const out: string[] = [];
  while (i > 0 && j > 0) {
    if (a[i - 1] === b[j - 1]) {
      out.push(a[i - 1] as string);
      i--;
      j--;
    } else if (((dp[i - 1] as number[])[j] as number) >= ((dp[i] as number[])[j - 1] as number)) i--;
    else j--;
  }
  return out.reverse().join("");
}

// O(n log n) time, O(n) space. Length of the longest strictly increasing subsequence (patience sorting).
export function lisLength(a: readonly number[]): number {
  const tails: number[] = [];
  for (const x of a) {
    let lo = 0;
    let hi = tails.length;
    while (lo < hi) {
      const mid = (lo + hi) >> 1;
      if ((tails[mid] as number) < x) lo = mid + 1;
      else hi = mid;
    }
    tails[lo] = x;
  }
  return tails.length;
}

// O(m * n) time, O(min(m, n)) space. Levenshtein distance (insert, delete, substitute).
export function editDistance(a: string, b: string): number {
  if (a.length < b.length) [a, b] = [b, a];
  let prev = Array.from({ length: b.length + 1 }, (_, j) => j);
  for (let i = 1; i <= a.length; i++) {
    const cur = [i];
    for (let j = 1; j <= b.length; j++) {
      const sub = (prev[j - 1] as number) + (a[i - 1] === b[j - 1] ? 0 : 1);
      cur[j] = Math.min(sub, (prev[j] as number) + 1, (cur[j - 1] as number) + 1);
    }
    prev = cur;
  }
  return prev[b.length] as number;
}

// O(amount * coins) time, O(amount) space. Fewest coins summing to amount, or -1 if impossible.
export function coinChangeMin(coins: readonly number[], amount: number): number {
  if (!Number.isInteger(amount) || amount < 0) throw new RangeError("amount must be a non-negative integer");
  const dp = new Array<number>(amount + 1).fill(Infinity);
  dp[0] = 0;
  for (let s = 1; s <= amount; s++) {
    for (const c of coins) {
      if (c > 0 && c <= s) dp[s] = Math.min(dp[s] as number, (dp[s - c] as number) + 1);
    }
  }
  return dp[amount] === Infinity ? -1 : (dp[amount] as number);
}

// O(amount * coins) time, O(amount) space. Number of distinct combinations (order ignored) that sum to amount.
export function coinChangeWays(coins: readonly number[], amount: number): number {
  if (!Number.isInteger(amount) || amount < 0) throw new RangeError("amount must be a non-negative integer");
  const dp = new Array<number>(amount + 1).fill(0);
  dp[0] = 1;
  for (const c of coins) {
    if (c <= 0) continue;
    for (let s = c; s <= amount; s++) dp[s] = (dp[s] as number) + (dp[s - c] as number);
  }
  return dp[amount] as number;
}

// O(rows * cols) time, O(cols) space. Unique right/down paths in a grid, where 1 marks an obstacle.
export function uniquePaths(grid: readonly (readonly number[])[]): number {
  if (grid.length === 0 || (grid[0] as readonly number[]).length === 0) return 0;
  const cols = (grid[0] as readonly number[]).length;
  const dp = new Array<number>(cols).fill(0);
  dp[0] = 1;
  for (const row of grid) {
    if (row.length !== cols) throw new Error("Grid must be rectangular");
    for (let j = 0; j < cols; j++) {
      if (row[j] === 1) dp[j] = 0;
      else if (j > 0) dp[j] = (dp[j] as number) + (dp[j - 1] as number);
    }
  }
  return dp[cols - 1] as number;
}

// O(n^3) time, O(n^2) space. Fewest scalar multiplications to multiply a chain where matrix i is dims[i] x dims[i+1].
export function matrixChainOrder(dims: readonly number[]): number {
  const n = dims.length - 1;
  if (n <= 1) return 0;
  const dp: number[][] = Array.from({ length: n }, () => new Array<number>(n).fill(0));
  for (let len = 2; len <= n; len++) {
    for (let i = 0; i + len - 1 < n; i++) {
      const j = i + len - 1;
      let best = Infinity;
      for (let k = i; k < j; k++) {
        const cost =
          ((dp[i] as number[])[k] as number) +
          ((dp[k + 1] as number[])[j] as number) +
          (dims[i] as number) * (dims[k + 1] as number) * (dims[j + 1] as number);
        if (cost < best) best = cost;
      }
      (dp[i] as number[])[j] = best;
    }
  }
  return (dp[0] as number[])[n - 1] as number;
}
