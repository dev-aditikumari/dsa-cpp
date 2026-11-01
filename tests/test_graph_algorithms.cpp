#include <algorithm>

#include "doctest.h"
#include "dsa/graph_algorithms.hpp"
#include "rng.hpp"

using namespace dsa;
using VV = std::vector<std::size_t>;

// 0-1, 0-2, 1-3, 2-3, 3-4 and an isolated vertex 5.
static Graph sample() {
    Graph g(6);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    return g;
}

static Graph random_graph(Rng& rng, int n, int edges, bool directed, int min_w, int max_w) {
    Graph g(static_cast<std::size_t>(n), directed);
    for (int i = 0; i < edges; ++i) {
        g.add_edge(rng.idx(0, static_cast<std::size_t>(n) - 1), rng.idx(0, static_cast<std::size_t>(n) - 1), rng.in(min_w, max_w));
    }
    return g;
}

// Brute force reference: Floyd-Warshall over the same edges.
static std::vector<std::vector<long long>> floyd(const Graph& g) {
    const std::size_t n = g.vertex_count();
    std::vector<std::vector<long long>> d(n, std::vector<long long>(n, kInfinity));
    for (std::size_t i = 0; i < n; ++i) d[i][i] = 0;
    for (const WeightedEdge& e : g.edges()) {
        d[e.from][e.to] = std::min(d[e.from][e.to], e.weight);
        if (!g.directed()) d[e.to][e.from] = std::min(d[e.to][e.from], e.weight);
    }
    for (std::size_t k = 0; k < n; ++k)
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                if (d[i][k] != kInfinity && d[k][j] != kInfinity) d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    return d;
}

TEST_CASE("bfs and dfs visit in the expected order") {
    Graph g = sample();
    CHECK(bfs(g, 0) == VV{0, 1, 2, 3, 4});
    CHECK(dfs(g, 0) == VV{0, 1, 3, 2, 4});
    CHECK(bfs(g, 5) == VV{5});
    CHECK_THROWS_AS(bfs(g, 9), std::out_of_range);
    CHECK_THROWS_AS(dfs(g, 9), std::out_of_range);
}

TEST_CASE("shortest unweighted path") {
    Graph g = sample();
    CHECK(shortest_path_unweighted(g, 0, 4).value() == VV{0, 1, 3, 4});
    CHECK(shortest_path_unweighted(g, 0, 0).value() == VV{0});
    CHECK_FALSE(shortest_path_unweighted(g, 0, 5).has_value());
}

TEST_CASE("traversals cover a 200000 vertex path without recursion") {
    Graph g(200000);
    for (std::size_t i = 1; i < 200000; ++i) g.add_edge(i - 1, i);
    CHECK(dfs(g, 0).size() == 200000);
    CHECK(bfs(g, 0).size() == 200000);
}

TEST_CASE("connected components") {
    auto comps = connected_components(sample());
    std::vector<std::size_t> sizes;
    for (const auto& c : comps) sizes.push_back(c.size());
    std::sort(sizes.begin(), sizes.end());
    CHECK(sizes == VV{1, 5});
}

TEST_CASE("topological sort orders dependencies first") {
    Graph g(6, true);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(3, 4);
    g.add_edge(3, 5);
    g.add_edge(5, 2);
    VV order = topological_sort(g);
    REQUIRE(order.size() == 6);
    auto pos = [&](std::size_t v) { return std::find(order.begin(), order.end(), v) - order.begin(); };
    for (const WeightedEdge& e : g.edges()) CHECK(pos(e.from) < pos(e.to));
}

TEST_CASE("topological sort rejects cycles and undirected graphs") {
    Graph g(2, true);
    g.add_edge(0, 1);
    g.add_edge(1, 0);
    CHECK_THROWS_AS(topological_sort(g), std::runtime_error);
    CHECK_THROWS_AS(topological_sort(Graph(2)), std::invalid_argument);
    CHECK(topological_sort(Graph(0, true)).empty());
}

TEST_CASE("dijkstra on a known graph") {
    Graph g(5, true);  // 0=s 1=a 2=b 3=t 4=x
    g.add_edge(0, 1, 7);
    g.add_edge(0, 2, 2);
    g.add_edge(2, 1, 3);
    g.add_edge(1, 3, 1);
    ShortestPaths r = dijkstra(g, 0);
    CHECK(r.dist[3] == 6);
    CHECK(r.dist[4] == kInfinity);
    CHECK(r.path_to(3).value() == VV{0, 2, 1, 3});
    CHECK_FALSE(r.path_to(4).has_value());
    CHECK(r.path_to(0).value() == VV{0});
    CHECK_THROWS_AS(r.path_to(9), std::out_of_range);
}

TEST_CASE("dijkstra rejects negative weights and unknown source") {
    Graph g(2, true);
    g.add_edge(0, 1, -1);
    CHECK_THROWS_AS(dijkstra(g, 0), std::invalid_argument);
    CHECK_THROWS_AS(dijkstra(g, 5), std::out_of_range);
}

TEST_CASE("dijkstra matches Floyd-Warshall on 200 random graphs") {
    Rng rng(121);
    for (int t = 0; t < 200; ++t) {
        int n = rng.in(1, 8);
        Graph g = random_graph(rng, n, rng.in(0, 20), t % 2 == 0, 0, 9);
        auto ref = floyd(g);
        std::size_t src = rng.idx(0, static_cast<std::size_t>(n) - 1);
        ShortestPaths r = dijkstra(g, src);
        for (std::size_t v = 0; v < static_cast<std::size_t>(n); ++v) REQUIRE(r.dist[v] == ref[src][v]);
    }
}

TEST_CASE("bellman-ford handles negative edges") {
    Graph g(3, true);
    g.add_edge(0, 1, 4);
    g.add_edge(0, 2, 5);
    g.add_edge(1, 2, -3);
    ShortestPaths r = bellman_ford(g, 0);
    CHECK(r.dist[2] == 1);
    CHECK(r.path_to(2).value() == VV{0, 1, 2});
}

TEST_CASE("bellman-ford matches Floyd-Warshall on random acyclic graphs with negative weights") {
    Rng rng(122);
    for (int t = 0; t < 300; ++t) {
        int n = rng.in(1, 7);
        Graph g(static_cast<std::size_t>(n), true);
        for (int i = 0, m = rng.in(0, 15); i < m; ++i) {
            auto a = static_cast<std::size_t>(rng.in(0, n - 1));
            auto b = static_cast<std::size_t>(rng.in(0, n - 1));
            if (a < b) g.add_edge(a, b, rng.in(-5, 9));  // edges only go upward, so there is no cycle
        }
        auto ref = floyd(g);
        std::size_t src = rng.idx(0, static_cast<std::size_t>(n) - 1);
        ShortestPaths r = bellman_ford(g, src);
        for (std::size_t v = 0; v < static_cast<std::size_t>(n); ++v) REQUIRE(r.dist[v] == ref[src][v]);
    }
}

TEST_CASE("bellman-ford detects a negative cycle") {
    Graph g(3, true);
    g.add_edge(0, 1, 1);
    g.add_edge(1, 2, -3);
    g.add_edge(2, 1, 1);
    CHECK_THROWS_AS(bellman_ford(g, 0), std::runtime_error);
}

TEST_CASE("kruskal and prim find a known MST weight") {
    Graph g(4);
    g.add_edge(0, 1, 1);
    g.add_edge(1, 2, 2);
    g.add_edge(0, 2, 3);
    g.add_edge(2, 3, 4);
    g.add_edge(1, 3, 10);
    CHECK(kruskal(g).weight == 7);
    CHECK(prim(g).weight == 7);
    CHECK(kruskal(g).edges.size() == 3);
}

TEST_CASE("MST edge cases") {
    CHECK(kruskal(Graph(0)).weight == 0);
    CHECK(prim(Graph(0)).edges.empty());
    Graph g(4);
    g.add_edge(0, 1, 2);
    g.add_edge(2, 3, 5);
    CHECK(kruskal(g).weight == 7);
    CHECK(prim(g).weight == 7);
    CHECK_THROWS_AS(kruskal(Graph(1, true)), std::invalid_argument);
    CHECK_THROWS_AS(prim(Graph(1, true)), std::invalid_argument);
}

TEST_CASE("kruskal and prim equal a brute-force subset search on 120 random graphs") {
    Rng rng(123);
    for (int t = 0; t < 120; ++t) {
        int n = rng.in(1, 6);
        Graph g = random_graph(rng, n, rng.in(0, 9), false, 1, 9);
        MST k = kruskal(g), p = prim(g);
        CHECK(p.weight == k.weight);
        CHECK(p.edges.size() == k.edges.size());
        const auto& edges = g.edges();
        std::size_t comps = connected_components(g).size();
        long long best = kInfinity;
        for (unsigned mask = 0; mask < (1u << edges.size()); ++mask) {
            Graph h(static_cast<std::size_t>(n));
            long long w = 0;
            for (std::size_t i = 0; i < edges.size(); ++i) {
                if (mask & (1u << i)) {
                    h.add_edge(edges[i].from, edges[i].to, edges[i].weight);
                    w += edges[i].weight;
                }
            }
            if (connected_components(h).size() == comps) best = std::min(best, w);
        }
        REQUIRE(k.weight == best);
    }
}

TEST_CASE("directed cycle detection") {
    Graph g(5, true);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    CHECK_FALSE(has_cycle(g));
    g.add_edge(3, 0);
    CHECK(has_cycle(g));
    Graph loop(1, true);
    loop.add_edge(0, 0);
    CHECK(has_cycle(loop));
    CHECK_FALSE(has_cycle(Graph(0, true)));
}

TEST_CASE("undirected cycle detection including parallel edges and self loops") {
    Graph g(3);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    CHECK_FALSE(has_cycle(g));
    g.add_edge(2, 0);
    CHECK(has_cycle(g));
    Graph par(2);
    par.add_edge(0, 1);
    par.add_edge(0, 1);
    CHECK(has_cycle(par));
    Graph loop(1);
    loop.add_edge(0, 0);
    CHECK(has_cycle(loop));
}

TEST_CASE("undirected has_cycle agrees with the forest edge-count rule") {
    Rng rng(124);
    for (int t = 0; t < 400; ++t) {
        int n = rng.in(1, 9);
        Graph g = random_graph(rng, n, rng.in(0, 10), false, 1, 1);
        std::size_t comps = connected_components(g).size();
        REQUIRE(has_cycle(g) == (g.edge_count() != static_cast<std::size_t>(n) - comps));
    }
}

TEST_CASE("directed has_cycle agrees with topological sort") {
    Rng rng(125);
    for (int t = 0; t < 400; ++t) {
        int n = rng.in(1, 8);
        Graph g = random_graph(rng, n, rng.in(0, 12), true, 1, 1);
        bool acyclic = true;
        try {
            topological_sort(g);
        } catch (const std::runtime_error&) {
            acyclic = false;
        }
        REQUIRE(has_cycle(g) == !acyclic);
    }
}
