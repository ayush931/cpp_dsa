/**
 * Deque from scratch
 *
 * Implement a double-ended queue with a circular dynamic array. Elements can
 * be inserted and removed at both ends in amortized O(1) time.
 *
 * Operations:
 * pushFront, pushBack, popFront, popBack, front, back, empty, size
 *
 * Example:
 * pushBack(1), pushBack(2), pushFront(0)
 * front() -> 0, back() -> 2
 * popFront() -> removes 0
 * popBack()  -> removes 2
 *
 * Time -> amortized O(1) per operation, Space -> O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

class Deque {
    vector<int> data;
    int head;   // index of the front element
    int count;  // number of stored elements

    void ensureCapacity() {
        if ((int)data.size() > count) {
            return;
        }

        int newCapacity = data.empty() ? 1 : (int)data.size() * 2;
        vector<int> grown(newCapacity);

        // Copy the logical order into the new buffer, starting at index 0.
        for (int i = 0; i < count; i++) {
            grown[i] = data[(head + i) % data.size()];
        }

        data.swap(grown);
        head = 0;
    }

   public:
    Deque() : head(0), count(0) {}

    void pushBack(int value) {
        ensureCapacity();
        data[(head + count) % data.size()] = value;
        count++;
    }

    void pushFront(int value) {
        ensureCapacity();
        head = (head - 1 + (int)data.size()) % data.size();
        data[head] = value;
        count++;
    }

    bool popFront() {
        if (empty()) {
            return false;
        }

        head = (head + 1) % data.size();
        count--;
        return true;
    }

    bool popBack() {
        if (empty()) {
            return false;
        }

        count--;
        return true;
    }

    int front() const {
        if (empty()) {
            return -1;
        }
        return data[head];
    }

    int back() const {
        if (empty()) {
            return -1;
        }
        return data[(head + count - 1) % data.size()];
    }

    bool empty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};

int main() {
    Deque dq;

    dq.pushBack(1);
    dq.pushBack(2);
    dq.pushBack(3);
    dq.pushFront(0);

    cout << dq.front() << " " << dq.back() << endl;  // 0 3
    cout << dq.size() << endl;                       // 4

    dq.popFront();
    dq.popBack();

    cout << dq.front() << " " << dq.back() << endl;  // 1 2

    dq.pushFront(-1);
    dq.pushBack(4);

    cout << dq.front() << " " << dq.back() << endl;  // -1 4
    cout << dq.size() << endl;                       // 4

    while (!dq.empty()) {
        cout << dq.front() << " ";
        dq.popFront();
    }
    cout << endl;  // -1 1 2 4

    cout << dq.empty() << endl;     // 1
    cout << dq.popFront() << endl;  // 0

    // Force several growth steps and check the order.
    Deque grown;

    for (int i = 1; i <= 10; i++) {
        grown.pushBack(i);
    }

    for (int i = 0; i < 5; i++) {
        grown.popFront();
    }

    for (int i = 0; i < 5; i++) {
        grown.pushFront(100 + i);
    }

    while (!grown.empty()) {
        cout << grown.front() << " ";
        grown.popFront();
    }
    cout << endl;  // 104 103 102 101 100 6 7 8 9 10

    return 0;
}

/**
 * Special cases: operations on an empty deque (pop returns false, front/back
 * return -1), repeated growth, wraparound at the end of the buffer, and
 * alternating pushFront/popFront.
 *
 * The -1 return value is a sentinel; use empty() to distinguish it from a
 * stored -1.
 */
