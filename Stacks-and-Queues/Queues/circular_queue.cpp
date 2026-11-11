/**
 * Circular Queue
 *
 * Design a circular queue with a fixed capacity. It supports enQueue,
 * deQueue, Front, Rear, isEmpty, and isFull in O(1) time.
 *
 * A circular queue reuses the free slots at the front of the array instead of
 * shifting all elements. The tail wraps around with modulo arithmetic.
 */

#include <iostream>
#include <vector>
using namespace std;

class MyCircularQueue {
    vector<int> data;
    int head;
    int count;

   public:
    MyCircularQueue(int capacity) {
        data.resize(capacity);
        head = 0;
        count = 0;
    }

    bool enQueue(int value) {
        if (isFull()) {
            return false;
        }

        int tail = (head + count) % data.size();
        data[tail] = value;
        count++;

        return true;
    }

    bool deQueue() {
        if (isEmpty()) {
            return false;
        }

        head = (head + 1) % data.size();
        count--;

        return true;
    }

    int Front() {
        if (isEmpty()) {
            return -1;
        }
        return data[head];
    }

    int Rear() {
        if (isEmpty()) {
            return -1;
        }
        return data[(head + count - 1) % data.size()];
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == (int)data.size();
    }

    int size() {
        return count;
    }
};

int main() {
    MyCircularQueue queue(3);

    cout << queue.enQueue(1) << endl;  // 1
    cout << queue.enQueue(2) << endl;  // 1
    cout << queue.enQueue(3) << endl;  // 1
    cout << queue.enQueue(4) << endl;  // 0 (full)

    cout << queue.Rear() << endl;    // 3
    cout << queue.isFull() << endl;  // 1

    cout << queue.deQueue() << endl;   // 1
    cout << queue.enQueue(4) << endl;  // 1 (reuses the freed slot)
    cout << queue.Rear() << endl;      // 4

    cout << queue.Front() << endl;  // 2
    cout << queue.size() << endl;   // 3

    queue.deQueue();
    queue.deQueue();
    queue.deQueue();

    cout << queue.isEmpty() << endl;  // 1
    cout << queue.Front() << endl;    // -1
    cout << queue.Rear() << endl;     // -1

    return 0;
}

/**
 * Time complexity -> O(1) for every operation
 * Space complexity -> O(k) for k slots
 *
 * Special cases: capacity 1, enqueue into a full queue, dequeue from an empty
 * queue, front/rear on an empty queue, wrapping around the end of the array,
 * using count to distinguish full from empty (head == tail is ambiguous
 * without a counter).
 */
