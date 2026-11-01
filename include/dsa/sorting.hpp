#pragma once
// Every sort takes the vector by value and returns the sorted copy; Compare(a, b) is "a goes before b" (default <).
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

// O(n^2) time (O(n) if already sorted), O(1) extra space, stable.
template <typename T, typename Compare = std::less<T>>
std::vector<T> bubble_sort(std::vector<T> a, Compare cmp = Compare()) {
    for (std::size_t end = a.size(); end > 1; --end) {
        bool swapped = false;
        for (std::size_t i = 0; i + 1 < end; ++i) {
            if (cmp(a[i + 1], a[i])) {
                std::swap(a[i], a[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    return a;
}

// O(n^2) time (O(n) if nearly sorted), O(1) extra space, stable.
template <typename T, typename Compare = std::less<T>>
std::vector<T> insertion_sort(std::vector<T> a, Compare cmp = Compare()) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        T x = std::move(a[i]);
        std::size_t j = i;
        while (j > 0 && cmp(x, a[j - 1])) {
            a[j] = std::move(a[j - 1]);
            --j;
        }
        a[j] = std::move(x);
    }
    return a;
}

// O(n^2) time always, O(1) extra space, not stable.
template <typename T, typename Compare = std::less<T>>
std::vector<T> selection_sort(std::vector<T> a, Compare cmp = Compare()) {
    for (std::size_t i = 0; i + 1 < a.size(); ++i) {
        std::size_t best = i;
        for (std::size_t j = i + 1; j < a.size(); ++j) {
            if (cmp(a[j], a[best])) best = j;
        }
        if (best != i) std::swap(a[i], a[best]);
    }
    return a;
}

// O(n log n) time, O(n) space, stable.
template <typename T, typename Compare = std::less<T>>
std::vector<T> merge_sort(const std::vector<T>& a, Compare cmp = Compare()) {
    if (a.size() <= 1) return a;
    std::size_t mid = a.size() / 2;
    std::vector<T> left = merge_sort(std::vector<T>(a.begin(), a.begin() + static_cast<std::ptrdiff_t>(mid)), cmp);
    std::vector<T> right = merge_sort(std::vector<T>(a.begin() + static_cast<std::ptrdiff_t>(mid), a.end()), cmp);
    std::vector<T> out;
    out.reserve(a.size());
    std::size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        // Taking from the right only when strictly smaller keeps equal elements in order.
        if (cmp(right[j], left[i])) out.push_back(right[j++]);
        else out.push_back(left[i++]);
    }
    while (i < left.size()) out.push_back(left[i++]);
    while (j < right.size()) out.push_back(right[j++]);
    return out;
}

namespace detail {
template <typename T, typename Compare>
const T& median_of_three(const T& x, const T& y, const T& z, Compare& cmp) {
    if (cmp(x, y)) {
        if (cmp(y, z)) return y;
        return cmp(x, z) ? z : x;
    }
    if (cmp(x, z)) return x;
    return cmp(y, z) ? z : y;
}

// Three-way partition quick sort on [lo, hi]; recurses into the smaller side so depth stays O(log n).
template <typename T, typename Compare>
void quick_sort_range(std::vector<T>& a, std::ptrdiff_t lo, std::ptrdiff_t hi, Compare& cmp) {
    while (lo < hi) {
        T pivot = median_of_three(a[static_cast<std::size_t>(lo)], a[static_cast<std::size_t>(lo + (hi - lo) / 2)],
                                  a[static_cast<std::size_t>(hi)], cmp);
        std::ptrdiff_t lt = lo, gt = hi, i = lo;
        while (i <= gt) {
            auto ui = static_cast<std::size_t>(i);
            if (cmp(a[ui], pivot)) {
                std::swap(a[static_cast<std::size_t>(lt++)], a[ui]);
                ++i;
            } else if (cmp(pivot, a[ui])) {
                std::swap(a[static_cast<std::size_t>(gt--)], a[ui]);
            } else {
                ++i;
            }
        }
        if (lt - lo < hi - gt) {
            quick_sort_range(a, lo, lt - 1, cmp);
            lo = gt + 1;
        } else {
            quick_sort_range(a, gt + 1, hi, cmp);
            hi = lt - 1;
        }
    }
}

template <typename T, typename Compare>
void sift_down(std::vector<T>& a, std::size_t i, std::size_t n, Compare& cmp) {
    for (;;) {
        std::size_t l = 2 * i + 1, r = l + 1, big = i;
        if (l < n && cmp(a[big], a[l])) big = l;
        if (r < n && cmp(a[big], a[r])) big = r;
        if (big == i) return;
        std::swap(a[i], a[big]);
        i = big;
    }
}
}  // namespace detail

// O(n log n) expected, O(n^2) worst case, O(log n) extra space, not stable. Fast on many duplicates.
template <typename T, typename Compare = std::less<T>>
std::vector<T> quick_sort(std::vector<T> a, Compare cmp = Compare()) {
    detail::quick_sort_range(a, 0, static_cast<std::ptrdiff_t>(a.size()) - 1, cmp);
    return a;
}

// O(n log n) time, O(1) extra space (in place on the copy), not stable.
template <typename T, typename Compare = std::less<T>>
std::vector<T> heap_sort(std::vector<T> a, Compare cmp = Compare()) {
    for (std::size_t i = a.size() / 2; i-- > 0;) detail::sift_down(a, i, a.size(), cmp);
    for (std::size_t end = a.size(); end > 1; --end) {
        std::swap(a[0], a[end - 1]);
        detail::sift_down(a, 0, end - 1, cmp);
    }
    return a;
}

// O(n + k) time and space where k = max - min + 1. Negatives are supported. Throws if k would exceed max_range.
inline std::vector<int> counting_sort(const std::vector<int>& a, long long max_range = 100000000) {
    if (a.empty()) return {};
    auto [mn, mx] = std::minmax_element(a.begin(), a.end());
    long long range = static_cast<long long>(*mx) - static_cast<long long>(*mn) + 1;
    if (range > max_range) throw std::invalid_argument("value range too large for counting sort");
    std::vector<std::size_t> counts(static_cast<std::size_t>(range), 0);
    for (int x : a) ++counts[static_cast<std::size_t>(static_cast<long long>(x) - *mn)];
    std::vector<int> out;
    out.reserve(a.size());
    for (std::size_t i = 0; i < counts.size(); ++i) {
        out.insert(out.end(), counts[i], static_cast<int>(static_cast<long long>(i) + *mn));
    }
    return out;
}

}  // namespace dsa
