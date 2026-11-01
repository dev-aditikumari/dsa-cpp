#pragma once
// LRU cache built from std::list (recency order) and std::unordered_map (key to list position).
// get/put O(1) average, space O(capacity).
#include <cstddef>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa {

template <typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) throw std::invalid_argument("capacity must be positive");
    }

    std::size_t size() const { return map_.size(); }
    std::size_t capacity() const { return capacity_; }

    // Returns a copy of the value and marks the key as most recently used.
    std::optional<V> get(const K& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return std::nullopt;
        order_.splice(order_.begin(), order_, it->second);
        return it->second->second;
    }

    void put(const K& key, V value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = std::move(value);
            order_.splice(order_.begin(), order_, it->second);
            return;
        }
        if (map_.size() == capacity_) {
            map_.erase(order_.back().first);
            order_.pop_back();
        }
        order_.emplace_front(key, std::move(value));
        map_[key] = order_.begin();
    }

    bool contains(const K& key) const { return map_.count(key) != 0; }

    // Keys from most to least recently used. Does not change recency.
    std::vector<K> keys() const {
        std::vector<K> out;
        for (const auto& entry : order_) out.push_back(entry.first);
        return out;
    }

private:
    std::size_t capacity_;
    std::list<std::pair<K, V>> order_;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map_;
};

}  // namespace dsa
