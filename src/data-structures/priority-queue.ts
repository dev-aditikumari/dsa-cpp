import { BinaryHeap } from "./heap";

interface Entry<T> {
  item: T;
  priority: number;
  order: number;
}

// Priority queue (lowest priority number first, FIFO among ties). enqueue/dequeue O(log n); space O(n).
export class PriorityQueue<T> {
  private heap = new BinaryHeap<Entry<T>>((a, b) =>
    a.priority !== b.priority ? a.priority - b.priority : a.order - b.order,
  );
  private counter = 0;

  get size(): number {
    return this.heap.size;
  }

  isEmpty(): boolean {
    return this.heap.isEmpty();
  }

  enqueue(item: T, priority: number): void {
    if (Number.isNaN(priority)) throw new RangeError("Priority must not be NaN");
    this.heap.push({ item, priority, order: this.counter++ });
  }

  dequeue(): T {
    if (this.heap.isEmpty()) throw new Error("Priority queue is empty");
    return this.heap.pop().item;
  }

  peek(): T {
    if (this.heap.isEmpty()) throw new Error("Priority queue is empty");
    return this.heap.peek().item;
  }
}
