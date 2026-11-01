import * as S from "../../src/algorithms/strings";
import * as I from "../../src/algorithms/interview";
import { makeRng, randArray, randInt } from "../helpers/rng";

describe("kmpSearch", () => {
  it("finds all occurrences including overlaps", () => {
    expect(S.kmpSearch("aaaaa", "aa")).toEqual([0, 1, 2, 3]);
    expect(S.kmpSearch("abxabcabcaby", "abcaby")).toEqual([6]);
    expect(S.kmpSearch("abc", "d")).toEqual([]);
    expect(S.kmpSearch("", "a")).toEqual([]);
    expect(S.kmpSearch("abc", "")).toEqual([]);
    expect(S.kmpSearch("ab", "abc")).toEqual([]);
  });

  it("matches indexOf-based search on random strings", () => {
    const rng = makeRng(151);
    const rs = (n: number) => Array.from({ length: n }, () => "ab"[randInt(rng, 0, 1)]).join("");
    for (let t = 0; t < 300; t++) {
      const text = rs(randInt(rng, 0, 30));
      const pat = rs(randInt(rng, 1, 4));
      const expected: number[] = [];
      for (let i = 0; i + pat.length <= text.length; i++) if (text.startsWith(pat, i)) expected.push(i);
      expect(S.kmpSearch(text, pat)).toEqual(expected);
    }
  });
});

describe("palindromes", () => {
  it("isPalindrome ignores case and punctuation", () => {
    expect(S.isPalindrome("A man, a plan, a canal: Panama")).toBe(true);
    expect(S.isPalindrome("race a car")).toBe(false);
    expect(S.isPalindrome("")).toBe(true);
    expect(S.isPalindrome(".,")).toBe(true);
    expect(S.isPalindrome("a")).toBe(true);
  });

  it("longestPalindrome", () => {
    expect(S.longestPalindrome("babad")).toBe("bab");
    expect(S.longestPalindrome("cbbd")).toBe("bb");
    expect(S.longestPalindrome("")).toBe("");
    expect(S.longestPalindrome("x")).toBe("x");
  });

  it("longestPalindrome length matches brute force", () => {
    const rng = makeRng(152);
    for (let t = 0; t < 200; t++) {
      const s = Array.from({ length: randInt(rng, 0, 14) }, () => "abc"[randInt(rng, 0, 2)]).join("");
      let best = 0;
      for (let i = 0; i < s.length; i++)
        for (let j = i + 1; j <= s.length; j++) {
          const sub = s.slice(i, j);
          if (sub === [...sub].reverse().join("")) best = Math.max(best, sub.length);
        }
      expect(S.longestPalindrome(s)).toHaveLength(best);
    }
  });
});

describe("anagrams", () => {
  it("groupAnagrams", () => {
    const g = S.groupAnagrams(["eat", "tea", "tan", "ate", "nat", "bat"]);
    expect(g).toEqual([["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]);
    expect(S.groupAnagrams([])).toEqual([]);
    expect(S.groupAnagrams([""])).toEqual([[""]]);
  });

  it("isAnagram", () => {
    expect(S.isAnagram("listen", "silent")).toBe(true);
    expect(S.isAnagram("aab", "abb")).toBe(false);
    expect(S.isAnagram("a", "ab")).toBe(false);
    expect(S.isAnagram("", "")).toBe(true);
  });
});

describe("interview problems", () => {
  it("twoSum finds a valid pair or null", () => {
    expect(I.twoSum([2, 7, 11, 15], 9)).toEqual([0, 1]);
    expect(I.twoSum([3, 3], 6)).toEqual([0, 1]);
    expect(I.twoSum([3], 6)).toBeNull();
    expect(I.twoSum([], 0)).toBeNull();
    const rng = makeRng(161);
    for (let t = 0; t < 200; t++) {
      const a = randArray(rng, 15, -10, 10);
      const target = randInt(rng, -15, 15);
      const r = I.twoSum(a, target);
      let exists = false;
      for (let i = 0; i < a.length; i++) for (let j = i + 1; j < a.length; j++) if ((a[i] as number) + (a[j] as number) === target) exists = true;
      if (r) {
        expect(r[0]).not.toBe(r[1]);
        expect((a[r[0]] as number) + (a[r[1]] as number)).toBe(target);
      } else expect(exists).toBe(false);
      expect(r !== null).toBe(exists);
    }
  });

  it("validParentheses", () => {
    expect(I.validParentheses("()[]{}")).toBe(true);
    expect(I.validParentheses("{[()]}")).toBe(true);
    expect(I.validParentheses("(]")).toBe(false);
    expect(I.validParentheses("(")).toBe(false);
    expect(I.validParentheses(")")).toBe(false);
    expect(I.validParentheses("")).toBe(true);
    expect(I.validParentheses("a(b)c")).toBe(true);
    expect(I.validParentheses("(".repeat(50000) + ")".repeat(50000))).toBe(true);
  });

  it("mergeIntervals", () => {
    expect(I.mergeIntervals([[1, 3], [2, 6], [8, 10], [15, 18]])).toEqual([[1, 6], [8, 10], [15, 18]]);
    expect(I.mergeIntervals([[1, 4], [4, 5]])).toEqual([[1, 5]]);
    expect(I.mergeIntervals([[1, 10], [2, 3]])).toEqual([[1, 10]]);
    expect(I.mergeIntervals([])).toEqual([]);
    expect(I.mergeIntervals([[5, 5]])).toEqual([[5, 5]]);
    expect(() => I.mergeIntervals([[3, 1]])).toThrow(RangeError);
  });

  it("mergeIntervals does not mutate input and covers the same integer points", () => {
    const input: [number, number][] = [[5, 7], [1, 2]];
    I.mergeIntervals(input);
    expect(input).toEqual([[5, 7], [1, 2]]);
    const rng = makeRng(162);
    for (let t = 0; t < 150; t++) {
      const iv: [number, number][] = Array.from({ length: randInt(rng, 0, 8) }, () => {
        const s = randInt(rng, 0, 20);
        return [s, s + randInt(rng, 0, 5)];
      });
      const merged = I.mergeIntervals(iv);
      for (let i = 1; i < merged.length; i++) expect((merged[i] as [number, number])[0]).toBeGreaterThan((merged[i - 1] as [number, number])[1]);
      for (let x = 0; x <= 26; x++) {
        const inInput = iv.some(([s, e]) => x >= s && x <= e);
        const inMerged = merged.some(([s, e]) => x >= s && x <= e);
        expect(inMerged).toBe(inInput);
      }
    }
  });

  it("kthLargest", () => {
    expect(I.kthLargest([3, 2, 1, 5, 6, 4], 2)).toBe(5);
    expect(I.kthLargest([3, 2, 3, 1, 2, 4, 5, 5, 6], 4)).toBe(4);
    expect(I.kthLargest([7], 1)).toBe(7);
    expect(() => I.kthLargest([], 1)).toThrow(RangeError);
    expect(() => I.kthLargest([1], 2)).toThrow(RangeError);
    expect(() => I.kthLargest([1], 0)).toThrow(RangeError);
  });

  it("kthLargest matches sort on random arrays", () => {
    const rng = makeRng(163);
    for (let t = 0; t < 200; t++) {
      const a = randArray(rng, 40, -10, 10).concat([0]);
      const k = randInt(rng, 1, a.length);
      expect(I.kthLargest(a, k)).toBe([...a].sort((x, y) => y - x)[k - 1]);
    }
  });

  it("maxSubarray matches brute force", () => {
    expect(I.maxSubarray([-2, 1, -3, 4, -1, 2, 1, -5, 4])).toBe(6);
    expect(I.maxSubarray([-3, -1, -2])).toBe(-1);
    expect(I.maxSubarray([5])).toBe(5);
    expect(() => I.maxSubarray([])).toThrow("empty");
    const rng = makeRng(164);
    for (let t = 0; t < 150; t++) {
      const a = randArray(rng, 15, -8, 8).concat([0]);
      let best = -Infinity;
      for (let i = 0; i < a.length; i++) {
        let s = 0;
        for (let j = i; j < a.length; j++) best = Math.max(best, (s += a[j] as number));
      }
      expect(I.maxSubarray(a)).toBe(best);
    }
  });

  it("reverseList", () => {
    const build = (xs: number[]): I.ListNode<number> | null => xs.reduceRight<I.ListNode<number> | null>((next, value) => ({ value, next }), null);
    const toArr = (n: I.ListNode<number> | null) => {
      const out: number[] = [];
      for (; n; n = n.next) out.push(n.value);
      return out;
    };
    expect(toArr(I.reverseList(build([1, 2, 3])))).toEqual([3, 2, 1]);
    expect(I.reverseList(null)).toBeNull();
    expect(toArr(I.reverseList(build([1])))).toEqual([1]);
  });
});
