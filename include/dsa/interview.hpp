#pragma once
// Interview-style problems.
#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "stack_queue.hpp"

namespace dsa {

// O(n) time, O(n) space. Indices (i < j) of two elements that sum to target.
inline std::optional<std::pair<std::size_t, std::size_t>> two_sum(const std::vector<int>& a, long long target) {
    std::unordered_map<long long, std::size_t> seen;
    for (std::size_t i = 0; i < a.size(); ++i) {
        auto it = seen.find(target - a[i]);
        if (it != seen.end()) return std::make_pair(it->second, i);
        seen.emplace(a[i], i);
    }
    return std::nullopt;
}

// O(n) time, O(n) space. True when (), [] and {} are balanced; other characters are ignored.
inline bool valid_parentheses(const std::string& s) {
    Stack<char> open;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            open.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (open.empty()) return false;
            char o = open.pop();
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{')) return false;
        }
    }
    return open.empty();
}

// O(n log n) time, O(n) space. Merges overlapping or touching [start, end] intervals.
inline std::vector<std::pair<int, int>> merge_intervals(std::vector<std::pair<int, int>> intervals) {
    for (const auto& iv : intervals) {
        if (iv.first > iv.second) throw std::invalid_argument("interval start is after its end");
    }
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::pair<int, int>> out;
    for (const auto& iv : intervals) {
        if (!out.empty() && iv.first <= out.back().second) out.back().second = std::max(out.back().second, iv.second);
        else out.push_back(iv);
    }
    return out;
}

// O(n log k) time, O(k) space. The k-th largest element, counting duplicates.
inline int kth_largest(const std::vector<int>& a, std::size_t k) {
    if (k == 0 || k > a.size()) throw std::invalid_argument("k must be between 1 and the size");
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    for (int x : a) {
        if (heap.size() < k) heap.push(x);
        else if (x > heap.top()) {
            heap.pop();
            heap.push(x);
        }
    }
    return heap.top();
}

// O(n) time, O(1) space. Kadane's maximum subarray sum. Throws on empty input.
inline long long max_subarray(const std::vector<int>& a) {
    if (a.empty()) throw std::invalid_argument("vector is empty");
    long long cur = a[0], best = a[0];
    for (std::size_t i = 1; i < a.size(); ++i) {
        cur = std::max<long long>(a[i], cur + a[i]);
        best = std::max(best, cur);
    }
    return best;
}

struct ListNode {
    int value;
    std::unique_ptr<ListNode> next;
    explicit ListNode(int v) : value(v) {}
};

// O(n) time, O(1) space. Reverses a singly linked list in place and returns the new head.
inline std::unique_ptr<ListNode> reverse_list(std::unique_ptr<ListNode> head) {
    std::unique_ptr<ListNode> prev;
    while (head) {
        std::unique_ptr<ListNode> next = std::move(head->next);
        head->next = std::move(prev);
        prev = std::move(head);
        head = std::move(next);
    }
    return prev;
}

}  // namespace dsa
