// Implementing queue using array

#include <iostream>
#include <vector>
using namespace std;

class Queue {
    vector<int> v;
    int front;

   public:
    Queue() {
        this->front = 0;
    }

    void enqueue(int data) {
        v.push_back(data);
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        }

        front++;

        // reclaim memory once every element has been removed
        if (front == (int)v.size()) {
            v.clear();
            front = 0;
        }
    }

    int getFront() {
        if (isEmpty()) {
            return -1;
        }
        return v[front];
    }

    int getBack() {
        if (isEmpty()) {
            return -1;
        }
        return v.back();
    }

    bool isEmpty() {
        return front >= (int)v.size();
    }

    int size() {
        return v.size() - front;
    }
};

int main() {
    Queue qu;
    qu.enqueue(10);
    qu.enqueue(20);
    qu.enqueue(30);

    qu.dequeue();  // removes 10
    qu.enqueue(40);

    cout << qu.getFront() << " " << qu.getBack() << endl;  // 20 40
    cout << qu.size() << endl;                             // 3

    qu.dequeue();  // removes 20
    qu.dequeue();  // removes 30

    while (not qu.isEmpty()) {
        cout << qu.getFront() << " ";
        qu.dequeue();
    }
    cout << endl;  // 40

    // the queue is reusable after it becomes empty
    qu.enqueue(50);
    cout << qu.getFront() << endl;  // 50

    return 0;
}

// Time -> enqueue O(1) amortized, dequeue O(1), front/back O(1)
// Space -> O(n) for all elements enqueued since the queue was last empty
// (dequeued slots are reclaimed only when the queue becomes empty)
// Special cases: dequeue on an empty queue, enqueue after the queue becomes
// empty, front on an empty queue (-1), repeated dequeue
// A fixed-size circular queue avoids the growing vector; see 05_circular_queue
