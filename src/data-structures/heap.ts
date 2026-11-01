import { Comparator, defaultCompare } from "../utils/compare";

// Binary heap (min by default; pass a reversed comparator for max). push/pop O(log n), peek O(1); space O(n).
export class BinaryHeap<T> {
  private data: T[] = [];

  constructor(private readonly compare: Comparator<T> = defaultCompare) {}

  static minHeap<T>(compare: Comparator<T> = defaultCompare): BinaryHeap<T> {
    return new BinaryHeap(compare);
  }

  static maxHeap<T>(compare: Comparator<T> = defaultCompare): BinaryHeap<T> {
    return new BinaryHeap((a, b) => compare(b, a));
  }

  static from<T>(items: Iterable<T>, compare: Comparator<T> = defaultCompare): BinaryHeap<T> {
    const heap = new BinaryHeap(compare);
    heap.data = [...items];
    for (let i = (heap.data.length >> 1) - 1; i >= 0; i--) heap.siftDown(i);
    return heap;
  }

  get size(): number {
    return this.data.length;
  }

  isEmpty(): boolean {
    return this.data.length === 0;
  }

  peek(): T {
    if (this.data.length === 0) throw new Error("Heap is empty");
    return this.data[0] as T;
  }

  push(value: T): void {
    this.data.push(value);
    this.siftUp(this.data.length - 1);
  }

  pop(): T {
    if (this.data.length === 0) throw new Error("Heap is empty");
    const top = this.data[0] as T;
    const last = this.data.pop() as T;
    if (this.data.length > 0) {
      this.data[0] = last;
      this.siftDown(0);
    }
    return top;
  }

  private siftUp(i: number): void {
    while (i > 0) {
      const parent = (i - 1) >> 1;
      if (this.compare(this.data[i] as T, this.data[parent] as T) >= 0) break;
      this.swap(i, parent);
      i = parent;
    }
  }

  private siftDown(i: number): void {
    const n = this.data.length;
    for (;;) {
      const l = 2 * i + 1;
      const r = l + 1;
      let best = i;
      if (l < n && this.compare(this.data[l] as T, this.data[best] as T) < 0) best = l;
      if (r < n && this.compare(this.data[r] as T, this.data[best] as T) < 0) best = r;
      if (best === i) return;
      this.swap(i, best);
      i = best;
    }
  }

  private swap(i: number, j: number): void {
    const tmp = this.data[i] as T;
    this.data[i] = this.data[j] as T;
    this.data[j] = tmp;
  }
}
