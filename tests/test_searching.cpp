#include <algorithm>
#include <cmath>
#include <string>

#include "doctest.h"
#include "dsa/searching.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;

TEST_CASE("binary search finds present and absent values") {
    V a{1, 3, 5, 7, 9};
    CHECK(binary_search_index(a, 7) == 3);
    CHECK(binary_search_index(a, 1) == 0);
    CHECK(binary_search_index(a, 9) == 4);
    CHECK(binary_search_index(a, 4) == -1);
    CHECK(binary_search_index(V{}, 1) == -1);
    CHECK(binary_search_index(std::vector<std::string>{"a", "c"}, std::string("c")) == 1);
}

TEST_CASE("lower and upper bound with duplicates") {
    V a{1, 2, 2, 2, 5};
    CHECK(lower_bound_index(a, 2) == 1);
    CHECK(upper_bound_index(a, 2) == 4);
    CHECK(lower_bound_index(a, 0) == 0);
    CHECK(upper_bound_index(a, 9) == 5);
    CHECK(lower_bound_index(a, 3) == 4);
    CHECK(count_occurrences(a, 2) == 3);
    CHECK(count_occurrences(a, 4) == 0);
    CHECK(lower_bound_index(V{}, 1) == 0);
}

TEST_CASE("bounds and binary search match std:: on 300 random sorted arrays") {
    Rng rng(111);
    for (int t = 0; t < 300; ++t) {
        V a = rng.vec(40, 0, 15);
        std::sort(a.begin(), a.end());
        int target = rng.in(-1, 16);
        CHECK(lower_bound_index(a, target) == static_cast<std::size_t>(std::lower_bound(a.begin(), a.end(), target) - a.begin()));
        CHECK(upper_bound_index(a, target) == static_cast<std::size_t>(std::upper_bound(a.begin(), a.end(), target) - a.begin()));
        long idx = binary_search_index(a, target);
        if (std::binary_search(a.begin(), a.end(), target)) CHECK(a[static_cast<std::size_t>(idx)] == target);
        else CHECK(idx == -1);
    }
}

TEST_CASE("rotated arrays: search and rotation point") {
    V a{4, 5, 6, 7, 0, 1, 2};
    CHECK(search_rotated(a, 0) == 4);
    CHECK(search_rotated(a, 6) == 2);
    CHECK(search_rotated(a, 3) == -1);
    CHECK(search_rotated({}, 1) == -1);
    CHECK(search_rotated({1}, 1) == 0);
    CHECK(find_rotation_point(a) == 4);
    CHECK(find_rotation_point({1, 2, 3}) == 0);
    CHECK(find_rotation_point({9}) == 0);
    CHECK_THROWS_AS(find_rotation_point({}), std::invalid_argument);
}

TEST_CASE("rotated search handles duplicates") {
    CHECK(search_rotated({1, 1, 1, 3, 1}, 3) == 3);
    CHECK(search_rotated({2, 2, 2, 0, 2}, 0) == 3);
    CHECK(search_rotated({1, 1, 1}, 2) == -1);
}

TEST_CASE("rotated search agrees with a linear scan on random rotations") {
    Rng rng(112);
    for (int t = 0; t < 200; ++t) {
        int n = rng.in(1, 30);
        V base;
        for (int i = 0; i < n; ++i) base.push_back(i * 2);
        auto r = static_cast<std::ptrdiff_t>(rng.in(0, n - 1));
        V a(base.begin() + r, base.end());
        a.insert(a.end(), base.begin(), base.begin() + r);
        CHECK(a[find_rotation_point(a)] == 0);
        for (int target = -1; target <= n * 2; ++target) {
            auto it = std::find(a.begin(), a.end(), target);
            long expected = it == a.end() ? -1 : it - a.begin();
            REQUIRE(search_rotated(a, target) == expected);
        }
    }
}

TEST_CASE("search on answer") {
    CHECK(search_on_answer(0, 100, [](long long x) { return x * x >= 50; }) == 8);
    CHECK(search_on_answer(0, 10, [](long long) { return false; }) == 11);
    CHECK(search_on_answer(0, 10, [](long long) { return true; }) == 0);
}

TEST_CASE("integer_sqrt matches std::sqrt for small values and handles huge ones") {
    for (long long x = 0; x <= 5000; ++x) CHECK(integer_sqrt(x) == static_cast<long long>(std::floor(std::sqrt(static_cast<double>(x)))));
    CHECK(integer_sqrt(1LL << 52) == (1LL << 26));
    CHECK(integer_sqrt(9223372036854775807LL) == 3037000499LL);
    CHECK_THROWS_AS(integer_sqrt(-1), std::invalid_argument);
}

TEST_CASE("ship_within_days known case, edges, and brute force") {
    CHECK(ship_within_days({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5) == 15);
    CHECK(ship_within_days({}, 3) == 0);
    CHECK_THROWS_AS(ship_within_days({1}, 0), std::invalid_argument);
    CHECK_THROWS_AS(ship_within_days({-1}, 1), std::invalid_argument);
    Rng rng(113);
    for (int t = 0; t < 150; ++t) {
        V w;
        int n = rng.in(1, 8);
        for (int i = 0; i < n; ++i) w.push_back(rng.in(1, 10));
        int days = rng.in(1, n);
        auto feasible = [&](int cap) {
            int used = 1, load = 0;
            for (int x : w) {
                if (load + x > cap) {
                    ++used;
                    load = 0;
                }
                load += x;
            }
            return used <= days;
        };
        int brute = *std::max_element(w.begin(), w.end());
        while (!feasible(brute)) ++brute;
        REQUIRE(ship_within_days(w, days) == brute);
    }
}
