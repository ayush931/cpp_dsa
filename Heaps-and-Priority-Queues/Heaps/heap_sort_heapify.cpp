/**
 * 1. Convert the array into heap data structure using heapify
 * 2. One by one delete the root node of the heap and replace it with the last
 * node in the heap and then heapify the root of the heap. Repeat this process
 * till the size of the heap is greater than 1.
 */

// Time complexity -> O(NlogN)

#include <bits/stdc++.h>
using namespace std;
int const N = 1e3;

void heapify(int arr[], int n, int curr) {
    while (2 * curr <= n) {
        int leftChild = 2 * curr;
        int rightChild = 2 * curr + 1;
        int maxChild = leftChild;

        if (rightChild <= n && arr[rightChild] > arr[leftChild]) {
            maxChild = rightChild;
        }

        if (arr[maxChild] <= arr[curr]) {
            return;
        }

        swap(arr[maxChild], arr[curr]);
        curr = maxChild;
    }
};

void remove(int arr[], int &size) {
    swap(arr[1], arr[size]);
    size--;
    heapify(arr, size, 1);
};

void heapSort(int arr[], int n) {
    // 1. heapify all parent nodes into max heap
    for (int i = n / 2; i > 0; i--) {
        heapify(arr, n, i);
    }

    // 2. keep deleting elements from max heap untill size becomes 0.
    while (n > 0) {
        remove(arr, n);
    }
};

int main() {
    int n = 7;
    int arr[N] = {-1, 60, 10, 80, 50, 5, 20, 70};
    heapSort(arr, n);

    for (int i = 1; i <= n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}