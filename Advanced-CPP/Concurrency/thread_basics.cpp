/**
 * std::thread -> a single thread of execution
 * join() -> wait for the thread to finish
 * detach() -> let the thread run independently (dangerous: it may outlive
 *             objects it references, and the program may end before it
 * finishes) hardware_concurrency() -> hint for the number of worker threads
 *
 * Every joinable thread must be joined or detached before its destructor runs,
 * otherwise std::terminate is called
 *
 * Thread creation cost is high, so create a few long-lived workers instead of
 * one thread per task (see the thread pool example)
 */

#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

void worker(int id, int iterations) {
    for (int i = 0; i < iterations; i++) {
        cout << "worker " << id << " step " << i << endl;
    }
}

int main() {
    cout << "hardware threads: " << thread::hardware_concurrency() << endl;

    thread first(worker, 1, 2);
    thread second(worker, 2, 2);

    first.join();
    second.join();

    // run many small tasks
    vector<thread> threads;

    for (int i = 0; i < 4; i++) {
        threads.push_back(thread([i]() {
            cout << "task " << i << " on thread " << this_thread::get_id()
                 << endl;
        }));
    }

    for (thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // a lambda capturing by reference must not outlive the referenced object
    int value = 42;
    thread safe([&value]() { cout << "captured: " << value << endl; });
    safe.join();

    return 0;
}

// Special cases: thread function throws (call std::terminate unless caught
// inside the thread), join twice (undefined), join after detach (invalid),
// creating more threads than cores (context switching overhead),
// main returning while detached threads still run (undefined behavior)
// Prefer std::jthread (C++20) for automatic join and stop tokens
