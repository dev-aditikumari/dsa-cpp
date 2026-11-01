#pragma once
// Hash map with separate chaining; doubles its bucket count when the load factor passes 0.75.
// insert/find/erase O(1) average, O(n) worst case. Space O(n).
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename K, typename V, typename Hash = std::hash<K>, typename Eq = std::equal_to<K>>
class HashMap {
    struct Node {
        K key;
        V value;
        std::unique_ptr<Node> next;
        Node(K k, V v) : key(std::move(k)), value(std::move(v)) {}
    };

public:
    explicit HashMap(std::size_t initial_buckets = 8, Hash hash = Hash(), Eq eq = Eq())
        : buckets_(initial_buckets == 0 ? 1 : initial_buckets), hash_(std::move(hash)), eq_(std::move(eq)) {}
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
    HashMap(HashMap&&) = delete;
    HashMap& operator=(HashMap&&) = delete;
    ~HashMap() {
        // Unlink chains one node at a time so a long chain cannot overflow the stack.
        for (auto& head : buckets_) {
            while (head) head = std::move(head->next);
        }
    }

    std::size_t size() const { return size_; }
    std::size_t bucket_count() const { return buckets_.size(); }
    bool empty() const { return size_ == 0; }

    // Returns true if a new key was added, false if an existing key was overwritten.
    bool insert_or_assign(K key, V value) {
        if (Node* n = find_node(key)) {
            n->value = std::move(value);
            return false;
        }
        std::size_t idx = index_for(key, buckets_.size());
        auto node = std::make_unique<Node>(std::move(key), std::move(value));
        node->next = std::move(buckets_[idx]);
        buckets_[idx] = std::move(node);
        ++size_;
        if (size_ * 4 > buckets_.size() * 3) rehash(buckets_.size() * 2);
        return true;
    }

    V* find(const K& key) {
        Node* n = find_node(key);
        return n ? &n->value : nullptr;
    }

    const V* find(const K& key) const {
        const Node* n = find_node(key);
        return n ? &n->value : nullptr;
    }

    bool contains(const K& key) const { return find_node(key) != nullptr; }

    const V& at(const K& key) const {
        const V* v = find(key);
        if (!v) throw std::out_of_range("key not found");
        return *v;
    }

    bool erase(const K& key) {
        std::unique_ptr<Node>* link = &buckets_[index_for(key, buckets_.size())];
        while (*link) {
            if (eq_((*link)->key, key)) {
                std::unique_ptr<Node> dead = std::move(*link);
                *link = std::move(dead->next);
                --size_;
                return true;
            }
            link = &(*link)->next;
        }
        return false;
    }

    template <typename F>
    void for_each(F&& f) const {
        for (const auto& head : buckets_) {
            for (const Node* n = head.get(); n; n = n->next.get()) f(n->key, n->value);
        }
    }

private:
    std::size_t index_for(const K& key, std::size_t buckets) const { return hash_(key) % buckets; }

    Node* find_node(const K& key) const {
        for (Node* n = buckets_[index_for(key, buckets_.size())].get(); n; n = n->next.get()) {
            if (eq_(n->key, key)) return n;
        }
        return nullptr;
    }

    void rehash(std::size_t new_count) {
        std::vector<std::unique_ptr<Node>> next(new_count);
        for (auto& head : buckets_) {
            while (head) {
                std::unique_ptr<Node> node = std::move(head);
                head = std::move(node->next);
                std::size_t idx = index_for(node->key, new_count);
                node->next = std::move(next[idx]);
                next[idx] = std::move(node);
            }
        }
        buckets_ = std::move(next);
    }

    std::vector<std::unique_ptr<Node>> buckets_;
    Hash hash_;
    Eq eq_;
    std::size_t size_ = 0;
};

}  // namespace dsa
