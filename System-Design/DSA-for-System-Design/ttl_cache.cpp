/**
 * TTL Cache (Time To Live)
 *
 * A TTL cache stores key -> value pairs where every entry expires after a
 * fixed lifetime. It is the data structure behind session stores, DNS caches,
 * CDN edge caches, and Redis `SET key value EX seconds`.
 *
 * Operations:
 *   - put(key, value, ttl): insert or replace, with an absolute expiry time
 *   - get(key, now): return the value, or a miss if the entry expired
 *
 * Brute force:
 *   - keep a vector of entries
 *   - get/put scan the vector -> O(n)
 *   - eviction scans for an expired entry, then for the earliest expiry -> O(n)
 *
 * Optimized:
 *   - hash map key -> {value, expiry} -> O(1) lookup
 *   - min-heap of (expiry, key) -> O(log n) to find the next entry to expire
 *   - stale heap entries are removed lazily: when the heap top does not match
 *     the current expiry in the map, it is discarded
 *
 * Expiration policies:
 *   - lazy: check the expiry on every get
 *   - active: a background thread pops expired entries from the heap
 *   - both together are used in production so memory is reclaimed even when a
 *     key is never read again
 */

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class TTLCacheBrute {
    struct Entry {
        int key;
        string value;
        long long expiry;
    };

    int capacity;
    vector<Entry> entries;

    int findIndex(int key) {
        for (int i = 0; i < entries.size(); i++) {
            if (entries[i].key == key) {
                return i;
            }
        }

        return -1;
    }

   public:
    TTLCacheBrute(int cap) {
        capacity = cap;
    }

    void put(int key, string value, long long ttl, long long now) {
        int index = findIndex(key);

        if (index != -1) {
            entries[index].value = value;
            entries[index].expiry = now + ttl;
            return;
        }

        if (entries.size() == capacity) {
            // Prefer an already expired entry.
            for (int i = 0; i < entries.size(); i++) {
                if (entries[i].expiry <= now) {
                    entries.erase(entries.begin() + i);
                    break;
                }
            }
        }

        if (entries.size() == capacity) {
            // Otherwise evict the entry that expires first.
            int victim = 0;

            for (int i = 1; i < entries.size(); i++) {
                if (entries[i].expiry < entries[victim].expiry) {
                    victim = i;
                }
            }

            entries.erase(entries.begin() + victim);
        }

        entries.push_back({key, value, now + ttl});
    }

    bool get(int key, long long now, string &result) {
        int index = findIndex(key);

        if (index == -1) {
            return false;
        }

        if (entries[index].expiry <= now) {
            entries.erase(entries.begin() + index);
            return false;
        }

        result = entries[index].value;
        return true;
    }

    int size() {
        return entries.size();
    }
};

class TTLCache {
    struct Entry {
        string value;
        long long expiry;
    };

    int capacity;
    unordered_map<int, Entry> entries;

    // (expiry, key), smallest expiry first.
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>>
        expiryHeap;

    // Discard heap entries that are stale or already gone.
    void removeStale(long long now) {
        while (!expiryHeap.empty()) {
            long long expiry = expiryHeap.top().first;
            int key = expiryHeap.top().second;

            auto found = entries.find(key);

            if (found == entries.end() || found->second.expiry != expiry) {
                expiryHeap.pop();  // stale record
                continue;
            }

            if (expiry > now) {
                break;  // the earliest valid entry is still alive
            }

            entries.erase(found);
            expiryHeap.pop();
        }
    }

   public:
    TTLCache(int cap) {
        capacity = cap;
    }

    void put(int key, string value, long long ttl, long long now) {
        if (capacity <= 0) {
            return;
        }

        removeStale(now);

        auto found = entries.find(key);

        if (found != entries.end()) {
            found->second.value = value;
            found->second.expiry = now + ttl;
            expiryHeap.push({now + ttl, key});
            return;
        }

        if (entries.size() == capacity) {
            // The heap top is the entry that expires first.
            while (!expiryHeap.empty()) {
                long long expiry = expiryHeap.top().first;
                int victim = expiryHeap.top().second;
                expiryHeap.pop();

                auto victimEntry = entries.find(victim);

                if (victimEntry != entries.end() &&
                    victimEntry->second.expiry == expiry) {
                    entries.erase(victimEntry);
                    break;
                }
            }
        }

        entries[key] = {value, now + ttl};
        expiryHeap.push({now + ttl, key});
    }

    bool get(int key, long long now, string &result) {
        removeStale(now);

        auto found = entries.find(key);

        if (found == entries.end()) {
            return false;
        }

        if (found->second.expiry <= now) {
            entries.erase(found);
            return false;
        }

        result = found->second.value;
        return true;
    }

    int size() {
        return entries.size();
    }
};

void printGet(TTLCache &cache, int key, long long now) {
    string value;

    if (cache.get(key, now, value)) {
        cout << key << " -> " << value << endl;
    } else {
        cout << key << " -> MISS" << endl;
    }
}

int main() {
    TTLCache cache(2);

    cache.put(1, "one", 5, 0);   // expires at 5
    cache.put(2, "two", 10, 0);  // expires at 10

    printGet(cache, 1, 3);  // one
    printGet(cache, 1, 6);  // MISS (expired)

    cache.put(3, "three", 4, 6);  // expires at 10, cache has 2 and 3
    printGet(cache, 2, 6);        // two
    printGet(cache, 3, 9);        // three

    cache.put(4, "four", 20, 11);  // 2 and 3 are both expired at 11
    printGet(cache, 2, 11);        // MISS
    printGet(cache, 3, 11);        // MISS
    printGet(cache, 4, 11);        // four

    cout << cache.size() << endl;  // 1

    // Updating a key refreshes its expiry.
    cache.put(4, "FOUR", 5, 12);  // expires at 17
    printGet(cache, 4, 16);       // FOUR
    printGet(cache, 4, 18);       // MISS

    // The brute-force version behaves the same way.
    TTLCacheBrute brute(2);
    brute.put(1, "one", 5, 0);
    brute.put(2, "two", 10, 0);

    string value;
    cout << brute.get(1, 3, value) << " " << value << endl;  // 1 one
    cout << brute.get(1, 6, value) << endl;                  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) per get/put
 *   optimized -> O(1) average get, O(log n) put, amortized O(log n) cleanup
 * Space complexity -> O(capacity) for the map and the heap
 *
 * Special cases: capacity 0, TTL 0 (expires immediately), a negative TTL,
 * updating an existing key, a key that is never read again (active expiration
 * is needed to reclaim it), many stale heap entries after repeated updates,
 * clock skew between machines, and a monotonic clock that can jump backward.
 *
 * Production notes:
 *   - use a monotonic clock for durations, not wall-clock time
 *   - shard the cache across nodes and use consistent hashing for placement
 *   - add an LRU list if the cache must also bound memory when nothing expires
 *   - a background sweeper avoids unbounded growth from unread keys
 */
