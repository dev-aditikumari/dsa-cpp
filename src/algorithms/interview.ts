import { BinaryHeap } from "../data-structures/heap";
import { Stack } from "../data-structures/stack";

// O(n) time, O(n) space. Indices of two numbers summing to target, or null.
export function twoSum(a: readonly number[], target: number): [number, number] | null {
  const seen = new Map<number, number>();
  for (let i = 0; i < a.length; i++) {
    const j = seen.get(target - (a[i] as number));
    if (j !== undefined) return [j, i];
    seen.set(a[i] as number, i);
  }
  return null;
}

// O(n) time, O(n) space. True when brackets (), [] and {} are balanced; other characters are ignored.
export function validParentheses(s: string): boolean {
  const pairs: Record<string, string> = { ")": "(", "]": "[", "}": "{" };
  const stack = new Stack<string>();
  for (const ch of s) {
    if (ch === "(" || ch === "[" || ch === "{") stack.push(ch);
    else if (ch in pairs) {
      if (stack.isEmpty() || stack.pop() !== pairs[ch]) return false;
    }
  }
  return stack.isEmpty();
}

// O(n log n) time, O(n) space. Merges overlapping or touching intervals [start, end].
export function mergeIntervals(intervals: readonly (readonly [number, number])[]): [number, number][] {
  const sorted = intervals.map(([s, e]): [number, number] => {
    if (s > e) throw new RangeError(`Invalid interval [${s}, ${e}]`);
    return [s, e];
  });
  sorted.sort((a, b) => a[0] - b[0] || a[1] - b[1]);
  const out: [number, number][] = [];
  for (const [s, e] of sorted) {
    const last = out[out.length - 1];
    if (last && s <= last[1]) last[1] = Math.max(last[1], e);
    else out.push([s, e]);
  }
  return out;
}

// O(n log k) time, O(k) space. The k-th largest element (counting duplicates) using a min heap of size k.
export function kthLargest(a: readonly number[], k: number): number {
  if (!Number.isInteger(k) || k < 1 || k > a.length) throw new RangeError("k must be between 1 and the array length");
  const heap = BinaryHeap.minHeap<number>();
  for (const x of a) {
    if (heap.size < k) heap.push(x);
    else if (x > heap.peek()) {
      heap.pop();
      heap.push(x);
    }
  }
  return heap.peek();
}

// O(n) time, O(1) space. Maximum subarray sum (Kadane). Throws on empty input.
export function maxSubarray(a: readonly number[]): number {
  if (a.length === 0) throw new Error("Array is empty");
  let cur = a[0] as number;
  let best = cur;
  for (let i = 1; i < a.length; i++) {
    cur = Math.max(a[i] as number, cur + (a[i] as number));
    best = Math.max(best, cur);
  }
  return best;
}

// O(n) time, O(1) space. Reverses a singly linked list given as nodes with `next`.
export interface ListNode<T> {
  value: T;
  next: ListNode<T> | null;
}
export function reverseList<T>(head: ListNode<T> | null): ListNode<T> | null {
  let prev: ListNode<T> | null = null;
  while (head) {
    const next: ListNode<T> | null = head.next;
    head.next = prev;
    prev = head;
    head = next;
  }
  return prev;
}
