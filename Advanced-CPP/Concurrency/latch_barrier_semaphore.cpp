/**
 * C++20 synchronization primitives
 *
 * std::latch
 *   - a one-shot countdown
 *   - count_down() decrements, wait() blocks until the count reaches zero
 *   - arrive_and_wait() decrements and waits
 *   - once zero, it stays zero; it cannot be reused
 *
 * std::barrier
 *   - a reusable phase barrier
 *   - every participating thread calls arrive_and_wait()
 *   - the last arrival runs an optional completion function, then all threads
 *     continue with the next phase
 *
 * std::counting_semaphore<Max>
 *   - a counter of available permits
 *   - acquire() waits for a permit, release() returns one
 *   - binary_semaphore is an alias for counting_semaphore<1>
 *
 * These are higher-level than a mutex + condition_variable and less error-prone
 * for common coordination patterns.
 */

#include <atomic>
#include <barrier>
#include <chrono>
#include <iostream>
#include <latch>
#include <semaphore>
#include <thread>
#include <vector>
using namespace std;

int main() {
    const int workers = 4;

    // 1. Latch: hold every worker until the starting signal.
    vector<int> results(workers, 0);
    latch startSignal(1);
    vector<jthread> threads;

    for (int i = 0; i < workers; i++) {
        threads.emplace_back([&results, &startSignal, i]() {
            startSignal.wait();  // block until main counts down
            results[i] = i * i;
        });
    }

    startSignal.count_down();  // release all workers at once
    threads.clear();           // jthread joins in its destructor

    for (int value : results) {
        cout << value << " ";
    }
    cout << endl;
    // 0 1 4 9

    // 2. Barrier: two phases, every worker must finish phase 1 before any
    // worker starts phase 2.
    vector<int> phaseOne(workers, 0);
    vector<int> phaseTwo(workers, 0);
    barrier syncPoint(workers);

    vector<jthread> barrierThreads;

    for (int i = 0; i < workers; i++) {
        barrierThreads.emplace_back([&, i]() {
            phaseOne[i] = i + 1;
            syncPoint.arrive_and_wait();  // phase 1 complete

            phaseTwo[i] = phaseOne[i] * 10;
            syncPoint.arrive_and_wait();  // phase 2 complete
        });
    }

    barrierThreads.clear();

    for (int value : phaseTwo) {
        cout << value << " ";
    }
    cout << endl;
    // 10 20 30 40

    // 3. Semaphore: at most two workers inside the critical section at once.
    counting_semaphore<2> slots(2);
    atomic<int> active{0};
    atomic<int> maximumActive{0};

    vector<jthread> limited;

    for (int i = 0; i < 6; i++) {
        limited.emplace_back([&]() {
            slots.acquire();

            int now = ++active;
            int previous = maximumActive.load();

            while (now > previous &&
                   !maximumActive.compare_exchange_weak(previous, now)) {
            }

            this_thread::sleep_for(chrono::milliseconds(20));

            active--;
            slots.release();
        });
    }

    limited.clear();

    cout << maximumActive.load() << endl;  // 2

    return 0;
}

/**
 * Time complexity -> O(1) per synchronization operation, plus any waiting
 * Space complexity -> O(1) per primitive, plus the waiter bookkeeping
 *
 * Special cases:
 *   - a latch count that is never reached -> waiters block forever
 *   - more arrivals than the barrier expects is undefined
 *   - a semaphore released more times than its maximum is undefined
 *   - a semaphore with zero permits blocks acquire() until release()
 *   - destroying a primitive while threads are waiting is undefined
 *
 * When to use which:
 *   - latch -> start gates, one-time initialization, waiting for N tasks
 *   - barrier -> iterative algorithms where every worker must finish a round
 *   - semaphore -> limiting concurrency (connection pools, bounded queues)
 *   - mutex + condition_variable -> complex predicates and state machines
 */
