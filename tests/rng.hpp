#pragma once
#include <cstddef>
#include <random>
#include <vector>

// Seeded generator so every random test is reproducible.
struct Rng {
    std::mt19937 gen;
    explicit Rng(unsigned seed) : gen(seed) {}
    int in(int lo, int hi) { return std::uniform_int_distribution<int>(lo, hi)(gen); }
    std::size_t idx(std::size_t lo, std::size_t hi) { return std::uniform_int_distribution<std::size_t>(lo, hi)(gen); }
    std::vector<int> vec(int max_len, int lo = -50, int hi = 50) {
        std::vector<int> v(static_cast<std::size_t>(in(0, max_len)));
        for (int& x : v) x = in(lo, hi);
        return v;
    }
};
