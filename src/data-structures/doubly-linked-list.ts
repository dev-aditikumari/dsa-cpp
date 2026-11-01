class DNode<T> {
  prev: DNode<T> | null = null;
  next: DNode<T> | null = null;
  constructor(public value: T) {}
}

// Doubly linked list. Both ends O(1), indexed access O(n); space O(n).
export class DoublyLinkedList<T> implements Iterable<T> {
  private head: DNode<T> | null = null;
  private tail: DNode<T> | null = null;
  private count = 0;

  get size(): number {
    return this.count;
  }

  push(value: T): void {
    const node = new DNode(value);
    if (this.tail) {
      node.prev = this.tail;
      this.tail.next = node;
    } else {
      this.head = node;
    }
    this.tail = node;
    this.count++;
  }

  unshift(value: T): void {
    const node = new DNode(value);
    if (this.head) {
      node.next = this.head;
      this.head.prev = node;
    } else {
      this.tail = node;
    }
    this.head = node;
    this.count++;
  }

  pop(): T {
    if (!this.tail) throw new Error("List is empty");
    return this.unlink(this.tail);
  }

  shift(): T {
    if (!this.head) throw new Error("List is empty");
    return this.unlink(this.head);
  }

  get(index: number): T {
    return this.nodeAt(index).value;
  }

  removeAt(index: number): T {
    return this.unlink(this.nodeAt(index));
  }

  toArray(): T[] {
    return [...this];
  }

  toArrayReversed(): T[] {
    const out: T[] = [];
    let cur = this.tail;
    while (cur) {
      out.push(cur.value);
      cur = cur.prev;
    }
    return out;
  }

  *[Symbol.iterator](): Iterator<T> {
    let cur = this.head;
    while (cur) {
      yield cur.value;
      cur = cur.next;
    }
  }

  private nodeAt(index: number): DNode<T> {
    if (!Number.isInteger(index) || index < 0 || index >= this.count) {
      throw new RangeError(`Index out of range: ${index}`);
    }
    let cur: DNode<T>;
    if (index < this.count / 2) {
      cur = this.head as DNode<T>;
      for (let i = 0; i < index; i++) cur = cur.next as DNode<T>;
    } else {
      cur = this.tail as DNode<T>;
      for (let i = this.count - 1; i > index; i--) cur = cur.prev as DNode<T>;
    }
    return cur;
  }

  private unlink(node: DNode<T>): T {
    if (node.prev) node.prev.next = node.next;
    else this.head = node.next;
    if (node.next) node.next.prev = node.prev;
    else this.tail = node.prev;
    this.count--;
    return node.value;
  }
}
