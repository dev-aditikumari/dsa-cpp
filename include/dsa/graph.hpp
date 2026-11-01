#pragma once
// Adjacency-list graph over vertices 0..n-1 with integer weights, directed or undirected.
// add_edge O(1) amortised, neighbors O(1), space O(V + E). Parallel edges and self loops are allowed.
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace dsa {

struct Edge {
    std::size_t to;
    long long weight;
};

struct WeightedEdge {
    std::size_t from;
    std::size_t to;
    long long weight;
};

class Graph {
public:
    explicit Graph(std::size_t vertices = 0, bool directed = false) : adj_(vertices), directed_(directed) {}

    bool directed() const { return directed_; }
    std::size_t vertex_count() const { return adj_.size(); }
    std::size_t edge_count() const { return edges_.size(); }

    std::size_t add_vertex() {
        adj_.emplace_back();
        return adj_.size() - 1;
    }

    void add_edge(std::size_t from, std::size_t to, long long weight = 1) {
        check(from);
        check(to);
        adj_[from].push_back({to, weight});
        if (!directed_ && from != to) adj_[to].push_back({from, weight});
        edges_.push_back({from, to, weight});
    }

    bool has_edge(std::size_t from, std::size_t to) const {
        check(from);
        check(to);
        for (const Edge& e : adj_[from]) {
            if (e.to == to) return true;
        }
        return false;
    }

    const std::vector<Edge>& neighbors(std::size_t v) const {
        check(v);
        return adj_[v];
    }

    // Each edge once, in insertion order.
    const std::vector<WeightedEdge>& edges() const { return edges_; }

private:
    void check(std::size_t v) const {
        if (v >= adj_.size()) throw std::out_of_range("unknown vertex");
    }

    std::vector<std::vector<Edge>> adj_;
    std::vector<WeightedEdge> edges_;
    bool directed_;
};

}  // namespace dsa
