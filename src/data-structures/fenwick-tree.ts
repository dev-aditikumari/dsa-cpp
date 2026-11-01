// Fenwick (binary indexed) tree over numbers. update/prefixSum/rangeSum O(log n); build O(n); space O(n).
export class FenwickTree {
  private tree: number[];

  constructor(values: readonly number[]) {
    const n = values.length;
    this.tree = new Array<number>(n + 1).fill(0);
    for (let i = 1; i <= n; i++) {
      this.tree[i] = (this.tree[i] as number) + (values[i - 1] as number);
      const parent = i + (i & -i);
      if (parent <= n) this.tree[parent] = (this.tree[parent] as number) + (this.tree[i] as number);
    }
  }

  get size(): number {
    return this.tree.length - 1;
  }

  // Adds delta to the element at index.
  add(index: number, delta: number): void {
    this.check(index);
    for (let i = index + 1; i < this.tree.length; i += i & -i) {
      this.tree[i] = (this.tree[i] as number) + delta;
    }
  }

  // Sum of elements [0, index], inclusive. Returns 0 for index -1.
  prefixSum(index: number): number {
    if (index === -1) return 0;
    this.check(index);
    let sum = 0;
    for (let i = index + 1; i > 0; i -= i & -i) sum += this.tree[i] as number;
    return sum;
  }

  // Sum of elements [left, right], inclusive.
  rangeSum(left: number, right: number): number {
    this.check(left);
    this.check(right);
    if (left > right) throw new RangeError("left must not exceed right");
    return this.prefixSum(right) - this.prefixSum(left - 1);
  }

  private check(i: number): void {
    if (!Number.isInteger(i) || i < 0 || i >= this.size) throw new RangeError(`Index out of range: ${i}`);
  }
}
