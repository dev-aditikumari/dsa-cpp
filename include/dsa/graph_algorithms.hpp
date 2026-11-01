#pragma once
// Graph algorithms over dsa::Graph (vertices 0..n-1). V = vertices, E = edges.
#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "union_find.hpp"

namespace dsa {

constexpr long long kInfinity = std::numeric_limits<long long>::max();

// O(V + E) time, O(V) space. Vertices in breadth-first order from start.
inline std::vector<std::size_t> bfs(const Graph& g, std::size_t start) {
    g.neighbors(start);
    std::vector<bool> seen(g.vertex_count(), false);
    std::queue<std::size_t> q;
    std::vector<std::size_t> order;
    seen[start] = true;
    q.push(start);
    while (!q.empty()) {
        std::size_t v = q.front();
        q.pop();
        order.push_back(v);
        for (const Edge& e : g.neighbors(v)) {
            if (!seen[e.to]) {
                seen[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return order;
}

// O(V + E) time, O(V) space. Iterative depth-first order from start, neighbors in insertion order.
inline std::vector<std::size_t> dfs(const Graph& g, std::size_t start) {
    g.neighbors(start);
    std::vector<bool> seen(g.vertex_count(), false);
    std::vector<std::size_t> order, stack{start};
    while (!stack.empty()) {
        std::size_t v = stack.back();
        stack.pop_back();
        if (seen[v]) continue;
        seen[v] = true;
        order.push_back(v);
        const auto& next = g.neighbors(v);
        for (std::size_t i = next.size(); i-- > 0;) {
            if (!seen[next[i].to]) stack.push_back(next[i].to);
        }
    }
    return order;
}

// O(V + E) time, O(V) space. Fewest-edge path from start to goal, or nullopt.
inline std::optional<std::vector<std::size_t>> shortest_path_unweighted(const Graph& g, std::size_t start, std::size_t goal) {
    g.neighbors(start);
    g.neighbors(goal);
    constexpr std::size_t none = static_cast<std::size_t>(-1);
    std::vector<std::size_t> parent(g.vertex_count(), none);
    std::vector<bool> seen(g.vertex_count(), false);
    std::queue<std::size_t> q;
    seen[start] = true;
    q.push(start);
    while (!q.empty() && !seen[goal]) {
        std::size_t v = q.front();
        q.pop();
        for (const Edge& e : g.neighbors(v)) {
            if (!seen[e.to]) {
                seen[e.to] = true;
                parent[e.to] = v;
                q.push(e.to);
            }
        }
    }
    if (!seen[goal]) return std::nullopt;
    std::vector<std::size_t> path;
    for (std::size_t cur = goal; cur != none; cur = parent[cur]) path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}

// O(V + E) time, O(V) space. Connected components of an undirected graph, each in BFS order.
inline std::vector<std::vector<std::size_t>> connected_components(const Graph& g) {
    std::vector<bool> seen(g.vertex_count(), false);
    std::vector<std::vector<std::size_t>> out;
    for (std::size_t v = 0; v < g.vertex_count(); ++v) {
        if (seen[v]) continue;
        out.push_back(bfs(g, v));
        for (std::size_t x : out.back()) seen[x] = true;
    }
    return out;
}

// O(V + E) time, O(V) space. Kahn's algorithm; throws if the graph is undirected or has a cycle.
inline std::vector<std::size_t> topological_sort(const Graph& g) {
    if (!g.directed()) throw std::invalid_argument("topological sort needs a directed graph");
    std::vector<std::size_t> indegree(g.vertex_count(), 0);
    for (const WeightedEdge& e : g.edges()) ++indegree[e.to];
    std::queue<std::size_t> q;
    for (std::size_t v = 0; v < g.vertex_count(); ++v) {
        if (indegree[v] == 0) q.push(v);
    }
    std::vector<std::size_t> order;
    while (!q.empty()) {
        std::size_t v = q.front();
        q.pop();
        order.push_back(v);
        for (const Edge& e : g.neighbors(v)) {
            if (--indegree[e.to] == 0) q.push(e.to);
        }
    }
    if (order.size() != g.vertex_count()) throw std::runtime_error("graph has a cycle");
    return order;
}

struct ShortestPaths {
    std::vector<long long> dist;       // kInfinity when unreachable
    std::vector<std::size_t> parent;   // vertex count when there is no parent

    // Path from the source to target, or nullopt when target is unreachable.
    std::optional<std::vector<std::size_t>> path_to(std::size_t target) const {
        if (target >= dist.size()) throw std::out_of_range("unknown vertex");
        if (dist[target] == kInfinity) return std::nullopt;
        std::vector<std::size_t> path;
        for (std::size_t cur = target; cur != dist.size(); cur = parent[cur]) path.push_back(cur);
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// O((V + E) log V) time, O(V) space. Throws on a negative weight.
inline ShortestPaths dijkstra(const Graph& g, std::size_t source) {
    g.neighbors(source);
    const std::size_t n = g.vertex_count();
    ShortestPaths r{std::vector<long long>(n, kInfinity), std::vector<std::size_t>(n, n)};
    using Item = std::pair<long long, std::size_t>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;
    r.dist[source] = 0;
    pq.push({0, source});
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        if (d > r.dist[v]) continue;
        for (const Edge& e : g.neighbors(v)) {
            if (e.weight < 0) throw std::invalid_argument("dijkstra does not support negative weights");
            if (d + e.weight < r.dist[e.to]) {
                r.dist[e.to] = d + e.weight;
                r.parent[e.to] = v;
                pq.push({r.dist[e.to], e.to});
            }
        }
    }
    return r;
}

// O(V * E) time, O(V) space. Allows negative weights; throws if a negative cycle is reachable from source.
inline ShortestPaths bellman_ford(const Graph& g, std::size_t source) {
    g.neighbors(source);
    const std::size_t n = g.vertex_count();
    ShortestPaths r{std::vector<long long>(n, kInfinity), std::vector<std::size_t>(n, n)};
    r.dist[source] = 0;
    auto relax = [&]() {
        bool changed = false;
        for (const WeightedEdge& e : g.edges()) {
            auto try_arc = [&](std::size_t from, std::size_t to) {
                if (r.dist[from] != kInfinity && r.dist[from] + e.weight < r.dist[to]) {
                    r.dist[to] = r.dist[from] + e.weight;
                    r.parent[to] = from;
                    changed = true;
                }
            };
            try_arc(e.from, e.to);
            if (!g.directed()) try_arc(e.to, e.from);
        }
        return changed;
    };
    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (!relax()) return r;
    }
    if (relax()) throw std::runtime_error("graph has a negative cycle");
    return r;
}

struct MST {
    long long weight = 0;
    std::vector<WeightedEdge> edges;
};

// O(E log E) time, O(V) space. Minimum spanning forest of an undirected graph.
inline MST kruskal(const Graph& g) {
    if (g.directed()) throw std::invalid_argument("kruskal needs an undirected graph");
    std::vector<WeightedEdge> sorted = g.edges();
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const WeightedEdge& a, const WeightedEdge& b) { return a.weight < b.weight; });
    UnionFind uf(g.vertex_count());
    MST mst;
    for (const WeightedEdge& e : sorted) {
        if (uf.unite(e.from, e.to)) {
            mst.edges.push_back(e);
            mst.weight += e.weight;
        }
    }
    return mst;
}

// O(E log V) time, O(V + E) space. Minimum spanning forest, growing a tree from each unvisited vertex.
inline MST prim(const Graph& g) {
    if (g.directed()) throw std::invalid_argument("prim needs an undirected graph");
    std::vector<bool> in_tree(g.vertex_count(), false);
    MST mst;
    auto cmp = [](const WeightedEdge& a, const WeightedEdge& b) { return a.weight > b.weight; };
    for (std::size_t root = 0; root < g.vertex_count(); ++root) {
        if (in_tree[root]) continue;
        in_tree[root] = true;
        std::priority_queue<WeightedEdge, std::vector<WeightedEdge>, decltype(cmp)> pq(cmp);
        for (const Edge& e : g.neighbors(root)) pq.push({root, e.to, e.weight});
        while (!pq.empty()) {
            WeightedEdge e = pq.top();
            pq.pop();
            if (in_tree[e.to]) continue;
            in_tree[e.to] = true;
            mst.edges.push_back(e);
            mst.weight += e.weight;
            for (const Edge& n : g.neighbors(e.to)) {
                if (!in_tree[n.to]) pq.push({e.to, n.to, n.weight});
            }
        }
    }
    return mst;
}

namespace detail {
inline bool has_directed_cycle(const Graph& g) {
    enum : char { kNew = 0, kOnPath = 1, kDone = 2 };
    std::vector<char> state(g.vertex_count(), kNew);
    for (std::size_t root = 0; root < g.vertex_count(); ++root) {
        if (state[root] != kNew) continue;
        std::vector<std::pair<std::size_t, std::size_t>> stack{{root, 0}};  // vertex, next neighbor index
        state[root] = kOnPath;
        while (!stack.empty()) {
            auto& [v, i] = stack.back();
            const auto& nbrs = g.neighbors(v);
            if (i == nbrs.size()) {
                state[v] = kDone;
                stack.pop_back();
                continue;
            }
            std::size_t to = nbrs[i++].to;
            if (state[to] == kOnPath) return true;
            if (state[to] == kNew) {
                state[to] = kOnPath;
                stack.push_back({to, 0});
            }
        }
    }
    return false;
}

inline bool has_undirected_cycle(const Graph& g) {
    constexpr std::size_t none = static_cast<std::size_t>(-1);
    std::vector<bool> seen(g.vertex_count(), false);
    for (std::size_t root = 0; root < g.vertex_count(); ++root) {
        if (seen[root]) continue;
        seen[root] = true;
        std::vector<std::pair<std::size_t, std::size_t>> stack{{root, none}};  // vertex, parent
        while (!stack.empty()) {
            auto [v, parent] = stack.back();
            stack.pop_back();
            bool skipped_parent = false;  // only one copy of the edge back to the parent is the tree edge
            for (const Edge& e : g.neighbors(v)) {
                if (e.to == parent && !skipped_parent) {
                    skipped_parent = true;
                    continue;
                }
                if (seen[e.to]) return true;
                seen[e.to] = true;
                stack.push_back({e.to, v});
            }
        }
    }
    return false;
}
}  // namespace detail

// O(V + E) time, O(V) space. A self loop or a pair of parallel edges counts as a cycle in an undirected graph.
inline bool has_cycle(const Graph& g) {
    return g.directed() ? detail::has_directed_cycle(g) : detail::has_undirected_cycle(g);
}

}  // namespace dsa
