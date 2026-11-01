// Segment tree with a generic associative combine. build O(n); query/update O(log n); space O(n).
export class SegmentTree<T> {
  private readonly n: number;
  private readonly tree: T[];

  constructor(
    values: readonly T[],
    private readonly combine: (a: T, b: T) => T,
    private readonly identity: T,
  ) {
    this.n = values.length;
    this.tree = new Array<T>(2 * this.n).fill(identity);
    for (let i = 0; i < this.n; i++) this.tree[this.n + i] = values[i] as T;
    for (let i = this.n - 1; i >= 1; i--) {
      this.tree[i] = combine(this.tree[2 * i] as T, this.tree[2 * i + 1] as T);
    }
  }

  get size(): number {
    return this.n;
  }

  set(index: number, value: T): void {
    this.check(index);
    let i = index + this.n;
    this.tree[i] = value;
    for (i >>= 1; i >= 1; i >>= 1) {
      this.tree[i] = this.combine(this.tree[2 * i] as T, this.tree[2 * i + 1] as T);
    }
  }

  // Combines elements in [left, right], inclusive, preserving order for non-commutative operations.
  query(left: number, right: number): T {
    this.check(left);
    this.check(right);
    if (left > right) throw new RangeError("left must not exceed right");
    let resL = this.identity;
    let resR = this.identity;
    let l = left + this.n;
    let r = right + this.n + 1;
    while (l < r) {
      if (l & 1) resL = this.combine(resL, this.tree[l++] as T);
      if (r & 1) resR = this.combine(this.tree[--r] as T, resR);
      l >>= 1;
      r >>= 1;
    }
    return this.combine(resL, resR);
  }

  private check(i: number): void {
    if (!Number.isInteger(i) || i < 0 || i >= this.n) throw new RangeError(`Index out of range: ${i}`);
  }
}
