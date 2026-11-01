#include <deque>
#include <string>

#include "doctest.h"
#include "dsa/linked_list.hpp"
#include "rng.hpp"

using dsa::DoublyLinkedList;
using dsa::SinglyLinkedList;

TEST_CASE("SinglyLinkedList basics") {
    SinglyLinkedList<int> l;
    CHECK(l.empty());
    l.push_back(2);
    l.push_back(3);
    l.push_front(1);
    CHECK(l.to_vector() == std::vector<int>{1, 2, 3});
    CHECK(l.at(1) == 2);
    CHECK(l.find(3) == 2);
    CHECK(l.find(9) == -1);
}

TEST_CASE("SinglyLinkedList pop keeps the tail consistent") {
    SinglyLinkedList<int> l;
    l.push_back(1);
    CHECK(l.pop_back() == 1);
    CHECK(l.size() == 0);
    l.push_back(5);
    l.push_back(6);
    CHECK(l.pop_back() == 6);
    l.push_back(7);
    CHECK(l.to_vector() == std::vector<int>{5, 7});
    CHECK(l.pop_front() == 5);
    CHECK(l.pop_front() == 7);
    l.push_back(8);
    CHECK(l.to_vector() == std::vector<int>{8});
}

TEST_CASE("SinglyLinkedList insert and erase") {
    SinglyLinkedList<int> l;
    l.insert(0, 1);
    l.insert(1, 3);
    l.insert(1, 2);
    CHECK(l.to_vector() == std::vector<int>{1, 2, 3});
    CHECK(l.erase(2) == 3);
    l.push_back(4);
    CHECK(l.to_vector() == std::vector<int>{1, 2, 4});
    CHECK(l.erase(1) == 2);
    CHECK(l.erase(0) == 1);
}

TEST_CASE("SinglyLinkedList reverse") {
    SinglyLinkedList<int> l;
    l.reverse();
    CHECK(l.to_vector().empty());
    l.push_back(1);
    l.reverse();
    CHECK(l.to_vector() == std::vector<int>{1});
    l.push_back(2);
    l.push_back(3);
    l.reverse();
    CHECK(l.to_vector() == std::vector<int>{3, 2, 1});
    l.push_back(9);
    CHECK(l.to_vector() == std::vector<int>{3, 2, 1, 9});
}

TEST_CASE("SinglyLinkedList errors") {
    SinglyLinkedList<int> l;
    CHECK_THROWS_AS(l.pop_back(), std::underflow_error);
    CHECK_THROWS_AS(l.pop_front(), std::underflow_error);
    CHECK_THROWS_AS(l.at(0), std::out_of_range);
    CHECK_THROWS_AS(l.insert(1, 1), std::out_of_range);
    CHECK_THROWS_AS(l.erase(0), std::out_of_range);
}

TEST_CASE("SinglyLinkedList destroys a 300000 element list without overflowing the stack") {
    SinglyLinkedList<int> l;
    for (int i = 0; i < 300000; ++i) l.push_back(i);
    CHECK(l.size() == 300000);
}

TEST_CASE("SinglyLinkedList works with move-only and string elements") {
    SinglyLinkedList<std::unique_ptr<int>> l;
    l.push_back(std::make_unique<int>(4));
    CHECK(*l.pop_front() == 4);
    SinglyLinkedList<std::string> s;
    s.push_back("a");
    CHECK(s.at(0) == "a");
}

TEST_CASE("SinglyLinkedList matches std::deque under random operations") {
    Rng rng(11);
    SinglyLinkedList<int> l;
    std::deque<int> ref;
    for (int i = 0; i < 3000; ++i) {
        int op = rng.in(0, 5), v = rng.in(0, 99);
        if (op == 0) {
            l.push_back(v);
            ref.push_back(v);
        } else if (op == 1) {
            l.push_front(v);
            ref.push_front(v);
        } else if (op == 2 && !ref.empty()) {
            CHECK(l.pop_back() == ref.back());
            ref.pop_back();
        } else if (op == 3 && !ref.empty()) {
            CHECK(l.pop_front() == ref.front());
            ref.pop_front();
        } else if (op == 4) {
            std::size_t idx = rng.idx(0, ref.size());
            l.insert(idx, v);
            ref.insert(ref.begin() + static_cast<std::ptrdiff_t>(idx), v);
        } else if (op == 5 && !ref.empty()) {
            std::size_t idx = rng.idx(0, ref.size() - 1);
            CHECK(l.erase(idx) == ref[idx]);
            ref.erase(ref.begin() + static_cast<std::ptrdiff_t>(idx));
        }
        REQUIRE(l.size() == ref.size());
    }
    CHECK(l.to_vector() == std::vector<int>(ref.begin(), ref.end()));
}

TEST_CASE("DoublyLinkedList both ends and reverse view") {
    DoublyLinkedList<std::string> l;
    l.push_back("b");
    l.push_back("c");
    l.push_front("a");
    CHECK(l.to_vector() == std::vector<std::string>{"a", "b", "c"});
    CHECK(l.to_vector_reversed() == std::vector<std::string>{"c", "b", "a"});
    CHECK(l.pop_back() == "c");
    CHECK(l.pop_front() == "a");
    CHECK(l.pop_back() == "b");
    CHECK(l.empty());
    CHECK(l.to_vector_reversed().empty());
}

TEST_CASE("DoublyLinkedList at and erase from either half") {
    DoublyLinkedList<int> l;
    for (int i = 0; i < 10; ++i) l.push_back(i);
    CHECK(l.at(1) == 1);
    CHECK(l.at(8) == 8);
    CHECK(l.erase(9) == 9);
    CHECK(l.erase(0) == 0);
    CHECK(l.erase(4) == 5);
    CHECK(l.to_vector() == std::vector<int>{1, 2, 3, 4, 6, 7, 8});
    CHECK(l.to_vector_reversed() == std::vector<int>{8, 7, 6, 4, 3, 2, 1});
}

TEST_CASE("DoublyLinkedList errors") {
    DoublyLinkedList<int> l;
    CHECK_THROWS_AS(l.pop_back(), std::underflow_error);
    CHECK_THROWS_AS(l.pop_front(), std::underflow_error);
    CHECK_THROWS_AS(l.at(0), std::out_of_range);
}

TEST_CASE("DoublyLinkedList matches std::deque under random operations") {
    Rng rng(12);
    DoublyLinkedList<int> l;
    std::deque<int> ref;
    for (int i = 0; i < 3000; ++i) {
        int op = rng.in(0, 4), v = rng.in(0, 99);
        if (op == 0) {
            l.push_back(v);
            ref.push_back(v);
        } else if (op == 1) {
            l.push_front(v);
            ref.push_front(v);
        } else if (op == 2 && !ref.empty()) {
            CHECK(l.pop_back() == ref.back());
            ref.pop_back();
        } else if (op == 3 && !ref.empty()) {
            CHECK(l.pop_front() == ref.front());
            ref.pop_front();
        } else if (op == 4 && !ref.empty()) {
            std::size_t idx = rng.idx(0, ref.size() - 1);
            CHECK(l.erase(idx) == ref[idx]);
            ref.erase(ref.begin() + static_cast<std::ptrdiff_t>(idx));
        }
    }
    std::vector<int> expected(ref.begin(), ref.end());
    CHECK(l.to_vector() == expected);
    CHECK(l.to_vector_reversed() == std::vector<int>(expected.rbegin(), expected.rend()));
}

TEST_CASE("DoublyLinkedList destroys a 300000 element list without overflowing the stack") {
    DoublyLinkedList<int> l;
    for (int i = 0; i < 300000; ++i) l.push_front(i);
    CHECK(l.size() == 300000);
}
