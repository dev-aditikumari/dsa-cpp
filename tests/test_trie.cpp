#include "doctest.h"
#include "dsa/trie.hpp"

using dsa::Trie;
using W = std::vector<std::string>;

TEST_CASE("Trie insert, contains and prefixes") {
    Trie t;
    for (const char* w : {"apple", "app", "apt", "bat"}) t.insert(w);
    CHECK(t.size() == 4);
    CHECK(t.contains("app"));
    CHECK_FALSE(t.contains("ap"));
    CHECK(t.starts_with("ap"));
    CHECK_FALSE(t.starts_with("c"));
    CHECK(t.with_prefix("ap") == W{"app", "apple", "apt"});
    CHECK(t.with_prefix("zz").empty());
}

TEST_CASE("Trie duplicates and the empty string") {
    Trie t;
    t.insert("a");
    t.insert("a");
    CHECK(t.size() == 1);
    CHECK_FALSE(t.contains(""));
    t.insert("");
    CHECK(t.contains(""));
    CHECK(t.with_prefix("") == W{"", "a"});
}

TEST_CASE("Trie erase prunes without breaking other words") {
    Trie t;
    t.insert("app");
    t.insert("apple");
    CHECK(t.erase("apple"));
    CHECK(t.contains("app"));
    CHECK_FALSE(t.starts_with("appl"));
    CHECK(t.erase("app"));
    CHECK_FALSE(t.starts_with("a"));
    CHECK_FALSE(t.erase("app"));
    CHECK_FALSE(t.erase("zebra"));
    t.insert("abc");
    CHECK_FALSE(t.erase("ab"));
    CHECK(t.size() == 1);
}

TEST_CASE("Trie stores UTF-8 bytes") {
    Trie t;
    t.insert("na\xC3\xAFve");
    CHECK(t.contains("na\xC3\xAFve"));
    CHECK(t.starts_with("na"));
}
