interface LNode<K, V> {
  key: K;
  value: V;
  prev: LNode<K, V> | null;
  next: LNode<K, V> | null;
}

// LRU cache: hash map plus doubly linked list. get/put O(1); space O(capacity).
export class LRUCache<K, V> {
  private map = new Map<K, LNode<K, V>>();
  private head: LNode<K, V> | null = null; // most recently used
  private tail: LNode<K, V> | null = null; // least recently used

  constructor(readonly capacity: number) {
    if (!Number.isInteger(capacity) || capacity < 1) {
      throw new RangeError("Capacity must be a positive integer");
    }
  }

  get size(): number {
    return this.map.size;
  }

  get(key: K): V | undefined {
    const node = this.map.get(key);
    if (!node) return undefined;
    this.moveToFront(node);
    return node.value;
  }

  put(key: K, value: V): void {
    const existing = this.map.get(key);
    if (existing) {
      existing.value = value;
      this.moveToFront(existing);
      return;
    }
    if (this.map.size >= this.capacity && this.tail) {
      const lru = this.tail;
      this.unlink(lru);
      this.map.delete(lru.key);
    }
    const node: LNode<K, V> = { key, value, prev: null, next: null };
    this.pushFront(node);
    this.map.set(key, node);
  }

  has(key: K): boolean {
    return this.map.has(key);
  }

  // Keys from most to least recently used. Does not change recency.
  keys(): K[] {
    const out: K[] = [];
    for (let n = this.head; n; n = n.next) out.push(n.key);
    return out;
  }

  private moveToFront(node: LNode<K, V>): void {
    if (this.head === node) return;
    this.unlink(node);
    this.pushFront(node);
  }

  private pushFront(node: LNode<K, V>): void {
    node.prev = null;
    node.next = this.head;
    if (this.head) this.head.prev = node;
    this.head = node;
    if (!this.tail) this.tail = node;
  }

  private unlink(node: LNode<K, V>): void {
    if (node.prev) node.prev.next = node.next;
    else this.head = node.next;
    if (node.next) node.next.prev = node.prev;
    else this.tail = node.prev;
    node.prev = null;
    node.next = null;
  }
}
