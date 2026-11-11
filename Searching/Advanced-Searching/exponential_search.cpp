/**
 * Exponential search
 *
 * Exponential search is designed for sorted arrays whose size is not known in
 * advance, or for targets that are close to the front:
 *
 *   1. check arr[0]
 *   2. double the bound: 1, 2, 4, 8, ... until arr[bound] >= target
 *   3. binary search inside [bound / 2, min(bound, n - 1)]
 *
 * If the target is at index i, the bound reaches it after O(log i) steps, and
 * the binary search also costs O(log i), so the total is O(log i) instead of
 * O(log n).
 *
 * Time        -> O(log i), where i is the target index; O(log n) when the
 *                target is near the end or absent
 * Space       -> O(1)
 * Requirement -> the array must be sorted
 *
 * Linear search is included as the brute-force reference. The final binary
 * search is a lower-bound search, so duplicates return the first occurrence.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int lowerBoundInRange(const vector<int> &arr, int low, int high, int target) {
    while (low <= high) {
        int middle = low + (high - low) / 2;

        if (arr[middle] < target) {
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }

    return low;
}

int exponentialSearch(const vector<int> &arr, int target) {
    int n = arr.size();
    if (n == 0) {
        return -1;
    }

    if (arr[0] == target) {
        return 0;
    }

    int bound = 1;

    while (bound < n && arr[bound] < target) {
        if (bound > n / 2) {
            bound = n;
            break;
        }

        bound *= 2;
    }

    int low = bound / 2;
    int high = min(bound, n - 1);
    int index = lowerBoundInRange(arr, low, high, target);

    if (index < n && arr[index] == target) {
        return index;
    }

    return -1;
}

int main() {
    vector<int> arr = {-10, -4, 0, 3, 3, 3, 7, 11, 18, 25, 40, 56, 71, 90};

    for (int target : {3, -10, 90, 7, 4, -20, 100}) {
        int exponential = exponentialSearch(arr, target);
        int linear = linearSearch(arr, target);

        cout << "target " << target << ": exponential = " << exponential
             << ", linear = " << linear
             << ", agree = " << (exponential == linear ? "yes" : "no") << endl;
    }

    vector<int> empty;
    cout << "empty array: " << exponentialSearch(empty, 5) << endl;
    cout << "single hit: " << exponentialSearch(vector<int>{5}, 5) << endl;
    cout << "single miss: " << exponentialSearch(vector<int>{5}, 1) << endl;

    return 0;
}

// Expected output:
// target 3: exponential = 3, linear = 3, agree = yes
// target -10: exponential = 0, linear = 0, agree = yes
// target 90: exponential = 13, linear = 13, agree = yes
// target 7: exponential = 6, linear = 6, agree = yes
// target 4: exponential = -1, linear = -1, agree = yes
// target -20: exponential = -1, linear = -1, agree = yes
// target 100: exponential = -1, linear = -1, agree = yes
// empty array: -1
// single hit: 0
// single miss: -1
//
// Special cases: empty array, one element, target at the first or last index,
// duplicates, negative values, target absent, and target outside the range.
//
// The doubling is guarded with bound > n / 2 to avoid integer overflow on very
// large arrays.
