#include <cmath>
#include <set>
#include <string>

#include "doctest.h"
#include "dsa/trees.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;

static BinarySearchTree<int>& fill(BinarySearchTree<int>& t) {
    for (int x : {8, 3, 10, 1, 6, 14, 4, 7, 13}) t.insert(x);
    return t;
}

TEST_CASE("BST traversals match known results") {
    BinarySearchTree<int> t;
    fill(t);
    CHECK(t.in_order() == V{1, 3, 4, 6, 7, 8, 10, 13, 14});
    CHECK(t.pre_order() == V{8, 3, 1, 6, 4, 7, 10, 14, 13});
    CHECK(t.post_order() == V{1, 4, 7, 6, 3, 13, 14, 10, 8});
    CHECK(t.level_order() == V{8, 3, 10, 1, 6, 14, 4, 7, 13});
    CHECK(t.height() == 4);
    CHECK(t.min() == 1);
    CHECK(t.max() == 14);
}

TEST_CASE("BST ignores duplicates") {
    BinarySearchTree<int> t;
    CHECK(t.insert(1));
    CHECK_FALSE(t.insert(1));
    CHECK(t.size() == 1);
}

TEST_CASE("BST erase covers leaf, one child, two children and root") {
    BinarySearchTree<int> t;
    fill(t);
    CHECK(t.erase(1));
    CHECK(t.erase(14));
    CHECK(t.erase(3));
    CHECK(t.erase(8));
    CHECK_FALSE(t.erase(99));
    CHECK(t.in_order() == V{4, 6, 7, 10, 13});
    CHECK(t.is_valid());
    CHECK_FALSE(t.contains(8));
    CHECK(t.contains(7));
}

TEST_CASE("BST empty and single element cases") {
    BinarySearchTree<std::string> t;
    CHECK(t.height() == 0);
    CHECK(t.in_order().empty());
    CHECK(t.post_order().empty());
    CHECK_FALSE(t.erase("x"));
    CHECK_THROWS_AS(t.min(), std::underflow_error);
    CHECK_THROWS_AS(t.max(), std::underflow_error);
    t.insert("x");
    CHECK(t.erase("x"));
    CHECK(t.empty());
}

TEST_CASE("BST handles 100000 sorted inserts (a chain) without recursion") {
    BinarySearchTree<int> t;
    for (int i = 0; i < 100000; ++i) t.insert(i);
    CHECK(t.height() == 100000);
    CHECK(t.in_order().size() == 100000);
    CHECK(t.post_order().front() == 99999);
}

TEST_CASE("BST keeps its invariants and matches std::set under random inserts and erases") {
    Rng rng(51);
    BinarySearchTree<int> t;
    std::set<int> ref;
    for (int i = 0; i < 5000; ++i) {
        int v = rng.in(0, 400);
        if (rng.in(0, 2) < 2) CHECK(t.insert(v) == ref.insert(v).second);
        else CHECK(t.erase(v) == (ref.erase(v) == 1));
        if (i % 50 == 0) {
            REQUIRE(t.is_valid());
            REQUIRE(t.in_order() == V(ref.begin(), ref.end()));
        }
    }
    CHECK(t.size() == ref.size());
}

TEST_CASE("AVL stays balanced on sorted inserts") {
    AVLTree<int> t;
    for (int i = 0; i < 1023; ++i) t.insert(i);
    CHECK(t.height() == 10);
    CHECK(t.is_valid());
    CHECK(t.in_order()[500] == 500);
}

TEST_CASE("AVL covers all four rotation cases") {
    for (V seq : {V{3, 2, 1}, V{1, 2, 3}, V{3, 1, 2}, V{1, 3, 2}}) {
        AVLTree<int> t;
        for (int x : seq) t.insert(x);
        CHECK(t.height() == 2);
        CHECK(t.in_order() == V{1, 2, 3});
        CHECK(t.is_valid());
    }
}

TEST_CASE("AVL insert and erase report whether the tree changed") {
    AVLTree<int> t;
    CHECK(t.insert(5));
    CHECK_FALSE(t.insert(5));
    CHECK(t.contains(5));
    CHECK_FALSE(t.erase(6));
    CHECK(t.erase(5));
    CHECK(t.size() == 0);
    CHECK(t.height() == 0);
}

TEST_CASE("AVL with a custom comparator") {
    AVLTree<std::string, std::greater<std::string>> t;
    for (const char* s : {"b", "a", "c"}) t.insert(s);
    CHECK(t.in_order() == std::vector<std::string>{"c", "b", "a"});
}

TEST_CASE("AVL keeps invariants and matches std::set under random inserts and erases") {
    Rng rng(52);
    AVLTree<int> t;
    std::set<int> ref;
    for (int i = 0; i < 8000; ++i) {
        int v = rng.in(0, 500);
        if (rng.in(0, 2) < 2) CHECK(t.insert(v) == ref.insert(v).second);
        else CHECK(t.erase(v) == (ref.erase(v) == 1));
        REQUIRE(t.size() == ref.size());
        if (i % 25 == 0) REQUIRE(t.is_valid());
    }
    CHECK(t.is_valid());
    CHECK(t.in_order() == V(ref.begin(), ref.end()));
    CHECK(t.height() <= 1.45 * std::log2(static_cast<double>(ref.size()) + 2));
}
