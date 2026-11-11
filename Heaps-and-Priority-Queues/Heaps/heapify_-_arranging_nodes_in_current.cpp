/**
 * Heapify:- arranging nodes in current order so that they follow properties of
 * minheap / maxheap
 */

#include <bits/stdc++.h>
using namespace std;
int const N = 1e3;

void heapify(int arr[], int &size, int curr) {
    while (2 * curr <= size) {
        int leftChild = 2 * curr;
        int rightChild = 2 * curr + 1;
        int minChild = leftChild;

        if (rightChild <= size && arr[rightChild] < arr[minChild]) {
            minChild = rightChild;
        }

        if (arr[minChild] >= arr[curr]) {
            return;
        }

        swap(arr[minChild], arr[curr]);
        curr = minChild;
    }
};

int main() {
    int arr[N] = {-1, 60, 10, 80, 50, 5, 20, 70};
    int size = 7;

    for (int i = size / 2; i > 0; i--) {
        // i -> parent node
        heapify(arr, size, i);
    }

    for (int i = 1; i <= size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}