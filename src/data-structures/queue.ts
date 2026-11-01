// Queue using a head index with periodic compaction. enqueue/dequeue O(1) amortised; space O(n).
export class Queue<T> {
  private items: T[] = [];
  private head = 0;

  get size(): number {
    return this.items.length - this.head;
  }

  isEmpty(): boolean {
    return this.size === 0;
  }

  enqueue(value: T): void {
    this.items.push(value);
  }

  dequeue(): T {
    if (this.isEmpty()) throw new Error("Queue is empty");
    const value = this.items[this.head] as T;
    this.head++;
    if (this.head > 32 && this.head * 2 > this.items.length) {
      this.items = this.items.slice(this.head);
      this.head = 0;
    }
    return value;
  }

  peek(): T {
    if (this.isEmpty()) throw new Error("Queue is empty");
    return this.items[this.head] as T;
  }
}
