# DSA in TypeScript

A collection of data structures and algorithms written in strict TypeScript, with Jest tests and a
complexity note on every implementation. The repo was originally a one-line `index.js`; that file is
kept in `legacy/`.

Stack: TypeScript 5 (strict, `noUncheckedIndexedAccess`), Jest 29 with ts-jest, ESLint 9 with typescript-eslint,
GitHub Actions.

## Install and run

Requires Node 20 or newer.

```
npm ci
npm test            # jest with coverage and a coverage threshold
npm run lint        # eslint
npm run typecheck   # tsc --noEmit over src and tests
npm run build       # compiles src to dist/
```

Everything is exported from `src/index.ts`, for example `import { LRUCache, dijkstra } from "./src"`.

## What is covered

n is the input size, V and E are vertices and edges, h is tree height.

### Data structures (`src/data-structures/`)

| Structure | File | Time | Space | Tests |
|---|---|---|---|---|
| Singly linked list | `linked-list.ts` | push/unshift O(1), pop/get/insert/remove O(n) | O(n) | `linked-list.test.ts` |
| Doubly linked list | `doubly-linked-list.ts` | both ends O(1), indexed O(n) | O(n) | `linked-list.test.ts` |
| Stack | `stack.ts` | O(1) amortised | O(n) | `stack-queue-deque.test.ts` |
| Queue | `queue.ts` | O(1) amortised | O(n) | `stack-queue-deque.test.ts` |
| Deque (circular buffer) | `deque.ts` | O(1) amortised | O(n) | `stack-queue-deque.test.ts` |
| Binary heap (min/max) | `heap.ts` | push/pop O(log n), peek O(1), build O(n) | O(n) | `heap.test.ts` |
| Priority queue | `priority-queue.ts` | O(log n) | O(n) | `heap.test.ts` |
| Hash map (chaining, resizing) | `hash-map.ts` | O(1) average, O(n) worst | O(n) | `hash-map.test.ts` |
| Binary search tree | `bst.ts` | O(h), O(n) when degenerate | O(n) | `trees.test.ts` |
| AVL tree | `avl-tree.ts` | O(log n) | O(n) | `trees.test.ts` |
| Trie | `trie.ts` | O(L) per word | O(total chars) | `trie.test.ts` |
| Graph (adjacency list) | `graph.ts` | add O(1), removeVertex O(V + E) | O(V + E) | `graph.test.ts` |
| Union-find | `union-find.ts` | O(alpha(n)) amortised | O(n) | `graph.test.ts` |
| LRU cache | `lru-cache.ts` | get/put O(1) | O(capacity) | `lru-cache.test.ts` |
| Fenwick tree | `fenwick-tree.ts` | O(log n) | O(n) | `range-trees.test.ts` |
| Segment tree | `segment-tree.ts` | query/update O(log n) | O(n) | `range-trees.test.ts` |

### Algorithms (`src/algorithms/`)

| Algorithm | File | Time | Space | Tests |
|---|---|---|---|---|
| Bubble, insertion, selection sort | `sorting.ts` | O(n^2) | O(1) extra | `sorting.test.ts` |
| Merge sort (stable) | `sorting.ts` | O(n log n) | O(n) | `sorting.test.ts` |
| Quick sort (3-way, median of three) | `sorting.ts` | O(n log n) expected, O(n^2) worst | O(log n) | `sorting.test.ts` |
| Heap sort | `sorting.ts` | O(n log n) | O(n) | `sorting.test.ts` |
| Counting sort (integers) | `sorting.ts` | O(n + k) | O(n + k) | `sorting.test.ts` |
| Binary search, lower/upper bound, count | `searching.ts` | O(log n) | O(1) | `searching.test.ts` |
| Rotated array search and rotation point | `searching.ts` | O(log n), O(n) with duplicates | O(1) | `searching.test.ts` |
| Search on answer, ship within days, integer sqrt | `searching.ts` | O(log range) probes | O(1) | `searching.test.ts` |
| BFS, DFS, unweighted shortest path, components | `graph-algorithms.ts` | O(V + E) | O(V) | `graph-algorithms.test.ts` |
| Topological sort (Kahn) | `graph-algorithms.ts` | O(V + E) | O(V) | `graph-algorithms.test.ts` |
| Dijkstra | `graph-algorithms.ts` | O((V + E) log V) | O(V) | `graph-algorithms.test.ts` |
| Bellman-Ford | `graph-algorithms.ts` | O(V * E) | O(V) | `graph-algorithms.test.ts` |
| Kruskal, Prim | `graph-algorithms.ts` | O(E log E), O(E log V) | O(V + E) | `graph-algorithms.test.ts` |
| Cycle detection (directed and undirected) | `graph-algorithms.ts` | O(V + E) | O(V) | `graph-algorithms.test.ts` |
| Fibonacci memo and tabulation | `dynamic-programming.ts` | O(n) | O(n), O(1) | `dynamic-programming.test.ts` |
| 0/1 knapsack | `dynamic-programming.ts` | O(n * W) | O(W) | `dynamic-programming.test.ts` |
| LCS | `dynamic-programming.ts` | O(m * n) | O(m * n) | `dynamic-programming.test.ts` |
| LIS length | `dynamic-programming.ts` | O(n log n) | O(n) | `dynamic-programming.test.ts` |
| Edit distance | `dynamic-programming.ts` | O(m * n) | O(min(m, n)) | `dynamic-programming.test.ts` |
| Coin change (min coins, ways) | `dynamic-programming.ts` | O(amount * coins) | O(amount) | `dynamic-programming.test.ts` |
| Grid paths with obstacles | `dynamic-programming.ts` | O(rows * cols) | O(cols) | `dynamic-programming.test.ts` |
| Matrix chain order | `dynamic-programming.ts` | O(n^3) | O(n^2) | `dynamic-programming.test.ts` |
| Two pointers, 3Sum, container with most water | `array-techniques.ts` | O(n) to O(n^2) | O(1) | `array-techniques.test.ts` |
| Sliding window (fixed and variable) | `array-techniques.ts` | O(n) | O(1) to O(alphabet) | `array-techniques.test.ts` |
| Prefix sums, subarray sum equals k | `array-techniques.ts` | O(n) build, O(1) query | O(n) | `array-techniques.test.ts` |
| Permutations, subsets, combinations | `backtracking.ts` | O(n * n!), O(n * 2^n), O(n * C(n,k)) | O(n) | `backtracking.test.ts` |
| N-queens, sudoku | `backtracking.ts` | exponential | O(n), O(81) | `backtracking.test.ts` |
| KMP search | `strings.ts` | O(n + m) | O(m) | `strings-interview.test.ts` |
| Palindromes, anagram check and grouping | `strings.ts` | O(n) to O(n^2) | O(1) to O(n) | `strings-interview.test.ts` |
| Two sum, valid parentheses, merge intervals, kth largest, max subarray, reverse list | `interview.ts` | see header comments | see header comments | `strings-interview.test.ts` |

Tests live in `tests/` and mirror `src/`. Several related structures share a test file as listed above.

## Testing approach

Besides example-based tests there are seeded randomized tests (mulberry32 generator in `tests/helpers/rng.ts`,
so runs are reproducible). Some of the comparisons:

- every sort equals `Array.prototype.sort` with a numeric comparator on 200 random arrays, plus 50000-element inputs
- BST and AVL invariants (ordering, stored heights, balance) are checked during random inserts and deletes
- hash map is compared with `Map` over 20000 random operations; LRU cache eviction order is compared with a reference
- Dijkstra and Bellman-Ford are compared with Floyd-Warshall; Kruskal and Prim with a brute-force subset search
- knapsack, LCS, coin change, matrix chain are compared with brute force on small inputs
- error paths: pop from empty stack, invalid indices, negative cycles, cyclic topological sort, bad arguments

## Results

Measured with `npm test` on Node 20.20.2:

- 16 test suites, 195 tests, all passing
- coverage: 99.49% statements, 97.87% branches, 100% functions, 99.86% lines
- enforced thresholds in `jest.config.js`: 95% statements, 90% branches, 95% functions, 95% lines

`npm run lint`, `npm run typecheck` and `npm run build` pass. The CI workflow passes `actionlint` but has not
run on GitHub yet.

## Adding a new problem

1. Add the function to the matching file in `src/algorithms/` (or a new file, then export it from `src/algorithms/index.ts`).
   Put a one-line comment above it with time and space complexity.
2. Add tests in the mirrored file under `tests/algorithms/`: a few known examples, the edge cases (empty, single element,
   duplicates), and if a simple brute force exists, a seeded random comparison using `makeRng`.
3. Run `npm run lint && npm run typecheck && npm test`.

## Layout

```
src/
  data-structures/   one file per structure, index.ts re-exports
  algorithms/        grouped by topic, index.ts re-exports
  utils/compare.ts   shared comparator type
tests/
  data-structures/   mirrors src
  algorithms/        mirrors src
  helpers/rng.ts     seeded random helpers
legacy/index.js      the original file
.github/workflows/ci.yml
```

## Limitations

- Not optimised for production use; the built-in `Array`, `Map` and `sort` are faster for real workloads.
- The default hash in `HashMap` hashes the string form of the key, so distinct objects all collide. Pass your own hash
  function for object keys.
- `BinarySearchTree` is unbalanced and ignores duplicate values (set semantics), as does `AVLTree`.
- Recursion is used in AVL operations (depth is O(log n)), `mergeSort`, `fibMemo` (depth n, so large n can overflow the stack;
  use `fibTab`), and `solveSudoku`.
- Sorting functions copy the input; none sort in place. `heapSort` uses O(n) extra space in this version.
- `countingSort` only accepts integers and allocates an array the size of the value range.
- Dijkstra rejects negative weights; undirected graphs keep parallel edges and self loops.
- Floating-point weights are accepted but not compared with a tolerance.
- The CI workflow was linted with actionlint only; it has not run on GitHub.
