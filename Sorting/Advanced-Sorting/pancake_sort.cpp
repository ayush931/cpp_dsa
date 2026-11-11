/**
 * Pancake sort
 *
 * The only allowed operation is a prefix reversal: choose k and reverse the
 * first k elements. To place the largest remaining element at the end of the
 * active prefix:
 *
 *   1. find the maximum in [0, size - 1]
 *   2. if it is not already at the front, flip it to the front
 *   3. flip the whole active prefix so it lands at index size - 1
 *
 * Every element needs at most two flips, so the algorithm uses at most
 * 2 * (n - 1) prefix reversals.
 *
 * Stability   -> not stable, because a prefix reversal reorders equal elements
 * In-place    -> yes
 * Time        -> O(n^2) comparisons
 * Space       -> O(1) extra; the returned flip list is O(n)
 *
 * A copy sorted with std::sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void flipPrefix(vector<int> &arr, int k) {
    reverse(arr.begin(), arr.begin() + k);
}

vector<int> pancakeSort(vector<int> &arr) {
    vector<int> flips;

    for (int size = arr.size(); size > 1; size--) {
        int maxIndex = 0;

        for (int i = 1; i < size; i++) {
            if (arr[i] > arr[maxIndex]) {
                maxIndex = i;
            }
        }

        if (maxIndex == size - 1) {
            continue;
        }

        if (maxIndex > 0) {
            flipPrefix(arr, maxIndex + 1);
            flips.push_back(maxIndex + 1);
        }

        flipPrefix(arr, size);
        flips.push_back(size);
    }

    return flips;
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
    vector<vector<int>> tests = {{3, 6, 2, 7, 1}, {},        {5},
                                 {1, 2, 3},       {3, 2, 1}, {2, 2, 1, 2},
                                 {-3, 4, -3, 0}};

    for (const vector<int> &original : tests) {
        vector<int> pancakes = original;
        vector<int> expected = original;

        vector<int> flips = pancakeSort(pancakes);
        sort(expected.begin(), expected.end());

        print(pancakes);
        cout << "  flips: ";
        print(flips);
        cout << "  sorted: " << (pancakes == expected ? "yes" : "no") << endl;
    }

    return 0;
}

// Expected output:
// [1, 2, 3, 6, 7]  flips: [4, 5, 3, 4, 2]  sorted: yes
// []  flips: []  sorted: yes
// [5]  flips: []  sorted: yes
// [1, 2, 3]  flips: []  sorted: yes
// [1, 2, 3]  flips: [3, 2]  sorted: yes
// [1, 2, 2, 2]  flips: [2, 4, 3, 2]  sorted: yes
// [-3, -3, 0, 4]  flips: [3, 4, 3, 2]  sorted: yes
//
// Special cases: empty array, one element, already sorted input, reverse
// sorted input, duplicates, and negative values.
