/**
 * Introsort
 *
 * Introsort is the hybrid behind many standard library sort implementations.
 * It combines three algorithms:
 *
 *   quicksort      -> fast average case, median-of-three pivot
 *   heapsort       -> fallback when recursion gets too deep, guaranteeing
 *                     O(n log n) worst-case time
 *   insertion sort -> finishes small partitions, where it beats quicksort
 *
 * The recursion depth limit is 2 * floor(log2(n)). If a partition chain
 * reaches that depth, the remaining range is sorted with heapsort instead of
 * quicksort, so the O(n^2) quicksort worst case cannot happen.
 *
 * Stability   -> not stable, because quicksort partitions exchange elements
 * In-place    -> yes
 * Time        -> O(n log n) worst and average, O(n log n) best for this version
 * Space       -> O(log n) recursion stack
 *
 * std::sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int INSERTION_THRESHOLD = 16;

void insertionSortRange(vector<int> &arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void siftDown(vector<int> &arr, int low, int root, int size) {
    while (true) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && arr[low + left] > arr[low + largest]) {
            largest = left;
        }

        if (right < size && arr[low + right] > arr[low + largest]) {
            largest = right;
        }

        if (largest == root) {
            break;
        }

        swap(arr[low + root], arr[low + largest]);
        root = largest;
    }
}

void heapSortRange(vector<int> &arr, int low, int high) {
    int size = high - low + 1;

    for (int i = size / 2 - 1; i >= 0; i--) {
        siftDown(arr, low, i, size);
    }

    for (int end = size - 1; end > 0; end--) {
        swap(arr[low], arr[low + end]);
        siftDown(arr, low, 0, end);
    }
}

int partitionRange(vector<int> &arr, int low, int high) {
    int middle = low + (high - low) / 2;

    // Median of three: order arr[low], arr[middle], arr[high].
    if (arr[middle] < arr[low]) {
        swap(arr[middle], arr[low]);
    }
    if (arr[high] < arr[low]) {
        swap(arr[high], arr[low]);
    }
    if (arr[high] < arr[middle]) {
        swap(arr[high], arr[middle]);
    }

    // Use the median as the pivot and move it to the end.
    swap(arr[middle], arr[high]);

    int pivot = arr[high];
    int store = low;

    for (int i = low; i < high; i++) {
        if (arr[i] < pivot) {
            swap(arr[i], arr[store]);
            store++;
        }
    }

    swap(arr[store], arr[high]);
    return store;
}

void introSortLoop(vector<int> &arr, int low, int high, int depthLimit) {
    while (high - low + 1 > INSERTION_THRESHOLD) {
        if (depthLimit == 0) {
            heapSortRange(arr, low, high);
            return;
        }

        depthLimit--;

        int pivotIndex = partitionRange(arr, low, high);

        // Recurse into the smaller side and loop on the larger side, which
        // keeps the recursion stack O(log n).
        if (pivotIndex - low < high - pivotIndex) {
            introSortLoop(arr, low, pivotIndex - 1, depthLimit);
            low = pivotIndex + 1;
        } else {
            introSortLoop(arr, pivotIndex + 1, high, depthLimit);
            high = pivotIndex - 1;
        }
    }

    insertionSortRange(arr, low, high);
}

void introSort(vector<int> &arr) {
    if (arr.size() < 2) {
        return;
    }

    int depthLimit = 0;

    for (int n = arr.size(); n > 1; n /= 2) {
        depthLimit++;
    }

    introSortLoop(arr, 0, arr.size() - 1, 2 * depthLimit);
}

void print(const vector<int> &arr) {
    cout << "[";

    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << arr[i];
    }

    cout << "]";
}

int main() {
    vector<vector<int>> tests = {{5, 1, 9, 3, 7, 3, 8, 2, 6, 4, 0, 5},
                                 {},
                                 {2},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 vector<int>(40, 7),
                                 {-3, 12, -3, 0, 8, -11, 4}};

    for (const vector<int> &original : tests) {
        vector<int> intro = original;
        vector<int> expected = original;

        introSort(intro);
        sort(expected.begin(), expected.end());

        print(intro);
        cout << "  matches std::sort: " << (intro == expected ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [0, 1, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9]  matches std::sort: yes
// []  matches std::sort: yes
// [2]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
// 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7]  matches std::sort: yes
// [-11, -3, -3, 0, 4, 8, 12]  matches std::sort: yes
//
// Special cases: empty array, one element, already sorted input, reverse
// sorted input, many duplicates, and negative values. The all-equal case also
// exercises the depth limit and the heapsort fallback.
