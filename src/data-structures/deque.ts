// Circular-buffer deque. All end operations O(1) amortised (doubling on growth); space O(n).
export class Deque<T> {
  private buf: (T | undefined)[];
  private head = 0;
  private count = 0;

  constructor(initialCapacity = 8) {
    if (!Number.isInteger(initialCapacity) || initialCapacity < 1) {
      throw new RangeError("Capacity must be a positive integer");
    }
    this.buf = new Array<T | undefined>(initialCapacity);
  }

  get size(): number {
    return this.count;
  }

  pushBack(value: T): void {
    this.grow();
    this.buf[(this.head + this.count) % this.buf.length] = value;
    this.count++;
  }

  pushFront(value: T): void {
    this.grow();
    this.head = (this.head - 1 + this.buf.length) % this.buf.length;
    this.buf[this.head] = value;
    this.count++;
  }

  popFront(): T {
    if (this.count === 0) throw new Error("Deque is empty");
    const value = this.buf[this.head] as T;
    this.buf[this.head] = undefined;
    this.head = (this.head + 1) % this.buf.length;
    this.count--;
    return value;
  }

  popBack(): T {
    if (this.count === 0) throw new Error("Deque is empty");
    const idx = (this.head + this.count - 1) % this.buf.length;
    const value = this.buf[idx] as T;
    this.buf[idx] = undefined;
    this.count--;
    return value;
  }

  peekFront(): T {
    if (this.count === 0) throw new Error("Deque is empty");
    return this.buf[this.head] as T;
  }

  peekBack(): T {
    if (this.count === 0) throw new Error("Deque is empty");
    return this.buf[(this.head + this.count - 1) % this.buf.length] as T;
  }

  toArray(): T[] {
    const out: T[] = [];
    for (let i = 0; i < this.count; i++) {
      out.push(this.buf[(this.head + i) % this.buf.length] as T);
    }
    return out;
  }

  private grow(): void {
    if (this.count < this.buf.length) return;
    const next = new Array<T | undefined>(this.buf.length * 2);
    for (let i = 0; i < this.count; i++) {
      next[i] = this.buf[(this.head + i) % this.buf.length];
    }
    this.buf = next;
    this.head = 0;
  }
}
