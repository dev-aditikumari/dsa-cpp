#include <algorithm>
#include <list>

#include "doctest.h"
#include "dsa/lru_cache.hpp"
#include "rng.hpp"

using dsa::LRUCache;

TEST_CASE("LRUCache evicts the least recently used key") {
    LRUCache<std::string, int> c(2);
    c.put("a", 1);
    c.put("b", 2);
    CHECK(c.get("a") == 1);
    c.put("c", 3);
    CHECK_FALSE(c.contains("b"));
    CHECK(c.keys() == std::vector<std::string>{"c", "a"});
}

TEST_CASE("LRUCache update refreshes a key without growing") {
    LRUCache<std::string, int> c(2);
    c.put("a", 1);
    c.put("b", 2);
    c.put("a", 10);
    c.put("c", 3);
    CHECK(c.get("a") == 10);
    CHECK_FALSE(c.get("b").has_value());
    CHECK(c.size() == 2);
}

TEST_CASE("LRUCache with capacity 1 and invalid capacity") {
    LRUCache<int, int> c(1);
    c.put(1, 1);
    c.put(2, 2);
    CHECK_FALSE(c.get(1).has_value());
    CHECK(c.get(2) == 2);
    CHECK_THROWS_AS((LRUCache<int, int>(0)), std::invalid_argument);
}

TEST_CASE("LRUCache matches a list-based reference under random operations") {
    Rng rng(71);
    const std::size_t cap = 7;
    LRUCache<int, int> c(cap);
    std::list<std::pair<int, int>> ref;  // front = most recent
    for (int i = 0; i < 15000; ++i) {
        int k = rng.in(0, 20);
        auto it = std::find_if(ref.begin(), ref.end(), [&](const auto& p) { return p.first == k; });
        if (rng.in(0, 1) == 0) {
            c.put(k, i);
            if (it != ref.end()) ref.erase(it);
            ref.emplace_front(k, i);
            if (ref.size() > cap) ref.pop_back();
        } else {
            auto got = c.get(k);
            REQUIRE(got.has_value() == (it != ref.end()));
            if (it != ref.end()) {
                CHECK(*got == it->second);
                ref.splice(ref.begin(), ref, it);
            }
        }
        std::vector<int> keys;
        for (const auto& p : ref) keys.push_back(p.first);
        REQUIRE(c.keys() == keys);
    }
}
