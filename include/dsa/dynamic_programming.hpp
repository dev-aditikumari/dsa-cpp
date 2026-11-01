#pragma once
// Classic dynamic programming problems. m, n are input sizes.
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace dsa {

constexpr unsigned kMaxFib = 93;  // fib(93) is the largest value that fits in uint64_t

namespace detail {
inline std::uint64_t fib_memo_impl(unsigned n, std::unordered_map<unsigned, std::uint64_t>& memo) {
    if (n < 2) return n;
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    std::uint64_t v = fib_memo_impl(n - 1, memo) + fib_memo_impl(n - 2, memo);
    memo[n] = v;
    return v;
}
}  // namespace detail

// O(n) time, O(n) space. Top-down with a memo table. Throws for n > 93 (uint64 overflow).
inline std::uint64_t fib_memo(unsigned n) {
    if (n > kMaxFib) throw std::out_of_range("fibonacci result would overflow uint64_t");
    std::unordered_map<unsigned, std::uint64_t> memo;
    return detail::fib_memo_impl(n, memo);
}

// O(n) time, O(1) space. Bottom-up. Throws for n > 93.
inline std::uint64_t fib_tab(unsigned n) {
    if (n > kMaxFib) throw std::out_of_range("fibonacci result would overflow uint64_t");
    std::uint64_t a = 0, b = 1;
    for (unsigned i = 0; i < n; ++i) {
        std::uint64_t next = a + b;  // the last iteration for n = 93 wraps, but only the unused b
        a = b;
        b = next;
    }
    return a;
}

// O(n * W) time, O(W) space. Best total value with total weight <= capacity, each item used at most once.
inline long long knapsack_01(const std::vector<int>& weights, const std::vector<long long>& values, int capacity) {
    if (weights.size() != values.size()) throw std::invalid_argument("weights and values differ in length");
    if (capacity < 0) throw std::invalid_argument("capacity must be non-negative");
    std::vector<long long> best(static_cast<std::size_t>(capacity) + 1, 0);
    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i] < 0) throw std::invalid_argument("weights must be non-negative");
        for (int c = capacity; c >= weights[i]; --c) {
            auto uc = static_cast<std::size_t>(c);
            best[uc] = std::max(best[uc], best[uc - static_cast<std::size_t>(weights[i])] + values[i]);
        }
    }
    return best[static_cast<std::size_t>(capacity)];
}

// O(m * n) time and space. One longest common subsequence.
inline std::string lcs(const std::string& a, const std::string& b) {
    std::vector<std::vector<int>> dp(a.size() + 1, std::vector<int>(b.size() + 1, 0));
    for (std::size_t i = 1; i <= a.size(); ++i) {
        for (std::size_t j = 1; j <= b.size(); ++j) {
            dp[i][j] = a[i - 1] == b[j - 1] ? dp[i - 1][j - 1] + 1 : std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    std::string out;
    std::size_t i = a.size(), j = b.size();
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            out.push_back(a[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(out.begin(), out.end());
    return out;
}

// O(n log n) time, O(n) space. Length of the longest strictly increasing subsequence.
inline std::size_t lis_length(const std::vector<int>& a) {
    std::vector<int> tails;
    for (int x : a) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

// O(m * n) time, O(min(m, n)) space. Levenshtein distance (insert, delete, substitute).
inline std::size_t edit_distance(std::string a, std::string b) {
    if (a.size() < b.size()) a.swap(b);
    std::vector<std::size_t> prev(b.size() + 1);
    for (std::size_t j = 0; j <= b.size(); ++j) prev[j] = j;
    for (std::size_t i = 1; i <= a.size(); ++i) {
        std::vector<std::size_t> cur(b.size() + 1);
        cur[0] = i;
        for (std::size_t j = 1; j <= b.size(); ++j) {
            std::size_t sub = prev[j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1);
            cur[j] = std::min({sub, prev[j] + 1, cur[j - 1] + 1});
        }
        prev = std::move(cur);
    }
    return prev[b.size()];
}

// O(amount * coins) time, O(amount) space. Fewest coins that sum to amount, or -1 if impossible.
inline long coin_change_min(const std::vector<int>& coins, int amount) {
    if (amount < 0) throw std::invalid_argument("amount must be non-negative");
    const long inf = std::numeric_limits<long>::max();
    std::vector<long> dp(static_cast<std::size_t>(amount) + 1, inf);
    dp[0] = 0;
    for (int s = 1; s <= amount; ++s) {
        for (int c : coins) {
            if (c > 0 && c <= s && dp[static_cast<std::size_t>(s - c)] != inf) {
                dp[static_cast<std::size_t>(s)] = std::min(dp[static_cast<std::size_t>(s)], dp[static_cast<std::size_t>(s - c)] + 1);
            }
        }
    }
    return dp[static_cast<std::size_t>(amount)] == inf ? -1 : dp[static_cast<std::size_t>(amount)];
}

// O(amount * coins) time, O(amount) space. Number of combinations (order ignored) that sum to amount.
inline unsigned long long coin_change_ways(const std::vector<int>& coins, int amount) {
    if (amount < 0) throw std::invalid_argument("amount must be non-negative");
    std::vector<unsigned long long> dp(static_cast<std::size_t>(amount) + 1, 0);
    dp[0] = 1;
    for (int c : coins) {
        if (c <= 0) continue;
        for (int s = c; s <= amount; ++s) dp[static_cast<std::size_t>(s)] += dp[static_cast<std::size_t>(s - c)];
    }
    return dp[static_cast<std::size_t>(amount)];
}

// O(rows * cols) time, O(cols) space. Right/down paths from the top left to the bottom right; 1 marks an obstacle.
inline unsigned long long unique_paths(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    const std::size_t cols = grid[0].size();
    std::vector<unsigned long long> dp(cols, 0);
    dp[0] = 1;
    for (const auto& row : grid) {
        if (row.size() != cols) throw std::invalid_argument("grid must be rectangular");
        for (std::size_t j = 0; j < cols; ++j) {
            if (row[j] == 1) dp[j] = 0;
            else if (j > 0) dp[j] += dp[j - 1];
        }
    }
    return dp[cols - 1];
}

// O(n^3) time, O(n^2) space. Fewest scalar multiplications for a chain where matrix i is dims[i] x dims[i+1].
inline long long matrix_chain_order(const std::vector<int>& dims) {
    if (dims.size() < 3) return 0;
    const std::size_t n = dims.size() - 1;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));
    for (std::size_t len = 2; len <= n; ++len) {
        for (std::size_t i = 0; i + len <= n; ++i) {
            std::size_t j = i + len - 1;
            long long best = std::numeric_limits<long long>::max();
            for (std::size_t k = i; k < j; ++k) {
                best = std::min(best, dp[i][k] + dp[k + 1][j] +
                                          static_cast<long long>(dims[i]) * dims[k + 1] * dims[j + 1]);
            }
            dp[i][j] = best;
        }
    }
    return dp[0][n - 1];
}

}  // namespace dsa
