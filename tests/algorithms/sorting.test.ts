import * as S from "../../src/algorithms/sorting";
import { makeRng, randArray, randInt } from "../helpers/rng";

const numeric = (a: number, b: number) => a - b;
const sorts: [string, (a: number[]) => number[]][] = [
  ["bubbleSort", (a) => S.bubbleSort(a)],
  ["insertionSort", (a) => S.insertionSort(a)],
  ["selectionSort", (a) => S.selectionSort(a)],
  ["mergeSort", (a) => S.mergeSort(a)],
  ["quickSort", (a) => S.quickSort(a)],
  ["heapSort", (a) => S.heapSort(a)],
  ["countingSort", (a) => S.countingSort(a)],
];

describe.each(sorts)("%s", (_name, sort) => {
  it("handles empty, single and already sorted input", () => {
    expect(sort([])).toEqual([]);
    expect(sort([7])).toEqual([7]);
    expect(sort([1, 2, 3])).toEqual([1, 2, 3]);
    expect(sort([3, 2, 1])).toEqual([1, 2, 3]);
  });

  it("handles duplicates and negatives", () => {
    expect(sort([3, -1, 3, 0, -1, 3])).toEqual([-1, -1, 0, 3, 3, 3]);
    expect(sort([5, 5, 5, 5])).toEqual([5, 5, 5, 5]);
  });

  it("does not mutate the input", () => {
    const input = [3, 1, 2];
    sort(input);
    expect(input).toEqual([3, 1, 2]);
  });

  it("equals Array.prototype.sort on 200 random arrays", () => {
    const rng = makeRng(101);
    for (let t = 0; t < 200; t++) {
      const arr = randArray(rng, 80, -30, 30);
      expect(sort(arr)).toEqual([...arr].sort(numeric));
    }
  });
});

describe("large input", () => {
  it.each([
    ["mergeSort", S.mergeSort],
    ["quickSort", S.quickSort],
    ["heapSort", S.heapSort],
    ["countingSort", S.countingSort],
  ] as const)("%s sorts 50000 random numbers", (_n, sort) => {
    const rng = makeRng(102);
    const arr = Array.from({ length: 50000 }, () => randInt(rng, -1000, 1000));
    expect(sort(arr)).toEqual([...arr].sort(numeric));
  });

  it("quickSort survives sorted, reversed and all-equal arrays of 50000", () => {
    const asc = Array.from({ length: 50000 }, (_, i) => i);
    expect(S.quickSort(asc)).toEqual(asc);
    expect(S.quickSort([...asc].reverse())).toEqual(asc);
    expect(S.quickSort(new Array<number>(50000).fill(4))).toEqual(new Array<number>(50000).fill(4));
  });
});

describe("comparator-based sorts", () => {
  type P = { k: number; id: number };
  const stable = [
    ["bubbleSort", S.bubbleSort],
    ["insertionSort", S.insertionSort],
    ["mergeSort", S.mergeSort],
  ] as const;

  it.each(stable)("%s is stable", (_n, sort) => {
    const rng = makeRng(103);
    const items: P[] = Array.from({ length: 200 }, (_, id) => ({ k: randInt(rng, 0, 5), id }));
    const out = sort(items, (a, b) => a.k - b.k);
    for (let i = 1; i < out.length; i++) {
      const p = out[i - 1] as P;
      const q = out[i] as P;
      expect(p.k < q.k || (p.k === q.k && p.id < q.id)).toBe(true);
    }
  });

  it("sort strings and use a descending comparator", () => {
    expect(S.quickSort(["pear", "apple", "fig"])).toEqual(["apple", "fig", "pear"]);
    expect(S.heapSort([1, 3, 2], (a, b) => b - a)).toEqual([3, 2, 1]);
    expect(S.selectionSort([1, 3, 2], (a, b) => b - a)).toEqual([3, 2, 1]);
  });
});

describe("countingSort errors", () => {
  it("rejects non-integers", () => {
    expect(() => S.countingSort([1, 1.5])).toThrow(TypeError);
  });
});
