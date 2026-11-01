#include <algorithm>
#include <set>

#include "doctest.h"
#include "dsa/backtracking.hpp"

using namespace dsa;
using V = std::vector<int>;

TEST_CASE("permutations count and distinctness") {
    auto p = permutations(V{1, 2, 3});
    CHECK(p.size() == 6);
    CHECK(std::set<V>(p.begin(), p.end()).size() == 6);
    CHECK(permutations(V{}).size() == 1);
    CHECK(permutations(std::vector<std::string>{"x"}).size() == 1);
    CHECK(permutations(V{1, 2, 3, 4, 5, 6}).size() == 720);
}

TEST_CASE("subsets") {
    auto s = subsets({1, 2, 3});
    CHECK(s.size() == 8);
    CHECK(std::count(s.begin(), s.end(), V{}) == 1);
    CHECK(std::count(s.begin(), s.end(), V{1, 2, 3}) == 1);
    CHECK(subsets({}).size() == 1);
    auto d = subsets({2, 1, 2});
    std::sort(d.begin(), d.end());
    CHECK(d == std::vector<V>{{}, {1}, {1, 2}, {1, 2, 2}, {2}, {2, 2}});
}

TEST_CASE("combinations") {
    CHECK(combinations(4, 2) == std::vector<V>{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}});
    CHECK(combinations(3, 0) == std::vector<V>{{}});
    CHECK(combinations(2, 3).empty());
    CHECK(combinations(10, 5).size() == 252);
    CHECK_THROWS_AS(combinations(-1, 1), std::invalid_argument);
}

TEST_CASE("n-queens solution counts") {
    const std::pair<int, std::size_t> expected[] = {{0, 1}, {1, 1}, {2, 0}, {3, 0}, {4, 2}, {5, 10}, {6, 4}, {7, 40}, {8, 92}};
    for (const auto& e : expected) {
        CAPTURE(e.first);
        CHECK(solve_n_queens(e.first).size() == e.second);
    }
    CHECK_THROWS_AS(solve_n_queens(-1), std::invalid_argument);
}

TEST_CASE("every 8-queens solution is valid") {
    for (const V& sol : solve_n_queens(8)) {
        for (std::size_t i = 0; i < 8; ++i)
            for (std::size_t j = i + 1; j < 8; ++j) {
                int diff = sol[i] > sol[j] ? sol[i] - sol[j] : sol[j] - sol[i];
                REQUIRE(sol[i] != sol[j]);
                REQUIRE(diff != static_cast<int>(j - i));
            }
    }
}

static SudokuBoard puzzle() {
    const char* rows[9] = {"530070000", "600195000", "098000060", "800060003", "400803001",
                           "700020006", "060000280", "000419005", "000080079"};
    SudokuBoard b{};
    for (std::size_t r = 0; r < 9; ++r)
        for (std::size_t c = 0; c < 9; ++c) b[r][c] = rows[r][c] - '0';
    return b;
}

TEST_CASE("sudoku solver fills a valid board and keeps the givens") {
    SudokuBoard given = puzzle(), board = puzzle();
    REQUIRE(solve_sudoku(board));
    for (std::size_t i = 0; i < 9; ++i) {
        std::set<int> row, col, box;
        for (std::size_t j = 0; j < 9; ++j) {
            row.insert(board[i][j]);
            col.insert(board[j][i]);
            box.insert(board[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3]);
        }
        CHECK(row.size() == 9);
        CHECK(col.size() == 9);
        CHECK(box.size() == 9);
        CHECK(*row.begin() == 1);
        CHECK(*row.rbegin() == 9);
    }
    for (std::size_t r = 0; r < 9; ++r)
        for (std::size_t c = 0; c < 9; ++c)
            if (given[r][c] != 0) CHECK(board[r][c] == given[r][c]);
}

TEST_CASE("sudoku solver reports an unsolvable board") {
    SudokuBoard b = puzzle();
    b[0][2] = 5;  // clashes with the 5 already in row 0
    b[0][1] = 0;
    b[0][0] = 5;
    CHECK_FALSE(solve_sudoku(b));
}
