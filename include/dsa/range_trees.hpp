#pragma once
// FenwickTree: add/prefix_sum/range_sum O(log n), build O(n), space O(n).
// SegmentTree: generic associative combine, query/set O(log n), build O(n), space O(n).
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

class FenwickTree {
public:
    explicit FenwickTree(const std::vector<long long>& values) : tree_(values.size() + 1, 0) {
        for (std::size_t i = 1; i <= values.size(); ++i) {
            tree_[i] += values[i - 1];
            std::size_t parent = i + (i & (~i + 1));
            if (parent < tree_.size()) tree_[parent] += tree_[i];
        }
    }

    std::size_t size() const { return tree_.size() - 1; }

    void add(std::size_t index, long long delta) {
        check(index);
        for (std::size_t i = index + 1; i < tree_.size(); i += i & (~i + 1)) tree_[i] += delta;
    }

    // Sum of elements [0, count), so prefix_sum(0) is 0.
    long long prefix_sum(std::size_t count) const {
        if (count > size()) throw std::out_of_range("prefix too long");
        long long sum = 0;
        for (std::size_t i = count; i > 0; i -= i & (~i + 1)) sum += tree_[i];
        return sum;
    }

    // Sum of elements [left, right], inclusive.
    long long range_sum(std::size_t left, std::size_t right) const {
        check(left);
        check(right);
        if (left > right) throw std::invalid_argument("left must not exceed right");
        return prefix_sum(right + 1) - prefix_sum(left);
    }

private:
    void check(std::size_t i) const {
        if (i >= size()) throw std::out_of_range("index out of range");
    }
    std::vector<long long> tree_;
};

template <typename T, typename Combine>
class SegmentTree {
public:
    SegmentTree(const std::vector<T>& values, Combine combine, T identity)
        : n_(values.size()), tree_(2 * values.size(), identity), combine_(std::move(combine)), identity_(std::move(identity)) {
        for (std::size_t i = 0; i < n_; ++i) tree_[n_ + i] = values[i];
        for (std::size_t i = n_; i-- > 1;) tree_[i] = combine_(tree_[2 * i], tree_[2 * i + 1]);
    }

    std::size_t size() const { return n_; }

    void set(std::size_t index, T value) {
        check(index);
        std::size_t i = index + n_;
        tree_[i] = std::move(value);
        for (i /= 2; i >= 1; i /= 2) tree_[i] = combine_(tree_[2 * i], tree_[2 * i + 1]);
    }

    // Combines elements [left, right] inclusive, keeping left-to-right order for non-commutative operations.
    T query(std::size_t left, std::size_t right) const {
        check(left);
        check(right);
        if (left > right) throw std::invalid_argument("left must not exceed right");
        T res_left = identity_;
        T res_right = identity_;
        std::size_t l = left + n_;
        std::size_t r = right + n_ + 1;
        while (l < r) {
            if (l & 1) res_left = combine_(res_left, tree_[l++]);
            if (r & 1) res_right = combine_(tree_[--r], res_right);
            l /= 2;
            r /= 2;
        }
        return combine_(res_left, res_right);
    }

private:
    void check(std::size_t i) const {
        if (i >= n_) throw std::out_of_range("index out of range");
    }

    std::size_t n_;
    std::vector<T> tree_;
    Combine combine_;
    T identity_;
};

}  // namespace dsa
