// Merge sort: divide the array in half, sort both halves, then merge them.
// The merge keeps equal elements in their original order, so the algorithm is
// stable. It needs O(n) extra space for the temporary arrays.

#include <iostream>
#include <vector>
using namespace std;

void merge(int arr[], int l, int mid, int r) {
    int an = mid - l + 1;
    int bn = r - mid;

    // create 2 temp arrays
    vector<int> a(an);
    vector<int> b(bn);

    for (int i = 0; i < an; i++) {
        a[i] = arr[l + i];
    }

    for (int j = 0; j < bn; j++) {
        b[j] = arr[mid + 1 + j];
    }

    int i = 0;  // initial index of first subarray, a
    int j = 0;  // initial index of second subarray, b
    int k = l;  // initial index of merged subarray

    while (i < an && j < bn) {
        // <= keeps the merge stable for equal keys
        if (a[i] <= b[j]) {
            arr[k++] = a[i++];
        } else {
            arr[k++] = b[j++];
        }
    }

    while (i < an) {
        arr[k++] = a[i++];
    }

    while (j < bn) {
        arr[k++] = b[j++];
    }
}

void mergeSort(int arr[], int l, int r) {
    // base case
    if (l >= r) {
        return;
    }

    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);

    merge(arr, l, mid, r);
}

int main() {
    int arr[] = {10, 28, 24, 6, 34, 18, 38, 44};
    int n = sizeof(arr) / sizeof(arr[0]);

    mergeSort(arr, 0, n - 1);

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 6 10 18 24 28 34 38 44

    return 0;
}

// Time complexity -> O(n log n) in all cases
// Space complexity -> O(n) for the temporary arrays plus O(log n) stack

// Applications: large data sets and linked lists. Merge sort is stable, but it
// uses O(n) extra memory and does more work than insertion sort on tiny inputs.
