/**
 * Cocktail shaker sort (bidirectional bubble sort)
 *
 * Bubble sort always walks left to right, so a small element near the right
 * end needs one full pass per position to reach the front. Cocktail shaker
 * sort alternates directions:
 *
 *   forward pass  -> moves the largest remaining element to the right end
 *   backward pass -> moves the smallest remaining element to the left end
 *
 * The sorted region therefore grows from both ends, and the active window
 * shrinks by one element on each side.
 *
 * Stability   -> stable, because only adjacent elements are swapped and equal
 *                elements never jump over each other
 * In-place    -> yes
 * Time        -> O(n^2) worst and average, O(n) best (already sorted; the
 *                first forward pass performs no swap and the loop stops)
 * Space       -> O(1)
 *
 * Bubble sort is included as the brute-force reference.
 */

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

void cocktailShakerSort(vector<int> &arr) {
    int low = 0;
    int high = (int)arr.size() - 1;
    bool swapped = true;

    while (swapped && low < high) {
        swapped = false;

        // Forward pass: the largest element of [low, high] reaches high.
        for (int i = low; i < high; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        high--;

        if (!swapped) {
            break;
        }

        swapped = false;

        // Backward pass: the smallest element of [low, high] reaches low.
        for (int i = high; i > low; i--) {
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        low++;
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
    vector<vector<int>> tests = {{5, 1, 4, 2, 8, 0, 2},
                                 {},
                                 {7},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {3, 3, 1, 2, 3, 1},
                                 {-4, 0, -10, 7, -4}};

    for (const vector<int> &original : tests) {
        vector<int> cocktail = original;
        vector<int> bubble = original;

        cocktailShakerSort(cocktail);
        bubbleSort(bubble);

        print(cocktail);
        cout << "  matches bubble sort: " << (cocktail == bubble ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [0, 1, 2, 2, 4, 5, 8]  matches bubble sort: yes
// []  matches bubble sort: yes
// [7]  matches bubble sort: yes
// [1, 2, 3, 4, 5]  matches bubble sort: yes
// [1, 2, 3, 4, 5]  matches bubble sort: yes
// [1, 1, 2, 3, 3, 3]  matches bubble sort: yes
// [-10, -4, -4, 0, 7]  matches bubble sort: yes
//
// Special cases: empty array, one element, already sorted input (best case),
// reverse sorted input, duplicates, and negative values.
