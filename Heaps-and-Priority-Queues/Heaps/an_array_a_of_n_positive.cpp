/**
 * Given an array A[] of N positive integers and two positive integers K1 and
 * K2. Find the sum of all elements between K1th and K2h smallest elements of
 * the array. It may be assumed that (1 <= k1 < k2 <= n). Input: N = 7, A[] =
 * {20, 8, 22, 4, 12, 10, 14}, K1 = 3, K2 = 6 Output: 26
 */

#include <bits/stdc++.h>
using namespace std;

void heapify(int arr[], int size, int curr) {
    while (2 * curr + 1 < size) {
        int leftChild = 2 * curr + 1;
        int rightChild = 2 * curr + 2;

        int minChild = leftChild;
        if (rightChild < size && arr[rightChild] < arr[leftChild]) {
            minChild = rightChild;
        }

        if (arr[minChild] >= arr[curr]) {
            return;
        }

        swap(arr[curr], arr[minChild]);
        curr = minChild;
    }
};

int remove(int arr[], int &size) {
    int removedValue = arr[0];
    arr[0] = arr[size - 1];
    size--;
    heapify(arr, size, 0);
    return removedValue;
};

int sumOfSmallest(int arr[], int size, int k1, int k2) {
    // 1. min heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }

    int sum = 0;
    int elements = k2 - k1 - 1;

    // 2. remove k1 elements
    while (k1--) {
        remove(arr, size);
    }

    // 3. calculate sum by removing elements
    while (elements--) {
        sum += remove(arr, size);
    }

    return sum;
};

int main() {
    int arr[7] = {20, 8, 22, 4, 12, 10, 14};
    int size = 7;

    int k1 = 3;
    int k2 = 6;

    cout << sumOfSmallest(arr, size, k1, k2) << endl;

    return 0;
}