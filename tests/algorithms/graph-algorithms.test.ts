import { Graph } from "../../src/data-structures/graph";
import * as G from "../../src/algorithms/graph-algorithms";
import { makeRng, randInt } from "../helpers/rng";

const sample = () => {
  const g = new Graph<string>();
  g.addEdge("a", "b");
  g.addEdge("a", "c");
  g.addEdge("b", "d");
  g.addEdge("c", "d");
  g.addEdge("d", "e");
  g.addVertex("island");
  return g;
};

describe("traversals", () => {
  it("bfs and dfs visit in expected order", () => {
    const g = sample();
    expect(G.bfs(g, "a")).toEqual(["a", "b", "c", "d", "e"]);
    expect(G.dfs(g, "a")).toEqual(["a", "b", "d", "c", "e"]);
    expect(G.bfs(g, "island")).toEqual(["island"]);
  });

  it("throw for an unknown start", () => {
    expect(() => G.bfs(sample(), "zz")).toThrow("Unknown vertex");
    expect(() => G.dfs(sample(), "zz")).toThrow("Unknown vertex");
  });

  it("shortest unweighted path", () => {
    const g = sample();
    expect(G.shortestPathUnweighted(g, "a", "e")).toEqual(["a", "b", "d", "e"]);
    expect(G.shortestPathUnweighted(g, "a", "a")).toEqual(["a"]);
    expect(G.shortestPathUnweighted(g, "a", "island")).toBeNull();
  });

  it("dfs and bfs cover a 100000-vertex path without overflowing", () => {
    const g = new Graph<number>();
    for (let i = 1; i < 100000; i++) g.addEdge(i - 1, i);
    expect(G.dfs(g, 0)).toHaveLength(100000);
    expect(G.bfs(g, 0)).toHaveLength(100000);
  });

  it("connected components", () => {
    const comps = G.connectedComponents(sample());
    expect(comps.map((c) => c.length).sort()).toEqual([1, 5]);
  });
});

describe("topologicalSort", () => {
  it("orders dependencies before dependents", () => {
    const g = new Graph<string>(true);
    g.addEdge("shirt", "tie");
    g.addEdge("tie", "jacket");
    g.addEdge("pants", "shoes");
    g.addEdge("pants", "belt");
    g.addEdge("belt", "jacket");
    const order = G.topologicalSort(g);
    expect(order).toHaveLength(g.vertexCount);
    for (const e of g.edges()) expect(order.indexOf(e.from)).toBeLessThan(order.indexOf(e.to));
  });

  it("rejects cycles and undirected graphs, accepts empty", () => {
    const g = new Graph<number>(true);
    g.addEdge(1, 2);
    g.addEdge(2, 1);
    expect(() => G.topologicalSort(g)).toThrow("cycle");
    expect(() => G.topologicalSort(new Graph<number>())).toThrow("directed");
    expect(G.topologicalSort(new Graph<number>(true))).toEqual([]);
  });
});

function randomGraph(rng: () => number, n: number, edges: number, directed: boolean, minW: number, maxW: number) {
  const g = new Graph<number>(directed);
  for (let i = 0; i < n; i++) g.addVertex(i);
  for (let i = 0; i < edges; i++) g.addEdge(randInt(rng, 0, n - 1), randInt(rng, 0, n - 1), randInt(rng, minW, maxW));
  return g;
}

// Brute force: Floyd-Warshall on the same adjacency data.
function floyd(g: Graph<number>): number[][] {
  const n = g.vertexCount;
  const d = Array.from({ length: n }, (_, i) => Array.from({ length: n }, (_, j) => (i === j ? 0 : Infinity)));
  for (const e of g.edges()) {
    const row = d[e.from] as number[];
    row[e.to] = Math.min(row[e.to] as number, e.weight);
    if (!g.directed) {
      const back = d[e.to] as number[];
      back[e.from] = Math.min(back[e.from] as number, e.weight);
    }
  }
  for (let k = 0; k < n; k++)
    for (let i = 0; i < n; i++)
      for (let j = 0; j < n; j++) {
        const via = ((d[i] as number[])[k] as number) + ((d[k] as number[])[j] as number);
        if (via < ((d[i] as number[])[j] as number)) (d[i] as number[])[j] = via;
      }
  return d;
}

describe("shortest paths", () => {
  it("dijkstra on a known graph", () => {
    const g = new Graph<string>(true);
    g.addEdge("s", "a", 7);
    g.addEdge("s", "b", 2);
    g.addEdge("b", "a", 3);
    g.addEdge("a", "t", 1);
    g.addVertex("x");
    const r = G.dijkstra(g, "s");
    expect(r.dist.get("t")).toBe(6);
    expect(r.dist.get("x")).toBe(Infinity);
    expect(G.pathTo(r, "t")).toEqual(["s", "b", "a", "t"]);
    expect(G.pathTo(r, "x")).toBeNull();
    expect(G.pathTo(r, "s")).toEqual(["s"]);
  });

  it("dijkstra rejects negative weights and unknown source", () => {
    const g = new Graph<number>(true);
    g.addEdge(0, 1, -1);
    expect(() => G.dijkstra(g, 0)).toThrow(RangeError);
    expect(() => G.dijkstra(g, 5)).toThrow();
  });

  it("dijkstra matches Floyd-Warshall on 150 random graphs (directed and undirected)", () => {
    const rng = makeRng(121);
    for (let t = 0; t < 150; t++) {
      const n = randInt(rng, 1, 8);
      const g = randomGraph(rng, n, randInt(rng, 0, 20), t % 2 === 0, 0, 9);
      const ref = floyd(g);
      const src = randInt(rng, 0, n - 1);
      const { dist } = G.dijkstra(g, src);
      for (let v = 0; v < n; v++) expect(dist.get(v)).toBe((ref[src] as number[])[v]);
    }
  });

  it("bellmanFord handles negative edges and matches Floyd-Warshall when there is no negative cycle", () => {
    const g = new Graph<string>(true);
    g.addEdge("a", "b", 4);
    g.addEdge("a", "c", 5);
    g.addEdge("b", "c", -3);
    const r = G.bellmanFord(g, "a");
    expect(r.dist.get("c")).toBe(1);
    expect(G.pathTo(r, "c")).toEqual(["a", "b", "c"]);

    const rng = makeRng(122);
    let checked = 0;
    for (let t = 0; t < 300; t++) {
      const n = randInt(rng, 1, 7);
      const dag = new Graph<number>(true);
      for (let i = 0; i < n; i++) dag.addVertex(i);
      // Edges only go from lower to higher index, so no cycles even with negative weights.
      for (let i = 0; i < randInt(rng, 0, 15); i++) {
        const a = randInt(rng, 0, n - 1);
        const b = randInt(rng, 0, n - 1);
        if (a < b) dag.addEdge(a, b, randInt(rng, -5, 9));
      }
      const ref = floyd(dag);
      const src = randInt(rng, 0, n - 1);
      const { dist } = G.bellmanFord(dag, src);
      for (let v = 0; v < n; v++) expect(dist.get(v)).toBe((ref[src] as number[])[v]);
      checked++;
    }
    expect(checked).toBe(300);
  });

  it("bellmanFord detects a negative cycle", () => {
    const g = new Graph<number>(true);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, -3);
    g.addEdge(2, 1, 1);
    expect(() => G.bellmanFord(g, 0)).toThrow("negative cycle");
  });
});

describe("minimum spanning trees", () => {
  it("kruskal and prim find the known MST weight", () => {
    const g = new Graph<string>();
    g.addEdge("a", "b", 1);
    g.addEdge("b", "c", 2);
    g.addEdge("a", "c", 3);
    g.addEdge("c", "d", 4);
    g.addEdge("b", "d", 10);
    expect(G.kruskal(g).weight).toBe(7);
    expect(G.prim(g).weight).toBe(7);
    expect(G.kruskal(g).edges).toHaveLength(3);
  });

  it("handle empty graphs, forests and directed input", () => {
    expect(G.kruskal(new Graph<number>()).weight).toBe(0);
    expect(G.prim(new Graph<number>()).edges).toEqual([]);
    const g = new Graph<number>();
    g.addEdge(0, 1, 2);
    g.addEdge(2, 3, 5);
    expect(G.kruskal(g).weight).toBe(7);
    expect(G.prim(g).weight).toBe(7);
    expect(() => G.kruskal(new Graph<number>(true))).toThrow();
    expect(() => G.prim(new Graph<number>(true))).toThrow();
  });

  it("agree with each other and with brute-force subset search on 100 random graphs", () => {
    const rng = makeRng(123);
    for (let t = 0; t < 100; t++) {
      const n = randInt(rng, 1, 6);
      const g = randomGraph(rng, n, randInt(rng, 0, 9), false, 1, 9);
      const k = G.kruskal(g);
      const p = G.prim(g);
      expect(p.weight).toBe(k.weight);
      expect(p.edges.length).toBe(k.edges.length);

      // Brute force: lightest edge subset that spans every component of g.
      const edges = g.edges();
      const comps = G.connectedComponents(g).length;
      let best = Infinity;
      for (let mask = 0; mask < 1 << edges.length; mask++) {
        const h = new Graph<number>();
        for (let i = 0; i < n; i++) h.addVertex(i);
        let w = 0;
        edges.forEach((e, i) => {
          if (mask & (1 << i)) {
            h.addEdge(e.from, e.to, e.weight);
            w += e.weight;
          }
        });
        if (G.connectedComponents(h).length === comps) best = Math.min(best, w);
      }
      expect(k.weight).toBe(best);
    }
  });
});

describe("hasCycle", () => {
  it("directed", () => {
    const g = new Graph<number>(true);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    expect(G.hasCycle(g)).toBe(false);
    g.addEdge(4, 1);
    expect(G.hasCycle(g)).toBe(true);
    const loop = new Graph<number>(true);
    loop.addEdge(1, 1);
    expect(G.hasCycle(loop)).toBe(true);
    expect(G.hasCycle(new Graph<number>(true))).toBe(false);
  });

  it("undirected, including parallel edges and self loops", () => {
    const g = new Graph<number>();
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    expect(G.hasCycle(g)).toBe(false);
    g.addEdge(3, 1);
    expect(G.hasCycle(g)).toBe(true);
    const par = new Graph<number>();
    par.addEdge(1, 2);
    par.addEdge(1, 2);
    expect(G.hasCycle(par)).toBe(true);
    const loop = new Graph<number>();
    loop.addEdge(1, 1);
    expect(G.hasCycle(loop)).toBe(true);
  });

  it("agrees with the edge-count rule (forest iff E = V - components) on random undirected graphs", () => {
    const rng = makeRng(124);
    for (let t = 0; t < 300; t++) {
      const n = randInt(rng, 1, 9);
      const g = randomGraph(rng, n, randInt(rng, 0, 10), false, 1, 1);
      const comps = G.connectedComponents(g).length;
      expect(G.hasCycle(g)).toBe(g.edgeCount !== n - comps);
    }
  });

  it("agrees with topological sort on random directed graphs", () => {
    const rng = makeRng(125);
    for (let t = 0; t < 300; t++) {
      const n = randInt(rng, 1, 8);
      const g = randomGraph(rng, n, randInt(rng, 0, 12), true, 1, 1);
      let acyclic = true;
      try {
        G.topologicalSort(g);
      } catch {
        acyclic = false;
      }
      expect(G.hasCycle(g)).toBe(!acyclic);
    }
  });
});
