/**
 * D-ary Min Heap
 *
 * A d-ary heap is a heap where every node has up to d children. For a node at
 * index i (0-based):
 *   parent      = (i - 1) / d
 *   first child = d * i + 1
 *   last child  = d * i + d
 *
 * Compared with a binary heap, insert and decrease-key are faster (the tree is
 * shallower), while extract-min scans d children instead of 2.
 *
 * Example with d = 4:
 * Insert 10, 4, 7, 1, 9, 2, 8
 * Extract all -> 1 2 4 7 8 9 10
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class DaryHeap {
   private:
    int d;
    vector<int> heap;

    void bubbleUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / d;

            if (heap[parent] <= heap[index]) {
                break;
            }

            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void bubbleDown(int index) {
        while (true) {
            int firstChild = d * index + 1;
            if (firstChild >= heap.size()) {
                break;
            }

            int lastChild = min(d * index + d, (int)heap.size() - 1);
            int smallest = firstChild;

            for (int child = firstChild + 1; child <= lastChild; child++) {
                if (heap[child] < heap[smallest]) {
                    smallest = child;
                }
            }

            if (heap[index] <= heap[smallest]) {
                break;
            }

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

   public:
    DaryHeap(int arity) {
        d = max(2, arity);
    }

    bool empty() const {
        return heap.empty();
    }

    int size() const {
        return heap.size();
    }

    void insert(int value) {
        heap.push_back(value);
        bubbleUp(heap.size() - 1);
    }

    int extractMin() {
        if (heap.empty()) {
            return -1;
        }

        int minimum = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            bubbleDown(0);
        }

        return minimum;
    }

    void print() const {
        for (int value : heap) {
            cout << value << " ";
        }
        cout << endl;
    }
};

int main() {
    DaryHeap heap(4);

    for (int value : {10, 4, 7, 1, 9, 2, 8}) {
        heap.insert(value);
    }

    heap.print();  // 1 2 7 4 9 10 8

    cout << "---\n";

    while (!heap.empty()) {
        cout << heap.extractMin() << " ";
    }
    cout << endl;
    // 1 2 4 7 8 9 10

    cout << "---\n";

    DaryHeap empty(4);
    cout << empty.empty() << " " << empty.extractMin() << endl;  // 1 -1

    DaryHeap single(4);
    single.insert(5);
    cout << single.extractMin() << " " << single.empty() << endl;  // 5 1

    return 0;
}

/**
 * Time complexity:
 *   insert -> O(log_d n)
 *   extractMin -> O(d * log_d n), because each level checks d children
 *   empty/size -> O(1)
 * Space complexity -> O(n) for the heap array
 *
 * Special cases: d = 2 (a binary heap), a large d (shallow but expensive
 * extract-min), empty heap, one element, duplicate values, and negative
 * values.
 */
