#pragma once
// Singly and doubly linked lists. Nodes are owned through unique_ptr, so there is no manual delete.
// Singly: push_front O(1), push_back O(1), pop_back/at/insert/erase O(n). Doubly: both ends O(1), at O(n). Space O(n).
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T>
class SinglyLinkedList {
    struct Node {
        T value;
        std::unique_ptr<Node> next;
        explicit Node(T v) : value(std::move(v)) {}
    };

public:
    SinglyLinkedList() = default;
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;
    SinglyLinkedList(SinglyLinkedList&&) = delete;
    SinglyLinkedList& operator=(SinglyLinkedList&&) = delete;
    // Iterative so a very long list cannot overflow the stack when it is destroyed.
    ~SinglyLinkedList() { clear(); }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        while (head_) head_ = std::move(head_->next);
        tail_ = nullptr;
        size_ = 0;
    }

    void push_back(T value) {
        auto node = std::make_unique<Node>(std::move(value));
        Node* raw = node.get();
        if (tail_) tail_->next = std::move(node);
        else head_ = std::move(node);
        tail_ = raw;
        ++size_;
    }

    void push_front(T value) {
        auto node = std::make_unique<Node>(std::move(value));
        node->next = std::move(head_);
        head_ = std::move(node);
        if (!tail_) tail_ = head_.get();
        ++size_;
    }

    T pop_front() {
        if (!head_) throw std::underflow_error("list is empty");
        std::unique_ptr<Node> node = std::move(head_);
        head_ = std::move(node->next);
        if (!head_) tail_ = nullptr;
        --size_;
        return std::move(node->value);
    }

    T pop_back() {
        if (!head_) throw std::underflow_error("list is empty");
        if (size_ == 1) return pop_front();
        Node* cur = head_.get();
        while (cur->next->next) cur = cur->next.get();
        T value = std::move(cur->next->value);
        cur->next.reset();
        tail_ = cur;
        --size_;
        return value;
    }

    const T& at(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        const Node* cur = head_.get();
        for (std::size_t i = 0; i < index; ++i) cur = cur->next.get();
        return cur->value;
    }

    void insert(std::size_t index, T value) {
        if (index > size_) throw std::out_of_range("index out of range");
        if (index == 0) return push_front(std::move(value));
        if (index == size_) return push_back(std::move(value));
        Node* prev = head_.get();
        for (std::size_t i = 0; i + 1 < index; ++i) prev = prev->next.get();
        auto node = std::make_unique<Node>(std::move(value));
        node->next = std::move(prev->next);
        prev->next = std::move(node);
        ++size_;
    }

    T erase(std::size_t index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        if (index == 0) return pop_front();
        Node* prev = head_.get();
        for (std::size_t i = 0; i + 1 < index; ++i) prev = prev->next.get();
        std::unique_ptr<Node> target = std::move(prev->next);
        prev->next = std::move(target->next);
        if (!prev->next) tail_ = prev;
        --size_;
        return std::move(target->value);
    }

    // Returns the index of the first element equal to value, or -1.
    long find(const T& value) const {
        long i = 0;
        for (const Node* cur = head_.get(); cur; cur = cur->next.get(), ++i) {
            if (cur->value == value) return i;
        }
        return -1;
    }

    void reverse() {
        std::unique_ptr<Node> prev;
        std::unique_ptr<Node> cur = std::move(head_);
        tail_ = cur.get();
        while (cur) {
            std::unique_ptr<Node> next = std::move(cur->next);
            cur->next = std::move(prev);
            prev = std::move(cur);
            cur = std::move(next);
        }
        head_ = std::move(prev);
    }

    std::vector<T> to_vector() const {
        std::vector<T> out;
        out.reserve(size_);
        for (const Node* cur = head_.get(); cur; cur = cur->next.get()) out.push_back(cur->value);
        return out;
    }

private:
    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

template <typename T>
class DoublyLinkedList {
    struct Node {
        T value;
        std::unique_ptr<Node> next;
        Node* prev = nullptr;  // non-owning, the previous node owns this one
        explicit Node(T v) : value(std::move(v)) {}
    };

public:
    DoublyLinkedList() = default;
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList(DoublyLinkedList&&) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&&) = delete;
    ~DoublyLinkedList() { clear(); }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        while (head_) head_ = std::move(head_->next);
        tail_ = nullptr;
        size_ = 0;
    }

    void push_back(T value) {
        auto node = std::make_unique<Node>(std::move(value));
        Node* raw = node.get();
        node->prev = tail_;
        if (tail_) tail_->next = std::move(node);
        else head_ = std::move(node);
        tail_ = raw;
        ++size_;
    }

    void push_front(T value) {
        auto node = std::make_unique<Node>(std::move(value));
        if (head_) head_->prev = node.get();
        else tail_ = node.get();
        node->next = std::move(head_);
        head_ = std::move(node);
        ++size_;
    }

    T pop_front() {
        if (!head_) throw std::underflow_error("list is empty");
        return unlink(head_.get());
    }

    T pop_back() {
        if (!tail_) throw std::underflow_error("list is empty");
        return unlink(tail_);
    }

    const T& at(std::size_t index) const { return node_at(index)->value; }

    T erase(std::size_t index) { return unlink(node_at(index)); }

    std::vector<T> to_vector() const {
        std::vector<T> out;
        for (const Node* cur = head_.get(); cur; cur = cur->next.get()) out.push_back(cur->value);
        return out;
    }

    std::vector<T> to_vector_reversed() const {
        std::vector<T> out;
        for (const Node* cur = tail_; cur; cur = cur->prev) out.push_back(cur->value);
        return out;
    }

private:
    Node* node_at(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        if (index < size_ / 2) {
            Node* cur = head_.get();
            for (std::size_t i = 0; i < index; ++i) cur = cur->next.get();
            return cur;
        }
        Node* cur = tail_;
        for (std::size_t i = size_ - 1; i > index; --i) cur = cur->prev;
        return cur;
    }

    T unlink(Node* node) {
        T value = std::move(node->value);
        Node* prev = node->prev;
        std::unique_ptr<Node> owned;
        if (prev) owned = std::move(prev->next);
        else owned = std::move(head_);
        std::unique_ptr<Node> next = std::move(owned->next);
        if (next) next->prev = prev;
        else tail_ = prev;
        if (prev) prev->next = std::move(next);
        else head_ = std::move(next);
        --size_;
        return value;
    }

    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace dsa
