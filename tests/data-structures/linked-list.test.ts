import { LinkedList } from "../../src/data-structures/linked-list";
import { DoublyLinkedList } from "../../src/data-structures/doubly-linked-list";
import { makeRng, randInt } from "../helpers/rng";

describe("LinkedList", () => {
  it("starts empty", () => {
    const l = new LinkedList<number>();
    expect(l.size).toBe(0);
    expect(l.toArray()).toEqual([]);
  });

  it("push, unshift, get, indexOf", () => {
    const l = new LinkedList<number>();
    l.push(2);
    l.push(3);
    l.unshift(1);
    expect(l.toArray()).toEqual([1, 2, 3]);
    expect(l.get(1)).toBe(2);
    expect(l.indexOf(3)).toBe(2);
    expect(l.indexOf(9)).toBe(-1);
  });

  it("pop and shift keep tail consistent", () => {
    const l = new LinkedList<number>();
    l.push(1);
    expect(l.pop()).toBe(1);
    expect(l.size).toBe(0);
    l.push(5);
    l.push(6);
    expect(l.pop()).toBe(6);
    l.push(7);
    expect(l.toArray()).toEqual([5, 7]);
    expect(l.shift()).toBe(5);
    expect(l.shift()).toBe(7);
    l.push(8);
    expect(l.toArray()).toEqual([8]);
  });

  it("insertAt and removeAt handle ends and middle", () => {
    const l = new LinkedList<number>();
    l.insertAt(0, 1);
    l.insertAt(1, 3);
    l.insertAt(1, 2);
    expect(l.toArray()).toEqual([1, 2, 3]);
    expect(l.removeAt(2)).toBe(3);
    l.push(4);
    expect(l.toArray()).toEqual([1, 2, 4]);
    expect(l.removeAt(1)).toBe(2);
    expect(l.removeAt(0)).toBe(1);
  });

  it("reverses, including empty and single element", () => {
    const l = new LinkedList<number>();
    l.reverse();
    expect(l.toArray()).toEqual([]);
    l.push(1);
    l.reverse();
    expect(l.toArray()).toEqual([1]);
    l.push(2);
    l.push(3);
    l.reverse();
    expect(l.toArray()).toEqual([3, 2, 1]);
    l.push(9);
    expect(l.toArray()).toEqual([3, 2, 1, 9]);
  });

  it("throws on invalid access", () => {
    const l = new LinkedList<number>();
    expect(() => l.pop()).toThrow("empty");
    expect(() => l.shift()).toThrow("empty");
    expect(() => l.get(0)).toThrow(RangeError);
    expect(() => l.insertAt(1, 1)).toThrow(RangeError);
    expect(() => l.removeAt(-1)).toThrow(RangeError);
    expect(() => l.get(1.5)).toThrow(RangeError);
  });

  it("matches an array under random operations", () => {
    const rng = makeRng(11);
    const l = new LinkedList<number>();
    const ref: number[] = [];
    for (let i = 0; i < 2000; i++) {
      const op = randInt(rng, 0, 5);
      const v = randInt(rng, 0, 99);
      if (op === 0) {
      l.push(v);
      ref.push(v);
    }
      else if (op === 1) {
      l.unshift(v);
      ref.unshift(v);
    }
      else if (op === 2 && ref.length) expect(l.pop()).toBe(ref.pop());
      else if (op === 3 && ref.length) expect(l.shift()).toBe(ref.shift());
      else if (op === 4) {
        const idx = randInt(rng, 0, ref.length);
        l.insertAt(idx, v);
        ref.splice(idx, 0, v);
      } else if (op === 5 && ref.length) {
        const idx = randInt(rng, 0, ref.length - 1);
        expect(l.removeAt(idx)).toBe(ref.splice(idx, 1)[0]);
      }
      expect(l.size).toBe(ref.length);
    }
    expect(l.toArray()).toEqual(ref);
  });
});

describe("DoublyLinkedList", () => {
  it("supports both ends and reverse iteration", () => {
    const l = new DoublyLinkedList<string>();
    l.push("b");
    l.push("c");
    l.unshift("a");
    expect(l.toArray()).toEqual(["a", "b", "c"]);
    expect(l.toArrayReversed()).toEqual(["c", "b", "a"]);
    expect(l.pop()).toBe("c");
    expect(l.shift()).toBe("a");
    expect(l.pop()).toBe("b");
    expect(l.size).toBe(0);
    expect(l.toArrayReversed()).toEqual([]);
  });

  it("get and removeAt work from either half", () => {
    const l = new DoublyLinkedList<number>();
    for (let i = 0; i < 10; i++) l.push(i);
    expect(l.get(1)).toBe(1);
    expect(l.get(8)).toBe(8);
    expect(l.removeAt(9)).toBe(9);
    expect(l.removeAt(0)).toBe(0);
    expect(l.removeAt(4)).toBe(5);
    expect(l.toArray()).toEqual([1, 2, 3, 4, 6, 7, 8]);
    expect(l.toArrayReversed()).toEqual([8, 7, 6, 4, 3, 2, 1]);
  });

  it("throws on empty and bad index", () => {
    const l = new DoublyLinkedList<number>();
    expect(() => l.pop()).toThrow("empty");
    expect(() => l.shift()).toThrow("empty");
    expect(() => l.get(0)).toThrow(RangeError);
  });

  it("matches an array under random operations", () => {
    const rng = makeRng(12);
    const l = new DoublyLinkedList<number>();
    const ref: number[] = [];
    for (let i = 0; i < 2000; i++) {
      const op = randInt(rng, 0, 4);
      const v = randInt(rng, 0, 99);
      if (op === 0) {
      l.push(v);
      ref.push(v);
    }
      else if (op === 1) {
      l.unshift(v);
      ref.unshift(v);
    }
      else if (op === 2 && ref.length) expect(l.pop()).toBe(ref.pop());
      else if (op === 3 && ref.length) expect(l.shift()).toBe(ref.shift());
      else if (op === 4 && ref.length) {
        const idx = randInt(rng, 0, ref.length - 1);
        expect(l.removeAt(idx)).toBe(ref.splice(idx, 1)[0]);
      }
    }
    expect(l.toArray()).toEqual(ref);
    expect(l.toArrayReversed()).toEqual([...ref].reverse());
  });
});
