/**
 * 1. Binary Heap:- It is a complete Binary tree.
 * All levels will be filled completely except maybe last level and last levle
 * has keys and left as possible.
 *
 * 2. Can be either
 * Min Heap:- Present node has smaller value than child nodes
 * Max Heap:- Present node has larger value than child nodes
 */

/**
 * Representation of a heap using an minHeapay
 * 1. Root node will be at index 1
 * 2. For node at index i, left child -> 2 * i and right child -> 2 * i + 1
 * 3. For node at index i, parent node -> i / 2
 */

#include <iostream>
using namespace std;
int const N = 1e3;

void insertMinHeap(int minHeap[], int &size, int value) {
    size++;
    minHeap[size] = value;
    int curr = size;

    while (curr / 2 > 0 && minHeap[curr / 2] > minHeap[curr]) {
        swap(minHeap[curr / 2], minHeap[curr]);
        curr = curr / 2;
    }
};

void insertMaxHeap(int maxHeap[], int &size, int value) {
    size++;
    maxHeap[size] = value;
    int curr = size;

    while (curr / 2 > 0 && maxHeap[curr / 2] < maxHeap[curr]) {
        swap(maxHeap[curr], maxHeap[curr / 2]);
        curr = curr / 2;
    }
};

void removeMinHeap(int minHeap[], int &size) {
    minHeap[1] = minHeap[size];
    size--;
    int curr = 1;

    while (2 * curr <= size) {  // looping till curr node has child nodes
        int leftChild = 2 * curr;
        int rightChild = 2 * curr + 1;
        int minChild = leftChild;
        if (rightChild <= size && minHeap[rightChild] < minHeap[leftChild]) {
            minChild = rightChild;
        }

        if (minHeap[minChild] >= minHeap[curr]) {
            return;
        }

        swap(minHeap[minChild], minHeap[curr]);
        curr = minChild;
    }
};

int main() {
    int minHeap[N] = {-1, 10, 20, 30, 40, 50};
    int size = 5;
    int value = 5;

    insertMinHeap(minHeap, size, value);

    for (int i = 0; i <= size; i++) {
        cout << minHeap[i] << " ";
    }
    cout << endl;

    int maxHeap[N] = {-1, 60, 50, 40, 30, 20, 10, 5};
    int sizeMax = 7;
    int valueMax = 100;

    insertMaxHeap(maxHeap, sizeMax, valueMax);

    for (int i = 0; i <= size; i++) {
        cout << maxHeap[i] << " ";
    }
    cout << endl;

    int minHeap1[N] = {-1, 10, 20, 30, 40, 50};
    int sizeDelete = 5;
    int valueDelete = 5;

    insertMinHeap(minHeap1, sizeDelete, valueDelete);

    for (int i = 0; i <= sizeDelete; i++) {
        cout << minHeap1[i] << " ";
    }
    cout << endl;

    removeMinHeap(minHeap1, sizeDelete);

    return 0;
}

// Time complexity -> O(logn)