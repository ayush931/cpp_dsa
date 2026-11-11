/**
 * Low-Level Design (LLD): Multi-Level Cache System with Pluggable Eviction
 * Policies
 *
 * Requirements & Architectural Design:
 *   1. Hierarchical Multi-Level Cache (L1, L2, ... Ln):
 *      - L1 is fastest with lowest capacity.
 *      - L2 is larger but slightly slower.
 *      - Cache miss at Level i cascades read to Level i+1.
 *      - Cache hit at Level i back-populates all upper levels (1 to i-1).
 *
 *   2. Pluggable Eviction Policies (Strategy Pattern):
 *      - LRU (Least Recently Used): uses doubly-linked list + hash map.
 *      - LFU (Least Frequently Used): uses frequency buckets.
 *      - FIFO (First-In, First-Out): uses queue + hash map.
 *
 *   3. Write Policies:
 *      - Write-Through: updates written to all cache levels synchronously.
 *
 *   4. Metrics & Observability:
 *      - Tracks hits, misses, and hit ratio individually per cache level.
 */

#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// ============================================================================
// 1. Eviction Policy Interface (Strategy Pattern)
// ============================================================================

template <typename K>
class IEvictionPolicy {
   public:
    virtual ~IEvictionPolicy() = default;
    virtual void keyAccessed(const K &key) = 0;
    virtual K evictKey() = 0;
    virtual void removeKey(const K &key) = 0;
};

// LRU Eviction Policy Implementation
template <typename K>
class LRUEvictionPolicy : public IEvictionPolicy<K> {
   private:
    list<K> dll;
    unordered_map<K, typename list<K>::iterator> key_map;

   public:
    void keyAccessed(const K &key) override {
        auto it = key_map.find(key);
        if (it != key_map.end()) {
            dll.erase(it->second);
        }
        dll.push_front(key);
        key_map[key] = dll.begin();
    }

    K evictKey() override {
        if (dll.empty()) {
            throw runtime_error("Cache empty");
        }
        K last = dll.back();
        dll.pop_back();
        key_map.erase(last);
        return last;
    }

    void removeKey(const K &key) override {
        auto it = key_map.find(key);
        if (it != key_map.end()) {
            dll.erase(it->second);
            key_map.erase(it);
        }
    }
};

// ============================================================================
// 2. Single Cache Level
// ============================================================================

template <typename K, typename V>
class CacheLevel {
   private:
    int level_id;
    size_t capacity;
    unordered_map<K, V> storage;
    unique_ptr<IEvictionPolicy<K>> eviction_policy;

    long long hits = 0;
    long long misses = 0;

   public:
    CacheLevel(int id, size_t cap, unique_ptr<IEvictionPolicy<K>> policy)
        : level_id(id), capacity(cap), eviction_policy(std::move(policy)) {
        if (capacity == 0 || !eviction_policy) {
            throw invalid_argument(
                "positive capacity and eviction policy required");
        }
    }

    int getId() const {
        return level_id;
    }

    long long getHits() const {
        return hits;
    }

    long long getMisses() const {
        return misses;
    }

    bool get(const K &key, V &out_val) {
        auto it = storage.find(key);
        if (it != storage.end()) {
            hits++;
            eviction_policy->keyAccessed(key);
            out_val = it->second;
            return true;
        }
        misses++;
        return false;
    }

    void put(const K &key, const V &val) {
        auto it = storage.find(key);
        if (it != storage.end()) {
            it->second = val;
            eviction_policy->keyAccessed(key);
            return;
        }

        if (storage.size() >= capacity) {
            K evicted_key = eviction_policy->evictKey();
            storage.erase(evicted_key);
            cout << "    [L" << level_id << "] Evicted key: " << evicted_key
                 << endl;
        }

        storage[key] = val;
        eviction_policy->keyAccessed(key);
    }

    bool contains(const K &key) const {
        return storage.find(key) != storage.end();
    }
};

// ============================================================================
// 3. Multi-Level Cache Orchestrator
// ============================================================================

template <typename K, typename V>
class MultiLevelCache {
   private:
    vector<shared_ptr<CacheLevel<K, V>>> levels;

   public:
    void addLevel(shared_ptr<CacheLevel<K, V>> level) {
        levels.push_back(level);
    }

    // Read with backfill to all faster levels
    bool get(const K &key, V &out_val) {
        cout << "Fetching key: \"" << key << "\"..." << endl;
        for (size_t i = 0; i < levels.size(); ++i) {
            if (levels[i]->get(key, out_val)) {
                cout << "  -> HIT at L" << levels[i]->getId()
                     << "! Value: " << out_val << endl;
                // Backfill all higher levels (0 to i - 1)
                for (size_t j = 0; j < i; ++j) {
                    cout << "  -> Backfilling L" << levels[j]->getId()
                         << " with key: " << key << endl;
                    levels[j]->put(key, out_val);
                }
                return true;
            }
            cout << "  -> MISS at L" << levels[i]->getId() << endl;
        }
        return false;
    }

    // Write-Through: writes across all cache levels
    void put(const K &key, const V &val) {
        cout << "Writing key: \"" << key << "\" = \"" << val << "\"..." << endl;
        for (auto &lvl : levels) {
            lvl->put(key, val);
        }
    }

    void printStats() const {
        cout << "\n--- Cache Performance Statistics ---" << endl;
        for (const auto &lvl : levels) {
            long long total = lvl->getHits() + lvl->getMisses();
            double hitRate =
                (total > 0) ? (100.0 * lvl->getHits() / total) : 0.0;
            cout << "  Level " << lvl->getId() << " | Hits: " << lvl->getHits()
                 << " | Misses: " << lvl->getMisses()
                 << " | Hit Ratio: " << hitRate << "%" << endl;
        }
    }
};

int main() {
    cout << "=== Low-Level Design: Multi-Level Cache System Demo ===" << endl;

    MultiLevelCache<string, string> mlc;

    // L1 Cache: Capacity 2, LRU eviction
    mlc.addLevel(make_shared<CacheLevel<string, string>>(
        1, 2, make_unique<LRUEvictionPolicy<string>>()));

    // L2 Cache: Capacity 4, LRU eviction
    mlc.addLevel(make_shared<CacheLevel<string, string>>(
        2, 4, make_unique<LRUEvictionPolicy<string>>()));

    // 1. Initial Writes
    cout << "\n--- 1. Writing Entries ---" << endl;
    mlc.put("user_1", "Alice");
    mlc.put("user_2", "Bob");

    // 2. Reading from L1
    cout << "\n--- 2. Reading Existing Entry ---" << endl;
    string value;
    mlc.get("user_1", value);

    // 3. Overflowing L1 to trigger eviction
    cout << "\n--- 3. Triggering L1 Eviction ---" << endl;
    mlc.put(
        "user_3",
        "Charlie");  // L1 evicts user_2, L2 still holds user_1, user_2, user_3

    // 4. Reading user_2 (Misses in L1, Hits in L2, Backfills L1!)
    cout << "\n--- 4. Reading Evicted Key (Cascading Read & Backfill) ---"
         << endl;
    mlc.get("user_2", value);

    // 5. Querying Non-existent Key
    cout << "\n--- 5. Querying Non-Existent Key ---" << endl;
    bool found = mlc.get("user_999", value);
    cout << "Found? " << (found ? "YES" : "NO") << endl;

    // Print Metrics
    mlc.printStats();

    return 0;
}

/**
 * Key Architectural Properties:
 *   - Clean Separation of Concerns: Storage, Eviction, and Cascading are
 * decoupled.
 *   - Extensibility: New eviction policies (LFU, FIFO, ARC) can be added with
 * zero changes to CacheLevel.
 *   - Realistic Multi-Tier Behavior: Demonstrates true L1/L2 hierarchical
 * lookup and backfill.
 */
