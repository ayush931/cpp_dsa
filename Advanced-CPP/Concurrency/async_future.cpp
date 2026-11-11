/**
 * std::async -> run a callable asynchronously and return a std::future
 * launch::async -> force a new thread
 * launch::deferred -> run lazily on get() or wait()
 * std::future::get() -> wait and return the value (can be called only once)
 *
 * std::promise / std::future -> manual one-shot channel
 * std::packaged_task -> wrap a callable so its result goes into a future
 *
 * Exceptions thrown inside the task are stored and rethrown by future::get()
 */

#include <future>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>
using namespace std;

long long sumRange(int start, int end) {
    long long total = 0;

    for (int i = start; i <= end; i++) {
        total += i;
    }

    return total;
}

int main() {
    future<long long> first = async(launch::async, sumRange, 1, 500000);
    future<long long> second = async(launch::async, sumRange, 500001, 1000000);

    long long total = first.get() + second.get();
    cout << total << endl;  // 500000500000

    // promise -> future channel
    promise<int> promiseValue;
    future<int> futureValue = promiseValue.get_future();

    thread producer([&promiseValue]() { promiseValue.set_value(99); });

    cout << futureValue.get() << endl;  // 99
    producer.join();

    // packaged_task
    packaged_task<int(int, int)> task([](int a, int b) { return a * b; });
    future<int> product = task.get_future();

    thread worker([task = std::move(task)]() mutable { task(6, 7); });
    cout << product.get() << endl;  // 42
    worker.join();

    // exceptions cross the future boundary
    future<int> failing = async(
        launch::async, []() -> int { throw runtime_error("task failed"); });

    try {
        failing.get();
    } catch (exception &error) {
        cout << error.what() << endl;
    }

    return 0;
}

// Special cases: future destructor from std::async blocks until the task ends;
// get() called twice throws future_error; broken promise if the promise is
// destroyed without set_value; deferred tasks run on get(); shared_future
// allows multiple readers
