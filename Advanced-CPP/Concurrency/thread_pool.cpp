/**
 * Thread pool -> a fixed number of worker threads consume tasks from a queue
 * Avoids creating and destroying a thread for every task
 * Tasks wait in a std::queue until a worker is free
 *
 * submit() returns a std::future so the caller can wait for the result
 * shutdown -> set stop, wake all workers, join them
 *
 * Work stealing -> idle workers take tasks from the back of another worker's
 * deque, which balances uneven task costs (used by TBB and Go's scheduler)
 */

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

class ThreadPool {
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;

   public:
    ThreadPool(int threadCount) {
        stop = false;

        for (int i = 0; i < threadCount; i++) {
            workers.push_back(thread([this]() {
                while (true) {
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock,
                                [this]() { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            }));
        }
    }

    future<int> submit(function<int()> job) {
        auto task = make_shared<packaged_task<int()>>(move(job));
        future<int> result = task->get_future();

        {
            lock_guard<mutex> lock(mtx);
            tasks.push([task]() { (*task)(); });
        }

        cv.notify_one();
        return result;
    }

    ~ThreadPool() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();

        for (thread &worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
};

int main() {
    ThreadPool pool(4);

    vector<future<int>> results;

    for (int i = 0; i < 8; i++) {
        results.push_back(pool.submit([i]() { return i * i; }));
    }

    int total = 0;

    for (future<int> &result : results) {
        total += result.get();
    }

    cout << total << endl;  // 0 + 1 + 4 + ... + 49 = 140

    return 0;
}

// Special cases: zero worker threads (tasks never run), task throws (stored
// in its future), shutdown with pending tasks (finish them first),
// nested submit from a worker can deadlock if the pool is exhausted,
// unbounded queue growth under load (use a bounded queue and backpressure)
