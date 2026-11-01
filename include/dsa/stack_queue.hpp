#pragma once
// Stack (vector-backed, O(1) amortised), Queue (ring buffer, O(1) amortised) and Deque (ring buffer, O(1) amortised
// at both ends). Space O(n). Empty access throws std::underflow_error.
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T>
class Stack {
public:
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }
    void push(T value) { items_.push_back(std::move(value)); }
    T pop() {
        if (items_.empty()) throw std::underflow_error("stack is empty");
        T value = std::move(items_.back());
        items_.pop_back();
        return value;
    }
    const T& top() const {
        if (items_.empty()) throw std::underflow_error("stack is empty");
        return items_.back();
    }

private:
    std::vector<T> items_;
};

template <typename T>
class Deque {
public:
    explicit Deque(std::size_t initial_capacity = 8) : buf_(initial_capacity == 0 ? 1 : initial_capacity) {}

    std::size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }

    void push_back(T value) {
        grow();
        buf_[(head_ + count_) % buf_.size()] = std::move(value);
        ++count_;
    }

    void push_front(T value) {
        grow();
        head_ = (head_ + buf_.size() - 1) % buf_.size();
        buf_[head_] = std::move(value);
        ++count_;
    }

    T pop_front() {
        check();
        T value = std::move(*buf_[head_]);
        buf_[head_].reset();
        head_ = (head_ + 1) % buf_.size();
        --count_;
        return value;
    }

    T pop_back() {
        check();
        std::size_t idx = (head_ + count_ - 1) % buf_.size();
        T value = std::move(*buf_[idx]);
        buf_[idx].reset();
        --count_;
        return value;
    }

    const T& front() const {
        check();
        return *buf_[head_];
    }

    const T& back() const {
        check();
        return *buf_[(head_ + count_ - 1) % buf_.size()];
    }

    std::vector<T> to_vector() const {
        std::vector<T> out;
        for (std::size_t i = 0; i < count_; ++i) out.push_back(*buf_[(head_ + i) % buf_.size()]);
        return out;
    }

private:
    void check() const {
        if (count_ == 0) throw std::underflow_error("deque is empty");
    }

    void grow() {
        if (count_ < buf_.size()) return;
        std::vector<std::optional<T>> next(buf_.size() * 2);
        for (std::size_t i = 0; i < count_; ++i) next[i] = std::move(buf_[(head_ + i) % buf_.size()]);
        buf_ = std::move(next);
        head_ = 0;
    }

    std::vector<std::optional<T>> buf_;
    std::size_t head_ = 0;
    std::size_t count_ = 0;
};

template <typename T>
class Queue {
public:
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }
    void push(T value) { items_.push_back(std::move(value)); }
    T pop() {
        if (items_.empty()) throw std::underflow_error("queue is empty");
        return items_.pop_front();
    }
    const T& front() const {
        if (items_.empty()) throw std::underflow_error("queue is empty");
        return items_.front();
    }

private:
    Deque<T> items_;
};

}  // namespace dsa
