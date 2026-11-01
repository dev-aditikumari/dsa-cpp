#pragma once
// String algorithms. Strings are treated as bytes (ASCII).
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace dsa {

// O(n + m) time, O(m) space. Start index of every (possibly overlapping) occurrence. Empty pattern matches nowhere.
inline std::vector<std::size_t> kmp_search(const std::string& text, const std::string& pattern) {
    std::vector<std::size_t> out;
    if (pattern.empty()) return out;
    std::vector<std::size_t> fail(pattern.size(), 0);
    for (std::size_t i = 1, k = 0; i < pattern.size(); ++i) {
        while (k > 0 && pattern[i] != pattern[k]) k = fail[k - 1];
        if (pattern[i] == pattern[k]) ++k;
        fail[i] = k;
    }
    for (std::size_t i = 0, k = 0; i < text.size(); ++i) {
        while (k > 0 && text[i] != pattern[k]) k = fail[k - 1];
        if (text[i] == pattern[k]) ++k;
        if (k == pattern.size()) {
            out.push_back(i + 1 - k);
            k = fail[k - 1];
        }
    }
    return out;
}

// O(n) time, O(1) space. Palindrome check over letters and digits, ignoring case and other characters.
inline bool is_palindrome(const std::string& s) {
    if (s.empty()) return true;
    std::size_t i = 0, j = s.size() - 1;
    while (i < j) {
        while (i < j && !std::isalnum(static_cast<unsigned char>(s[i]))) ++i;
        while (i < j && !std::isalnum(static_cast<unsigned char>(s[j]))) --j;
        if (std::tolower(static_cast<unsigned char>(s[i])) != std::tolower(static_cast<unsigned char>(s[j]))) return false;
        if (i == j) break;
        ++i;
        --j;
    }
    return true;
}

// O(n^2) time, O(1) space. Longest palindromic substring, leftmost on ties.
inline std::string longest_palindrome(const std::string& s) {
    std::size_t start = 0, len = 0;
    auto expand = [&](long l, long r) {
        while (l >= 0 && r < static_cast<long>(s.size()) && s[static_cast<std::size_t>(l)] == s[static_cast<std::size_t>(r)]) {
            --l;
            ++r;
        }
        auto found = static_cast<std::size_t>(r - l - 1);
        if (found > len) {
            len = found;
            start = static_cast<std::size_t>(l + 1);
        }
    };
    for (long i = 0; i < static_cast<long>(s.size()); ++i) {
        expand(i, i);
        expand(i, i + 1);
    }
    return s.substr(start, len);
}

// O(n + m) time, O(alphabet) space. Whether two strings use the same letters the same number of times.
inline bool is_anagram(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    std::unordered_map<char, long> counts;
    for (char c : a) ++counts[c];
    for (char c : b) {
        if (--counts[c] < 0) return false;
    }
    return true;
}

// O(total characters * log k) time, O(total characters) space. Groups of anagrams, ordered by first appearance.
inline std::vector<std::vector<std::string>> group_anagrams(const std::vector<std::string>& words) {
    std::map<std::string, std::size_t> index;
    std::vector<std::vector<std::string>> groups;
    for (const std::string& w : words) {
        std::string key = w;
        std::sort(key.begin(), key.end());
        auto [it, inserted] = index.try_emplace(key, groups.size());
        if (inserted) groups.emplace_back();
        groups[it->second].push_back(w);
    }
    return groups;
}

}  // namespace dsa
