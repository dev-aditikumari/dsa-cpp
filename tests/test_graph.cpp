#include <set>

#include "doctest.h"
#include "dsa/graph.hpp"
#include "dsa/union_find.hpp"
#include "rng.hpp"

using namespace dsa;

TEST_CASE("undirected edges are symmetric") {
    Graph g(2);
    g.add_edge(0, 1, 4);
    CHECK(g.has_edge(0, 1));
    CHECK(g.has_edge(1, 0));
    CHECK(g.edge_count() == 1);
    CHECK(g.neighbors(1).size() == 1);
    CHECK(g.neighbors(1)[0].to == 0);
    CHECK(g.neighbors(1)[0].weight == 4);
}

TEST_CASE("directed edges are one way") {
    Graph g(3, true);
    g.add_edge(1, 2);
    CHECK(g.has_edge(1, 2));
    CHECK_FALSE(g.has_edge(2, 1));
}

TEST_CASE("self loops and parallel edges") {
    Graph g(3);
    g.add_edge(0, 0);
    g.add_edge(1, 2);
    g.add_edge(1, 2);
    CHECK(g.neighbors(0).size() == 1);
    CHECK(g.neighbors(1).size() == 2);
    CHECK(g.edge_count() == 3);
    CHECK(g.add_vertex() == 3);
    CHECK(g.vertex_count() == 4);
}

TEST_CASE("graph rejects unknown vertices") {
    Graph g(2);
    CHECK_THROWS_AS(g.neighbors(5), std::out_of_range);
    CHECK_THROWS_AS(g.add_edge(0, 2), std::out_of_range);
    CHECK_THROWS_AS(g.has_edge(2, 0), std::out_of_range);
}

TEST_CASE("UnionFind unions and queries") {
    UnionFind uf(5);
    CHECK(uf.component_count() == 5);
    CHECK(uf.unite(0, 1));
    CHECK_FALSE(uf.unite(1, 0));
    uf.unite(3, 4);
    CHECK(uf.connected(0, 1));
    CHECK_FALSE(uf.connected(0, 3));
    uf.unite(1, 4);
    CHECK(uf.connected(0, 3));
    CHECK(uf.component_count() == 2);
}

TEST_CASE("UnionFind rejects bad input and allows size 0") {
    UnionFind uf(2);
    CHECK_THROWS_AS(uf.find(2), std::out_of_range);
    CHECK(UnionFind(0).component_count() == 0);
}

TEST_CASE("UnionFind matches a label-based reference on random unions") {
    Rng rng(61);
    const int n = 60;
    UnionFind uf(n);
    std::vector<int> label(n);
    for (int i = 0; i < n; ++i) label[static_cast<std::size_t>(i)] = i;
    for (int i = 0; i < 400; ++i) {
        auto a = static_cast<std::size_t>(rng.in(0, n - 1));
        auto b = static_cast<std::size_t>(rng.in(0, n - 1));
        int la = label[a], lb = label[b];
        CHECK(uf.unite(a, b) == (la != lb));
        for (int& l : label) {
            if (l == lb) l = la;
        }
        auto x = static_cast<std::size_t>(rng.in(0, n - 1));
        auto y = static_cast<std::size_t>(rng.in(0, n - 1));
        CHECK(uf.connected(x, y) == (label[x] == label[y]));
    }
    std::set<int> distinct(label.begin(), label.end());
    CHECK(uf.component_count() == distinct.size());
}

TEST_CASE("UnionFind handles a 200000 element chain") {
    const std::size_t n = 200000;
    UnionFind uf(n);
    for (std::size_t i = 1; i < n; ++i) uf.unite(i - 1, i);
    CHECK(uf.component_count() == 1);
    CHECK(uf.connected(0, n - 1));
}
