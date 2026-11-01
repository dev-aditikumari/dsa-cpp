import { Comparator, defaultCompare } from "../utils/compare";

interface ANode<T> {
  value: T;
  height: number;
  left: ANode<T> | null;
  right: ANode<T> | null;
}

const h = <T>(n: ANode<T> | null): number => (n ? n.height : 0);
const update = <T>(n: ANode<T>): void => {
  n.height = 1 + Math.max(h(n.left), h(n.right));
};

function rotateRight<T>(y: ANode<T>): ANode<T> {
  const x = y.left as ANode<T>;
  y.left = x.right;
  x.right = y;
  update(y);
  update(x);
  return x;
}

function rotateLeft<T>(x: ANode<T>): ANode<T> {
  const y = x.right as ANode<T>;
  x.right = y.left;
  y.left = x;
  update(x);
  update(y);
  return y;
}

function rebalance<T>(n: ANode<T>): ANode<T> {
  update(n);
  const balance = h(n.left) - h(n.right);
  if (balance > 1) {
    const l = n.left as ANode<T>;
    if (h(l.left) < h(l.right)) n.left = rotateLeft(l);
    return rotateRight(n);
  }
  if (balance < -1) {
    const r = n.right as ANode<T>;
    if (h(r.right) < h(r.left)) n.right = rotateRight(r);
    return rotateLeft(n);
  }
  return n;
}

// AVL tree (self-balancing BST, set semantics). insert/contains/delete O(log n); space O(n).
export class AVLTree<T> {
  private root: ANode<T> | null = null;
  private count = 0;

  constructor(private readonly compare: Comparator<T> = defaultCompare) {}

  get size(): number {
    return this.count;
  }

  height(): number {
    return h(this.root);
  }

  insert(value: T): boolean {
    const before = this.count;
    this.root = this.insertNode(this.root, value);
    return this.count > before;
  }

  delete(value: T): boolean {
    const before = this.count;
    this.root = this.deleteNode(this.root, value);
    return this.count < before;
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

  inOrder(): T[] {
    const out: T[] = [];
    const walk = (n: ANode<T> | null): void => {
      if (!n) return;
      walk(n.left);
      out.push(n.value);
      walk(n.right);
    };
    walk(this.root);
    return out;
  }

  // Verifies ordering, stored heights, and |balance| <= 1 on every node.
  isValid(): boolean {
    const check = (n: ANode<T> | null): number => {
      if (!n) return 0;
      const lh = check(n.left);
      const rh = check(n.right);
      if (lh < 0 || rh < 0) return -1;
      if (Math.abs(lh - rh) > 1) return -1;
      if (n.height !== 1 + Math.max(lh, rh)) return -1;
      return n.height;
    };
    if (check(this.root) < 0) return false;
    const vals = this.inOrder();
    for (let i = 1; i < vals.length; i++) {
      if (this.compare(vals[i - 1] as T, vals[i] as T) >= 0) return false;
    }
    return vals.length === this.count;
  }

  private insertNode(n: ANode<T> | null, value: T): ANode<T> {
    if (!n) {
      this.count++;
      return { value, height: 1, left: null, right: null };
    }
    const c = this.compare(value, n.value);
    if (c === 0) return n;
    if (c < 0) n.left = this.insertNode(n.left, value);
    else n.right = this.insertNode(n.right, value);
    return rebalance(n);
  }

  private deleteNode(n: ANode<T> | null, value: T): ANode<T> | null {
    if (!n) return null;
    const c = this.compare(value, n.value);
    if (c < 0) n.left = this.deleteNode(n.left, value);
    else if (c > 0) n.right = this.deleteNode(n.right, value);
    else {
      if (!n.left || !n.right) {
        this.count--;
        return n.left ?? n.right;
      }
      let succ = n.right;
      while (succ.left) succ = succ.left;
      n.value = succ.value;
      // Removing the successor value decrements the count once.
      n.right = this.deleteNode(n.right, succ.value);
    }
    return rebalance(n);
  }
}
