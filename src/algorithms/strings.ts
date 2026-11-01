// O(n + m) time, O(m) space. Indices of every occurrence of pattern in text (KMP). Empty pattern matches nowhere.
export function kmpSearch(text: string, pattern: string): number[] {
  const out: number[] = [];
  if (pattern.length === 0) return out;
  const fail = new Array<number>(pattern.length).fill(0);
  for (let i = 1, k = 0; i < pattern.length; i++) {
    while (k > 0 && pattern[i] !== pattern[k]) k = fail[k - 1] as number;
    if (pattern[i] === pattern[k]) k++;
    fail[i] = k;
  }
  for (let i = 0, k = 0; i < text.length; i++) {
    while (k > 0 && text[i] !== pattern[k]) k = fail[k - 1] as number;
    if (text[i] === pattern[k]) k++;
    if (k === pattern.length) {
      out.push(i - k + 1);
      k = fail[k - 1] as number;
    }
  }
  return out;
}

// O(n) time, O(1) space. Palindrome check over letters and digits only, ignoring case.
export function isPalindrome(s: string): boolean {
  const alnum = /[\p{L}\p{N}]/u;
  let i = 0;
  let j = s.length - 1;
  while (i < j) {
    while (i < j && !alnum.test(s[i] as string)) i++;
    while (i < j && !alnum.test(s[j] as string)) j--;
    if ((s[i] as string).toLowerCase() !== (s[j] as string).toLowerCase()) return false;
    i++;
    j--;
  }
  return true;
}

// O(n^2) time, O(1) space. Longest palindromic substring by expanding around centers (leftmost on ties).
export function longestPalindrome(s: string): string {
  let start = 0;
  let len = 0;
  const expand = (l: number, r: number): void => {
    while (l >= 0 && r < s.length && s[l] === s[r]) {
      l--;
      r++;
    }
    if (r - l - 1 > len) {
      len = r - l - 1;
      start = l + 1;
    }
  };
  for (let i = 0; i < s.length; i++) {
    expand(i, i);
    expand(i, i + 1);
  }
  return s.slice(start, start + len);
}

// O(total characters * log k) time, O(total characters) space. Groups words that are anagrams of each other.
export function groupAnagrams(words: readonly string[]): string[][] {
  const groups = new Map<string, string[]>();
  for (const w of words) {
    const key = [...w].sort().join("");
    const g = groups.get(key);
    if (g) g.push(w);
    else groups.set(key, [w]);
  }
  return [...groups.values()];
}

// O(n + m) time, O(1) space for a fixed alphabet. Whether two strings are anagrams.
export function isAnagram(a: string, b: string): boolean {
  if (a.length !== b.length) return false;
  const counts = new Map<string, number>();
  for (const ch of a) counts.set(ch, (counts.get(ch) ?? 0) + 1);
  for (const ch of b) {
    const c = counts.get(ch);
    if (!c) return false;
    counts.set(ch, c - 1);
  }
  return true;
}
