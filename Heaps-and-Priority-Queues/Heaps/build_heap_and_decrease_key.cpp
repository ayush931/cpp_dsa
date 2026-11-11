/**
 * Build a Min Heap and Decrease a Key
 *
 * Building a heap bottom-up takes O(n): heapify the parent nodes from the
 * last parent up to the root. Building it by inserting one value at a time
 * takes O(n log n). Decreasing a key at a known index takes O(log n): update
 * the value and bubble it up until the heap property is restored.
 *
 * Example:
 * Input array: [9, 4, 7, 1, 8, 2]
 * Heap array:  [1, 4, 2, 9, 8, 7]
 *
 * Decrease the value 9 at index 4 to 0:
 * Heap array:  [0, 1, 2, 4, 8, 7]
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
   private:
    vector<int> heap;  // heap[0] is a dummy; real values start at index 1

    void bubbleUp(int index) {
        while (index > 1 && heap[index / 2] > heap[index]) {
            swap(heap[index / 2], heap[index]);
            index /= 2;
        }
    }

    void bubbleDown(int index) {
        while (2 * index < heap.size()) {
            int left = 2 * index;
            int right = 2 * index + 1;
            int smallest = left;

            if (right < heap.size() && heap[right] < heap[left]) {
                smallest = right;
            }

            if (heap[smallest] >= heap[index]) {
                return;
            }

            swap(heap[smallest], heap[index]);
            index = smallest;
        }
    }

   public:
    MinHeap() {
        heap.push_back(-1);  // unused sentinel
    }

    bool empty() const {
        return heap.size() == 1;
    }

    int size() const {
        return heap.size() - 1;
    }

    void insert(int value) {
        heap.push_back(value);
        bubbleUp(heap.size() - 1);
    }

    // O(n log n): each insertion bubbles up at most O(log n) levels.
    void buildSlow(const vector<int> &values) {
        heap.clear();
        heap.push_back(-1);

        for (int value : values) {
            insert(value);
        }
    }

    // O(n): heapify every parent from the bottom up.
    void build(const vector<int> &values) {
        heap.clear();
        heap.push_back(-1);

        for (int value : values) {
            heap.push_back(value);
        }

        for (int index = size() / 2; index >= 1; index--) {
            bubbleDown(index);
        }
    }

    int extractMin() {
        if (empty()) {
            return -1;
        }

        int minimum = heap[1];
        heap[1] = heap.back();
        heap.pop_back();

        if (!empty()) {
            bubbleDown(1);
        }

        return minimum;
    }

    // index is 1-based, matching the heap array layout.
    bool decreaseKey(int index, int newValue) {
        if (index < 1 || index > size()) {
            return false;
        }
        if (newValue > heap[index]) {
            return false;  // this is not a decrease
        }

        heap[index] = newValue;
        bubbleUp(index);

        return true;
    }

    void print() const {
        for (int index = 1; index <= size(); index++) {
            cout << heap[index] << " ";
        }
        cout << endl;
    }
};

int main() {
    vector<int> values = {9, 4, 7, 1, 8, 2};

    MinHeap slow;
    slow.buildSlow(values);
    slow.print();  // 1 4 2 9 8 7

    MinHeap heap;
    heap.build(values);
    heap.print();  // 1 4 2 9 8 7

    cout << "---\n";

    cout << heap.decreaseKey(4, 0) << endl;  // 1
    heap.print();                            // 0 1 2 4 8 7

    cout << heap.decreaseKey(1, 5) << endl;  // 0 (5 is not smaller than 0)
    heap.print();                            // 0 1 2 4 8 7

    cout << "---\n";

    while (!heap.empty()) {
        cout << heap.extractMin() << " ";
    }
    cout << endl;
    // 0 1 2 4 7 8

    MinHeap emptyHeap;
    cout << emptyHeap.empty() << " " << emptyHeap.extractMin() << endl;
    // 1 -1

    return 0;
}

/**
 * Time complexity:
 *   buildSlow -> O(n log n)
 *   build -> O(n)
 *   insert -> O(log n)
 *   extractMin -> O(log n)
 *   decreaseKey -> O(log n)
 * Space complexity -> O(n) for the heap array
 *
 * Special cases: empty heap, one element, decreasing the root, decreasing a
 * leaf, an invalid index, a value that is not actually smaller, and duplicate
 * values.
 */
