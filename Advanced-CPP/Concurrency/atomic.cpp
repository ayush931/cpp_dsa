/**
 * std::atomic -> operations that are indivisible, no mutex required
 * fetch_add, fetch_sub, exchange, compare_exchange_weak / strong
 *
 * Memory orders (from weakest to strongest):
 * relaxed -> only atomicity, no ordering guarantees
 * acquire -> no reads/writes after it can move before it
 * release -> no reads/writes before it can move after it
 * acq_rel -> acquire + release
 * seq_cst -> sequential consistency, the default and the easiest to reason
 * about
 *
 * compare_exchange -> CAS (compare and swap), the base of lock-free algorithms
 * It takes an expected value by reference and updates it on failure
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

atomic<int> counter(0);
atomic<bool> ready(false);
int payload = 0;

void relaxedIncrement() {
    for (int i = 0; i < 100000; i++) {
        counter.fetch_add(1, memory_order_relaxed);
    }
}

void producer() {
    payload = 42;
    ready.store(true, memory_order_release);  // publish payload
}

void consumer() {
    while (!ready.load(memory_order_acquire)) {  // wait for payload
    }
    cout << "payload: " << payload << endl;
}

int main() {
    thread a(relaxedIncrement);
    thread b(relaxedIncrement);

    a.join();
    b.join();

    cout << counter.load() << endl;  // 200000

    thread writer(producer);
    thread reader(consumer);
    writer.join();
    reader.join();

    // lock-free counter with CAS
    atomic<int> value(0);

    auto casIncrement = [&value]() {
        int expected = value.load();

        while (!value.compare_exchange_weak(expected, expected + 1)) {
            // expected is updated with the current value on failure
        }
    };

    thread c(casIncrement);
    thread d(casIncrement);
    c.join();
    d.join();

    cout << value.load() << endl;  // 2

    // std::atomic_flag is the only type guaranteed to be lock-free
    atomic_flag flag = ATOMIC_FLAG_INIT;
    cout << flag.test_and_set() << endl;  // 0, first set
    cout << flag.test_and_set() << endl;  // 1, already set
    flag.clear();

    return 0;
}

// Special cases: atomic<double> has no fetch_add before C++20, use CAS;
// std::atomic<T> for large T may use an internal lock (check is_lock_free);
// ABA problem in lock-free stacks; false sharing of atomics on one cache line
// seq_cst is the default; only weaken it after proving the ordering is enough
