import { Comparator, defaultCompare } from "../utils/compare";
import { BinaryHeap } from "../data-structures/heap";

// All sorts return a new array and leave the input untouched.

// O(n^2) time (O(n) if already sorted), O(1) extra space, stable.
export function bubbleSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  const a = [...input];
  for (let end = a.length - 1; end > 0; end--) {
    let swapped = false;
    for (let i = 0; i < end; i++) {
      if (cmp(a[i] as T, a[i + 1] as T) > 0) {
        [a[i], a[i + 1]] = [a[i + 1] as T, a[i] as T];
        swapped = true;
      }
    }
    if (!swapped) break;
  }
  return a;
}

// O(n^2) time (O(n) if nearly sorted), O(1) extra space, stable.
export function insertionSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  const a = [...input];
  for (let i = 1; i < a.length; i++) {
    const x = a[i] as T;
    let j = i - 1;
    while (j >= 0 && cmp(a[j] as T, x) > 0) {
      a[j + 1] = a[j] as T;
      j--;
    }
    a[j + 1] = x;
  }
  return a;
}

// O(n^2) time always, O(1) extra space, not stable.
export function selectionSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  const a = [...input];
  for (let i = 0; i < a.length - 1; i++) {
    let min = i;
    for (let j = i + 1; j < a.length; j++) if (cmp(a[j] as T, a[min] as T) < 0) min = j;
    if (min !== i) [a[i], a[min]] = [a[min] as T, a[i] as T];
  }
  return a;
}

// O(n log n) time, O(n) space, stable.
export function mergeSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  if (input.length <= 1) return [...input];
  const mid = input.length >> 1;
  const left = mergeSort(input.slice(0, mid), cmp);
  const right = mergeSort(input.slice(mid), cmp);
  const out: T[] = [];
  let i = 0;
  let j = 0;
  while (i < left.length && j < right.length) {
    // <= keeps equal elements in original order
    out.push(cmp(left[i] as T, right[j] as T) <= 0 ? (left[i++] as T) : (right[j++] as T));
  }
  while (i < left.length) out.push(left[i++] as T);
  while (j < right.length) out.push(right[j++] as T);
  return out;
}

// O(n log n) expected, O(n^2) worst case, O(log n) extra space, not stable.
// Three-way partitioning keeps arrays with many duplicates fast; recursing on the smaller side bounds stack depth.
export function quickSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  const a = [...input];
  const sort = (lo: number, hi: number): void => {
    while (lo < hi) {
      const mid = lo + ((hi - lo) >> 1);
      const pivot = medianOfThree(a[lo] as T, a[mid] as T, a[hi] as T, cmp);
      let lt = lo;
      let gt = hi;
      let i = lo;
      while (i <= gt) {
        const c = cmp(a[i] as T, pivot);
        if (c < 0) {
          [a[lt], a[i]] = [a[i] as T, a[lt] as T];
          lt++;
          i++;
        } else if (c > 0) {
          [a[gt], a[i]] = [a[i] as T, a[gt] as T];
          gt--;
        } else {
          i++;
        }
      }
      if (lt - lo < hi - gt) {
        sort(lo, lt - 1);
        lo = gt + 1;
      } else {
        sort(gt + 1, hi);
        hi = lt - 1;
      }
    }
  };
  sort(0, a.length - 1);
  return a;
}

function medianOfThree<T>(x: T, y: T, z: T, cmp: Comparator<T>): T {
  if (cmp(x, y) < 0) {
    if (cmp(y, z) < 0) return y;
    return cmp(x, z) < 0 ? z : x;
  }
  if (cmp(x, z) < 0) return x;
  return cmp(y, z) < 0 ? z : y;
}

// O(n log n) time, O(n) space (this version pops from a separate heap rather than sorting in place), not stable.
export function heapSort<T>(input: readonly T[], cmp: Comparator<T> = defaultCompare): T[] {
  const heap = BinaryHeap.from(input, cmp);
  const out: T[] = [];
  while (!heap.isEmpty()) out.push(heap.pop());
  return out;
}

// O(n + k) time and space where k = max - min + 1. Integers only; negatives are supported.
export function countingSort(input: readonly number[]): number[] {
  if (input.length === 0) return [];
  let min = input[0] as number;
  let max = min;
  for (const x of input) {
    if (!Number.isInteger(x)) throw new TypeError(`countingSort needs integers, got ${x}`);
    if (x < min) min = x;
    if (x > max) max = x;
  }
  const counts = new Array<number>(max - min + 1).fill(0);
  for (const x of input) counts[x - min] = (counts[x - min] as number) + 1;
  const out: number[] = [];
  counts.forEach((c, i) => {
    for (let k = 0; k < c; k++) out.push(i + min);
  });
  return out;
}
