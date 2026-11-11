/**
 * LRU + TTL Cache
 *
 * Two independent policies bound a cache:
 *
 *   TTL -> an entry is invalid after `now + ttl`, even if it is popular
 *   LRU -> when the cache is full, evict the least recently used entry
 *
 * A TTL-only cache (see 10_ttl_cache.cpp) evicts the entry that expires first.
 * That is correct for freshness but wrong for memory pressure: a hot entry with
 * a long TTL can be evicted while a cold entry with a short TTL survives. An
 * LRU-only cache has no notion of staleness. This file combines both:
 *
 *   - a hash map gives O(1) key lookup
 *   - a doubly linked list gives O(1) move-to-front and evict-from-back
 *   - a min-heap of (expiry, key) gives active expiration
 *
 * Invariants:
 *   - `order` contains exactly the keys in `entries`, once each
 *   - every entry's `position` points at its node in `order`
 *   - the heap may contain stale records; a record is valid only when the
 *     key exists and its current expiry equals the heap record's expiry
 *   - `entries.size() <= capacity` after every operation
 *   - `get` moves a live key to the front; `put` does the same
 *
 * The heap is an optimization for active expiration. Lazy expiration alone
 * (checking only on `get`) leaks memory when a key is never read again. The
 * `purgeExpired` pass reclaims those entries whenever the cache is touched.
 *
 * This is a single-threaded model. A concurrent cache needs a mutex around the
 * map, list, and heap, or a sharded design with one lock per shard. A
 * production cache also needs a maximum memory size, not just a key count.
 */

#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class LRUTTLCache {
    struct Entry {
        string value;
        long long expiry;
        list<int>::iterator position;
    };

    int capacity;
    unordered_map<int, Entry> entries;
    list<int> order;  // front = most recently used, back = least recently used

    // (expiry, key), smallest expiry first.
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>>
        expiryHeap;

    bool eraseKey(int key) {
        auto entry = entries.find(key);
        if (entry == entries.end()) {
            return false;
        }

        order.erase(entry->second.position);
        entries.erase(entry);
        return true;
    }

    void purgeExpired(long long now) {
        while (!expiryHeap.empty()) {
            long long expiry = expiryHeap.top().first;
            int key = expiryHeap.top().second;

            auto entry = entries.find(key);

            if (entry == entries.end() || entry->second.expiry != expiry) {
                expiryHeap.pop();  // stale heap record
                continue;
            }

            if (expiry > now) {
                break;  // the earliest live entry is still valid
            }

            eraseKey(key);
            expiryHeap.pop();
        }
    }

   public:
    explicit LRUTTLCache(int capacity) : capacity(capacity) {}

    // Inserts or updates a key. A negative TTL is treated as immediate expiry.
    bool put(int key, const string &value, long long ttl, long long now) {
        if (capacity <= 0) {
            return false;
        }
        if (ttl < 0) {
            ttl = 0;
        }

        purgeExpired(now);

        auto entry = entries.find(key);

        if (entry != entries.end()) {
            entry->second.value = value;
            entry->second.expiry = now + ttl;
            order.splice(order.begin(), order, entry->second.position);
            expiryHeap.push({now + ttl, key});
            return true;
        }

        if (static_cast<int>(entries.size()) >= capacity) {
            eraseKey(order.back());  // evict the least recently used key
        }

        order.push_front(key);
        entries[key] = {value, now + ttl, order.begin()};
        expiryHeap.push({now + ttl, key});

        return true;
    }

    bool get(int key, long long now, string &result) {
        purgeExpired(now);

        auto entry = entries.find(key);
        if (entry == entries.end()) {
            return false;
        }

        if (entry->second.expiry <= now) {
            eraseKey(key);
            return false;
        }

        order.splice(order.begin(), order, entry->second.position);
        result = entry->second.value;
        return true;
    }

    bool contains(int key, long long now) {
        purgeExpired(now);
        return entries.count(key) > 0;
    }

    int size() const {
        return static_cast<int>(entries.size());
    }

    int capacityValue() const {
        return capacity;
    }

    // Most-recent-first key order, for tests and debugging.
    vector<int> keys() const {
        return vector<int>(order.begin(), order.end());
    }
};

void printGet(LRUTTLCache &cache, int key, long long now) {
    string value;

    if (cache.get(key, now, value)) {
        cout << key << " -> " << value << endl;
    } else {
        cout << key << " -> MISS" << endl;
    }
}

void printKeys(LRUTTLCache &cache) {
    cout << "[";

    vector<int> keys = cache.keys();

    for (size_t i = 0; i < keys.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << keys[i];
    }

    cout << "]" << endl;
}

int main() {
    LRUTTLCache cache(2);

    cache.put(1, "one", 5, 0);   // expires at 5
    cache.put(2, "two", 10, 0);  // expires at 10

    printGet(cache, 1, 3);  // 1 -> one
    printKeys(cache);       // [1, 2]  (1 is now most recently used)

    // Full, so the least recently used key (2) is evicted.
    cache.put(3, "three", 20, 4);
    printGet(cache, 2, 4);  // 2 -> MISS
    printGet(cache, 1, 4);  // 1 -> one
    printKeys(cache);       // [1, 3]

    // At t=5, key 1 expires; the purge runs before the new insertion.
    cache.put(4, "four", 2, 5);  // expires at 7
    printKeys(cache);            // [4, 3]
    printGet(cache, 3, 6);       // 3 -> three
    printKeys(cache);            // [3, 4]

    // Key 4 expires at 7; key 5 takes its place.
    cache.put(5, "five", 10, 7);  // expires at 17
    printKeys(cache);             // [5, 3]
    printGet(cache, 3, 7);        // 3 -> three
    printKeys(cache);             // [3, 5]
    printGet(cache, 4, 7);        // 4 -> MISS

    // Key 6 replaces the least recently used key (5), not the hot key 3.
    cache.put(6, "six", 1, 8);  // expires at 9
    printKeys(cache);           // [6, 3]
    printGet(cache, 5, 8);      // 5 -> MISS
    printGet(cache, 3, 8);      // 3 -> three
    printKeys(cache);           // [3, 6]

    // Key 6 expires at 9, so the purge removes it before key 7 is inserted.
    cache.put(7, "seven", 100, 9);
    printKeys(cache);       // [7, 3]
    printGet(cache, 3, 9);  // 3 -> three
    printGet(cache, 6, 9);  // 6 -> MISS (expired at 9)
    printGet(cache, 7, 9);  // 7 -> seven
    printKeys(cache);       // [7, 3]

    // TTL 0 is immediately expired; key 8 evicts the least recently used live
    // key (3) first, then the next lookup removes key 8 itself.
    cache.put(8, "eight", 0, 10);
    printGet(cache, 8, 10);        // 8 -> MISS
    cout << cache.size() << endl;  // 1

    // Updating an existing key refreshes its expiry and moves it to the front.
    cache.put(7, "SEVEN", 3, 11);  // expires at 14
    printGet(cache, 7, 13);        // 7 -> SEVEN
    printGet(cache, 7, 14);        // 7 -> MISS

    // Capacity 0 rejects every write but never crashes.
    LRUTTLCache disabled(0);
    cout << disabled.put(1, "x", 10, 0) << " " << disabled.size() << endl;
    // 0 0

    return 0;
}

/**
 * Time complexity:
 *   get -> O(1) average for the map and list, plus amortized O(log n) for the
 *   lazy heap cleanup
 *   put -> O(1) average, plus O(log n) heap push and amortized cleanup
 *   purgeExpired -> each stale heap record is popped at most once, so the total
 *   work across all calls is O(number of heap records)
 * Space complexity -> O(capacity) for the map and list, plus O(number of puts)
 * stale heap records between cleanups
 *
 * Special cases:
 *   - capacity 0 -> no entries can be stored
 *   - TTL 0 -> the entry expires immediately
 *   - a negative TTL -> treated as 0
 *   - updating a key refreshes its TTL and makes it most recently used
 *   - an expired key is removed on access or during a later purge
 *   - a full cache evicts the least recently used live key, not an expired one
 *     (expired keys are purged first)
 *   - repeated updates create stale heap records; they are discarded lazily
 *   - a key that is never read again is reclaimed by the next purge
 *   - clock skew between machines is a correctness issue; use a monotonic clock
 *     per cache node and do not compare absolute times across nodes
 *
 * Why both policies: TTL alone cannot bound memory when every entry has a long
 * TTL. LRU alone serves stale data forever when a key is hot but the source has
 * changed. Combining them gives a bounded, fresh cache. The cost is one extra
 * pointer per entry and a heap record per update.
 *
 * Extensions:
 *   - per-entry size and a byte-based capacity
 *   - segmented LRU (probation/protected) to resist scan pollution
 *   - a background sweeper instead of lazy purge on access
 *   - metrics for hit rate, evictions, expirations, and stale heap records
 *   - sharding by key with consistent hashing for a distributed cache
 */
