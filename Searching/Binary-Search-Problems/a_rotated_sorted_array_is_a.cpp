// A rotated sorted array is a sorted array on which a rotation has been
// performed some number of times. Given a rotated sorted array of distinct
// elements, return the index of the minimum element. Follow 0-based indexing.
//
// Example:
//   [4, 5, 6, 7, 0, 1, 2] -> 4   (value 0)
//   [0, 1, 2, 3, 4]       -> 0
//
// The minimum is the only index where input[mid] is smaller than input[mid -
// 1]. Comparing input[mid] with input[hi] lets us discard one half safely.

#include <bits/stdc++.h>
using namespace std;

int findMinimumInSortedRotated(vector<int> &input) {
    if (input.empty()) {
        return -1;
    }

    int lo = 0;
    int hi = input.size() - 1;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (input[mid] > input[hi]) {
            // The minimum is in the right half.
            lo = mid + 1;
        } else {
            // input[mid] <= input[hi]: mid can itself be the minimum.
            hi = mid;
        }
    }

    return lo;
}

int main() {
    int n;
    cin >> n;

    vector<int> input(n);

    for (int i = 0; i < n; i++) {
        cin >> input[i];
    }

    cout << findMinimumInSortedRotated(input) << endl;

    return 0;
}

// Time complexity -> O(log n)
// Space complexity -> O(1)
