/**
 * Gnome sort (stupid sort)
 *
 * A gnome sorts a row of flower pots by walking left to right:
 *
 *   if the current pair is in order, step forward
 *   otherwise swap the pair and step back
 *
 * Stepping back re-checks the previous pair, so the algorithm behaves like
 * insertion sort that uses swaps instead of shifting a key into a hole. The
 * prefix before the current position is always sorted.
 *
 * Stability   -> stable, because only adjacent elements are swapped
 * In-place    -> yes
 * Time        -> O(n^2) worst and average, O(n) best (already sorted)
 * Space       -> O(1)
 *
 * Insertion sort is included as the brute-force reference. It has the same
 * asymptotic bounds but performs one assignment per shift instead of one swap
 * per inversion, so it is faster in practice.
 */

#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int> &arr) {
    for (int i = 1; i < (int)arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void gnomeSort(vector<int> &arr) {
    int i = 0;
    int n = arr.size();

    while (i < n) {
        if (i == 0 || arr[i - 1] <= arr[i]) {
            i++;
        } else {
            swap(arr[i - 1], arr[i]);
            i--;
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
    vector<vector<int>> tests = {{34, 2, 10, -9, 7, 2}, {},           {1},
                                 {1, 2, 3, 4},          {4, 3, 2, 1}, {6, 6, 6},
                                 {-5, 3, -5, 0, 3}};

    for (const vector<int> &original : tests) {
        vector<int> gnome = original;
        vector<int> insertion = original;

        gnomeSort(gnome);
        insertionSort(insertion);

        print(gnome);
        cout << "  matches insertion sort: "
             << (gnome == insertion ? "yes" : "no") << endl;
    }

    return 0;
}

// Expected output:
// [-9, 2, 2, 7, 10, 34]  matches insertion sort: yes
// []  matches insertion sort: yes
// [1]  matches insertion sort: yes
// [1, 2, 3, 4]  matches insertion sort: yes
// [1, 2, 3, 4]  matches insertion sort: yes
// [6, 6, 6]  matches insertion sort: yes
// [-5, -5, 0, 3, 3]  matches insertion sort: yes
//
// Special cases: empty array, one element, already sorted input, reverse
// sorted input, all equal values, duplicates, and negative values.
