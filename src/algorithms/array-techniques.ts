// O(n) time, O(1) space. Pair of indices in a sorted array summing to target, or null (two pointers).
export function twoSumSorted(a: readonly number[], target: number): [number, number] | null {
  let i = 0;
  let j = a.length - 1;
  while (i < j) {
    const s = (a[i] as number) + (a[j] as number);
    if (s === target) return [i, j];
    if (s < target) i++;
    else j--;
  }
  return null;
}

// O(n^2) time, O(1) extra space. Unique triplets summing to zero.
export function threeSum(input: readonly number[]): [number, number, number][] {
  const a = [...input].sort((x, y) => x - y);
  const out: [number, number, number][] = [];
  for (let i = 0; i < a.length - 2; i++) {
    if (i > 0 && a[i] === a[i - 1]) continue;
    let l = i + 1;
    let r = a.length - 1;
    while (l < r) {
      const s = (a[i] as number) + (a[l] as number) + (a[r] as number);
      if (s < 0) l++;
      else if (s > 0) r--;
      else {
        out.push([a[i] as number, a[l] as number, a[r] as number]);
        l++;
        r--;
        while (l < r && a[l] === a[l - 1]) l++;
        while (l < r && a[r] === a[r + 1]) r--;
      }
    }
  }
  return out;
}

// O(n) time, O(1) space. Largest area between two vertical lines (two pointers).
export function maxWater(heights: readonly number[]): number {
  let i = 0;
  let j = heights.length - 1;
  let best = 0;
  while (i < j) {
    const hi = heights[i] as number;
    const hj = heights[j] as number;
    best = Math.max(best, Math.min(hi, hj) * (j - i));
    if (hi < hj) i++;
    else j--;
  }
  return best;
}

// O(n) time, O(1) space. Maximum sum of any k consecutive elements (fixed sliding window).
export function maxSumWindow(a: readonly number[], k: number): number {
  if (!Number.isInteger(k) || k < 1 || k > a.length) throw new RangeError("k must be between 1 and the array length");
  let sum = 0;
  for (let i = 0; i < k; i++) sum += a[i] as number;
  let best = sum;
  for (let i = k; i < a.length; i++) {
    sum += (a[i] as number) - (a[i - k] as number);
    best = Math.max(best, sum);
  }
  return best;
}

// O(n) time, O(min(n, alphabet)) space. Length of the longest substring without repeated characters.
export function longestUniqueSubstring(s: string): number {
  const last = new Map<string, number>();
  let start = 0;
  let best = 0;
  for (let i = 0; i < s.length; i++) {
    const ch = s[i] as string;
    const prev = last.get(ch);
    if (prev !== undefined && prev >= start) start = prev + 1;
    last.set(ch, i);
    best = Math.max(best, i - start + 1);
  }
  return best;
}

// O(n) time, O(1) space. Shortest subarray with sum >= target (non-negative numbers), or 0 if none.
export function minSubarrayLen(a: readonly number[], target: number): number {
  let best = Infinity;
  let sum = 0;
  let left = 0;
  for (let right = 0; right < a.length; right++) {
    sum += a[right] as number;
    while (sum >= target && left <= right) {
      best = Math.min(best, right - left + 1);
      sum -= a[left++] as number;
    }
  }
  return best === Infinity ? 0 : best;
}

// O(n) build, O(1) per query, O(n) space. Prefix sums with inclusive range queries.
export class PrefixSums {
  private readonly prefix: number[];

  constructor(a: readonly number[]) {
    this.prefix = [0];
    for (const x of a) this.prefix.push((this.prefix[this.prefix.length - 1] as number) + x);
  }

  rangeSum(left: number, right: number): number {
    if (!Number.isInteger(left) || !Number.isInteger(right) || left < 0 || right >= this.prefix.length - 1 || left > right) {
      throw new RangeError(`Invalid range [${left}, ${right}]`);
    }
    return (this.prefix[right + 1] as number) - (this.prefix[left] as number);
  }
}

// O(n) time, O(n) space. Number of subarrays whose sum equals k (prefix sums plus a hash map).
export function subarraySumEquals(a: readonly number[], k: number): number {
  const seen = new Map<number, number>([[0, 1]]);
  let sum = 0;
  let count = 0;
  for (const x of a) {
    sum += x;
    count += seen.get(sum - k) ?? 0;
    seen.set(sum, (seen.get(sum) ?? 0) + 1);
  }
  return count;
}
