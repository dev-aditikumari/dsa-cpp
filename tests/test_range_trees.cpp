#include <algorithm>
#include <numeric>
#include <string>

#include "doctest.h"
#include "dsa/range_trees.hpp"
#include "rng.hpp"

using namespace dsa;

static long long naive_sum(const std::vector<long long>& a, std::size_t l, std::size_t r) {
    return std::accumulate(a.begin() + static_cast<std::ptrdiff_t>(l), a.begin() + static_cast<std::ptrdiff_t>(r) + 1, 0LL);
}

TEST_CASE("FenwickTree prefix and range sums") {
    FenwickTree f({1, 2, 3, 4, 5});
    CHECK(f.prefix_sum(0) == 0);
    CHECK(f.prefix_sum(1) == 1);
    CHECK(f.prefix_sum(5) == 15);
    CHECK(f.range_sum(1, 3) == 9);
    f.add(2, 10);
    CHECK(f.range_sum(2, 2) == 13);
}

TEST_CASE("FenwickTree errors and empty input") {
    FenwickTree f({1});
    CHECK_THROWS_AS(f.prefix_sum(2), std::out_of_range);
    CHECK_THROWS_AS(f.add(1, 1), std::out_of_range);
    CHECK_THROWS_AS(f.range_sum(1, 0), std::out_of_range);
    FenwickTree g({1, 2});
    CHECK_THROWS_AS(g.range_sum(1, 0), std::invalid_argument);
    CHECK(FenwickTree({}).size() == 0);
}

TEST_CASE("FenwickTree matches naive sums under random updates") {
    Rng rng(81);
    std::vector<long long> arr(40);
    for (auto& x : arr) x = rng.in(-50, 50);
    FenwickTree f(arr);
    for (int i = 0; i < 2000; ++i) {
        std::size_t idx = rng.idx(0, arr.size() - 1);
        int d = rng.in(-20, 20);
        arr[idx] += d;
        f.add(idx, d);
        std::size_t l = rng.idx(0, arr.size() - 1);
        std::size_t r = rng.idx(l, arr.size() - 1);
        REQUIRE(f.range_sum(l, r) == naive_sum(arr, l, r));
    }
}

TEST_CASE("SegmentTree min queries and updates") {
    auto mn = [](int a, int b) { return std::min(a, b); };
    SegmentTree<int, decltype(mn)> s({5, 2, 8, 1, 9}, mn, 1 << 30);
    CHECK(s.query(0, 4) == 1);
    CHECK(s.query(0, 2) == 2);
    s.set(3, 7);
    CHECK(s.query(2, 4) == 7);
    CHECK(s.size() == 5);
}

TEST_CASE("SegmentTree keeps order for non-commutative operations") {
    auto cat = [](const std::string& a, const std::string& b) { return a + b; };
    SegmentTree<std::string, decltype(cat)> s({"a", "b", "c", "d", "e"}, cat, "");
    CHECK(s.query(1, 3) == "bcd");
    CHECK(s.query(0, 4) == "abcde");
    s.set(2, "X");
    CHECK(s.query(0, 4) == "abXde");
}

TEST_CASE("SegmentTree rejects bad ranges") {
    auto add = [](int a, int b) { return a + b; };
    SegmentTree<int, decltype(add)> s({1, 2}, add, 0);
    CHECK_THROWS_AS(s.query(1, 0), std::invalid_argument);
    CHECK_THROWS_AS(s.query(0, 2), std::out_of_range);
    CHECK_THROWS_AS(s.set(2, 0), std::out_of_range);
}

TEST_CASE("SegmentTree matches naive max and sum for odd and even sizes") {
    Rng rng(82);
    auto mx = [](long long a, long long b) { return std::max(a, b); };
    auto sm = [](long long a, long long b) { return a + b; };
    for (int t = 0; t < 100; ++t) {
        std::vector<long long> arr(static_cast<std::size_t>(rng.in(1, 41)));
        for (auto& x : arr) x = rng.in(-50, 50);
        SegmentTree<long long, decltype(mx)> smax(arr, mx, -(1LL << 60));
        SegmentTree<long long, decltype(sm)> ssum(arr, sm, 0);
        for (int q = 0; q < 20; ++q) {
            std::size_t i = rng.idx(0, arr.size() - 1);
            long long v = rng.in(-50, 50);
            arr[i] = v;
            smax.set(i, v);
            ssum.set(i, v);
            std::size_t l = rng.idx(0, arr.size() - 1);
            std::size_t r = rng.idx(l, arr.size() - 1);
            REQUIRE(smax.query(l, r) == *std::max_element(arr.begin() + static_cast<std::ptrdiff_t>(l),
                                                           arr.begin() + static_cast<std::ptrdiff_t>(r) + 1));
            REQUIRE(ssum.query(l, r) == naive_sum(arr, l, r));
        }
    }
}
