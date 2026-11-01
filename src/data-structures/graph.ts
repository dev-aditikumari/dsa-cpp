export interface Edge<T> {
  to: T;
  weight: number;
}

export interface WeightedEdge<T> {
  from: T;
  to: T;
  weight: number;
}

// Adjacency-list graph, directed or undirected, optionally weighted.
// addVertex/addEdge O(1), neighbors O(1), removeVertex O(V + E); space O(V + E).
export class Graph<T = number> {
  private adj = new Map<T, Edge<T>[]>();
  private edgeList: WeightedEdge<T>[] = [];

  constructor(readonly directed = false) {}

  get vertexCount(): number {
    return this.adj.size;
  }

  get edgeCount(): number {
    return this.edgeList.length;
  }

  addVertex(v: T): void {
    if (!this.adj.has(v)) this.adj.set(v, []);
  }

  hasVertex(v: T): boolean {
    return this.adj.has(v);
  }

  addEdge(from: T, to: T, weight = 1): void {
    if (Number.isNaN(weight)) throw new RangeError("Weight must not be NaN");
    this.addVertex(from);
    this.addVertex(to);
    (this.adj.get(from) as Edge<T>[]).push({ to, weight });
    if (!this.directed && from !== to) (this.adj.get(to) as Edge<T>[]).push({ to: from, weight });
    this.edgeList.push({ from, to, weight });
  }

  hasEdge(from: T, to: T): boolean {
    return (this.adj.get(from) ?? []).some((e) => e.to === to);
  }

  neighbors(v: T): readonly Edge<T>[] {
    const edges = this.adj.get(v);
    if (!edges) throw new Error("Unknown vertex");
    return edges;
  }

  vertices(): T[] {
    return [...this.adj.keys()];
  }

  // Each edge once, in insertion order. Undirected edges keep the direction they were added in.
  edges(): readonly WeightedEdge<T>[] {
    return this.edgeList;
  }

  removeVertex(v: T): boolean {
    if (!this.adj.delete(v)) return false;
    for (const [k, list] of this.adj) {
      this.adj.set(
        k,
        list.filter((e) => e.to !== v),
      );
    }
    this.edgeList = this.edgeList.filter((e) => e.from !== v && e.to !== v);
    return true;
  }
}
