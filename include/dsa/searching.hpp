#pragma once
// Binary search family. All O(log n) time and O(1) space unless noted. Inputs must be sorted where stated.
#include <algorithm>
#include <cstddef>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace dsa {

// Index of target in a sorted vector, or -1.
template <typename T, typename Compare = std::less<T>>
long binary_search_index(const std::vector<T>& a, const T& target, Compare cmp = Compare()) {
    std::size_t lo = 0, hi = a.size();
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (cmp(a[mid], target)) lo = mid + 1;
        else if (cmp(target, a[mid])) hi = mid;
        else return static_cast<long>(mid);
    }
    return -1;
}

// First index whose element is not less than target (a.size() if none).
template <typename T, typename Compare = std::less<T>>
std::size_t lower_bound_index(const std::vector<T>& a, const T& target, Compare cmp = Compare()) {
    std::size_t lo = 0, hi = a.size();
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (cmp(a[mid], target)) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// First index whose element is greater than target (a.size() if none).
template <typename T, typename Compare = std::less<T>>
std::size_t upper_bound_index(const std::vector<T>& a, const T& target, Compare cmp = Compare()) {
    std::size_t lo = 0, hi = a.size();
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (!cmp(target, a[mid])) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

template <typename T, typename Compare = std::less<T>>
std::size_t count_occurrences(const std::vector<T>& a, const T& target, Compare cmp = Compare()) {
    return upper_bound_index(a, target, cmp) - lower_bound_index(a, target, cmp);
}

// Index of target in a sorted vector that was rotated, or -1. O(n) worst case when there are many duplicates.
inline long search_rotated(const std::vector<int>& a, int target) {
    long lo = 0, hi = static_cast<long>(a.size()) - 1;
    while (lo <= hi) {
        long mid = lo + (hi - lo) / 2;
        int l = a[static_cast<std::size_t>(lo)], m = a[static_cast<std::size_t>(mid)], h = a[static_cast<std::size_t>(hi)];
        if (m == target) return mid;
        if (l == m && m == h) {
            ++lo;
            --hi;
        } else if (l <= m) {
            if (target >= l && target < m) hi = mid - 1;
            else lo = mid + 1;
        } else if (target > m && target <= h) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// Index of the minimum in a rotated sorted vector of distinct values. Throws on empty input.
inline std::size_t find_rotation_point(const std::vector<int>& a) {
    if (a.empty()) throw std::invalid_argument("vector is empty");
    std::size_t lo = 0, hi = a.size() - 1;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// Smallest integer in [lo, hi] for which ok(x) is true, assuming ok is false...false true...true.
// Returns hi + 1 if none qualifies. Uses O(log(hi - lo)) calls to ok.
template <typename Pred>
long long search_on_answer(long long lo, long long hi, Pred ok) {
    long long l = lo, h = hi + 1;
    while (l < h) {
        long long mid = l + (h - l) / 2;
        if (ok(mid)) h = mid;
        else l = mid + 1;
    }
    return l;
}

// O(n log(sum)) time. Least ship capacity that moves all packages, in order, within the given number of days.
inline long long ship_within_days(const std::vector<int>& weights, int days) {
    if (days < 1) throw std::invalid_argument("days must be positive");
    if (weights.empty()) return 0;
    long long heaviest = 0, total = 0;
    for (int w : weights) {
        if (w < 0) throw std::invalid_argument("weights must be non-negative");
        heaviest = std::max<long long>(heaviest, w);
        total += w;
    }
    return search_on_answer(heaviest, total, [&](long long cap) {
        int used = 1;
        long long load = 0;
        for (int w : weights) {
            if (load + w > cap) {
                ++used;
                load = 0;
            }
            load += w;
        }
        return used <= days;
    });
}

// Floor of the square root of a non-negative integer.
inline long long integer_sqrt(long long x) {
    if (x < 0) throw std::invalid_argument("x must be non-negative");
    long long hi = std::min<long long>(x, 3037000499LL);  // floor(sqrt(2^63 - 1)), keeps m * m from overflowing
    return search_on_answer(0, hi, [&](long long m) { return m * m > x; }) - 1;
}

}  // namespace dsa
