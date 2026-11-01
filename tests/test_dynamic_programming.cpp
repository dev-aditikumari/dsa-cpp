#include <algorithm>
#include <functional>
#include <string>

#include "doctest.h"
#include "dsa/dynamic_programming.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;

static std::string random_string(Rng& rng, int max_len, const char* alphabet, int alpha_size) {
    std::string s;
    for (int i = 0, n = rng.in(0, max_len); i < n; ++i) s.push_back(alphabet[rng.in(0, alpha_size - 1)]);
    return s;
}

static bool is_subsequence(const std::string& s, const std::string& t) {
    std::size_t i = 0;
    for (char c : t) {
        if (i < s.size() && c == s[i]) ++i;
    }
    return i == s.size();
}

TEST_CASE("fibonacci memo and tabulation agree") {
    CHECK(fib_tab(0) == 0);
    CHECK(fib_tab(1) == 1);
    CHECK(fib_tab(10) == 55);
    CHECK(fib_tab(90) == 2880067194370816120ULL);
    CHECK(fib_tab(93) == 12200160415121876738ULL);
    for (unsigned n = 0; n <= 93; ++n) CHECK(fib_memo(n) == fib_tab(n));
}

TEST_CASE("fibonacci rejects results that overflow") {
    CHECK_THROWS_AS(fib_tab(94), std::out_of_range);
    CHECK_THROWS_AS(fib_memo(94), std::out_of_range);
}

TEST_CASE("knapsack known and edge cases") {
    CHECK(knapsack_01({1, 3, 4, 5}, {1, 4, 5, 7}, 7) == 9);
    CHECK(knapsack_01({}, {}, 10) == 0);
    CHECK(knapsack_01({5}, {10}, 4) == 0);
    CHECK(knapsack_01({5}, {10}, 5) == 10);
    CHECK(knapsack_01({1, 2}, {3, 4}, 0) == 0);
    CHECK_THROWS_AS(knapsack_01({1}, {1, 2}, 3), std::invalid_argument);
    CHECK_THROWS_AS(knapsack_01({1}, {1}, -1), std::invalid_argument);
    CHECK_THROWS_AS(knapsack_01({-1}, {1}, 3), std::invalid_argument);
}

TEST_CASE("knapsack matches brute force over all subsets") {
    Rng rng(131);
    for (int t = 0; t < 200; ++t) {
        int n = rng.in(0, 9);
        V w(static_cast<std::size_t>(n));
        std::vector<long long> v(static_cast<std::size_t>(n));
        for (auto& x : w) x = rng.in(0, 8);
        for (auto& x : v) x = rng.in(0, 20);
        int cap = rng.in(0, 20);
        long long best = 0;
        for (unsigned mask = 0; mask < (1u << n); ++mask) {
            int tw = 0;
            long long tv = 0;
            for (std::size_t i = 0; i < w.size(); ++i) {
                if (mask & (1u << i)) {
                    tw += w[i];
                    tv += v[i];
                }
            }
            if (tw <= cap) best = std::max(best, tv);
        }
        REQUIRE(knapsack_01(w, v, cap) == best);
    }
}

TEST_CASE("lcs known cases") {
    CHECK(lcs("abcde", "ace") == "ace");
    CHECK(lcs("", "abc") == "");
    CHECK(lcs("abc", "xyz") == "");
    CHECK(lcs("same", "same") == "same");
}

TEST_CASE("lcs is a common subsequence of maximal length (brute force)") {
    Rng rng(132);
    for (int t = 0; t < 200; ++t) {
        std::string a = random_string(rng, 8, "abc", 3), b = random_string(rng, 8, "abc", 3);
        std::string out = lcs(a, b);
        REQUIRE(is_subsequence(out, a));
        REQUIRE(is_subsequence(out, b));
        std::size_t best = 0;
        for (unsigned mask = 0; mask < (1u << a.size()); ++mask) {
            std::string sub;
            for (std::size_t i = 0; i < a.size(); ++i) {
                if (mask & (1u << i)) sub.push_back(a[i]);
            }
            if (sub.size() > best && is_subsequence(sub, b)) best = sub.size();
        }
        REQUIRE(out.size() == best);
    }
}

TEST_CASE("lis known cases") {
    CHECK(lis_length({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    CHECK(lis_length({}) == 0);
    CHECK(lis_length({5}) == 1);
    CHECK(lis_length({2, 2, 2}) == 1);
    CHECK(lis_length({5, 4, 3}) == 1);
}

TEST_CASE("lis matches the quadratic DP on random arrays") {
    Rng rng(133);
    for (int t = 0; t < 300; ++t) {
        V a = rng.vec(40, 0, 20);
        std::vector<std::size_t> dp(a.size(), 1);
        for (std::size_t i = 0; i < a.size(); ++i)
            for (std::size_t j = 0; j < i; ++j)
                if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
        std::size_t expected = a.empty() ? 0 : *std::max_element(dp.begin(), dp.end());
        REQUIRE(lis_length(a) == expected);
    }
}

TEST_CASE("edit distance known cases") {
    CHECK(edit_distance("kitten", "sitting") == 3);
    CHECK(edit_distance("", "abc") == 3);
    CHECK(edit_distance("abc", "") == 3);
    CHECK(edit_distance("", "") == 0);
    CHECK(edit_distance("same", "same") == 0);
}

TEST_CASE("edit distance is symmetric, obeys the triangle inequality and length bounds") {
    Rng rng(134);
    for (int t = 0; t < 300; ++t) {
        std::string a = random_string(rng, 7, "ab", 2), b = random_string(rng, 7, "ab", 2), c = random_string(rng, 7, "ab", 2);
        REQUIRE(edit_distance(a, b) == edit_distance(b, a));
        REQUIRE(edit_distance(a, c) <= edit_distance(a, b) + edit_distance(b, c));
        REQUIRE(edit_distance(a, b) <= std::max(a.size(), b.size()));
        std::size_t diff = a.size() > b.size() ? a.size() - b.size() : b.size() - a.size();
        REQUIRE(edit_distance(a, b) >= diff);
    }
}

TEST_CASE("coin change minimum coins") {
    CHECK(coin_change_min({1, 2, 5}, 11) == 3);
    CHECK(coin_change_min({2}, 3) == -1);
    CHECK(coin_change_min({1}, 0) == 0);
    CHECK(coin_change_min({}, 5) == -1);
    CHECK(coin_change_min({186, 419, 83, 408}, 6249) == 20);
    CHECK_THROWS_AS(coin_change_min({1}, -1), std::invalid_argument);
}

TEST_CASE("coin change number of ways") {
    CHECK(coin_change_ways({1, 2, 5}, 5) == 4);
    CHECK(coin_change_ways({2}, 3) == 0);
    CHECK(coin_change_ways({}, 0) == 1);
    CHECK(coin_change_ways({1, 2, 5}, 0) == 1);
    CHECK(coin_change_ways({0, 1}, 3) == 1);
    CHECK_THROWS_AS(coin_change_ways({1}, -1), std::invalid_argument);
}

TEST_CASE("coin change minimum matches brute-force recursion") {
    Rng rng(135);
    for (int t = 0; t < 200; ++t) {
        V coins;
        for (int i = 0, n = rng.in(1, 4); i < n; ++i) coins.push_back(rng.in(1, 7));
        int amount = rng.in(0, 25);
        std::function<long(int)> brute = [&](int rem) -> long {
            if (rem == 0) return 0;
            long best = -1;
            for (int c : coins) {
                if (c > rem) continue;
                long sub = brute(rem - c);
                if (sub >= 0 && (best < 0 || sub + 1 < best)) best = sub + 1;
            }
            return best;
        };
        REQUIRE(coin_change_min(coins, amount) == brute(amount));
    }
}

TEST_CASE("unique paths with obstacles") {
    CHECK(unique_paths({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}) == 6);
    CHECK(unique_paths({{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}) == 2);
    CHECK(unique_paths({{1}}) == 0);
    CHECK(unique_paths({{0}}) == 1);
    CHECK(unique_paths({}) == 0);
    CHECK(unique_paths({{0, 1}}) == 0);
    CHECK(unique_paths(std::vector<std::vector<int>>(10, std::vector<int>(10, 0))) == 48620);
    CHECK_THROWS_AS(unique_paths({{0, 0}, {0}}), std::invalid_argument);
}

TEST_CASE("matrix chain order known values") {
    CHECK(matrix_chain_order({10, 30, 5, 60}) == 4500);
    CHECK(matrix_chain_order({40, 20, 30, 10, 30}) == 26000);
    CHECK(matrix_chain_order({5, 10}) == 0);
    CHECK(matrix_chain_order({5}) == 0);
    CHECK(matrix_chain_order({}) == 0);
}

TEST_CASE("matrix chain order matches exhaustive parenthesisation") {
    Rng rng(136);
    for (int t = 0; t < 100; ++t) {
        V d;
        for (int i = 0, n = rng.in(2, 8); i < n; ++i) d.push_back(rng.in(1, 12));
        std::function<long long(std::size_t, std::size_t)> brute = [&](std::size_t i, std::size_t j) -> long long {
            if (i == j) return 0;
            long long best = -1;
            for (std::size_t k = i; k < j; ++k) {
                long long c = brute(i, k) + brute(k + 1, j) + static_cast<long long>(d[i]) * d[k + 1] * d[j + 1];
                if (best < 0 || c < best) best = c;
            }
            return best;
        };
        REQUIRE(matrix_chain_order(d) == brute(0, d.size() - 2));
    }
}
