class TrieNode {
  children = new Map<string, TrieNode>();
  terminal = false;
}

// Trie over strings. insert/has/startsWith O(L) for word length L; space O(total characters).
export class Trie {
  private root = new TrieNode();
  private count = 0;

  get size(): number {
    return this.count;
  }

  insert(word: string): void {
    let node = this.root;
    for (const ch of word) {
      let next = node.children.get(ch);
      if (!next) {
        next = new TrieNode();
        node.children.set(ch, next);
      }
      node = next;
    }
    if (!node.terminal) {
      node.terminal = true;
      this.count++;
    }
  }

  has(word: string): boolean {
    return this.find(word)?.terminal === true;
  }

  startsWith(prefix: string): boolean {
    return this.find(prefix) !== null;
  }

  // Returns all words with the given prefix in lexicographic order.
  withPrefix(prefix: string): string[] {
    const start = this.find(prefix);
    const out: string[] = [];
    if (!start) return out;
    const walk = (node: TrieNode, acc: string): void => {
      if (node.terminal) out.push(acc);
      for (const ch of [...node.children.keys()].sort()) {
        walk(node.children.get(ch) as TrieNode, acc + ch);
      }
    };
    walk(start, prefix);
    return out;
  }

  delete(word: string): boolean {
    const path: [TrieNode, string][] = [];
    let node = this.root;
    for (const ch of word) {
      const next = node.children.get(ch);
      if (!next) return false;
      path.push([node, ch]);
      node = next;
    }
    if (!node.terminal) return false;
    node.terminal = false;
    this.count--;
    for (let i = path.length - 1; i >= 0; i--) {
      const [parent, ch] = path[i] as [TrieNode, string];
      const child = parent.children.get(ch) as TrieNode;
      if (child.terminal || child.children.size > 0) break;
      parent.children.delete(ch);
    }
    return true;
  }

  private find(s: string): TrieNode | null {
    let node = this.root;
    for (const ch of s) {
      const next = node.children.get(ch);
      if (!next) return null;
      node = next;
    }
    return node;
  }
}
