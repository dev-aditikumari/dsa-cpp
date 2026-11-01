#include <cmath>
#include <queue>

#include "doctest.h"
#include "dsa/heap.hpp"
#include "rng.hpp"

using namespace dsa;

TEST_CASE("MinHeap pops ascending") {
    MinHeap<int> h;
    for (int x : {5, 1, 4, 1, 3}) h.push(x);
    CHECK(h.top() == 1);
    std::vector<int> out;
    while (!h.empty()) out.push_back(h.pop());
    CHECK(out == std::vector<int>{1, 1, 3, 4, 5});
}

TEST_CASE("MaxHeap pops descending") {
    MaxHeap<int> h;
    for (int x : {5, 1, 4}) h.push(x);
    CHECK(h.pop() == 5);
    CHECK(h.pop() == 4);
    CHECK(h.pop() == 1);
}

TEST_CASE("BinaryHeap with a custom comparator and strings") {
    struct P {
        std::string n;
        int age;
    };
    BinaryHeap<P, bool (*)(const P&, const P&)> h([](const P& a, const P& b) { return a.age < b.age; });
    h.push({"a", 30});
    h.push({"b", 20});
    CHECK(h.pop().n == "b");
    MinHeap<std::string> s(std::vector<std::string>{"pear", "apple", "fig"});
    CHECK(s.pop() == "apple");
}

TEST_CASE("BinaryHeap throws on empty") {
    MinHeap<int> h;
    CHECK_THROWS_AS(h.pop(), std::underflow_error);
    CHECK_THROWS_AS(h.top(), std::underflow_error);
}

TEST_CASE("heapify from a vector sorts 200 random arrays") {
    Rng rng(31);
    for (int t = 0; t < 200; ++t) {
        std::vector<int> v = rng.vec(60);
        MinHeap<int> h(v);
        std::vector<int> out;
        while (!h.empty()) out.push_back(h.pop());
        std::sort(v.begin(), v.end());
        CHECK(out == v);
    }
}

TEST_CASE("MaxHeap matches std::priority_queue under random push and pop") {
    Rng rng(32);
    MaxHeap<int> h;
    std::priority_queue<int> ref;
    for (int i = 0; i < 5000; ++i) {
        if (rng.in(0, 2) > 0 || ref.empty()) {
            int v = rng.in(0, 100);
            h.push(v);
            ref.push(v);
        } else {
            CHECK(h.top() == ref.top());
            CHECK(h.pop() == ref.top());
            ref.pop();
        }
        REQUIRE(h.size() == ref.size());
    }
}

TEST_CASE("PriorityQueue orders by priority and FIFO among ties") {
    PriorityQueue<std::string> pq;
    pq.push("low", 5);
    pq.push("first", 1);
    pq.push("second", 1);
    pq.push("mid", 3);
    CHECK(pq.top() == "first");
    CHECK(pq.pop() == "first");
    CHECK(pq.pop() == "second");
    CHECK(pq.pop() == "mid");
    CHECK(pq.pop() == "low");
    CHECK(pq.empty());
    CHECK(pq.size() == 0);
}

TEST_CASE("PriorityQueue rejects NaN and empty access") {
    PriorityQueue<int> pq;
    CHECK_THROWS_AS(pq.push(1, std::nan("")), std::invalid_argument);
    CHECK_THROWS_AS(pq.pop(), std::underflow_error);
    CHECK_THROWS_AS(pq.top(), std::underflow_error);
}
