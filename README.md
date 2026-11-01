# Data Structures and Algorithms in C++

Header-only C++17 implementations of common data structures and algorithms, with a doctest test suite
that is built with `-Wall -Wextra -Wpedantic -Werror` and run under AddressSanitizer and
UndefinedBehaviorSanitizer. Every header has a short complexity note. The standard library is the only dependency
(doctest is vendored for the tests). The original one-line `index.js` from this repo is kept in `legacy/`.

## Requirements

- A C++17 compiler (checked locally with Apple clang 21; also compiles with `-std=c++20`)
- `make`

## Build and test

```
make test          # build tests in build/ and run them
make sanitize      # rebuild with -fsanitize=address,undefined into build-san/ and run
make headers       # compile every header on its own
make coverage LLVM_PROFDATA="xcrun llvm-profdata" LLVM_COV="xcrun llvm-cov"   # clang line coverage report (macOS)
make clean
```

Variables: `CXX=g++`, `STD=-std=c++20`, `EXTRA="-fsanitize=thread"` and so on. Use the library by adding `include/` to your
include path and writing `#include "dsa/all.hpp"` (or a single header such as `dsa/heap.hpp`). Everything lives in
namespace `dsa`.

## Data structures (`include/dsa/`)

n is the number of elements, V and E are vertices and edges, h is tree height.

| Structure | Header | Time | Space | Test file |
|---|---|---|---|---|
| Singly linked list | `linked_list.hpp` | push/pop front O(1), push_back O(1), pop_back/at/insert/erase O(n) | O(n) | `test_linked_list.cpp` |
| Doubly linked list | `linked_list.hpp` | both ends O(1), at/erase by index O(n) | O(n) | `test_linked_list.cpp` |
| Stack | `stack_queue.hpp` | O(1) amortised | O(n) | `test_stack_queue_deque.cpp` |
| Queue | `stack_queue.hpp` | O(1) amortised | O(n) | `test_stack_queue_deque.cpp` |
| Deque (ring buffer) | `stack_queue.hpp` | O(1) amortised at both ends | O(n) | `test_stack_queue_deque.cpp` |
| Binary heap (min or max) | `heap.hpp` | push/pop O(log n), top O(1), build O(n) | O(n) | `test_heap.cpp` |
| Priority queue (FIFO on ties) | `heap.hpp` | O(log n) | O(n) | `test_heap.cpp` |
| Hash map (chaining, rehash at load 0.75) | `hash_map.hpp` | O(1) average, O(n) worst | O(n) | `test_hash_map.cpp` |
| Binary search tree | `trees.hpp` | O(h), O(n) when degenerate | O(n) | `test_trees.cpp` |
| AVL tree | `trees.hpp` | O(log n) | O(n) | `test_trees.cpp` |
| Trie | `trie.hpp` | O(L) per word | O(total characters) | `test_trie.cpp` |
| Graph (adjacency list) | `graph.hpp` | add_edge O(1), neighbors O(1) | O(V + E) | `test_graph.cpp` |
| Union-find (rank, path compression) | `union_find.hpp` | O(alpha(n)) amortised | O(n) | `test_graph.cpp` |
| LRU cache | `lru_cache.hpp` | get/put O(1) average | O(capacity) | `test_lru_cache.cpp` |
| Fenwick tree | `range_trees.hpp` | O(log n) | O(n) | `test_range_trees.cpp` |
| Segment tree | `range_trees.hpp` | query/update O(log n) | O(n) | `test_range_trees.cpp` |

## Algorithms (`include/dsa/`)

| Algorithm | Header | Time | Space | Test file |
|---|---|---|---|---|
| Bubble, insertion, selection sort | `sorting.hpp` | O(n^2) | O(1) extra | `test_sorting.cpp` |
| Merge sort (stable) | `sorting.hpp` | O(n log n) | O(n) | `test_sorting.cpp` |
| Quick sort (3-way, median of three) | `sorting.hpp` | O(n log n) expected, O(n^2) worst | O(log n) | `test_sorting.cpp` |
| Heap sort | `sorting.hpp` | O(n log n) | O(1) extra | `test_sorting.cpp` |
| Counting sort | `sorting.hpp` | O(n + k) | O(n + k) | `test_sorting.cpp` |
| Binary search, lower/upper bound, count | `searching.hpp` | O(log n) | O(1) | `test_searching.cpp` |
| Rotated array search, rotation point | `searching.hpp` | O(log n), O(n) with duplicates | O(1) | `test_searching.cpp` |
| Search on answer, ship within days, integer sqrt | `searching.hpp` | O(log range) probes | O(1) | `test_searching.cpp` |
| BFS, DFS, unweighted path, components | `graph_algorithms.hpp` | O(V + E) | O(V) | `test_graph_algorithms.cpp` |
| Topological sort (Kahn) | `graph_algorithms.hpp` | O(V + E) | O(V) | `test_graph_algorithms.cpp` |
| Dijkstra | `graph_algorithms.hpp` | O((V + E) log V) | O(V) | `test_graph_algorithms.cpp` |
| Bellman-Ford | `graph_algorithms.hpp` | O(V * E) | O(V) | `test_graph_algorithms.cpp` |
| Kruskal, Prim | `graph_algorithms.hpp` | O(E log E), O(E log V) | O(V + E) | `test_graph_algorithms.cpp` |
| Cycle detection (directed and undirected) | `graph_algorithms.hpp` | O(V + E) | O(V) | `test_graph_algorithms.cpp` |
| Fibonacci (memo, tabulation) | `dynamic_programming.hpp` | O(n) | O(n), O(1) | `test_dynamic_programming.cpp` |
| 0/1 knapsack | `dynamic_programming.hpp` | O(n * W) | O(W) | `test_dynamic_programming.cpp` |
| LCS | `dynamic_programming.hpp` | O(m * n) | O(m * n) | `test_dynamic_programming.cpp` |
| LIS length | `dynamic_programming.hpp` | O(n log n) | O(n) | `test_dynamic_programming.cpp` |
| Edit distance | `dynamic_programming.hpp` | O(m * n) | O(min(m, n)) | `test_dynamic_programming.cpp` |
| Coin change (min coins, ways) | `dynamic_programming.hpp` | O(amount * coins) | O(amount) | `test_dynamic_programming.cpp` |
| Grid paths with obstacles | `dynamic_programming.hpp` | O(rows * cols) | O(cols) | `test_dynamic_programming.cpp` |
| Matrix chain order | `dynamic_programming.hpp` | O(n^3) | O(n^2) | `test_dynamic_programming.cpp` |
| Two pointers, 3Sum, most water | `array_techniques.hpp` | O(n) to O(n^2) | O(1) | `test_array_techniques.cpp` |
| Sliding window (fixed and variable) | `array_techniques.hpp` | O(n) | O(1) to O(alphabet) | `test_array_techniques.cpp` |
| Prefix sums, subarray sum equals k | `array_techniques.hpp` | O(n) build, O(1) query | O(n) | `test_array_techniques.cpp` |
| Permutations, subsets, combinations | `backtracking.hpp` | O(n * n!), O(n * 2^n), O(k * C(n,k)) | O(n) plus output | `test_backtracking.cpp` |
| N-queens, sudoku | `backtracking.hpp` | exponential | O(n), O(81) | `test_backtracking.cpp` |
| KMP search | `strings.hpp` | O(n + m) | O(m) | `test_strings_interview.cpp` |
| Palindromes, anagram check and grouping | `strings.hpp` | O(n) to O(n^2) | O(1) to O(n) | `test_strings_interview.cpp` |
| Two sum, valid parentheses, merge intervals, kth largest, max subarray, reverse list | `interview.hpp` | see comment above each function | see comment above each function | `test_strings_interview.cpp` |

## Design notes

- Nodes are owned by `std::unique_ptr`; there is no `new` or `delete`. Destructors of lists, trees and the hash map
  unlink iteratively, so a 300000 node list or a 100000 node degenerate tree does not overflow the stack.
- Containers are non-copyable and non-movable (copy and move are deleted) to keep ownership simple. Pass them by reference.
- Empty-container access throws (`std::underflow_error`), bad indexes throw `std::out_of_range`, bad arguments throw
  `std::invalid_argument`.
- Sorting functions take the vector by value and return a sorted copy. `Compare(a, b)` means "a goes before b".
- Vertices of `Graph` are integers `0..n-1`; weights are `long long`.

## What was run

All of this was run on macOS (arm64) with Apple clang 21.0.0. The `g++` command on that machine is the same clang.

- `make test`: 149 test cases, 186052 assertions, all passing (doctest). Built with `-std=c++17 -Wall -Wextra -Wpedantic -Werror`.
- Same suite with `STD=-std=c++20`: all passing.
- `make sanitize` (AddressSanitizer plus UndefinedBehaviorSanitizer, `-fno-sanitize-recover=all`): all 149 test cases pass with no reports.
  LeakSanitizer is not supported on macOS, so leaks were checked separately with the macOS `leaks --atExit` tool: 0 leaks.
  Valgrind was not used.
- `make headers`: every header compiles by itself with the warning flags.
- Randomised tests compare against `std::sort`, `std::priority_queue`, `std::unordered_map`, `std::set`, `std::deque`,
  Floyd-Warshall (shortest paths), brute-force subset search (MST, knapsack, LCS), and plain reference implementations.
  Seeds are fixed (`mt19937`), so runs are repeatable.
- Line coverage with clang (`make coverage`, headers only): 99.95% of lines (1 of 1903 missed), 100% of functions,
  96.94% of branches (30 of 980 missed).
- Mutation check, done by hand: six deliberate bugs were put into the library one at a time and each made the suite fail,
  then each was reverted. They were: hash map erase not decrementing the size, AVL rebalance threshold off, merge sort
  made unstable, LRU `get` not refreshing recency, `lower_bound_index` comparison flipped, Kruskal sorting heaviest first.
- cppcheck 2.22 (`--enable=warning,style,performance,portability`) and clang-tidy 23 (`bugprone-*`, `clang-analyzer-*`,
  `performance-*`) were run on the headers and tests. The two real findings (uninitialised struct members in the priority
  queue, a missing `const` in the trie) were fixed. The remaining reports are style suggestions, test-code notes
  such as unchecked `optional` access after a `CHECK`, and a few false positives. They were not changed.
- The GitHub Actions workflow (`.github/workflows/ci.yml`: g++ and clang++, C++17 and C++20, plus an ASan/UBSan job)
  passes `actionlint` but has not run on GitHub. It has not been compiled with GNU g++ at all; only Apple clang was available.

## Limitations

- Educational code, not tuned for production. The standard containers are faster and more complete.
- `HashMap` uses `std::hash` by default and has no iterators, only `for_each`. Its bucket count doubles but never shrinks.
- `BinarySearchTree` and `AVLTree` store unique values (set semantics) and need copyable elements. The AVL
  operations recurse, which is fine because the depth is O(log n).
- `merge_sort` and `fib_memo` recurse; `fib_memo` is limited to n <= 93 because the result must fit in `uint64_t`.
- `counting_sort` takes `int` only and throws if the value range is larger than 100 million by default.
- `Trie` and the string algorithms work on bytes, so they do not understand Unicode code points.
- `LRUCache` returns copies of values (`std::optional<V>`) and uses `std::list` and `std::unordered_map` internally.
- `solve_sudoku` is plain backtracking and can be slow on adversarial puzzles.
- Containers are not thread-safe.
