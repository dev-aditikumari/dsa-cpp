#include <deque>

#include "doctest.h"
#include "dsa/stack_queue.hpp"
#include "rng.hpp"

using namespace dsa;

TEST_CASE("Stack is LIFO") {
    Stack<int> s;
    CHECK(s.empty());
    s.push(1);
    s.push(2);
    CHECK(s.top() == 2);
    CHECK(s.size() == 2);
    CHECK(s.pop() == 2);
    CHECK(s.pop() == 1);
    CHECK(s.empty());
}

TEST_CASE("Stack throws when empty") {
    Stack<int> s;
    CHECK_THROWS_AS(s.pop(), std::underflow_error);
    CHECK_THROWS_AS(s.top(), std::underflow_error);
}

TEST_CASE("Queue is FIFO") {
    Queue<std::string> q;
    q.push("a");
    q.push("b");
    CHECK(q.front() == "a");
    CHECK(q.pop() == "a");
    CHECK(q.pop() == "b");
    CHECK(q.empty());
    CHECK_THROWS_AS(q.pop(), std::underflow_error);
    CHECK_THROWS_AS(q.front(), std::underflow_error);
}

TEST_CASE("Queue keeps order across 10000 items and interleaved use") {
    Queue<int> q;
    for (int i = 0; i < 10000; ++i) q.push(i);
    for (int i = 0; i < 9000; ++i) CHECK(q.pop() == i);
    for (int i = 10000; i < 10100; ++i) q.push(i);
    CHECK(q.size() == 1100);
    CHECK(q.pop() == 9000);
}

TEST_CASE("Deque works from both ends and grows") {
    Deque<int> d(2);
    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    d.push_front(0);
    CHECK(d.to_vector() == std::vector<int>{0, 1, 2, 3});
    CHECK(d.front() == 0);
    CHECK(d.back() == 3);
    CHECK(d.pop_front() == 0);
    CHECK(d.pop_back() == 3);
    CHECK(d.size() == 2);
}

TEST_CASE("Deque throws when empty and tolerates capacity 0") {
    Deque<int> d(0);
    CHECK_THROWS_AS(d.pop_front(), std::underflow_error);
    CHECK_THROWS_AS(d.pop_back(), std::underflow_error);
    CHECK_THROWS_AS(d.front(), std::underflow_error);
    CHECK_THROWS_AS(d.back(), std::underflow_error);
    d.push_back(1);
    CHECK(d.front() == 1);
}

TEST_CASE("Deque matches std::deque under random operations with wrap-around") {
    Rng rng(21);
    Deque<int> d(1);
    std::deque<int> ref;
    for (int i = 0; i < 6000; ++i) {
        int op = rng.in(0, 3);
        if (op == 0) {
            d.push_back(i);
            ref.push_back(i);
        } else if (op == 1) {
            d.push_front(i);
            ref.push_front(i);
        } else if (op == 2 && !ref.empty()) {
            CHECK(d.pop_back() == ref.back());
            ref.pop_back();
        } else if (op == 3 && !ref.empty()) {
            CHECK(d.pop_front() == ref.front());
            ref.pop_front();
        }
        REQUIRE(d.size() == ref.size());
    }
    CHECK(d.to_vector() == std::vector<int>(ref.begin(), ref.end()));
}
