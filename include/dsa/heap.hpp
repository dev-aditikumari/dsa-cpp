#pragma once
// BinaryHeap: push/pop O(log n), top O(1), build from a vector O(n), space O(n).
// Compare(a, b) returns true when a must come out before b, so std::less gives a min heap.
// PriorityQueue: smallest priority first, FIFO among equal priorities, O(log n) per operation.
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class BinaryHeap {
public:
    BinaryHeap() = default;
    explicit BinaryHeap(Compare cmp) : cmp_(std::move(cmp)) {}

    explicit BinaryHeap(std::vector<T> items, Compare cmp = Compare()) : data_(std::move(items)), cmp_(std::move(cmp)) {
        for (std::size_t i = data_.size() / 2; i-- > 0;) sift_down(i);
    }

    std::size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }

    const T& top() const {
        if (data_.empty()) throw std::underflow_error("heap is empty");
        return data_.front();
    }

    void push(T value) {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    T pop() {
        if (data_.empty()) throw std::underflow_error("heap is empty");
        T result = std::move(data_.front());
        if (data_.size() > 1) data_.front() = std::move(data_.back());
        data_.pop_back();
        if (!data_.empty()) sift_down(0);
        return result;
    }

private:
    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t parent = (i - 1) / 2;
            if (!cmp_(data_[i], data_[parent])) break;
            std::swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void sift_down(std::size_t i) {
        const std::size_t n = data_.size();
        for (;;) {
            std::size_t l = 2 * i + 1;
            std::size_t r = l + 1;
            std::size_t best = i;
            if (l < n && cmp_(data_[l], data_[best])) best = l;
            if (r < n && cmp_(data_[r], data_[best])) best = r;
            if (best == i) return;
            std::swap(data_[i], data_[best]);
            i = best;
        }
    }

    std::vector<T> data_;
    Compare cmp_;
};

template <typename T>
using MinHeap = BinaryHeap<T, std::less<T>>;
template <typename T>
using MaxHeap = BinaryHeap<T, std::greater<T>>;

template <typename T>
class PriorityQueue {
public:
    std::size_t size() const { return heap_.size(); }
    bool empty() const { return heap_.empty(); }

    void push(T item, double priority) {
        if (priority != priority) throw std::invalid_argument("priority must not be NaN");
        heap_.push(Entry{std::move(item), priority, counter_++});
    }

    const T& top() const { return heap_.top().item; }

    T pop() { return heap_.pop().item; }

private:
    struct Entry {
        T item;
        double priority = 0;
        std::size_t order = 0;
    };
    struct Before {
        bool operator()(const Entry& a, const Entry& b) const {
            return a.priority != b.priority ? a.priority < b.priority : a.order < b.order;
        }
    };
    BinaryHeap<Entry, Before> heap_;
    std::size_t counter_ = 0;
};

}  // namespace dsa
