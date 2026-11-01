#pragma once
// Backtracking searches. Results are returned in full, so output size dominates memory.
#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace dsa {

// O(n * n!) time. All permutations in index order (equal items still give repeated permutations).
template <typename T>
std::vector<std::vector<T>> permutations(const std::vector<T>& items) {
    std::vector<std::vector<T>> out;
    std::vector<bool> used(items.size(), false);
    std::vector<T> cur;
    auto go = [&](auto& self) -> void {
        if (cur.size() == items.size()) {
            out.push_back(cur);
            return;
        }
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (used[i]) continue;
            used[i] = true;
            cur.push_back(items[i]);
            self(self);
            cur.pop_back();
            used[i] = false;
        }
    };
    go(go);
    return out;
}

// O(n * 2^n) time. All distinct subsets (duplicates in the input do not produce repeats), elements ascending.
inline std::vector<std::vector<int>> subsets(std::vector<int> a) {
    std::sort(a.begin(), a.end());
    std::vector<std::vector<int>> out;
    std::vector<int> cur;
    auto go = [&](auto& self, std::size_t start) -> void {
        out.push_back(cur);
        for (std::size_t i = start; i < a.size(); ++i) {
            if (i > start && a[i] == a[i - 1]) continue;
            cur.push_back(a[i]);
            self(self, i + 1);
            cur.pop_back();
        }
    };
    go(go, 0);
    return out;
}

// O(k * C(n, k)) time. All k-element combinations of 1..n in lexicographic order.
inline std::vector<std::vector<int>> combinations(int n, int k) {
    if (n < 0 || k < 0) throw std::invalid_argument("n and k must be non-negative");
    std::vector<std::vector<int>> out;
    std::vector<int> cur;
    auto go = [&](auto& self, int start) -> void {
        if (static_cast<int>(cur.size()) == k) {
            out.push_back(cur);
            return;
        }
        for (int i = start; i <= n - (k - static_cast<int>(cur.size())) + 1; ++i) {
            cur.push_back(i);
            self(self, i + 1);
            cur.pop_back();
        }
    };
    go(go, 1);
    return out;
}

// Roughly O(n!) time. Every n-queens solution; each lists the column of the queen in each row.
inline std::vector<std::vector<int>> solve_n_queens(int n) {
    if (n < 0) throw std::invalid_argument("n must be non-negative");
    std::vector<std::vector<int>> out;
    std::vector<bool> col(static_cast<std::size_t>(n), false);
    std::vector<bool> d1(static_cast<std::size_t>(2 * n), false), d2(static_cast<std::size_t>(2 * n), false);
    std::vector<int> placement;
    auto go = [&](auto& self, int row) -> void {
        if (row == n) {
            out.push_back(placement);
            return;
        }
        for (int c = 0; c < n; ++c) {
            auto uc = static_cast<std::size_t>(c);
            auto a = static_cast<std::size_t>(row - c + n);
            auto b = static_cast<std::size_t>(row + c);
            if (col[uc] || d1[a] || d2[b]) continue;
            col[uc] = d1[a] = d2[b] = true;
            placement.push_back(c);
            self(self, row + 1);
            placement.pop_back();
            col[uc] = d1[a] = d2[b] = false;
        }
    };
    go(go, 0);
    return out;
}

using SudokuBoard = std::array<std::array<int, 9>, 9>;

// Exponential worst case. Fills a 9x9 board in place (0 is an empty cell). Returns false if there is no solution.
inline bool solve_sudoku(SudokuBoard& board) {
    auto ok = [&](std::size_t r, std::size_t c, int v) {
        for (std::size_t i = 0; i < 9; ++i) {
            if (board[r][i] == v || board[i][c] == v) return false;
        }
        std::size_t br = r - r % 3, bc = c - c % 3;
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 3; ++j) {
                if (board[br + i][bc + j] == v) return false;
            }
        }
        return true;
    };
    auto solve = [&](auto& self) -> bool {
        for (std::size_t r = 0; r < 9; ++r) {
            for (std::size_t c = 0; c < 9; ++c) {
                if (board[r][c] != 0) continue;
                for (int v = 1; v <= 9; ++v) {
                    if (!ok(r, c, v)) continue;
                    board[r][c] = v;
                    if (self(self)) return true;
                    board[r][c] = 0;
                }
                return false;
            }
        }
        return true;
    };
    return solve(solve);
}

}  // namespace dsa
