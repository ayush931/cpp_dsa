/**
 * Heap sort -> build a max heap, then repeatedly swap the root with the last
 * element and heapify the reduced heap Time complexity -> O(n log n) in best,
 * average and worst case Space complexity -> O(1) In-place, unstable
 */

#include <iostream>
#include <vector>
using namespace std;

void heapify(vector<int> &arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
};

void heapSort(vector<int> &arr) {
    int n = arr.size();

    // build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // move the largest element to the end one by one
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
};

int main() {
    vector<int> arr = {9, 4, 7, 1, 8, 2};

    heapSort(arr);

    for (int el : arr) {
        cout << el << " ";
    }
    cout << endl;

    return 0;
}
