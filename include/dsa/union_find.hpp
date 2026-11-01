#pragma once
// Union-find with path compression and union by rank. find/unite O(alpha(n)) amortised, space O(n).
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace dsa {

class UnionFind {
public:
    explicit UnionFind(std::size_t n) : parent_(n), rank_(n, 0), sets_(n) { std::iota(parent_.begin(), parent_.end(), 0); }

    std::size_t component_count() const { return sets_; }

    std::size_t find(std::size_t x) {
        if (x >= parent_.size()) throw std::out_of_range("element out of range");
        std::size_t root = x;
        while (parent_[root] != root) root = parent_[root];
        while (parent_[x] != root) {
            std::size_t next = parent_[x];
            parent_[x] = root;
            x = next;
        }
        return root;
    }

    // Returns false if a and b were already in the same set.
    bool unite(std::size_t a, std::size_t b) {
        std::size_t ra = find(a);
        std::size_t rb = find(b);
        if (ra == rb) return false;
        if (rank_[ra] < rank_[rb]) std::swap(ra, rb);
        parent_[rb] = ra;
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
        --sets_;
        return true;
    }

    bool connected(std::size_t a, std::size_t b) { return find(a) == find(b); }

private:
    std::vector<std::size_t> parent_;
    std::vector<int> rank_;
    std::size_t sets_;
};

}  // namespace dsa
