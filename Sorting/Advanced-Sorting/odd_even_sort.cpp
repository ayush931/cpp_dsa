/**
 * Odd-Even Transposition Sort
 *
 * Odd-even sort is a variation of bubble sort that alternates between two
 * independent compare-and-swap phases:
 *   1. Compare pairs (1, 2), (3, 4), (5, 6), ...
 *   2. Compare pairs (0, 1), (2, 3), (4, 5), ...
 *
 * Each phase is a set of non-overlapping comparisons, so the phase can run in
 * parallel on hardware. The algorithm repeats the two phases until a full
 * round makes no swaps.
 *
 * Stability   -> stable: a pair is swapped only when the left value is
 *                strictly greater than the right value
 * In-place    -> yes, O(1) extra space
 * Time        -> O(n^2) worst and average, O(n) best for an already sorted
 * array Space       -> O(1)
 *
 * The implementation below is the sequential reference version. A brute-force
 * std::sort result is used to verify every test case.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void oddEvenSort(vector<int> &arr) {
    int n = static_cast<int>(arr.size());
    bool changed = true;

    while (changed) {
        changed = false;

        // Odd phase: pairs starting at index 1.
        for (int i = 1; i + 1 < n; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                changed = true;
            }
        }

        // Even phase: pairs starting at index 0.
        for (int i = 0; i + 1 < n; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                changed = true;
            }
        }
    }
}

void print(const vector<int> &arr) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(arr.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << arr[i];
    }

    cout << "]";
}

int main() {
    vector<vector<int>> tests = {{5, 1, 4, 2, 8, 0, 2},
                                 {},
                                 {7},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {3, 3, 1, 1, 2, 2},
                                 {-4, 0, -1, 9, -10, 3}};

    for (const vector<int> &original : tests) {
        vector<int> sorted = original;
        vector<int> expected = original;

        oddEvenSort(sorted);
        sort(expected.begin(), expected.end());

        print(sorted);
        cout << "  matches std::sort: " << (sorted == expected ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [0, 1, 2, 2, 4, 5, 8]  matches std::sort: yes
// []  matches std::sort: yes
// [7]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 1, 2, 2, 3, 3]  matches std::sort: yes
// [-10, -4, -1, 0, 3, 9]  matches std::sort: yes
//
// Special cases: empty array, single element, already sorted input (one round
// with no swaps), reverse sorted input, duplicates, and negative values.
