/**
 * Ring Buffer (Circular Buffer)
 *
 * A ring buffer is a fixed-size buffer that wraps around. It is used for
 * producer-consumer queues, logging, audio/video streaming, and network
 * packet buffers.
 *
 * Two common policies when the buffer is full:
 *   - reject the new item (backpressure)
 *   - overwrite the oldest item (latest data wins)
 *
 * All operations are O(1). The buffer never allocates after construction.
 */

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template <typename T>
class RingBuffer {
    vector<T> data;
    int head;
    int count;
    bool overwrite;

   public:
    RingBuffer(int capacity, bool overwriteOldest = false) {
        if (capacity < 0) {
            capacity = 0;
        }

        data.resize(capacity);
        head = 0;
        count = 0;
        overwrite = overwriteOldest;
    }

    bool push(const T &value) {
        if (data.empty()) {
            return false;
        }

        if (count == data.size()) {
            if (!overwrite) {
                return false;
            }

            // Replace the oldest element and advance the head.
            data[head] = value;
            head = (head + 1) % data.size();
            return true;
        }

        int tail = (head + count) % data.size();
        data[tail] = value;
        count++;

        return true;
    }

    bool pop(T &value) {
        if (count == 0) {
            return false;
        }

        value = data[head];
        head = (head + 1) % data.size();
        count--;

        return true;
    }

    T &front() {
        if (count == 0) {
            throw out_of_range("empty ring buffer");
        }
        return data[head];
    }

    T &back() {
        if (count == 0) {
            throw out_of_range("empty ring buffer");
        }
        return data[(head + count - 1) % data.size()];
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == data.size();
    }

    int size() {
        return count;
    }

    int capacity() {
        return data.size();
    }
};

void print(RingBuffer<int> &buffer) {
    while (!buffer.isEmpty()) {
        int value;
        buffer.pop(value);
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    RingBuffer<int> rejecting(3, false);

    cout << rejecting.push(1) << endl;  // 1
    cout << rejecting.push(2) << endl;  // 1
    cout << rejecting.push(3) << endl;  // 1
    cout << rejecting.push(4) << endl;  // 0 (full, reject)

    int value;
    rejecting.pop(value);
    cout << value << endl;  // 1

    rejecting.push(4);
    print(rejecting);  // 2 3 4

    RingBuffer<int> overwriting(3, true);
    overwriting.push(1);
    overwriting.push(2);
    overwriting.push(3);
    overwriting.push(4);  // overwrites 1

    print(overwriting);  // 2 3 4

    RingBuffer<int> zero(0);
    cout << zero.push(1) << endl;    // 0
    cout << zero.isEmpty() << endl;  // 1

    return 0;
}

/**
 * Time complexity -> O(1) for push, pop, front, back, size
 * Space complexity -> O(capacity)
 *
 * Special cases: capacity 0, one slot, full buffer with reject policy, full
 * buffer with overwrite policy, repeated wrap-around, pop on an empty buffer,
 * front/back on an empty buffer (undefined here; check isEmpty first).
 *
 * A thread-safe ring buffer adds a mutex and condition variables for the
 * producer and consumer. A lock-free ring buffer uses atomic head/tail indices
 * and memory ordering.
 */
