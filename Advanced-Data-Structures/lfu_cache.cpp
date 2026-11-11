/**
 * LFU Cache (Least Frequently Used)
 *
 * Design a cache with a fixed capacity that supports:
 *   - get(key): return the value, or -1 if absent
 *   - put(key, value): insert or update
 *
 * When the cache is full, evict the key with the smallest access frequency.
 * If several keys have the same frequency, evict the least recently used one.
 *
 * Both operations must run in O(1) average time.
 *
 * Brute force -> keep a vector of entries and scan for the victim -> O(n)
 * Optimized -> frequency buckets + a list per bucket -> O(1)
 */

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;

class LFUCacheBrute {
    struct Entry {
        int key;
        int value;
        int frequency;
        int lastUsed;
    };

    int capacity;
    int clock;
    vector<Entry> entries;

    Entry *findEntry(int key) {
        for (Entry &entry : entries) {
            if (entry.key == key) {
                return &entry;
            }
        }

        return NULL;
    }

   public:
    LFUCacheBrute(int cap) {
        capacity = cap;
        clock = 0;
    }

    int get(int key) {
        Entry *entry = findEntry(key);

        if (entry == NULL) {
            return -1;
        }

        entry->frequency++;
        entry->lastUsed = clock++;

        return entry->value;
    }

    void put(int key, int value) {
        if (capacity <= 0) {
            return;
        }

        Entry *entry = findEntry(key);

        if (entry != NULL) {
            entry->value = value;
            entry->frequency++;
            entry->lastUsed = clock++;
            return;
        }

        if (entries.size() == capacity) {
            int victim = 0;

            for (int i = 1; i < entries.size(); i++) {
                if (entries[i].frequency < entries[victim].frequency ||
                    (entries[i].frequency == entries[victim].frequency &&
                     entries[i].lastUsed < entries[victim].lastUsed)) {
                    victim = i;
                }
            }

            entries.erase(entries.begin() + victim);
        }

        entries.push_back({key, value, 1, clock++});
    }
};

class LFUCache {
    struct Node {
        int key;
        int value;
        int frequency;
    };

    int capacity;
    int minFrequency;
    unordered_map<int, list<Node>::iterator> positions;
    unordered_map<int, list<Node>> frequencyLists;

    void increaseFrequency(int key) {
        auto positionIt = positions.find(key);
        auto nodeIt = positionIt->second;

        int oldFrequency = nodeIt->frequency;
        Node node = *nodeIt;

        frequencyLists[oldFrequency].erase(nodeIt);

        if (frequencyLists[oldFrequency].empty()) {
            frequencyLists.erase(oldFrequency);

            if (minFrequency == oldFrequency) {
                minFrequency++;
            }
        }

        node.frequency = oldFrequency + 1;
        frequencyLists[node.frequency].push_front(node);
        positions[key] = frequencyLists[node.frequency].begin();
    }

    void evict() {
        auto listIt = frequencyLists.find(minFrequency);
        Node victim = listIt->second.back();

        positions.erase(victim.key);
        listIt->second.pop_back();

        if (listIt->second.empty()) {
            frequencyLists.erase(listIt);
        }
    }

   public:
    LFUCache(int cap) {
        capacity = cap;
        minFrequency = 0;
    }

    int get(int key) {
        if (positions.find(key) == positions.end()) {
            return -1;
        }

        int value = positions[key]->value;
        increaseFrequency(key);

        return value;
    }

    void put(int key, int value) {
        if (capacity <= 0) {
            return;
        }

        if (positions.find(key) != positions.end()) {
            positions[key]->value = value;
            increaseFrequency(key);
            return;
        }

        if (positions.size() == capacity) {
            evict();
        }

        frequencyLists[1].push_front({key, value, 1});
        positions[key] = frequencyLists[1].begin();
        minFrequency = 1;
    }
};

int main() {
    LFUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);

    cout << cache.get(1) << endl;  // 1 (frequency of 1 becomes 2)
    cache.put(3, 3);               // evicts 2 (frequency 1)

    cout << cache.get(2) << endl;  // -1
    cout << cache.get(3) << endl;  // 3
    cout << cache.get(1) << endl;  // 1

    cache.put(4, 4);  // key 3 has frequency 2, key 1 has 3; evict 3
    cout << cache.get(1) << endl;  // 1
    cout << cache.get(3) << endl;  // -1
    cout << cache.get(4) << endl;  // 4

    LFUCacheBrute brute(2);
    brute.put(1, 1);
    brute.put(2, 2);
    cout << brute.get(1) << endl;  // 1
    brute.put(3, 3);
    cout << brute.get(2) << endl;  // -1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) per get/put (scan for the victim)
 *   optimized -> O(1) average per get/put
 * Space complexity -> O(capacity)
 *
 * Special cases: capacity 0, get on an empty cache, updating an existing key,
 * ties in frequency (LRU among the minimum frequency), repeated gets of the
 * same key, capacity 1.
 */
