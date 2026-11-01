import { Graph } from "../data-structures/graph";
import { BinaryHeap } from "../data-structures/heap";
import { Queue } from "../data-structures/queue";
import { UnionFind } from "../data-structures/union-find";

// O(V + E) time, O(V) space. Vertices in breadth-first order from start.
export function bfs<T>(g: Graph<T>, start: T): T[] {
  const seen = new Set<T>([start]);
  const q = new Queue<T>();
  const order: T[] = [];
  q.enqueue(start);
  while (!q.isEmpty()) {
    const v = q.dequeue();
    order.push(v);
    for (const e of g.neighbors(v)) {
      if (!seen.has(e.to)) {
        seen.add(e.to);
        q.enqueue(e.to);
      }
    }
  }
  return order;
}

// O(V + E) time, O(V) space. Iterative depth-first order from start (neighbors visited in insertion order).
export function dfs<T>(g: Graph<T>, start: T): T[] {
  g.neighbors(start);
  const seen = new Set<T>();
  const order: T[] = [];
  const stack: T[] = [start];
  while (stack.length) {
    const v = stack.pop() as T;
    if (seen.has(v)) continue;
    seen.add(v);
    order.push(v);
    const next = g.neighbors(v);
    for (let i = next.length - 1; i >= 0; i--) {
      const to = (next[i] as { to: T }).to;
      if (!seen.has(to)) stack.push(to);
    }
  }
  return order;
}

// O(V + E) time, O(V) space. Fewest-edge path from start to goal, or null when unreachable.
export function shortestPathUnweighted<T>(g: Graph<T>, start: T, goal: T): T[] | null {
  g.neighbors(start);
  const parent = new Map<T, T | null>([[start, null]]);
  const q = new Queue<T>();
  q.enqueue(start);
  while (!q.isEmpty()) {
    const v = q.dequeue();
    if (v === goal) break;
    for (const e of g.neighbors(v)) {
      if (!parent.has(e.to)) {
        parent.set(e.to, v);
        q.enqueue(e.to);
      }
    }
  }
  if (!parent.has(goal)) return null;
  const path: T[] = [];
  for (let cur: T | null | undefined = goal; cur !== null && cur !== undefined; cur = parent.get(cur)) {
    path.push(cur);
  }
  return path.reverse();
}

// O(V + E) time, O(V) space. Kahn's algorithm. Throws when the directed graph has a cycle.
export function topologicalSort<T>(g: Graph<T>): T[] {
  if (!g.directed) throw new Error("Topological sort needs a directed graph");
  const indegree = new Map<T, number>();
  for (const v of g.vertices()) indegree.set(v, 0);
  for (const v of g.vertices()) {
    for (const e of g.neighbors(v)) indegree.set(e.to, (indegree.get(e.to) as number) + 1);
  }
  const q = new Queue<T>();
  for (const [v, d] of indegree) if (d === 0) q.enqueue(v);
  const order: T[] = [];
  while (!q.isEmpty()) {
    const v = q.dequeue();
    order.push(v);
    for (const e of g.neighbors(v)) {
      const d = (indegree.get(e.to) as number) - 1;
      indegree.set(e.to, d);
      if (d === 0) q.enqueue(e.to);
    }
  }
  if (order.length !== g.vertexCount) throw new Error("Graph has a cycle");
  return order;
}

export interface ShortestPaths<T> {
  dist: Map<T, number>;
  prev: Map<T, T | null>;
}

// O((V + E) log V) time, O(V) space. Non-negative weights only; unreachable vertices get Infinity.
export function dijkstra<T>(g: Graph<T>, source: T): ShortestPaths<T> {
  g.neighbors(source);
  const dist = new Map<T, number>();
  const prev = new Map<T, T | null>();
  for (const v of g.vertices()) {
    dist.set(v, Infinity);
    prev.set(v, null);
  }
  dist.set(source, 0);
  const heap = new BinaryHeap<[number, T]>((a, b) => a[0] - b[0]);
  heap.push([0, source]);
  while (!heap.isEmpty()) {
    const [d, v] = heap.pop();
    if (d > (dist.get(v) as number)) continue;
    for (const e of g.neighbors(v)) {
      if (e.weight < 0) throw new RangeError("Dijkstra does not support negative weights");
      const nd = d + e.weight;
      if (nd < (dist.get(e.to) as number)) {
        dist.set(e.to, nd);
        prev.set(e.to, v);
        heap.push([nd, e.to]);
      }
    }
  }
  return { dist, prev };
}

// O(V * E) time, O(V) space. Handles negative weights; throws if a negative cycle is reachable from source.
export function bellmanFord<T>(g: Graph<T>, source: T): ShortestPaths<T> {
  g.neighbors(source);
  const dist = new Map<T, number>();
  const prev = new Map<T, T | null>();
  for (const v of g.vertices()) {
    dist.set(v, Infinity);
    prev.set(v, null);
  }
  dist.set(source, 0);
  const arcs: { from: T; to: T; weight: number }[] = [];
  for (const v of g.vertices()) for (const e of g.neighbors(v)) arcs.push({ from: v, to: e.to, weight: e.weight });
  const relax = (): boolean => {
    let changed = false;
    for (const { from, to, weight } of arcs) {
      const d = dist.get(from) as number;
      if (d !== Infinity && d + weight < (dist.get(to) as number)) {
        dist.set(to, d + weight);
        prev.set(to, from);
        changed = true;
      }
    }
    return changed;
  };
  for (let i = 0; i < g.vertexCount - 1; i++) if (!relax()) return { dist, prev };
  if (relax()) throw new Error("Graph has a negative cycle");
  return { dist, prev };
}

// O(k) time. Rebuilds the path to target from a Dijkstra or Bellman-Ford result, or null if unreachable.
export function pathTo<T>(result: ShortestPaths<T>, target: T): T[] | null {
  if (result.dist.get(target) === undefined || result.dist.get(target) === Infinity) return null;
  const path: T[] = [];
  for (let cur: T | null | undefined = target; cur !== null && cur !== undefined; cur = result.prev.get(cur)) {
    path.push(cur);
  }
  return path.reverse();
}

export interface MST<T> {
  weight: number;
  edges: { from: T; to: T; weight: number }[];
}

// O(E log E) time, O(V) space. Minimum spanning forest of an undirected graph.
export function kruskal<T>(g: Graph<T>): MST<T> {
  if (g.directed) throw new Error("Kruskal needs an undirected graph");
  const index = new Map<T, number>();
  g.vertices().forEach((v, i) => index.set(v, i));
  const uf = new UnionFind(g.vertexCount);
  const sorted = [...g.edges()].sort((a, b) => a.weight - b.weight);
  const edges: MST<T>["edges"] = [];
  let weight = 0;
  for (const e of sorted) {
    if (uf.union(index.get(e.from) as number, index.get(e.to) as number)) {
      edges.push(e);
      weight += e.weight;
    }
  }
  return { weight, edges };
}

// O(E log V) time, O(V + E) space. Minimum spanning forest of an undirected graph, growing a tree from each component.
export function prim<T>(g: Graph<T>): MST<T> {
  if (g.directed) throw new Error("Prim needs an undirected graph");
  const inTree = new Set<T>();
  const edges: MST<T>["edges"] = [];
  let weight = 0;
  for (const root of g.vertices()) {
    if (inTree.has(root)) continue;
    inTree.add(root);
    const heap = new BinaryHeap<{ from: T; to: T; weight: number }>((a, b) => a.weight - b.weight);
    for (const e of g.neighbors(root)) heap.push({ from: root, ...e });
    while (!heap.isEmpty()) {
      const e = heap.pop();
      if (inTree.has(e.to)) continue;
      inTree.add(e.to);
      edges.push(e);
      weight += e.weight;
      for (const n of g.neighbors(e.to)) if (!inTree.has(n.to)) heap.push({ from: e.to, ...n });
    }
  }
  return { weight, edges };
}

// O(V + E) time, O(V) space. True if the graph contains a cycle (directed: any directed cycle;
// undirected: any cycle, with a self loop or a parallel edge counting as one).
export function hasCycle<T>(g: Graph<T>): boolean {
  return g.directed ? hasDirectedCycle(g) : hasUndirectedCycle(g);
}

function hasDirectedCycle<T>(g: Graph<T>): boolean {
  const state = new Map<T, 1 | 2>(); // 1 = on the current path, 2 = finished
  for (const root of g.vertices()) {
    if (state.has(root)) continue;
    const stack: { v: T; i: number }[] = [{ v: root, i: 0 }];
    state.set(root, 1);
    while (stack.length) {
      const top = stack[stack.length - 1] as { v: T; i: number };
      const nbrs = g.neighbors(top.v);
      if (top.i === nbrs.length) {
        state.set(top.v, 2);
        stack.pop();
        continue;
      }
      const to = (nbrs[top.i++] as { to: T }).to;
      const s = state.get(to);
      if (s === 1) return true;
      if (s === undefined) {
        state.set(to, 1);
        stack.push({ v: to, i: 0 });
      }
    }
  }
  return false;
}

function hasUndirectedCycle<T>(g: Graph<T>): boolean {
  const seen = new Set<T>();
  for (const root of g.vertices()) {
    if (seen.has(root)) continue;
    seen.add(root);
    // Each stack entry remembers the vertex we came from; one copy of the parent edge is skipped.
    const stack: { v: T; parent: T | undefined }[] = [{ v: root, parent: undefined }];
    while (stack.length) {
      const cur = stack.pop() as { v: T; parent: T | undefined };
      let skipped = false;
      for (const e of g.neighbors(cur.v)) {
        if (e.to === cur.parent && !skipped) {
          skipped = true;
          continue;
        }
        if (seen.has(e.to)) return true;
        seen.add(e.to);
        stack.push({ v: e.to, parent: cur.v });
      }
    }
  }
  return false;
}

// O(V + E) time, O(V) space. Connected components of an undirected graph.
export function connectedComponents<T>(g: Graph<T>): T[][] {
  const seen = new Set<T>();
  const out: T[][] = [];
  for (const v of g.vertices()) {
    if (seen.has(v)) continue;
    const comp = bfs(g, v);
    comp.forEach((x) => seen.add(x));
    out.push(comp);
  }
  return out;
}
