import { Trie } from "../../src/data-structures/trie";

describe("Trie", () => {
  it("inserts, finds and prefix-checks", () => {
    const t = new Trie();
    ["apple", "app", "apt", "bat"].forEach((w) => t.insert(w));
    expect(t.size).toBe(4);
    expect(t.has("app")).toBe(true);
    expect(t.has("ap")).toBe(false);
    expect(t.startsWith("ap")).toBe(true);
    expect(t.startsWith("c")).toBe(false);
    expect(t.withPrefix("ap")).toEqual(["app", "apple", "apt"]);
    expect(t.withPrefix("zz")).toEqual([]);
  });

  it("handles duplicates and the empty string", () => {
    const t = new Trie();
    t.insert("a");
    t.insert("a");
    expect(t.size).toBe(1);
    expect(t.has("")).toBe(false);
    t.insert("");
    expect(t.has("")).toBe(true);
    expect(t.withPrefix("")).toEqual(["", "a"]);
  });

  it("deletes and prunes without breaking other words", () => {
    const t = new Trie();
    ["app", "apple"].forEach((w) => t.insert(w));
    expect(t.delete("apple")).toBe(true);
    expect(t.has("app")).toBe(true);
    expect(t.startsWith("appl")).toBe(false);
    expect(t.delete("app")).toBe(true);
    expect(t.startsWith("a")).toBe(false);
    expect(t.delete("app")).toBe(false);
    expect(t.delete("zebra")).toBe(false);
    t.insert("abc");
    expect(t.delete("ab")).toBe(false);
    expect(t.size).toBe(1);
  });

  it("handles unicode", () => {
    const t = new Trie();
    t.insert("naïve");
    t.insert("日本語");
    expect(t.has("日本語")).toBe(true);
    expect(t.startsWith("日本")).toBe(true);
  });
});
