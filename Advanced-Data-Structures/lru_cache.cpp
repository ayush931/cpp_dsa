/**
 * LRU (Least Recently Used) cache
 * get(key) -> return the value and mark it as recently used
 * put(key, value) -> insert or update, evict the least recently used key when
 * full
 *
 * Brute force -> vector of pairs with timestamps, O(n) per operation
 * Optimized -> hash map + doubly linked list, O(1) per operation
 */

#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;

class LRUCacheBrute {
    int capacity;
    vector<pair<int, int>> items;
    vector<int> lastUsed;
    int time;

   public:
    LRUCacheBrute(int cap) {
        capacity = max(0, cap);
        time = 0;
    }

    int get(int key) {
        time++;

        for (int i = 0; i < items.size(); i++) {
            if (items[i].first == key) {
                lastUsed[i] = time;
                return items[i].second;
            }
        }

        return -1;
    }

    void put(int key, int value) {
        if (capacity == 0) {
            return;
        }

        time++;

        for (int i = 0; i < items.size(); i++) {
            if (items[i].first == key) {
                items[i].second = value;
                lastUsed[i] = time;
                return;
            }
        }

        if (items.size() == capacity) {
            int lruIndex = 0;
            for (int i = 1; i < lastUsed.size(); i++) {
                if (lastUsed[i] < lastUsed[lruIndex]) {
                    lruIndex = i;
                }
            }
            items.erase(items.begin() + lruIndex);
            lastUsed.erase(lastUsed.begin() + lruIndex);
        }

        items.push_back({key, value});
        lastUsed.push_back(time);
    }
};

class LRUCache {
    int capacity;
    list<pair<int, int>> order;  // front = most recently used
    unordered_map<int, list<pair<int, int>>::iterator> cache;

   public:
    LRUCache(int cap) {
        capacity = max(0, cap);
    }

    int get(int key) {
        if (!cache.count(key)) {
            return -1;
        }

        auto it = cache[key];
        order.splice(order.begin(), order, it);
        return it->second;
    }

    void put(int key, int value) {
        if (capacity == 0) {
            return;
        }

        if (cache.count(key)) {
            auto it = cache[key];
            it->second = value;
            order.splice(order.begin(), order, it);
            return;
        }

        if (order.size() == capacity) {
            auto last = order.back();
            cache.erase(last.first);
            order.pop_back();
        }

        order.push_front({key, value});
        cache[key] = order.begin();
    }
};

int main() {
    LRUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl;  // 1
    cache.put(3, 3);               // evicts key 2
    cout << cache.get(2) << endl;  // -1
    cache.put(4, 4);               // evicts key 1
    cout << cache.get(1) << endl;  // -1
    cout << cache.get(3) << endl;  // 3
    cout << cache.get(4) << endl;  // 4

    LRUCache zeroCapacity(0);
    zeroCapacity.put(1, 1);               // ignored, not an error
    cout << zeroCapacity.get(1) << endl;  // -1

    return 0;
}

// Special cases: capacity 0, updating an existing key, get on a missing key,
// repeated puts of the same key
