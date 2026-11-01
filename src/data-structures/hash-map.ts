interface Bucket<K, V> {
  key: K;
  value: V;
  next: Bucket<K, V> | null;
}

// SameValueZero, the same equality Map uses.
function sameKey(a: unknown, b: unknown): boolean {
  return a === b || (a !== a && b !== b);
}

// Hash map with separate chaining and doubling resize at load factor 0.75.
// get/set/delete O(1) average, O(n) worst case; space O(n).
export class HashMap<K, V> implements Iterable<[K, V]> {
  private buckets: (Bucket<K, V> | null)[];
  private count = 0;

  constructor(
    initialCapacity = 8,
    private readonly hash: (key: K) => number = HashMap.defaultHash,
  ) {
    if (!Number.isInteger(initialCapacity) || initialCapacity < 1) {
      throw new RangeError("Capacity must be a positive integer");
    }
    this.buckets = new Array<Bucket<K, V> | null>(initialCapacity).fill(null);
  }

  get size(): number {
    return this.count;
  }

  get capacity(): number {
    return this.buckets.length;
  }

  set(key: K, value: V): void {
    const idx = this.indexFor(key, this.buckets.length);
    for (let b = this.buckets[idx]; b; b = b.next) {
      if (sameKey(b.key, key)) {
        b.value = value;
        return;
      }
    }
    this.buckets[idx] = { key, value, next: this.buckets[idx] ?? null };
    this.count++;
    if (this.count > this.buckets.length * 0.75) this.resize(this.buckets.length * 2);
  }

  get(key: K): V | undefined {
    for (let b = this.buckets[this.indexFor(key, this.buckets.length)]; b; b = b.next) {
      if (sameKey(b.key, key)) return b.value;
    }
    return undefined;
  }

  has(key: K): boolean {
    for (let b = this.buckets[this.indexFor(key, this.buckets.length)]; b; b = b.next) {
      if (sameKey(b.key, key)) return true;
    }
    return false;
  }

  delete(key: K): boolean {
    const idx = this.indexFor(key, this.buckets.length);
    let prev: Bucket<K, V> | null = null;
    for (let b = this.buckets[idx] ?? null; b; b = b.next) {
      if (sameKey(b.key, key)) {
        if (prev) prev.next = b.next;
        else this.buckets[idx] = b.next;
        this.count--;
        return true;
      }
      prev = b;
    }
    return false;
  }

  *[Symbol.iterator](): Iterator<[K, V]> {
    for (const head of this.buckets) {
      for (let b = head; b; b = b.next) yield [b.key, b.value];
    }
  }

  keys(): K[] {
    return [...this].map(([k]) => k);
  }

  private indexFor(key: K, capacity: number): number {
    return (this.hash(key) >>> 0) % capacity;
  }

  private resize(newCapacity: number): void {
    const next = new Array<Bucket<K, V> | null>(newCapacity).fill(null);
    for (const head of this.buckets) {
      let b = head;
      while (b) {
        const following: Bucket<K, V> | null = b.next;
        const idx = this.indexFor(b.key, newCapacity);
        b.next = next[idx] ?? null;
        next[idx] = b;
        b = following;
      }
    }
    this.buckets = next;
  }

  // FNV-1a over the string form of the key; the type tag keeps 1 and "1" apart.
  private static defaultHash(key: unknown): number {
    const s = `${typeof key}:${String(key)}`;
    let h = 2166136261;
    for (let i = 0; i < s.length; i++) {
      h ^= s.charCodeAt(i);
      h = Math.imul(h, 16777619);
    }
    return h >>> 0;
  }
}
