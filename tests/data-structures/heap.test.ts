import { BinaryHeap } from "../../src/data-structures/heap";
import { PriorityQueue } from "../../src/data-structures/priority-queue";
import { makeRng, randArray, randInt } from "../helpers/rng";

describe("BinaryHeap", () => {
  it("min heap pops ascending", () => {
    const h = BinaryHeap.minHeap<number>();
    [5, 1, 4, 1, 3].forEach((x) => h.push(x));
    expect(h.peek()).toBe(1);
    const out: number[] = [];
    while (!h.isEmpty()) out.push(h.pop());
    expect(out).toEqual([1, 1, 3, 4, 5]);
  });

  it("max heap pops descending", () => {
    const h = BinaryHeap.maxHeap<number>();
    [5, 1, 4].forEach((x) => h.push(x));
    expect(h.pop()).toBe(5);
    expect(h.pop()).toBe(4);
    expect(h.pop()).toBe(1);
  });

  it("works with a custom comparator on objects and strings", () => {
    const h = new BinaryHeap<{ n: string; age: number }>((a, b) => a.age - b.age);
    h.push({ n: "a", age: 30 });
    h.push({ n: "b", age: 20 });
    expect(h.pop().n).toBe("b");
    const s = BinaryHeap.from(["pear", "apple", "fig"]);
    expect(s.pop()).toBe("apple");
  });

  it("throws on empty", () => {
    const h = new BinaryHeap<number>();
    expect(() => h.pop()).toThrow("empty");
    expect(() => h.peek()).toThrow("empty");
  });

  it("heapify from array matches sort on 200 random arrays", () => {
    const rng = makeRng(31);
    for (let t = 0; t < 200; t++) {
      const arr = randArray(rng, 60);
      const h = BinaryHeap.from(arr);
      const out: number[] = [];
      while (!h.isEmpty()) out.push(h.pop());
      expect(out).toEqual([...arr].sort((a, b) => a - b));
    }
  });

  it("interleaved push/pop matches a sorted-array reference", () => {
    const rng = makeRng(32);
    const h = new BinaryHeap<number>();
    const ref: number[] = [];
    for (let i = 0; i < 3000; i++) {
      if (randInt(rng, 0, 2) > 0 || ref.length === 0) {
        const v = randInt(rng, 0, 100);
        h.push(v);
        ref.push(v);
        ref.sort((a, b) => a - b);
      } else {
        expect(h.pop()).toBe(ref.shift());
      }
      expect(h.size).toBe(ref.length);
    }
  });
});

describe("PriorityQueue", () => {
  it("orders by priority and FIFO among ties", () => {
    const pq = new PriorityQueue<string>();
    pq.enqueue("low", 5);
    pq.enqueue("first", 1);
    pq.enqueue("second", 1);
    pq.enqueue("mid", 3);
    expect(pq.peek()).toBe("first");
    expect([pq.dequeue(), pq.dequeue(), pq.dequeue(), pq.dequeue()]).toEqual([
      "first",
      "second",
      "mid",
      "low",
    ]);
    expect(pq.isEmpty()).toBe(true);
    expect(pq.size).toBe(0);
  });

  it("rejects NaN priority and empty access", () => {
    const pq = new PriorityQueue<number>();
    expect(() => pq.enqueue(1, NaN)).toThrow(RangeError);
    expect(() => pq.dequeue()).toThrow("empty");
    expect(() => pq.peek()).toThrow("empty");
  });
});
