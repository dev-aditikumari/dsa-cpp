import { Comparator, defaultCompare } from "../utils/compare";

// O(log n) time, O(1) space. Returns an index of target in a sorted array, or -1.
export function binarySearch<T>(a: readonly T[], target: T, cmp: Comparator<T> = defaultCompare): number {
  let lo = 0;
  let hi = a.length - 1;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    const c = cmp(a[mid] as T, target);
    if (c === 0) return mid;
    if (c < 0) lo = mid + 1;
    else hi = mid - 1;
  }
  return -1;
}

// O(log n) time, O(1) space. First index whose element is >= target (a.length if none).
export function lowerBound<T>(a: readonly T[], target: T, cmp: Comparator<T> = defaultCompare): number {
  let lo = 0;
  let hi = a.length;
  while (lo < hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (cmp(a[mid] as T, target) < 0) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

// O(log n) time, O(1) space. First index whose element is > target (a.length if none).
export function upperBound<T>(a: readonly T[], target: T, cmp: Comparator<T> = defaultCompare): number {
  let lo = 0;
  let hi = a.length;
  while (lo < hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (cmp(a[mid] as T, target) <= 0) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

// O(log n) time, O(1) space. How many times target occurs in a sorted array.
export function countOccurrences<T>(a: readonly T[], target: T, cmp: Comparator<T> = defaultCompare): number {
  return upperBound(a, target, cmp) - lowerBound(a, target, cmp);
}

// O(log n) time on distinct values, O(1) space. Index of target in a sorted array that was rotated, or -1.
// With duplicates the worst case degrades to O(n).
export function searchRotated(a: readonly number[], target: number): number {
  let lo = 0;
  let hi = a.length - 1;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    const m = a[mid] as number;
    if (m === target) return mid;
    const l = a[lo] as number;
    const h = a[hi] as number;
    if (l === m && m === h) {
      lo++;
      hi--;
    } else if (l <= m) {
      if (target >= l && target < m) hi = mid - 1;
      else lo = mid + 1;
    } else if (target > m && target <= h) lo = mid + 1;
    else hi = mid - 1;
  }
  return -1;
}

// O(log n) time, O(1) space. Index of the minimum in a rotated sorted array of distinct values.
export function findRotationPoint(a: readonly number[]): number {
  if (a.length === 0) throw new Error("Array is empty");
  let lo = 0;
  let hi = a.length - 1;
  while (lo < hi) {
    const mid = lo + ((hi - lo) >> 1);
    if ((a[mid] as number) > (a[hi] as number)) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

// O(log(range)) probes, O(1) space. Smallest integer in [lo, hi] for which ok() is true, given ok is monotone
// (false then true). Returns hi + 1 when none qualifies.
export function searchOnAnswer(lo: number, hi: number, ok: (x: number) => boolean): number {
  if (!Number.isInteger(lo) || !Number.isInteger(hi)) throw new TypeError("Bounds must be integers");
  let l = lo;
  let h = hi + 1;
  while (l < h) {
    const mid = l + Math.floor((h - l) / 2);
    if (ok(mid)) h = mid;
    else l = mid + 1;
  }
  return l;
}

// O(n log(sum)) time, O(1) space. Smallest capacity that ships all packages in order within `days` days.
export function shipWithinDays(weights: readonly number[], days: number): number {
  if (days < 1 || !Number.isInteger(days)) throw new RangeError("days must be a positive integer");
  if (weights.length === 0) return 0;
  const heaviest = Math.max(...weights);
  const total = weights.reduce((x, y) => x + y, 0);
  return searchOnAnswer(heaviest, total, (cap) => {
    let used = 1;
    let load = 0;
    for (const w of weights) {
      if (load + w > cap) {
        used++;
        load = 0;
      }
      load += w;
    }
    return used <= days;
  });
}

// O(log x) time, O(1) space. Floor of the square root of a non-negative integer.
export function integerSqrt(x: number): number {
  if (!Number.isInteger(x) || x < 0) throw new RangeError("x must be a non-negative integer");
  return searchOnAnswer(0, x, (m) => m * m > x) - 1;
}
