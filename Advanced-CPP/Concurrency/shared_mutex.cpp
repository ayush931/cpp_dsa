/**
 * std::shared_mutex (C++17) -> reader-writer lock
 *
 * Many threads may read at the same time, but a writer needs exclusive access.
 *   - std::shared_lock  -> shared ownership (reader)
 *   - std::unique_lock  -> exclusive ownership (writer)
 *
 * Use it for read-heavy data: caches, configuration, routing tables, symbol
 * tables. For write-heavy or very short critical sections, a plain std::mutex
 * is often faster because the reader-writer bookkeeping is not free.
 *
 * Fairness is implementation-defined. Some implementations let new readers
 * enter while a writer waits, which can starve the writer. If that matters, use
 * a fair lock or a queue-based design.
 *
 * There is no upgrade operation: you cannot turn a shared_lock into a
 * unique_lock. Release the read lock, then acquire the write lock.
 */

#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>
using namespace std;

class Cache {
    mutable shared_mutex mutex_;  // mutable: readers may lock in a const method
    map<int, string> data;

   public:
    void put(int key, const string &value) {
        unique_lock<shared_mutex> lock(mutex_);  // one writer at a time
        data[key] = value;
    }

    bool get(int key, string &result) const {
        shared_lock<shared_mutex> lock(mutex_);  // many readers at a time
        auto found = data.find(key);

        if (found == data.end()) {
            return false;
        }

        result = found->second;
        return true;
    }

    size_t size() const {
        shared_lock<shared_mutex> lock(mutex_);
        return data.size();
    }
};

int main() {
    Cache cache;
    atomic<int> successfulReads{0};
    vector<thread> threads;

    // Fill the cache before the concurrent phase so every read is guaranteed to
    // hit. Without this, a reader could run before any writer and the test
    // would be timing-dependent.
    for (int key = 0; key < 100; key++) {
        cache.put(key, "initial-" + to_string(key));
    }

    // Writers update keys that already exist.
    for (int writer = 0; writer < 2; writer++) {
        threads.emplace_back([&cache, writer]() {
            for (int key = 0; key < 50; key++) {
                cache.put(writer * 50 + key, "value-" + to_string(key));
            }
        });
    }

    // Readers run at the same time as the writers. Every key is present, so all
    // 4 * 100 reads succeed.
    for (int reader = 0; reader < 4; reader++) {
        threads.emplace_back([&cache, &successfulReads]() {
            for (int key = 0; key < 100; key++) {
                string value;
                if (cache.get(key, value)) {
                    successfulReads++;
                }
            }
        });
    }

    for (thread &worker : threads) {
        worker.join();
    }

    cout << cache.size() << endl;            // 100
    cout << successfulReads.load() << endl;  // 400

    // Sequential use is also safe.
    cache.put(999, "last");
    string value;
    cout << cache.get(999, value) << " " << value << endl;  // 1 last

    return 0;
}

/**
 * Time complexity -> O(log n) per map operation plus lock overhead
 * Space complexity -> O(n) for the cached data
 *
 * Special cases:
 *   - a reader that tries to take the write lock while holding the read lock
 *     deadlocks, because the write lock waits for all readers
 *   - recursive shared locking is not guaranteed to work
 *   - writer starvation under a continuous stream of readers
 *   - a const method must mark the mutex `mutable` to lock it
 *   - `shared_mutex` is not copyable or movable
 *
 * Alternatives:
 *   - std::mutex for simple mutual exclusion
 *   - a sharded cache: hash the key to one of N independent mutexes
 *   - RCU (read-copy-update) for read-mostly data with rare updates
 *   - a lock-free snapshot: build a new immutable map and swap a pointer
 */
