class SNode<T> {
  next: SNode<T> | null = null;
  constructor(public value: T) {}
}

// Singly linked list. push/unshift O(1), pop/get/remove O(n); space O(n).
export class LinkedList<T> implements Iterable<T> {
  private head: SNode<T> | null = null;
  private tail: SNode<T> | null = null;
  private count = 0;

  get size(): number {
    return this.count;
  }

  push(value: T): void {
    const node = new SNode(value);
    if (this.tail) this.tail.next = node;
    else this.head = node;
    this.tail = node;
    this.count++;
  }

  unshift(value: T): void {
    const node = new SNode(value);
    node.next = this.head;
    this.head = node;
    if (!this.tail) this.tail = node;
    this.count++;
  }

  shift(): T {
    if (!this.head) throw new Error("List is empty");
    const node = this.head;
    this.head = node.next;
    if (!this.head) this.tail = null;
    this.count--;
    return node.value;
  }

  pop(): T {
    if (!this.head) throw new Error("List is empty");
    if (this.count === 1) return this.shift();
    let cur = this.head;
    while (cur.next && cur.next.next) cur = cur.next;
    const last = cur.next as SNode<T>;
    cur.next = null;
    this.tail = cur;
    this.count--;
    return last.value;
  }

  get(index: number): T {
    this.checkIndex(index);
    let cur = this.head as SNode<T>;
    for (let i = 0; i < index; i++) cur = cur.next as SNode<T>;
    return cur.value;
  }

  insertAt(index: number, value: T): void {
    if (!Number.isInteger(index) || index < 0 || index > this.count) {
      throw new RangeError(`Index out of range: ${index}`);
    }
    if (index === 0) return this.unshift(value);
    if (index === this.count) return this.push(value);
    let prev = this.head as SNode<T>;
    for (let i = 0; i < index - 1; i++) prev = prev.next as SNode<T>;
    const node = new SNode(value);
    node.next = prev.next;
    prev.next = node;
    this.count++;
  }

  removeAt(index: number): T {
    this.checkIndex(index);
    if (index === 0) return this.shift();
    let prev = this.head as SNode<T>;
    for (let i = 0; i < index - 1; i++) prev = prev.next as SNode<T>;
    const target = prev.next as SNode<T>;
    prev.next = target.next;
    if (target === this.tail) this.tail = prev;
    this.count--;
    return target.value;
  }

  indexOf(value: T): number {
    let i = 0;
    for (const v of this) {
      if (v === value) return i;
      i++;
    }
    return -1;
  }

  reverse(): void {
    let prev: SNode<T> | null = null;
    let cur = this.head;
    this.tail = this.head;
    while (cur) {
      const next: SNode<T> | null = cur.next;
      cur.next = prev;
      prev = cur;
      cur = next;
    }
    this.head = prev;
  }

  toArray(): T[] {
    return [...this];
  }

  *[Symbol.iterator](): Iterator<T> {
    let cur = this.head;
    while (cur) {
      yield cur.value;
      cur = cur.next;
    }
  }

  private checkIndex(index: number): void {
    if (!Number.isInteger(index) || index < 0 || index >= this.count) {
      throw new RangeError(`Index out of range: ${index}`);
    }
  }
}
