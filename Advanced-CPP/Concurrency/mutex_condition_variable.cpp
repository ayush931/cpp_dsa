/**
 * Race condition -> two threads access shared data and at least one writes,
 * without synchronization. The result depends on timing.
 *
 * std::mutex -> lock() / unlock(), usually through std::lock_guard or
 *               std::unique_lock
 * std::condition_variable -> wait until a predicate becomes true
 *   wait(lock, predicate) releases the mutex while waiting and reacquires it
 *   before returning, so it must be used with unique_lock
 * notify_one() -> wake one waiter
 * notify_all() -> wake all waiters
 *
 * Always re-check the predicate after waking (spurious wakeups are allowed)
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

class ProducerConsumer {
    mutex mtx;
    condition_variable notEmpty;
    condition_variable notFull;
    queue<int> buffer;
    int capacity;
    bool finished;

   public:
    ProducerConsumer(int cap) {
        capacity = cap;
        finished = false;
    }

    void produce(int value) {
        unique_lock<mutex> lock(mtx);
        notFull.wait(lock, [this]() { return buffer.size() < capacity; });

        buffer.push(value);
        cout << "produced " << value << endl;

        lock.unlock();
        notEmpty.notify_one();
    }

    int consume() {
        unique_lock<mutex> lock(mtx);
        notEmpty.wait(lock, [this]() { return !buffer.empty() || finished; });

        if (buffer.empty()) {
            return -1;
        }

        int value = buffer.front();
        buffer.pop();
        cout << "consumed " << value << endl;

        lock.unlock();
        notFull.notify_one();

        return value;
    }

    void finish() {
        unique_lock<mutex> lock(mtx);
        finished = true;
        lock.unlock();
        notEmpty.notify_all();
    }
};

int main() {
    ProducerConsumer pc(3);

    thread producer([&pc]() {
        for (int i = 1; i <= 6; i++) {
            pc.produce(i);
        }
        pc.finish();
    });

    thread consumer([&pc]() {
        while (true) {
            int value = pc.consume();
            if (value == -1) {
                break;
            }
        }
    });

    producer.join();
    consumer.join();

    // lock_guard -> simple scoped lock, cannot unlock early
    mutex counterMutex;
    int counter = 0;

    auto increment = [&]() {
        for (int i = 0; i < 10000; i++) {
            lock_guard<mutex> lock(counterMutex);
            counter++;
        }
    };

    thread a(increment);
    thread b(increment);

    a.join();
    b.join();

    cout << counter << endl;  // 20000

    return 0;
}

// Special cases: forgetting to notify (waiter sleeps forever), notifying
// before waiting (lost wakeup), holding the lock while notifying (works but
// less efficient), exception while holding a lock (lock_guard still unlocks),
// condition_variable_any for non-mutex lock types
