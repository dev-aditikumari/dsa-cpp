#include <map>
#include <string>
#include <unordered_map>

#include "doctest.h"
#include "dsa/hash_map.hpp"
#include "rng.hpp"

using dsa::HashMap;

TEST_CASE("HashMap insert, find, overwrite and erase") {
    HashMap<std::string, int> m;
    CHECK(m.insert_or_assign("a", 1));
    CHECK(m.insert_or_assign("b", 2));
    CHECK_FALSE(m.insert_or_assign("a", 3));
    CHECK(m.size() == 2);
    CHECK(m.at("a") == 3);
    CHECK(m.contains("b"));
    CHECK(m.find("zzz") == nullptr);
    CHECK(m.erase("a"));
    CHECK_FALSE(m.erase("a"));
    CHECK(m.size() == 1);
    CHECK_THROWS_AS(m.at("a"), std::out_of_range);
}

TEST_CASE("HashMap survives total collisions") {
    struct ConstHash {
        std::size_t operator()(int) const { return 7; }
    };
    HashMap<int, int, ConstHash> m(4);
    for (int i = 0; i < 60; ++i) m.insert_or_assign(i, i * 2);
    for (int i = 0; i < 60; ++i) CHECK(m.at(i) == i * 2);
    CHECK(m.erase(30));
    CHECK(m.erase(0));
    CHECK(m.erase(59));
    CHECK(m.size() == 57);
    CHECK_FALSE(m.contains(30));
}

TEST_CASE("HashMap grows when the load factor is exceeded") {
    HashMap<int, int> m(2);
    for (int i = 0; i < 100; ++i) m.insert_or_assign(i, i);
    CHECK(m.bucket_count() * 3 >= m.size() * 4);
    CHECK(m.bucket_count() > 2);
    int seen = 0;
    m.for_each([&](int k, int v) {
        CHECK(k == v);
        ++seen;
    });
    CHECK(seen == 100);
}

TEST_CASE("HashMap with a zero bucket hint still works") {
    HashMap<int, int> m(0);
    m.insert_or_assign(1, 1);
    CHECK(m.at(1) == 1);
}

TEST_CASE("HashMap matches std::unordered_map under 30000 random operations") {
    Rng rng(41);
    HashMap<int, int> m;
    std::unordered_map<int, int> ref;
    for (int i = 0; i < 30000; ++i) {
        int k = rng.in(0, 300), op = rng.in(0, 2);
        if (op == 0) {
            CHECK(m.insert_or_assign(k, i) == (ref.count(k) == 0));
            ref[k] = i;
        } else if (op == 1) {
            CHECK(m.erase(k) == (ref.erase(k) == 1));
        } else {
            const int* got = m.find(k);
            auto it = ref.find(k);
            REQUIRE((got != nullptr) == (it != ref.end()));
            if (got) CHECK(*got == it->second);
        }
        REQUIRE(m.size() == ref.size());
    }
    std::map<int, int> a;
    m.for_each([&](int k, int v) { a[k] = v; });
    CHECK(a == std::map<int, int>(ref.begin(), ref.end()));
}

TEST_CASE("HashMap holds move-only values and frees a long chain") {
    struct ConstHash {
        std::size_t operator()(int) const { return 0; }
    };
    HashMap<int, std::unique_ptr<int>, ConstHash> m(1024);
    for (int i = 0; i < 2000; ++i) m.insert_or_assign(i, std::make_unique<int>(i));
    CHECK(*m.at(1999) == 1999);
}
