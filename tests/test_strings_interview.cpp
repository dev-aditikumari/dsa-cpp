#include <algorithm>
#include <set>

#include "doctest.h"
#include "dsa/interview.hpp"
#include "dsa/strings.hpp"
#include "rng.hpp"

using namespace dsa;
using V = std::vector<int>;
using Idx = std::vector<std::size_t>;

static std::string rand_str(Rng& rng, int max_len, const char* alphabet, int size) {
    std::string s;
    for (int i = 0, n = rng.in(0, max_len); i < n; ++i) s.push_back(alphabet[rng.in(0, size - 1)]);
    return s;
}

TEST_CASE("kmp_search finds every occurrence including overlaps") {
    CHECK(kmp_search("aaaaa", "aa") == Idx{0, 1, 2, 3});
    CHECK(kmp_search("abxabcabcaby", "abcaby") == Idx{6});
    CHECK(kmp_search("abc", "d").empty());
    CHECK(kmp_search("", "a").empty());
    CHECK(kmp_search("abc", "").empty());
    CHECK(kmp_search("ab", "abc").empty());
}

TEST_CASE("kmp_search matches std::string::find on random strings") {
    Rng rng(151);
    for (int t = 0; t < 400; ++t) {
        std::string text = rand_str(rng, 30, "ab", 2), pat = rand_str(rng, 4, "ab", 2);
        if (pat.empty()) pat = "a";
        Idx expected;
        for (std::size_t pos = text.find(pat); pos != std::string::npos; pos = text.find(pat, pos + 1)) expected.push_back(pos);
        REQUIRE(kmp_search(text, pat) == expected);
    }
}

TEST_CASE("palindromes") {
    CHECK(is_palindrome("A man, a plan, a canal: Panama"));
    CHECK_FALSE(is_palindrome("race a car"));
    CHECK(is_palindrome(""));
    CHECK(is_palindrome(".,"));
    CHECK(is_palindrome("a"));
    CHECK(longest_palindrome("babad") == "bab");
    CHECK(longest_palindrome("cbbd") == "bb");
    CHECK(longest_palindrome("") == "");
    CHECK(longest_palindrome("x") == "x");
}

TEST_CASE("longest_palindrome length matches brute force") {
    Rng rng(152);
    for (int t = 0; t < 300; ++t) {
        std::string s = rand_str(rng, 14, "abc", 3);
        std::size_t best = 0;
        for (std::size_t i = 0; i < s.size(); ++i)
            for (std::size_t j = i + 1; j <= s.size(); ++j) {
                std::string sub = s.substr(i, j - i);
                if (std::equal(sub.begin(), sub.end(), sub.rbegin())) best = std::max(best, sub.size());
            }
        REQUIRE(longest_palindrome(s).size() == best);
    }
}

TEST_CASE("anagrams") {
    using G = std::vector<std::vector<std::string>>;
    CHECK(group_anagrams({"eat", "tea", "tan", "ate", "nat", "bat"}) == G{{"eat", "tea", "ate"}, {"tan", "nat"}, {"bat"}});
    CHECK(group_anagrams({}).empty());
    CHECK(group_anagrams({""}) == G{{""}});
    CHECK(is_anagram("listen", "silent"));
    CHECK_FALSE(is_anagram("aab", "abb"));
    CHECK_FALSE(is_anagram("a", "ab"));
    CHECK(is_anagram("", ""));
}

TEST_CASE("two_sum returns a valid pair or nothing, matching brute force") {
    CHECK(two_sum({2, 7, 11, 15}, 9).value() == std::make_pair<std::size_t, std::size_t>(0, 1));
    CHECK(two_sum({3, 3}, 6).has_value());
    CHECK_FALSE(two_sum({3}, 6).has_value());
    CHECK_FALSE(two_sum({}, 0).has_value());
    Rng rng(161);
    for (int t = 0; t < 300; ++t) {
        V a = rng.vec(15, -10, 10);
        int target = rng.in(-15, 15);
        bool exists = false;
        for (std::size_t i = 0; i < a.size(); ++i)
            for (std::size_t j = i + 1; j < a.size(); ++j) exists = exists || a[i] + a[j] == target;
        auto r = two_sum(a, target);
        REQUIRE(r.has_value() == exists);
        if (r) {
            REQUIRE(r->first < r->second);
            REQUIRE(a[r->first] + a[r->second] == target);
        }
    }
}

TEST_CASE("valid_parentheses") {
    CHECK(valid_parentheses("()[]{}"));
    CHECK(valid_parentheses("{[()]}"));
    CHECK_FALSE(valid_parentheses("(]"));
    CHECK_FALSE(valid_parentheses("("));
    CHECK_FALSE(valid_parentheses(")"));
    CHECK(valid_parentheses(""));
    CHECK(valid_parentheses("a(b)c"));
    CHECK(valid_parentheses(std::string(100000, '(') + std::string(100000, ')')));
}

TEST_CASE("merge_intervals") {
    using I = std::vector<std::pair<int, int>>;
    CHECK(merge_intervals({{1, 3}, {2, 6}, {8, 10}, {15, 18}}) == I{{1, 6}, {8, 10}, {15, 18}});
    CHECK(merge_intervals({{1, 4}, {4, 5}}) == I{{1, 5}});
    CHECK(merge_intervals({{1, 10}, {2, 3}}) == I{{1, 10}});
    CHECK(merge_intervals({}).empty());
    CHECK(merge_intervals({{5, 5}}) == I{{5, 5}});
    CHECK_THROWS_AS(merge_intervals({{3, 1}}), std::invalid_argument);
}

TEST_CASE("merge_intervals covers exactly the same integer points as the input") {
    Rng rng(162);
    for (int t = 0; t < 200; ++t) {
        std::vector<std::pair<int, int>> iv;
        for (int i = 0, n = rng.in(0, 8); i < n; ++i) {
            int s = rng.in(0, 20);
            iv.emplace_back(s, s + rng.in(0, 5));
        }
        auto merged = merge_intervals(iv);
        for (std::size_t i = 1; i < merged.size(); ++i) REQUIRE(merged[i].first > merged[i - 1].second);
        for (int x = 0; x <= 26; ++x) {
            auto covers = [&](const std::vector<std::pair<int, int>>& v) {
                return std::any_of(v.begin(), v.end(), [&](const auto& p) { return x >= p.first && x <= p.second; });
            };
            REQUIRE(covers(merged) == covers(iv));
        }
    }
}

TEST_CASE("kth_largest known cases, errors and sort comparison") {
    CHECK(kth_largest({3, 2, 1, 5, 6, 4}, 2) == 5);
    CHECK(kth_largest({3, 2, 3, 1, 2, 4, 5, 5, 6}, 4) == 4);
    CHECK(kth_largest({7}, 1) == 7);
    CHECK_THROWS_AS(kth_largest({}, 1), std::invalid_argument);
    CHECK_THROWS_AS(kth_largest({1}, 2), std::invalid_argument);
    CHECK_THROWS_AS(kth_largest({1}, 0), std::invalid_argument);
    Rng rng(163);
    for (int t = 0; t < 300; ++t) {
        V a = rng.vec(40, -10, 10);
        a.push_back(0);
        std::size_t k = rng.idx(1, a.size());
        V sorted = a;
        std::sort(sorted.begin(), sorted.end(), std::greater<int>());
        REQUIRE(kth_largest(a, k) == sorted[k - 1]);
    }
}

TEST_CASE("max_subarray known cases and brute force") {
    CHECK(max_subarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    CHECK(max_subarray({-3, -1, -2}) == -1);
    CHECK(max_subarray({5}) == 5);
    CHECK_THROWS_AS(max_subarray({}), std::invalid_argument);
    Rng rng(164);
    for (int t = 0; t < 200; ++t) {
        V a = rng.vec(15, -8, 8);
        a.push_back(0);
        long long best = -(1LL << 60);
        for (std::size_t i = 0; i < a.size(); ++i) {
            long long s = 0;
            for (std::size_t j = i; j < a.size(); ++j) best = std::max(best, s += a[j]);
        }
        REQUIRE(max_subarray(a) == best);
    }
}

static std::unique_ptr<ListNode> build(const V& xs) {
    std::unique_ptr<ListNode> head;
    for (std::size_t i = xs.size(); i-- > 0;) {
        auto n = std::make_unique<ListNode>(xs[i]);
        n->next = std::move(head);
        head = std::move(n);
    }
    return head;
}

static V to_vec(const std::unique_ptr<ListNode>& head) {
    V out;
    for (const ListNode* n = head.get(); n; n = n->next.get()) out.push_back(n->value);
    return out;
}

TEST_CASE("reverse_list") {
    CHECK(to_vec(reverse_list(build({1, 2, 3}))) == V{3, 2, 1});
    CHECK(reverse_list(nullptr) == nullptr);
    CHECK(to_vec(reverse_list(build({1}))) == V{1});
}
