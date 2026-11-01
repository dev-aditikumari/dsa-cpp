import { Comparator, defaultCompare } from "../utils/compare";

interface BNode<T> {
  value: T;
  left: BNode<T> | null;
  right: BNode<T> | null;
}

// Binary search tree (set semantics, duplicates ignored). insert/contains/delete O(h), O(n) when degenerate;
// traversals O(n); space O(n). Traversals are iterative so deep trees do not overflow the call stack.
export class BinarySearchTree<T> {
  private root: BNode<T> | null = null;
  private count = 0;

  constructor(private readonly compare: Comparator<T> = defaultCompare) {}

  get size(): number {
    return this.count;
  }

  insert(value: T): boolean {
    const node: BNode<T> = { value, left: null, right: null };
    if (!this.root) {
      this.root = node;
      this.count++;
      return true;
    }
    let cur = this.root;
    for (;;) {
      const c = this.compare(value, cur.value);
      if (c === 0) return false;
      const side = c < 0 ? "left" : "right";
      const next = cur[side];
      if (!next) {
        cur[side] = node;
        this.count++;
        return true;
      }
      cur = next;
    }
  }

  contains(value: T): boolean {
    let cur = this.root;
    while (cur) {
      const c = this.compare(value, cur.value);
      if (c === 0) return true;
      cur = c < 0 ? cur.left : cur.right;
    }
    return false;
  }

  min(): T {
    if (!this.root) throw new Error("Tree is empty");
    let cur = this.root;
    while (cur.left) cur = cur.left;
    return cur.value;
  }

  max(): T {
    if (!this.root) throw new Error("Tree is empty");
    let cur = this.root;
    while (cur.right) cur = cur.right;
    return cur.value;
  }

  delete(value: T): boolean {
    let parent: BNode<T> | null = null;
    let cur = this.root;
    let side: "left" | "right" = "left";
    while (cur) {
      const c = this.compare(value, cur.value);
      if (c === 0) break;
      parent = cur;
      side = c < 0 ? "left" : "right";
      cur = cur[side];
    }
    if (!cur) return false;

    let replacement: BNode<T> | null;
    if (!cur.left || !cur.right) {
      replacement = cur.left ?? cur.right;
    } else {
      // Two children: splice out the in-order successor and put its value here.
      let succParent = cur;
      let succ = cur.right;
      while (succ.left) {
        succParent = succ;
        succ = succ.left;
      }
      cur.value = succ.value;
      if (succParent === cur) succParent.right = succ.right;
      else succParent.left = succ.right;
      this.count--;
      return true;
    }
    if (!parent) this.root = replacement;
    else parent[side] = replacement;
    this.count--;
    return true;
  }

  inOrder(): T[] {
    const out: T[] = [];
    const stack: BNode<T>[] = [];
    let cur = this.root;
    while (cur || stack.length) {
      while (cur) {
        stack.push(cur);
        cur = cur.left;
      }
      const node = stack.pop() as BNode<T>;
      out.push(node.value);
      cur = node.right;
    }
    return out;
  }

  preOrder(): T[] {
    const out: T[] = [];
    const stack: BNode<T>[] = this.root ? [this.root] : [];
    while (stack.length) {
      const node = stack.pop() as BNode<T>;
      out.push(node.value);
      if (node.right) stack.push(node.right);
      if (node.left) stack.push(node.left);
    }
    return out;
  }

  postOrder(): T[] {
    const out: T[] = [];
    const stack: BNode<T>[] = this.root ? [this.root] : [];
    while (stack.length) {
      const node = stack.pop() as BNode<T>;
      out.push(node.value);
      if (node.left) stack.push(node.left);
      if (node.right) stack.push(node.right);
    }
    return out.reverse();
  }

  levelOrder(): T[] {
    const out: T[] = [];
    const queue: BNode<T>[] = this.root ? [this.root] : [];
    for (let i = 0; i < queue.length; i++) {
      const node = queue[i] as BNode<T>;
      out.push(node.value);
      if (node.left) queue.push(node.left);
      if (node.right) queue.push(node.right);
    }
    return out;
  }

  // Number of nodes on the longest root-to-leaf path; 0 for an empty tree.
  height(): number {
    let h = 0;
    let level: BNode<T>[] = this.root ? [this.root] : [];
    while (level.length) {
      h++;
      const next: BNode<T>[] = [];
      for (const n of level) {
        if (n.left) next.push(n.left);
        if (n.right) next.push(n.right);
      }
      level = next;
    }
    return h;
  }

  // Checks the ordering invariant: strictly increasing in-order sequence.
  isValid(): boolean {
    const values = this.inOrder();
    for (let i = 1; i < values.length; i++) {
      if (this.compare(values[i - 1] as T, values[i] as T) >= 0) return false;
    }
    return values.length === this.count;
  }
}
