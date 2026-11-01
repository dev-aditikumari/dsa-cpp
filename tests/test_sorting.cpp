#include <algorithm>
#include <climits>
#include <functional>
#include <string>

#include "doctest.h"
#include "dsa/sorting.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;

using SortFn = V (*)(V);
static const std::vector<std::pair<const char*, SortFn>>& all_sorts() {
    static const std::vector<std::pair<const char*, SortFn>> sorts = {
        {"bubble", [](V a) { return bubble_sort(std::move(a)); }},
        {"insertion", [](V a) { return insertion_sort(std::move(a)); }},
        {"selection", [](V a) { return selection_sort(std::move(a)); }},
        {"merge", [](V a) { return merge_sort(a); }},
        {"quick", [](V a) { return quick_sort(std::move(a)); }},
        {"heap", [](V a) { return heap_sort(std::move(a)); }},
        {"counting", [](V a) { return counting_sort(a); }},
    };
    return sorts;
}

TEST_CASE("every sort handles empty, single, sorted and reversed input") {
    for (const auto& entry : all_sorts()) {
        const char* name = entry.first;
        SortFn sort = entry.second;
        CAPTURE(name);
        CHECK(sort({}).empty());
        CHECK(sort({7}) == V{7});
        CHECK(sort({1, 2, 3}) == V{1, 2, 3});
        CHECK(sort({3, 2, 1}) == V{1, 2, 3});
    }
}

TEST_CASE("every sort handles duplicates and negatives") {
    for (const auto& entry : all_sorts()) {
        const char* name = entry.first;
        SortFn sort = entry.second;
        CAPTURE(name);
        CHECK(sort({3, -1, 3, 0, -1, 3}) == V{-1, -1, 0, 3, 3, 3});
        CHECK(sort({5, 5, 5, 5}) == V{5, 5, 5, 5});
    }
}

TEST_CASE("every sort equals std::sort on 200 random arrays") {
    Rng rng(101);
    for (int t = 0; t < 200; ++t) {
        V arr = rng.vec(80, -30, 30);
        V expected = arr;
        std::sort(expected.begin(), expected.end());
        for (const auto& entry : all_sorts()) {
            const char* name = entry.first;
            SortFn sort = entry.second;
            CAPTURE(name);
            REQUIRE(sort(arr) == expected);
        }
    }
}

TEST_CASE("the fast sorts handle 100000 random numbers") {
    Rng rng(102);
    V arr(100000);
    for (int& x : arr) x = rng.in(-1000, 1000);
    V expected = arr;
    std::sort(expected.begin(), expected.end());
    CHECK(merge_sort(arr) == expected);
    CHECK(quick_sort(arr) == expected);
    CHECK(heap_sort(arr) == expected);
    CHECK(counting_sort(arr) == expected);
}

TEST_CASE("quick sort handles sorted, reversed and all-equal arrays of 100000") {
    V asc(100000);
    for (std::size_t i = 0; i < asc.size(); ++i) asc[i] = static_cast<int>(i);
    CHECK(quick_sort(asc) == asc);
    CHECK(quick_sort(V(asc.rbegin(), asc.rend())) == asc);
    CHECK(quick_sort(V(100000, 4)) == V(100000, 4));
}

struct Item {
    int key;
    int id;
};

TEST_CASE("bubble, insertion and merge sort are stable") {
    Rng rng(103);
    std::vector<Item> items;
    for (int id = 0; id < 300; ++id) items.push_back({rng.in(0, 5), id});
    auto by_key = [](const Item& a, const Item& b) { return a.key < b.key; };
    std::vector<std::vector<Item>> outs = {bubble_sort(items, by_key), insertion_sort(items, by_key), merge_sort(items, by_key)};
    for (const auto& out : outs) {
        for (std::size_t i = 1; i < out.size(); ++i) {
            bool ok = out[i - 1].key < out[i].key || (out[i - 1].key == out[i].key && out[i - 1].id < out[i].id);
            REQUIRE(ok);
        }
    }
}

TEST_CASE("sorting strings and descending order") {
    CHECK(quick_sort(std::vector<std::string>{"pear", "apple", "fig"}) == std::vector<std::string>{"apple", "fig", "pear"});
    CHECK(heap_sort(V{1, 3, 2}, std::greater<int>()) == V{3, 2, 1});
    CHECK(selection_sort(V{1, 3, 2}, std::greater<int>()) == V{3, 2, 1});
}

TEST_CASE("sorts do not change the caller's vector") {
    const V input{3, 1, 2};
    merge_sort(input);
    CHECK(input == V{3, 1, 2});
}

TEST_CASE("counting sort limits") {
    CHECK(counting_sort({INT_MIN, INT_MIN + 3}) == V{INT_MIN, INT_MIN + 3});
    CHECK_THROWS_AS(counting_sort({INT_MIN, INT_MAX}), std::invalid_argument);
    CHECK_THROWS_AS(counting_sort({0, 100}, 50), std::invalid_argument);
}
