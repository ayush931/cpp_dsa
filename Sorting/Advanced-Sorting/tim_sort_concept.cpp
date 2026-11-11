/**
 * Tim sort concept (simplified natural merge sort)
 *
 * Tim sort starts from natural runs: maximal stretches that are already
 * ordered. An ascending run is kept as is; a strictly descending run is
 * reversed, which is safe because strictness means there are no equal elements
 * to reorder. Runs shorter than MIN_RUN are extended with insertion sort.
 * Finally, adjacent runs are merged pairwise until one run remains.
 *
 * This is the core idea of Tim sort. A production implementation also uses a
 * stack of runs with size invariants, galloping merges, and temporary buffers
 * sized to the smaller run.
 *
 * Stability   -> stable: insertion sort is stable, and merge takes from the
 *                left run when the values are equal
 * In-place    -> no, O(n) extra space for the merge buffer
 * Time        -> O(n log n) worst and average, O(n) best when the input is
 *                already one sorted run
 * Space       -> O(n)
 *
 * std::sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

const int MIN_RUN = 4;

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

// Finds natural runs, reverses descending ones, and extends short runs.
// Each pair is an inclusive [start, end] range.
vector<pair<int, int>> findRuns(vector<int> &arr) {
    int n = arr.size();
    vector<pair<int, int>> runs;
    int i = 0;

    while (i < n) {
        int start = i;

        if (i + 1 < n && arr[i] > arr[i + 1]) {
            // Strictly descending run: reverse it into an ascending run.
            while (i + 1 < n && arr[i] > arr[i + 1]) {
                i++;
            }
            reverse(arr.begin() + start, arr.begin() + i + 1);
        } else {
            // Ascending run, equal values included.
            while (i + 1 < n && arr[i] <= arr[i + 1]) {
                i++;
            }
        }

        int end = i;

        if (end - start + 1 < MIN_RUN) {
            int extendedEnd = min(n - 1, start + MIN_RUN - 1);
            insertionSortRange(arr, start, extendedEnd);
            end = extendedEnd;
        }

        runs.push_back({start, end});
        i = end + 1;
    }

    return runs;
}

void mergeRuns(vector<int> &arr, int low, int middle, int high,
               vector<int> &buffer) {
    int left = low;
    int right = middle + 1;
    int write = low;

    while (left <= middle && right <= high) {
        // <= keeps equal elements from the left run first, which is what makes
        // the merge stable.
        if (arr[left] <= arr[right]) {
            buffer[write++] = arr[left++];
        } else {
            buffer[write++] = arr[right++];
        }
    }

    while (left <= middle) {
        buffer[write++] = arr[left++];
    }
    while (right <= high) {
        buffer[write++] = arr[right++];
    }

    for (int i = low; i <= high; i++) {
        arr[i] = buffer[i];
    }
}

void naturalMergeSort(vector<int> &arr) {
    if (arr.size() < 2) {
        return;
    }

    vector<int> buffer(arr.size());
    vector<pair<int, int>> runs = findRuns(arr);

    while (runs.size() > 1) {
        vector<pair<int, int>> mergedRuns;

        for (int i = 0; i < (int)runs.size(); i += 2) {
            if (i + 1 == (int)runs.size()) {
                mergedRuns.push_back(runs[i]);
                continue;
            }

            int low = runs[i].first;
            int middle = runs[i].second;
            int high = runs[i + 1].second;

            mergeRuns(arr, low, middle, high, buffer);
            mergedRuns.push_back({low, high});
        }

        runs = mergedRuns;
    }
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
    vector<vector<int>> tests = {{1, 2, 3, 7, 6, 5, 4, 8, 9, 0, 2, 2},
                                 {},
                                 {6},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {2, 2, 1, 1, 3, 3},
                                 {-5, -1, -3, -2, 0, 4}};

    for (const vector<int> &original : tests) {
        vector<int> sorted = original;
        vector<int> expected = original;

        naturalMergeSort(sorted);
        sort(expected.begin(), expected.end());

        print(sorted);
        cout << "  matches std::sort: " << (sorted == expected ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [0, 1, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9]  matches std::sort: yes
// []  matches std::sort: yes
// [6]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 1, 2, 2, 3, 3]  matches std::sort: yes
// [-5, -3, -2, -1, 0, 4]  matches std::sort: yes
//
// Special cases: empty array, one element, one long sorted run (best case),
// reverse sorted input (one descending run), duplicates, and negative values.
