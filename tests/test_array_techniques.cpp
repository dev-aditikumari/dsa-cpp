#include <algorithm>
#include <set>

#include "doctest.h"
#include "dsa/array_techniques.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;

TEST_CASE("two_sum_sorted") {
    CHECK(two_sum_sorted({1, 2, 4, 7, 11}, 9).value() == std::make_pair<std::size_t, std::size_t>(1, 3));
    CHECK_FALSE(two_sum_sorted({1, 2, 3}, 100).has_value());
    CHECK_FALSE(two_sum_sorted({5}, 10).has_value());
    CHECK_FALSE(two_sum_sorted({}, 0).has_value());
    CHECK(two_sum_sorted({3, 3}, 6).has_value());
}

TEST_CASE("three_sum known cases") {
    using T = std::vector<std::array<int, 3>>;
    CHECK(three_sum({-1, 0, 1, 2, -1, -4}) == T{{-1, -1, 2}, {-1, 0, 1}});
    CHECK(three_sum({0, 0, 0, 0}) == T{{0, 0, 0}});
    CHECK(three_sum({}).empty());
    CHECK(three_sum({1, 2}).empty());
}

TEST_CASE("three_sum returns the same unique triplets as brute force") {
    Rng rng(141);
    for (int t = 0; t < 200; ++t) {
        V a = rng.vec(12, -5, 5);
        std::set<std::array<int, 3>> expected;
        for (std::size_t i = 0; i < a.size(); ++i)
            for (std::size_t j = i + 1; j < a.size(); ++j)
                for (std::size_t k = j + 1; k < a.size(); ++k)
                    if (a[i] + a[j] + a[k] == 0) {
                        std::array<int, 3> tri{a[i], a[j], a[k]};
                        std::sort(tri.begin(), tri.end());
                        expected.insert(tri);
                    }
        auto got = three_sum(a);
        REQUIRE(std::set<std::array<int, 3>>(got.begin(), got.end()) == expected);
        REQUIRE(got.size() == expected.size());
    }
}

TEST_CASE("max_water known and brute force") {
    CHECK(max_water({1, 8, 6, 2, 5, 4, 8, 3, 7}) == 49);
    CHECK(max_water({}) == 0);
    CHECK(max_water({4}) == 0);
    Rng rng(142);
    for (int t = 0; t < 200; ++t) {
        V h = rng.vec(20, 0, 15);
        long long best = 0;
        for (std::size_t i = 0; i < h.size(); ++i)
            for (std::size_t j = i + 1; j < h.size(); ++j)
                best = std::max(best, static_cast<long long>(std::min(h[i], h[j])) * static_cast<long long>(j - i));
        REQUIRE(max_water(h) == best);
    }
}

TEST_CASE("max_sum_window known, errors and brute force") {
    CHECK(max_sum_window({2, 1, 5, 1, 3, 2}, 3) == 9);
    CHECK(max_sum_window({4}, 1) == 4);
    CHECK(max_sum_window({-3, -1, -2}, 2) == -3);
    CHECK_THROWS_AS(max_sum_window({1, 2}, 3), std::invalid_argument);
    CHECK_THROWS_AS(max_sum_window({1, 2}, 0), std::invalid_argument);
    Rng rng(143);
    for (int t = 0; t < 200; ++t) {
        V a = rng.vec(25);
        a.push_back(1);
        std::size_t k = rng.idx(1, a.size());
        long long best = -(1LL << 60);
        for (std::size_t i = 0; i + k <= a.size(); ++i) {
            long long s = 0;
            for (std::size_t j = i; j < i + k; ++j) s += a[j];
            best = std::max(best, s);
        }
        REQUIRE(max_sum_window(a, k) == best);
    }
}

TEST_CASE("longest_unique_substring known and brute force") {
    CHECK(longest_unique_substring("abcabcbb") == 3);
    CHECK(longest_unique_substring("bbbbb") == 1);
    CHECK(longest_unique_substring("pwwkew") == 3);
    CHECK(longest_unique_substring("abba") == 2);
    CHECK(longest_unique_substring("") == 0);
    Rng rng(144);
    for (int t = 0; t < 200; ++t) {
        std::string s;
        for (int i = 0, n = rng.in(0, 15); i < n; ++i) s.push_back("abcd"[rng.in(0, 3)]);
        std::size_t best = 0;
        for (std::size_t i = 0; i < s.size(); ++i)
            for (std::size_t j = i + 1; j <= s.size(); ++j) {
                std::set<char> uniq(s.begin() + static_cast<std::ptrdiff_t>(i), s.begin() + static_cast<std::ptrdiff_t>(j));
                if (uniq.size() == j - i) best = std::max(best, j - i);
            }
        REQUIRE(longest_unique_substring(s) == best);
    }
}

TEST_CASE("min_subarray_len") {
    CHECK(min_subarray_len({2, 3, 1, 2, 4, 3}, 7) == 2);
    CHECK(min_subarray_len({1, 1, 1}, 100) == 0);
    CHECK(min_subarray_len({}, 1) == 0);
    CHECK(min_subarray_len({9}, 7) == 1);
}

TEST_CASE("PrefixSums range queries and errors") {
    PrefixSums p({1, 2, 3, 4});
    CHECK(p.range_sum(0, 3) == 10);
    CHECK(p.range_sum(1, 2) == 5);
    CHECK(p.range_sum(2, 2) == 3);
    CHECK_THROWS_AS(p.range_sum(2, 1), std::out_of_range);
    CHECK_THROWS_AS(p.range_sum(0, 4), std::out_of_range);
    CHECK_THROWS_AS(PrefixSums({}).range_sum(0, 0), std::out_of_range);
}

TEST_CASE("subarray_sum_equals known and brute force (zeros and negatives included)") {
    CHECK(subarray_sum_equals({1, 1, 1}, 2) == 2);
    CHECK(subarray_sum_equals({}, 0) == 0);
    CHECK(subarray_sum_equals({0, 0, 0}, 0) == 6);
    Rng rng(145);
    for (int t = 0; t < 200; ++t) {
        V a = rng.vec(20, -3, 3);
        int k = rng.in(-4, 4);
        long long count = 0;
        for (std::size_t i = 0; i < a.size(); ++i) {
            long long s = 0;
            for (std::size_t j = i; j < a.size(); ++j) {
                s += a[j];
                if (s == k) ++count;
            }
        }
        REQUIRE(subarray_sum_equals(a, k) == count);
    }
}
