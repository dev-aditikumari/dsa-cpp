import { Stack } from "../../src/data-structures/stack";
import { Queue } from "../../src/data-structures/queue";
import { Deque } from "../../src/data-structures/deque";
import { makeRng, randInt } from "../helpers/rng";

describe("Stack", () => {
  it("is LIFO", () => {
    const s = new Stack<number>();
    expect(s.isEmpty()).toBe(true);
    s.push(1);
    s.push(2);
    expect(s.peek()).toBe(2);
    expect(s.size).toBe(2);
    expect(s.pop()).toBe(2);
    expect(s.pop()).toBe(1);
    expect(s.isEmpty()).toBe(true);
  });

  it("throws when empty", () => {
    const s = new Stack<number>();
    expect(() => s.pop()).toThrow("Stack is empty");
    expect(() => s.peek()).toThrow("Stack is empty");
  });

  it("can hold undefined values", () => {
    const s = new Stack<undefined>();
    s.push(undefined);
    expect(s.size).toBe(1);
    expect(s.pop()).toBeUndefined();
  });
});

describe("Queue", () => {
  it("is FIFO", () => {
    const q = new Queue<string>();
    q.enqueue("a");
    q.enqueue("b");
    expect(q.peek()).toBe("a");
    expect(q.dequeue()).toBe("a");
    expect(q.dequeue()).toBe("b");
    expect(q.isEmpty()).toBe(true);
  });

  it("throws when empty", () => {
    const q = new Queue<number>();
    expect(() => q.dequeue()).toThrow("Queue is empty");
    expect(() => q.peek()).toThrow("Queue is empty");
  });

  it("stays correct across compaction with 10000 items", () => {
    const q = new Queue<number>();
    for (let i = 0; i < 10000; i++) q.enqueue(i);
    for (let i = 0; i < 9000; i++) expect(q.dequeue()).toBe(i);
    for (let i = 10000; i < 10100; i++) q.enqueue(i);
    expect(q.size).toBe(1100);
    expect(q.dequeue()).toBe(9000);
    expect(q.peek()).toBe(9001);
  });
});

describe("Deque", () => {
  it("works from both ends", () => {
    const d = new Deque<number>(2);
    d.pushBack(2);
    d.pushFront(1);
    d.pushBack(3);
    d.pushFront(0);
    expect(d.toArray()).toEqual([0, 1, 2, 3]);
    expect(d.peekFront()).toBe(0);
    expect(d.peekBack()).toBe(3);
    expect(d.popFront()).toBe(0);
    expect(d.popBack()).toBe(3);
    expect(d.size).toBe(2);
  });

  it("throws when empty or capacity invalid", () => {
    const d = new Deque<number>();
    expect(() => d.popFront()).toThrow("empty");
    expect(() => d.popBack()).toThrow("empty");
    expect(() => d.peekFront()).toThrow("empty");
    expect(() => d.peekBack()).toThrow("empty");
    expect(() => new Deque(0)).toThrow(RangeError);
  });

  it("matches an array under random operations with wrap-around", () => {
    const rng = makeRng(21);
    const d = new Deque<number>(1);
    const ref: number[] = [];
    for (let i = 0; i < 5000; i++) {
      const op = randInt(rng, 0, 3);
      if (op === 0) {
      d.pushBack(i);
      ref.push(i);
    }
      else if (op === 1) {
      d.pushFront(i);
      ref.unshift(i);
    }
      else if (op === 2 && ref.length) expect(d.popBack()).toBe(ref.pop());
      else if (op === 3 && ref.length) expect(d.popFront()).toBe(ref.shift());
      if (i % 500 === 0) expect(d.toArray()).toEqual(ref);
    }
    expect(d.toArray()).toEqual(ref);
  });
});
