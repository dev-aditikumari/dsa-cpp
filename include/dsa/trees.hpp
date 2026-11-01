#pragma once
// BinarySearchTree: insert/contains/erase O(h), O(n) when degenerate; traversals O(n); space O(n). Set semantics.
// All operations are iterative (including destruction), so a 100000-node chain does not overflow the stack.
// AVLTree: insert/contains/erase O(log n); recursion depth is O(log n). Set semantics.
#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
    struct Node {
        T value;
        std::unique_ptr<Node> left, right;
        explicit Node(T v) : value(std::move(v)) {}
    };

public:
    explicit BinarySearchTree(Compare cmp = Compare()) : cmp_(std::move(cmp)) {}
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    ~BinarySearchTree() {
        std::vector<std::unique_ptr<Node>> pending;
        if (root_) pending.push_back(std::move(root_));
        while (!pending.empty()) {
            std::unique_ptr<Node> n = std::move(pending.back());
            pending.pop_back();
            if (n->left) pending.push_back(std::move(n->left));
            if (n->right) pending.push_back(std::move(n->right));
        }
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    bool insert(T value) {
        std::unique_ptr<Node>* link = &root_;
        while (*link) {
            if (cmp_(value, (*link)->value)) link = &(*link)->left;
            else if (cmp_((*link)->value, value)) link = &(*link)->right;
            else return false;
        }
        *link = std::make_unique<Node>(std::move(value));
        ++size_;
        return true;
    }

    bool contains(const T& value) const {
        const Node* cur = root_.get();
        while (cur) {
            if (cmp_(value, cur->value)) cur = cur->left.get();
            else if (cmp_(cur->value, value)) cur = cur->right.get();
            else return true;
        }
        return false;
    }

    const T& min() const {
        if (!root_) throw std::underflow_error("tree is empty");
        const Node* cur = root_.get();
        while (cur->left) cur = cur->left.get();
        return cur->value;
    }

    const T& max() const {
        if (!root_) throw std::underflow_error("tree is empty");
        const Node* cur = root_.get();
        while (cur->right) cur = cur->right.get();
        return cur->value;
    }

    bool erase(const T& value) {
        std::unique_ptr<Node>* link = &root_;
        while (*link) {
            if (cmp_(value, (*link)->value)) link = &(*link)->left;
            else if (cmp_((*link)->value, value)) link = &(*link)->right;
            else break;
        }
        if (!*link) return false;
        Node* node = link->get();
        if (!node->left) {
            *link = std::move(node->right);
        } else if (!node->right) {
            *link = std::move(node->left);
        } else {
            // Two children: move the in-order successor's value up and splice the successor out.
            std::unique_ptr<Node>* succ = &node->right;
            while ((*succ)->left) succ = &(*succ)->left;
            node->value = std::move((*succ)->value);
            std::unique_ptr<Node> dead = std::move(*succ);
            *succ = std::move(dead->right);
        }
        --size_;
        return true;
    }

    std::vector<T> in_order() const {
        std::vector<T> out;
        std::vector<const Node*> stack;
        const Node* cur = root_.get();
        while (cur || !stack.empty()) {
            while (cur) {
                stack.push_back(cur);
                cur = cur->left.get();
            }
            cur = stack.back();
            stack.pop_back();
            out.push_back(cur->value);
            cur = cur->right.get();
        }
        return out;
    }

    std::vector<T> pre_order() const {
        std::vector<T> out;
        std::vector<const Node*> stack;
        if (root_) stack.push_back(root_.get());
        while (!stack.empty()) {
            const Node* n = stack.back();
            stack.pop_back();
            out.push_back(n->value);
            if (n->right) stack.push_back(n->right.get());
            if (n->left) stack.push_back(n->left.get());
        }
        return out;
    }

    std::vector<T> post_order() const {
        std::vector<T> out;
        std::vector<const Node*> stack;
        if (root_) stack.push_back(root_.get());
        while (!stack.empty()) {
            const Node* n = stack.back();
            stack.pop_back();
            out.push_back(n->value);
            if (n->left) stack.push_back(n->left.get());
            if (n->right) stack.push_back(n->right.get());
        }
        std::reverse(out.begin(), out.end());
        return out;
    }

    std::vector<T> level_order() const {
        std::vector<T> out;
        std::vector<const Node*> queue;
        if (root_) queue.push_back(root_.get());
        for (std::size_t i = 0; i < queue.size(); ++i) {
            out.push_back(queue[i]->value);
            if (queue[i]->left) queue.push_back(queue[i]->left.get());
            if (queue[i]->right) queue.push_back(queue[i]->right.get());
        }
        return out;
    }

    // Number of nodes on the longest root-to-leaf path; 0 when empty.
    std::size_t height() const {
        std::size_t h = 0;
        std::vector<const Node*> level;
        if (root_) level.push_back(root_.get());
        while (!level.empty()) {
            ++h;
            std::vector<const Node*> next;
            for (const Node* n : level) {
                if (n->left) next.push_back(n->left.get());
                if (n->right) next.push_back(n->right.get());
            }
            level = std::move(next);
        }
        return h;
    }

    // True when the in-order sequence is strictly increasing and matches size().
    bool is_valid() const {
        std::vector<T> v = in_order();
        for (std::size_t i = 1; i < v.size(); ++i) {
            if (!cmp_(v[i - 1], v[i])) return false;
        }
        return v.size() == size_;
    }

private:
    std::unique_ptr<Node> root_;
    Compare cmp_;
    std::size_t size_ = 0;
};

template <typename T, typename Compare = std::less<T>>
class AVLTree {
    struct Node {
        T value;
        int height = 1;
        std::unique_ptr<Node> left, right;
        explicit Node(T v) : value(std::move(v)) {}
    };
    using Link = std::unique_ptr<Node>;

public:
    explicit AVLTree(Compare cmp = Compare()) : cmp_(std::move(cmp)) {}
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    std::size_t size() const { return size_; }
    int height() const { return h(root_); }

    bool insert(T value) {
        std::size_t before = size_;
        insert_at(root_, std::move(value));
        return size_ > before;
    }

    bool erase(const T& value) {
        std::size_t before = size_;
        erase_at(root_, value);
        return size_ < before;
    }

    bool contains(const T& value) const {
        const Node* cur = root_.get();
        while (cur) {
            if (cmp_(value, cur->value)) cur = cur->left.get();
            else if (cmp_(cur->value, value)) cur = cur->right.get();
            else return true;
        }
        return false;
    }

    std::vector<T> in_order() const {
        std::vector<T> out;
        walk(root_.get(), out);
        return out;
    }

    // Checks ordering, stored heights and that every balance factor is in [-1, 1].
    bool is_valid() const {
        if (check(root_.get()) < 0) return false;
        std::vector<T> v = in_order();
        for (std::size_t i = 1; i < v.size(); ++i) {
            if (!cmp_(v[i - 1], v[i])) return false;
        }
        return v.size() == size_;
    }

private:
    static int h(const Link& n) { return n ? n->height : 0; }
    static void update(Node& n) { n.height = 1 + std::max(h(n.left), h(n.right)); }

    static void rotate_right(Link& y) {
        Link x = std::move(y->left);
        y->left = std::move(x->right);
        update(*y);
        x->right = std::move(y);
        update(*x);
        y = std::move(x);
    }

    static void rotate_left(Link& x) {
        Link y = std::move(x->right);
        x->right = std::move(y->left);
        update(*x);
        y->left = std::move(x);
        update(*y);
        x = std::move(y);
    }

    static void rebalance(Link& n) {
        update(*n);
        int balance = h(n->left) - h(n->right);
        if (balance > 1) {
            if (h(n->left->left) < h(n->left->right)) rotate_left(n->left);
            rotate_right(n);
        } else if (balance < -1) {
            if (h(n->right->right) < h(n->right->left)) rotate_right(n->right);
            rotate_left(n);
        }
    }

    void insert_at(Link& n, T value) {
        if (!n) {
            n = std::make_unique<Node>(std::move(value));
            ++size_;
            return;
        }
        if (cmp_(value, n->value)) insert_at(n->left, std::move(value));
        else if (cmp_(n->value, value)) insert_at(n->right, std::move(value));
        else return;
        rebalance(n);
    }

    void erase_at(Link& n, const T& value) {
        if (!n) return;
        if (cmp_(value, n->value)) {
            erase_at(n->left, value);
        } else if (cmp_(n->value, value)) {
            erase_at(n->right, value);
        } else if (!n->left || !n->right) {
            Link child = n->left ? std::move(n->left) : std::move(n->right);
            n = std::move(child);
            --size_;
            if (!n) return;
        } else {
            Node* succ = n->right.get();
            while (succ->left) succ = succ->left.get();
            n->value = succ->value;
            erase_at(n->right, n->value);
        }
        rebalance(n);
    }

    static void walk(const Node* n, std::vector<T>& out) {
        if (!n) return;
        walk(n->left.get(), out);
        out.push_back(n->value);
        walk(n->right.get(), out);
    }

    static int check(const Node* n) {
        if (!n) return 0;
        int l = check(n->left.get());
        int r = check(n->right.get());
        if (l < 0 || r < 0 || std::abs(l - r) > 1) return -1;
        if (n->height != 1 + std::max(l, r)) return -1;
        return n->height;
    }

    Link root_;
    Compare cmp_;
    std::size_t size_ = 0;
};

}  // namespace dsa
