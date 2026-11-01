import { Graph } from "../../src/data-structures/graph";
import { UnionFind } from "../../src/data-structures/union-find";
import { makeRng, randInt } from "../helpers/rng";

describe("Graph", () => {
  it("undirected edges are symmetric", () => {
    const g = new Graph<string>();
    g.addEdge("a", "b", 4);
    expect(g.hasEdge("a", "b")).toBe(true);
    expect(g.hasEdge("b", "a")).toBe(true);
    expect(g.edgeCount).toBe(1);
    expect(g.vertexCount).toBe(2);
    expect(g.neighbors("b")).toEqual([{ to: "a", weight: 4 }]);
  });

  it("directed edges are one way", () => {
    const g = new Graph<number>(true);
    g.addEdge(1, 2);
    expect(g.hasEdge(1, 2)).toBe(true);
    expect(g.hasEdge(2, 1)).toBe(false);
    expect(g.hasEdge(9, 1)).toBe(false);
  });

  it("supports isolated vertices, self loops and parallel edges", () => {
    const g = new Graph<number>();
    g.addVertex(1);
    g.addVertex(1);
    g.addEdge(2, 2);
    g.addEdge(3, 4);
    g.addEdge(3, 4);
    expect(g.vertexCount).toBe(4);
    expect(g.neighbors(2)).toHaveLength(1);
    expect(g.neighbors(3)).toHaveLength(2);
    expect(g.edgeCount).toBe(3);
  });

  it("removes a vertex and its edges", () => {
    const g = new Graph<number>();
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    expect(g.removeVertex(2)).toBe(true);
    expect(g.removeVertex(2)).toBe(false);
    expect(g.vertices()).toEqual([1, 3]);
    expect(g.neighbors(1)).toEqual([]);
    expect(g.edgeCount).toBe(0);
  });

  it("throws on unknown vertex and NaN weight", () => {
    const g = new Graph<number>();
    expect(() => g.neighbors(1)).toThrow("Unknown vertex");
    expect(() => g.addEdge(1, 2, NaN)).toThrow(RangeError);
  });
});

describe("UnionFind", () => {
  it("unions and queries", () => {
    const uf = new UnionFind(5);
    expect(uf.componentCount).toBe(5);
    expect(uf.union(0, 1)).toBe(true);
    expect(uf.union(1, 0)).toBe(false);
    uf.union(3, 4);
    expect(uf.connected(0, 1)).toBe(true);
    expect(uf.connected(0, 3)).toBe(false);
    uf.union(1, 4);
    expect(uf.connected(0, 3)).toBe(true);
    expect(uf.componentCount).toBe(2);
  });

  it("rejects bad input", () => {
    expect(() => new UnionFind(-1)).toThrow(RangeError);
    const uf = new UnionFind(2);
    expect(() => uf.find(2)).toThrow(RangeError);
    expect(() => uf.find(0.5)).toThrow(RangeError);
    expect(new UnionFind(0).componentCount).toBe(0);
  });

  it("matches a naive label-based reference on random unions", () => {
    const rng = makeRng(61);
    const n = 60;
    const uf = new UnionFind(n);
    const label = Array.from({ length: n }, (_, i) => i);
    for (let i = 0; i < 300; i++) {
      const a = randInt(rng, 0, n - 1);
      const b = randInt(rng, 0, n - 1);
      const la = label[a] as number;
      const lb = label[b] as number;
      expect(uf.union(a, b)).toBe(la !== lb);
      for (let k = 0; k < n; k++) if (label[k] === lb) label[k] = la;
      const x = randInt(rng, 0, n - 1);
      const y = randInt(rng, 0, n - 1);
      expect(uf.connected(x, y)).toBe(label[x] === label[y]);
    }
    expect(uf.componentCount).toBe(new Set(label).size);
  });

  it("handles a 100000-element chain", () => {
    const n = 100000;
    const uf = new UnionFind(n);
    for (let i = 1; i < n; i++) uf.union(i - 1, i);
    expect(uf.componentCount).toBe(1);
    expect(uf.connected(0, n - 1)).toBe(true);
  });
});
