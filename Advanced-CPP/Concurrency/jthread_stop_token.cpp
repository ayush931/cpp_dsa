/**
 * std::jthread and cooperative cancellation (C++20)
 *
 * std::jthread is a thread that joins automatically in its destructor. It also
 * carries a std::stop_source and passes a std::stop_token to the callable.
 *
 * Cancellation is cooperative: the thread must check the token and return.
 *   - stop_token.stop_requested() -> true after a stop request
 *   - stop_source.request_stop()  -> ask the thread to stop
 *   - std::stop_callback          -> run a function when stop is requested
 *
 * This avoids detached threads, manual join() calls, and the classic bug where
 * a program exits while a worker still touches destroyed objects.
 */

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>
using namespace std;

int main() {
    // 1. A worker that polls its stop token.
    int counter = 0;

    jthread worker([&counter](stop_token token) {
        while (!token.stop_requested()) {
            counter++;
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    });

    this_thread::sleep_for(chrono::milliseconds(30));
    worker.request_stop();
    worker.join();  // wait for the worker before reading the counter

    cout << (counter > 0) << endl;  // 1

    // 2. A stop_callback runs on the thread that requests the stop.
    stop_source source;
    stop_token token = source.get_token();
    bool callbackRan = false;

    stop_callback callback(token, [&callbackRan]() { callbackRan = true; });

    source.request_stop();

    cout << token.stop_requested() << " " << callbackRan << endl;  // 1 1

    // 3. A worker that waits on a condition variable and wakes up on stop.
    //    condition_variable_any supports any lock type and can be combined with
    //    a stop_token through its wait overloads.
    mutex sleeperMutex;
    condition_variable_any sleeperCondition;

    jthread sleeper([&](stop_token stop) {
        unique_lock<mutex> lock(sleeperMutex);
        sleeperCondition.wait(lock, stop, []() { return false; });
    });

    this_thread::sleep_for(chrono::milliseconds(20));
    sleeper.request_stop();

    cout << sleeper.get_stop_token().stop_requested() << endl;  // 1

    return 0;
}

/**
 * Time complexity -> O(1) per check; request_stop runs the registered
 * callbacks
 * Space complexity -> O(1) per thread plus the stop-state shared block
 *
 * Special cases:
 *   - a thread that never checks the token cannot be stopped
 *   - request_stop() is idempotent; it returns false if a stop was already
 *     requested
 *   - a stop_callback runs exactly once; if stop was already requested when it
 *     is registered, it runs immediately in the constructor
 *   - destroying a jthread joins; if the thread ignores cancellation, the
 *     destructor blocks
 *   - calling request_stop() from a stop_callback is safe; the state is
 *     already marked as stopping, so the nested request returns false
 *
 * std::jthread is not a replacement for a thread pool: it still creates one OS
 * thread per object. Use it for long-lived workers, background tasks, and RAII
 * cleanup, not for thousands of short jobs.
 */
