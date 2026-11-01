export type Comparator<T> = (a: T, b: T) => number;

// O(1) time, O(1) space. Works for numbers, strings and bigints.
export function defaultCompare<T>(a: T, b: T): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}
