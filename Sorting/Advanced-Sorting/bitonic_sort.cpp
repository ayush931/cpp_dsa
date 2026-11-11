/**
 * Bitonic sort
 *
 * A sequence is bitonic when it first increases and then decreases (or can be
 * rotated into that shape). Bitonic sort works in two phases:
 *
 *   build  -> recursively sort the left half ascending and the right half
 *             descending, which makes the whole range bitonic
 *   merge  -> compare element i with element i + k across the two halves,
 *             then recursively merge each half
 *
 * The core algorithm requires the length to be a power of two. For any other
 * length, sortArbitrary pads with a large sentinel, sorts, and trims. The
 * padded version uses O(n) extra memory.
 *
 * Stability   -> not stable, because compare-and-swap exchanges distant
 *                elements and equal keys can change order
 * In-place    -> yes for the power-of-two core; the wrapper pads a copy
 * Time        -> O(n log^2 n) comparisons
 * Space       -> O(log n) recursion stack for the core, O(n) for padding
 *
 * std::sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

void compareAndSwap(vector<int> &arr, int i, int j, bool ascending) {
    if (ascending ? arr[i] > arr[j] : arr[i] < arr[j]) {
        swap(arr[i], arr[j]);
    }
}

void bitonicMerge(vector<int> &arr, int low, int count, bool ascending) {
    if (count <= 1) {
        return;
    }

    int half = count / 2;

    for (int i = low; i < low + half; i++) {
        compareAndSwap(arr, i, i + half, ascending);
    }

    bitonicMerge(arr, low, half, ascending);
    bitonicMerge(arr, low + half, half, ascending);
}

void bitonicSort(vector<int> &arr, int low, int count, bool ascending) {
    if (count <= 1) {
        return;
    }

    int half = count / 2;

    bitonicSort(arr, low, half, true);
    bitonicSort(arr, low + half, half, false);
    bitonicMerge(arr, low, count, ascending);
}

vector<int> sortArbitrary(vector<int> arr) {
    int n = arr.size();
    if (n <= 1) {
        return arr;
    }

    int size = 1;
    while (size < n) {
        size *= 2;
    }

    arr.resize(size, numeric_limits<int>::max());
    bitonicSort(arr, 0, size, true);
    arr.resize(n);

    return arr;
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
    vector<vector<int>> powerOfTwoTests = {{3, 7, 4, 8, 6, 2, 1, 5},
                                           {8, 7, 6, 5, 4, 3, 2, 1},
                                           {1, 1, 2, 2, 3, 3, 4, 4}};

    for (const vector<int> &original : powerOfTwoTests) {
        vector<int> bitonic = original;
        vector<int> expected = original;

        bitonicSort(bitonic, 0, bitonic.size(), true);
        sort(expected.begin(), expected.end());

        print(bitonic);
        cout << "  sorted: " << (bitonic == expected ? "yes" : "no") << endl;
    }

    vector<vector<int>> arbitraryTests = {
        {9, 1, 5, 3, 7, 2}, {}, {4}, {-2, 8, 0, -2, 5}};

    for (const vector<int> &original : arbitraryTests) {
        vector<int> sorted = sortArbitrary(original);
        vector<int> expected = original;

        sort(expected.begin(), expected.end());

        print(sorted);
        cout << "  padded sort: " << (sorted == expected ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [1, 2, 3, 4, 5, 6, 7, 8]  sorted: yes
// [1, 2, 3, 4, 5, 6, 7, 8]  sorted: yes
// [1, 1, 2, 2, 3, 3, 4, 4]  sorted: yes
// [1, 2, 3, 5, 7, 9]  padded sort: yes
// []  padded sort: yes
// [4]  padded sort: yes
// [-2, -2, 0, 5, 8]  padded sort: yes
//
// Special cases: empty array, one element, already sorted, reverse sorted,
// duplicates, negative values, and lengths that are not a power of two.
//
// The sentinel is numeric_limits<int>::max(), so an input that already
// contains INT_MAX still sorts correctly: the real values and the padding are
// equal, and trimming the padding leaves the real values at the end.
