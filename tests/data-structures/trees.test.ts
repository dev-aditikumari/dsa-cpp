import { BinarySearchTree } from "../../src/data-structures/bst";
import { AVLTree } from "../../src/data-structures/avl-tree";
import { makeRng, randInt } from "../helpers/rng";

describe("BinarySearchTree", () => {
  const build = () => {
    const t = new BinarySearchTree<number>();
    [8, 3, 10, 1, 6, 14, 4, 7, 13].forEach((x) => t.insert(x));
    return t;
  };

  it("traversals match known results", () => {
    const t = build();
    expect(t.inOrder()).toEqual([1, 3, 4, 6, 7, 8, 10, 13, 14]);
    expect(t.preOrder()).toEqual([8, 3, 1, 6, 4, 7, 10, 14, 13]);
    expect(t.postOrder()).toEqual([1, 4, 7, 6, 3, 13, 14, 10, 8]);
    expect(t.levelOrder()).toEqual([8, 3, 10, 1, 6, 14, 4, 7, 13]);
    expect(t.height()).toBe(4);
    expect(t.min()).toBe(1);
    expect(t.max()).toBe(14);
  });

  it("ignores duplicates", () => {
    const t = new BinarySearchTree<number>();
    expect(t.insert(1)).toBe(true);
    expect(t.insert(1)).toBe(false);
    expect(t.size).toBe(1);
  });

  it("deletes leaf, one child, two children and root", () => {
    const t = build();
    expect(t.delete(1)).toBe(true);
    expect(t.delete(14)).toBe(true);
    expect(t.delete(3)).toBe(true);
    expect(t.delete(8)).toBe(true);
    expect(t.delete(99)).toBe(false);
    expect(t.inOrder()).toEqual([4, 6, 7, 10, 13]);
    expect(t.isValid()).toBe(true);
    expect(t.contains(8)).toBe(false);
    expect(t.contains(7)).toBe(true);
  });

  it("handles empty tree and single element", () => {
    const t = new BinarySearchTree<string>();
    expect(t.height()).toBe(0);
    expect(t.inOrder()).toEqual([]);
    expect(t.postOrder()).toEqual([]);
    expect(t.delete("x")).toBe(false);
    expect(() => t.min()).toThrow("empty");
    expect(() => t.max()).toThrow("empty");
    t.insert("x");
    expect(t.delete("x")).toBe(true);
    expect(t.size).toBe(0);
  });

  it("copes with 20000 sorted inserts (degenerate shape)", () => {
    const t = new BinarySearchTree<number>();
    for (let i = 0; i < 20000; i++) t.insert(i);
    expect(t.height()).toBe(20000);
    expect(t.inOrder().length).toBe(20000);
    expect(t.postOrder()[0]).toBe(19999);
  });

  it("keeps invariants after random inserts and deletes", () => {
    const rng = makeRng(51);
    const t = new BinarySearchTree<number>();
    const ref = new Set<number>();
    for (let i = 0; i < 4000; i++) {
      const v = randInt(rng, 0, 400);
      if (randInt(rng, 0, 2) < 2) {
        expect(t.insert(v)).toBe(!ref.has(v));
        ref.add(v);
      } else expect(t.delete(v)).toBe(ref.delete(v));
      if (i % 100 === 0) {
        expect(t.isValid()).toBe(true);
        expect(t.inOrder()).toEqual([...ref].sort((a, b) => a - b));
      }
    }
    expect(t.size).toBe(ref.size);
  });
});

describe("AVLTree", () => {
  it("stays balanced on sorted inserts", () => {
    const t = new AVLTree<number>();
    for (let i = 0; i < 1023; i++) t.insert(i);
    expect(t.height()).toBe(10);
    expect(t.isValid()).toBe(true);
    expect(t.inOrder()[500]).toBe(500);
  });

  it("covers all four rotation cases", () => {
    for (const seq of [[3, 2, 1], [1, 2, 3], [3, 1, 2], [1, 3, 2]]) {
      const t = new AVLTree<number>();
      seq.forEach((x) => t.insert(x));
      expect(t.height()).toBe(2);
      expect(t.inOrder()).toEqual([1, 2, 3]);
      expect(t.isValid()).toBe(true);
    }
  });

  it("insert and delete report whether the tree changed", () => {
    const t = new AVLTree<number>();
    expect(t.insert(5)).toBe(true);
    expect(t.insert(5)).toBe(false);
    expect(t.contains(5)).toBe(true);
    expect(t.delete(6)).toBe(false);
    expect(t.delete(5)).toBe(true);
    expect(t.size).toBe(0);
    expect(t.height()).toBe(0);
  });

  it("works with string keys and a custom comparator", () => {
    const t = new AVLTree<string>((a, b) => b.localeCompare(a));
    ["b", "a", "c"].forEach((x) => t.insert(x));
    expect(t.inOrder()).toEqual(["c", "b", "a"]);
  });

  it("keeps AVL invariants after random inserts and deletes", () => {
    const rng = makeRng(52);
    const t = new AVLTree<number>();
    const ref = new Set<number>();
    for (let i = 0; i < 6000; i++) {
      const v = randInt(rng, 0, 500);
      if (randInt(rng, 0, 2) < 2) {
        t.insert(v);
        ref.add(v);
      } else {
        expect(t.delete(v)).toBe(ref.delete(v));
      }
      expect(t.size).toBe(ref.size);
      if (i % 25 === 0) expect(t.isValid()).toBe(true);
    }
    expect(t.isValid()).toBe(true);
    expect(t.inOrder()).toEqual([...ref].sort((a, b) => a - b));
    expect(t.height()).toBeLessThanOrEqual(1.45 * Math.log2(ref.size + 2));
  });
});
