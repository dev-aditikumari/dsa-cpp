#pragma once
// Two pointers, sliding window and prefix sums.
#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa {

// O(n) time, O(1) space. Indices of two elements of a sorted vector summing to target.
inline std::optional<std::pair<std::size_t, std::size_t>> two_sum_sorted(const std::vector<int>& a, int target) {
    if (a.size() < 2) return std::nullopt;
    std::size_t i = 0, j = a.size() - 1;
    while (i < j) {
        long long s = static_cast<long long>(a[i]) + a[j];
        if (s == target) return std::make_pair(i, j);
        if (s < target) ++i;
        else --j;
    }
    return std::nullopt;
}

// O(n^2) time, O(1) extra space. Unique triplets that sum to zero, each sorted ascending.
inline std::vector<std::array<int, 3>> three_sum(std::vector<int> a) {
    std::sort(a.begin(), a.end());
    std::vector<std::array<int, 3>> out;
    for (std::size_t i = 0; i + 2 < a.size(); ++i) {
        if (i > 0 && a[i] == a[i - 1]) continue;
        std::size_t l = i + 1, r = a.size() - 1;
        while (l < r) {
            long long s = static_cast<long long>(a[i]) + a[l] + a[r];
            if (s < 0) ++l;
            else if (s > 0) --r;
            else {
                out.push_back({a[i], a[l], a[r]});
                ++l;
                --r;
                while (l < r && a[l] == a[l - 1]) ++l;
                while (l < r && a[r] == a[r + 1]) --r;
            }
        }
    }
    return out;
}

// O(n) time, O(1) space. Largest area held between two vertical lines.
inline long long max_water(const std::vector<int>& h) {
    if (h.size() < 2) return 0;
    std::size_t i = 0, j = h.size() - 1;
    long long best = 0;
    while (i < j) {
        best = std::max(best, static_cast<long long>(std::min(h[i], h[j])) * static_cast<long long>(j - i));
        if (h[i] < h[j]) ++i;
        else --j;
    }
    return best;
}

// O(n) time, O(1) space. Largest sum of k consecutive elements.
inline long long max_sum_window(const std::vector<int>& a, std::size_t k) {
    if (k == 0 || k > a.size()) throw std::invalid_argument("k must be between 1 and the size");
    long long sum = 0;
    for (std::size_t i = 0; i < k; ++i) sum += a[i];
    long long best = sum;
    for (std::size_t i = k; i < a.size(); ++i) {
        sum += a[i] - a[i - k];
        best = std::max(best, sum);
    }
    return best;
}

// O(n) time, O(alphabet) space. Length of the longest substring with no repeated byte.
inline std::size_t longest_unique_substring(const std::string& s) {
    std::array<long, 256> last;
    last.fill(-1);
    long start = 0;
    std::size_t best = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        auto c = static_cast<unsigned char>(s[i]);
        if (last[c] >= start) start = last[c] + 1;
        last[c] = static_cast<long>(i);
        best = std::max(best, i - static_cast<std::size_t>(start) + 1);
    }
    return best;
}

// O(n) time, O(1) space. Shortest subarray of non-negative numbers with sum >= target; 0 if none.
inline std::size_t min_subarray_len(const std::vector<int>& a, long long target) {
    std::size_t best = 0, left = 0;
    long long sum = 0;
    for (std::size_t right = 0; right < a.size(); ++right) {
        sum += a[right];
        while (sum >= target && left <= right) {
            if (best == 0 || right - left + 1 < best) best = right - left + 1;
            sum -= a[left++];
        }
    }
    return best;
}

// O(n) build, O(1) query, O(n) space. Inclusive range sums.
class PrefixSums {
public:
    explicit PrefixSums(const std::vector<int>& a) : prefix_(a.size() + 1, 0) {
        for (std::size_t i = 0; i < a.size(); ++i) prefix_[i + 1] = prefix_[i] + a[i];
    }

    long long range_sum(std::size_t left, std::size_t right) const {
        if (left > right || right + 1 >= prefix_.size()) {
            throw std::out_of_range("invalid range");
        }
        return prefix_[right + 1] - prefix_[left];
    }

private:
    std::vector<long long> prefix_;
};

// O(n) time, O(n) space. Number of subarrays whose sum is k (prefix sums plus a hash map).
inline long long subarray_sum_equals(const std::vector<int>& a, long long k) {
    std::unordered_map<long long, long long> seen{{0, 1}};
    long long sum = 0, count = 0;
    for (int x : a) {
        sum += x;
        auto it = seen.find(sum - k);
        if (it != seen.end()) count += it->second;
        ++seen[sum];
    }
    return count;
}

}  // namespace dsa
