/**
 * Sentinel search
 *
 * A normal linear search checks two conditions on every iteration:
 *
 *   while (i < n && arr[i] != target) i++;
 *
 * Sentinel search appends the target to the end of the array. The target is
 * guaranteed to be found, so the loop only checks the value:
 *
 *   while (arr[i] != target) i++;
 *
 * If the found index is n, the target was not in the original array. The
 * sentinel removes one comparison per element, which can matter in tight
 * loops, though the asymptotic cost is unchanged.
 *
 * The vector is modified temporarily and restored with pop_back(), so the
 * caller's data is unchanged. push_back may reallocate, but only O(1)
 * amortized extra space is used.
 *
 * Time        -> O(n)
 * Space       -> O(1) amortized extra
 * Requirement -> none; the array does not need to be sorted
 *
 * A plain linear search is included as the brute-force reference.
 */

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

int sentinelSearch(vector<int> &arr, int target) {
    int n = arr.size();
    arr.push_back(target);

    int i = 0;
    while (arr[i] != target) {
        i++;
    }

    arr.pop_back();

    return i < n ? i : -1;
}

int main() {
    vector<int> arr = {9, -3, 0, 9, 4, 7, 1};

    for (int target : {9, 1, -3, 5, 100}) {
        int sentinel = sentinelSearch(arr, target);
        int linear = linearSearch(arr, target);

        cout << "target " << target << ": sentinel = " << sentinel
             << ", linear = " << linear
             << ", agree = " << (sentinel == linear ? "yes" : "no") << endl;
    }

    cout << "array restored: "
         << (arr == vector<int>{9, -3, 0, 9, 4, 7, 1} ? "yes" : "no") << endl;

    vector<int> empty;
    cout << "empty array: " << sentinelSearch(empty, 5) << endl;
    cout << "empty array restored: " << (empty.empty() ? "yes" : "no") << endl;

    vector<int> single = {42};
    cout << "single hit: " << sentinelSearch(single, 42) << endl;
    cout << "single miss: " << sentinelSearch(single, 7) << endl;

    return 0;
}

// Expected output:
// target 9: sentinel = 0, linear = 0, agree = yes
// target 1: sentinel = 6, linear = 6, agree = yes
// target -3: sentinel = 1, linear = 1, agree = yes
// target 5: sentinel = -1, linear = -1, agree = yes
// target 100: sentinel = -1, linear = -1, agree = yes
// array restored: yes
// empty array: -1
// empty array restored: yes
// single hit: 0
// single miss: -1
//
// Special cases: empty array, one element, target at the first or last index,
// duplicates, target absent, and negative values.
//
// Sentinel search is a micro-optimization for linear scans. On a sorted array,
// binary search or interpolation search is asymptotically better; on unsorted
// data, a sentinel scan is still O(n).
