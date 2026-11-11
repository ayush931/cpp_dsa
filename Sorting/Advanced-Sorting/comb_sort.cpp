/**
 * Comb sort
 *
 * Bubble sort compares neighbours, so a "turtle" (a small value near the end)
 * moves left only one position per pass. Comb sort starts with a large gap and
 * shrinks it by a factor of 1.3 after every pass:
 *
 *   gap = max(1, floor(gap / 1.3))
 *
 * Elements that are gap positions apart are compared and swapped. The final
 * pass uses gap 1, which is ordinary bubble sort, but by then most turtles have
 * already moved close to their final positions.
 *
 * The shrink factor 1.3 comes from empirical measurements; values between 1.2
 * and 1.4 all work well.
 *
 * Stability   -> not stable, because swaps happen across long distances
 * In-place    -> yes
 * Time        -> O(n^2) worst, close to O(n log n) average in practice,
 *                O(n log n) best (the gap still shrinks to 1)
 * Space       -> O(1)
 *
 * Bubble sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int> &arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void combSort(vector<int> &arr) {
    int n = arr.size();
    int gap = n;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = max(1, (int)(gap / 1.3));
        swapped = false;

        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
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
    vector<vector<int>> tests = {{8, 4, 1, 56, 3, -44, 23, -6, 28, 0},
                                 {},
                                 {9},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {4, 4, 2, 2, 4, 2},
                                 {-3, -8, 0, -3, 5}};

    for (const vector<int> &original : tests) {
        vector<int> comb = original;
        vector<int> bubble = original;

        combSort(comb);
        bubbleSort(bubble);

        print(comb);
        cout << "  matches bubble sort: " << (comb == bubble ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [-44, -6, 0, 1, 3, 4, 8, 23, 28, 56]  matches bubble sort: yes
// []  matches bubble sort: yes
// [9]  matches bubble sort: yes
// [1, 2, 3, 4, 5]  matches bubble sort: yes
// [1, 2, 3, 4, 5]  matches bubble sort: yes
// [2, 2, 2, 4, 4, 4]  matches bubble sort: yes
// [-8, -3, -3, 0, 5]  matches bubble sort: yes
//
// Special cases: empty array, one element, already sorted input, reverse
// sorted input, duplicates, and negative values.
