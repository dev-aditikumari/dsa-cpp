#pragma once
// Trie over std::string bytes. insert/contains/starts_with/erase O(L) for word length L; space O(total characters).
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dsa {

class Trie {
    struct Node {
        std::map<char, std::unique_ptr<Node>> children;
        bool terminal = false;
    };

public:
    Trie() : root_(std::make_unique<Node>()) {}
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    std::size_t size() const { return size_; }

    void insert(const std::string& word) {
        Node* node = root_.get();
        for (char ch : word) {
            auto& child = node->children[ch];
            if (!child) child = std::make_unique<Node>();
            node = child.get();
        }
        if (!node->terminal) {
            node->terminal = true;
            ++size_;
        }
    }

    bool contains(const std::string& word) const {
        const Node* n = find(word);
        return n && n->terminal;
    }

    bool starts_with(const std::string& prefix) const { return find(prefix) != nullptr; }

    // All stored words beginning with prefix, in lexicographic order.
    std::vector<std::string> with_prefix(const std::string& prefix) const {
        std::vector<std::string> out;
        const Node* start = find(prefix);
        if (start) collect(*start, prefix, out);
        return out;
    }

    bool erase(const std::string& word) {
        std::vector<std::pair<Node*, char>> path;
        Node* node = root_.get();
        for (char ch : word) {
            auto it = node->children.find(ch);
            if (it == node->children.end()) return false;
            path.emplace_back(node, ch);
            node = it->second.get();
        }
        if (!node->terminal) return false;
        node->terminal = false;
        --size_;
        // Remove nodes that no longer lead to any word.
        for (std::size_t i = path.size(); i-- > 0;) {
            const Node* child = path[i].first->children[path[i].second].get();
            if (child->terminal || !child->children.empty()) break;
            path[i].first->children.erase(path[i].second);
        }
        return true;
    }

private:
    const Node* find(const std::string& s) const {
        const Node* node = root_.get();
        for (char ch : s) {
            auto it = node->children.find(ch);
            if (it == node->children.end()) return nullptr;
            node = it->second.get();
        }
        return node;
    }

    static void collect(const Node& node, const std::string& acc, std::vector<std::string>& out) {
        if (node.terminal) out.push_back(acc);
        for (const auto& [ch, child] : node.children) collect(*child, acc + ch, out);
    }

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};

}  // namespace dsa
