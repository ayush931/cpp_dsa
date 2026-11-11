/**
 * Given an array arr[] and an integer K where K is smaller than size of array,
 * the task is to find the Kth smallest element in the given array. It is given
 * that all array elements are distinct. Note:- I and r denotes the starting and
 * ending index of the array. Input: N = 6, arr[] = {7, 10, 4, 3, 20, 15}, K = 3
 * Output: 7
 */

#include <bits/stdc++.h>
using namespace std;
int const N = 1e3;

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

void remove(int arr[], int &size) {
    arr[0] = arr[size - 1];
    size--;
    heapify(arr, size, 0);
};

int kthSmallestElement(int arr[], int size, int k) {
    // 1. create min heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }

    // 2. remove k-1 elements
    k -= 1;
    while (k--) {
        remove(arr, size);
    }

    return arr[0];
};

int main() {
    int arr[N] = {60, 10, 80, 50, 5, 20, 70};
    int size = 7;

    cout << kthSmallestElement(arr, size, 7) << endl;

    return 0;
}

// Time complexity -> O(N) to build the heap + O(k log N) for k - 1 removals
// Space complexity -> O(1) extra (the array is rearranged in place)