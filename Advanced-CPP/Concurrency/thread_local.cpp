/**
 * thread_local storage
 *
 * A `thread_local` variable has one instance per thread. Each thread sees its
 * own copy, initialized before first use in that thread and destroyed when the
 * thread exits.
 *
 * Uses:
 *   - per-thread caches and scratch buffers (no locking)
 *   - error state such as errno
 *   - random number engines (avoid sharing a generator between threads)
 *   - per-thread allocators or arenas
 *   - recursion depth / request context
 *
 * `thread_local` is not a synchronization tool: two threads cannot see each
 * other's copy. Use std::atomic or a mutex for shared state.
 *
 * Storage duration:
 *   - global/static thread_local -> one per thread for the program lifetime
 *   - function-local thread_local -> initialized on the first call in that
 *     thread, destroyed at thread exit
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

thread_local int perThreadCounter = 0;
atomic<int> sharedCounter{0};

int nextRequestId() {
    static thread_local int id = 0;  // one counter per thread
    return ++id;
}

void worker(int threadNumber, vector<int> &lastValue) {
    for (int i = 0; i < 5; i++) {
        perThreadCounter++;
        sharedCounter++;
        lastValue[threadNumber] = nextRequestId();
    }
}

int main() {
    const int threadCount = 3;

    vector<int> lastValue(threadCount, 0);
    vector<thread> threads;

    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(worker, i, ref(lastValue));
    }

    for (thread &thread : threads) {
        thread.join();
    }

    // The shared counter saw every increment.
    cout << sharedCounter.load() << endl;  // 15

    // Each thread's local counter reached 5, but the main thread's copy is
    // still 0 because it never ran the worker loop.
    cout << perThreadCounter << endl;  // 0

    // Every thread's request id reached 5 independently.
    for (int value : lastValue) {
        cout << value << " ";
    }
    cout << endl;
    // 5 5 5

    return 0;
}

/**
 * Time complexity -> O(1) per access, usually a TLS register or a table lookup
 * Space complexity -> O(threads * size of the variable)
 *
 * Special cases:
 *   - a thread_local object with a non-trivial destructor is destroyed at
 *     thread exit; order relative to other thread_locals is unspecified
 *   - a thread_local in a dynamically loaded library may be destroyed when the
 *     library unloads
 *   - accessing a thread_local during static destruction can be unsafe
 *   - thread_local does not make a shared object safe; it only gives each
 *     thread its own object
 *   - excessive thread_local storage increases per-thread memory, which
 *     matters for large thread pools
 *
 * A thread_local is often faster than a mutex-protected global because there
 * is no contention. The tradeoff is memory: every thread pays for its copy.
 */
