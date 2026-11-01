// Array-backed stack. push/pop/peek O(1) amortised; space O(n).
export class Stack<T> {
  private items: T[] = [];

  get size(): number {
    return this.items.length;
  }

  isEmpty(): boolean {
    return this.items.length === 0;
  }

  push(value: T): void {
    this.items.push(value);
  }

  pop(): T {
    if (this.items.length === 0) throw new Error("Stack is empty");
    return this.items.pop() as T;
  }

  peek(): T {
    if (this.items.length === 0) throw new Error("Stack is empty");
    return this.items[this.items.length - 1] as T;
  }
}
