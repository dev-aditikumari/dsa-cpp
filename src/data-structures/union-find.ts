// Union-find with path compression and union by rank. find/union O(alpha(n)) amortised; space O(n).
export class UnionFind {
  private parent: number[];
  private rank: number[];
  private sets: number;

  constructor(n: number) {
    if (!Number.isInteger(n) || n < 0) throw new RangeError("Size must be a non-negative integer");
    this.parent = Array.from({ length: n }, (_, i) => i);
    this.rank = new Array<number>(n).fill(0);
    this.sets = n;
  }

  get componentCount(): number {
    return this.sets;
  }

  find(x: number): number {
    this.check(x);
    let root = x;
    while (this.parent[root] !== root) root = this.parent[root] as number;
    while (this.parent[x] !== root) {
      const next = this.parent[x] as number;
      this.parent[x] = root;
      x = next;
    }
    return root;
  }

  union(a: number, b: number): boolean {
    const ra = this.find(a);
    const rb = this.find(b);
    if (ra === rb) return false;
    const rankA = this.rank[ra] as number;
    const rankB = this.rank[rb] as number;
    if (rankA < rankB) this.parent[ra] = rb;
    else if (rankA > rankB) this.parent[rb] = ra;
    else {
      this.parent[rb] = ra;
      this.rank[ra] = rankA + 1;
    }
    this.sets--;
    return true;
  }

  connected(a: number, b: number): boolean {
    return this.find(a) === this.find(b);
  }

  private check(x: number): void {
    if (!Number.isInteger(x) || x < 0 || x >= this.parent.length) {
      throw new RangeError(`Element out of range: ${x}`);
    }
  }
}
