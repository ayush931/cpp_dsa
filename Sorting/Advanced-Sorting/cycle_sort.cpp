/**
 * Cycle sort
 *
 * Cycle sort writes every element directly to its final position. For a cycle
 * of k elements it performs k - 1 swaps, so the total number of writes is
 * O(n), which is the minimum possible for an in-place comparison sort.
 *
 * The algorithm picks a starting element, counts how many elements are smaller
 * than it to find its final position, swaps it there, and follows the cycle
 * until it returns to the start. Duplicates are handled by skipping positions
 * that already hold an equal value.
 *
 * Stability   -> not stable, because elements jump across the array
 * In-place    -> yes
 * Time        -> O(n^2) comparisons
 * Writes      -> O(n) swaps, the reason to choose cycle sort
 * Space       -> O(1)
 *
 * Selection sort is included for comparison: it also uses O(1) extra space but
 * always performs exactly n - 1 swaps (2 * (n - 1) writes), no matter how the
 * input is arranged.
 */

#include <iostream>
#include <vector>
using namespace std;

long long selectionSortWrites(vector<int> arr) {
    long long writes = 0;

    for (int i = 0; i + 1 < (int)arr.size(); i++) {
        int minIndex = i;

        for (int j = i + 1; j < (int)arr.size(); j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
            writes += 2;
        }
    }

    return writes;
}

long long cycleSort(vector<int> &arr) {
    int n = arr.size();
    long long writes = 0;

    for (int start = 0; start <= n - 2; start++) {
        int item = arr[start];
        int pos = start;

        // Count how many elements are smaller than item.
        for (int i = start + 1; i < n; i++) {
            if (arr[i] < item) {
                pos++;
            }
        }

        // item is already in its final position.
        if (pos == start) {
            continue;
        }

        // Skip duplicates that already sit in their final positions.
        while (item == arr[pos]) {
            pos++;
        }

        swap(item, arr[pos]);
        writes += 2;

        // Follow the cycle until it returns to start.
        while (pos != start) {
            pos = start;

            for (int i = start + 1; i < n; i++) {
                if (arr[i] < item) {
                    pos++;
                }
            }

            while (item == arr[pos]) {
                pos++;
            }

            swap(item, arr[pos]);
            writes += 2;
        }
    }

    return writes;
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
    vector<vector<int>> tests = {{5, 2, 9, 1, 5, 6}, {},           {7},
                                 {1, 2, 3, 4},       {4, 3, 2, 1}, {3, 3, 3, 3},
                                 {-2, 8, -2, 0, 5}};

    for (const vector<int> &original : tests) {
        vector<int> cycle = original;
        vector<int> selection = original;

        long long cycleWrites = cycleSort(cycle);
        long long selectionWrites = selectionSortWrites(selection);

        print(cycle);
        cout << "  cycle writes: " << cycleWrites
             << "  selection writes: " << selectionWrites
             << "  sorted: " << (cycle == selection ? "yes" : "no") << endl;
    }

    return 0;
}

// Expected output:
// [1, 2, 5, 5, 6, 9]  cycle writes: 10  selection writes: 10  sorted: yes
// []  cycle writes: 0  selection writes: 0  sorted: yes
// [7]  cycle writes: 0  selection writes: 0  sorted: yes
// [1, 2, 3, 4]  cycle writes: 0  selection writes: 0  sorted: yes
// [1, 2, 3, 4]  cycle writes: 4  selection writes: 6  sorted: yes
// [3, 3, 3, 3]  cycle writes: 0  selection writes: 0  sorted: yes
// [-2, -2, 0, 5, 8]  cycle writes: 4  selection writes: 8  sorted: yes
//
// Special cases: empty array, one element, already sorted input, reverse
// sorted input, all equal values, duplicates, and negative values.
//
// Note: the first case contains duplicates and ties selection sort's write
// count. The reverse sorted and negative-value cases show the advantage: every
// element is already in a cycle that needs only one swap per element.
