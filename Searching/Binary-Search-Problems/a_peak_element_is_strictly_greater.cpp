// A peak element is strictly greater than its neighbours. Given a 0-indexed
// integer array nums, return the index of any peak element. Imagine that
// nums[-1] = nums[n] = -infinity, so an endpoint can be a peak too.
//
// Example:
//   [1, 2, 3, 1]          -> 2
//   [1, 2, 1, 3, 5, 6, 4] -> 5 (or 1)
//
// Binary search: if nums[mid] < nums[mid + 1] the slope is rising, so a peak
// must exist to the right. Otherwise a peak exists at mid or to the left.

#include <bits/stdc++.h>
using namespace std;

int findPeak(vector<int> &input) {
    int n = input.size();
    if (n == 0) {
        return -1;
    }
    if (n == 1) {
        return 0;
    }

    int lo = 0;
    int hi = n - 1;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (input[mid] < input[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return lo;
}

int main() {
    vector<int> input = {3, 4, 5, 1, 9, 10, 2, 6, 8, 2};
    cout << findPeak(input) << "\n";  // 8

    vector<int> ascending = {1, 2, 3, 4};
    cout << findPeak(ascending) << "\n";  // 3

    vector<int> single = {7};
    cout << findPeak(single) << "\n";  // 0

    return 0;
}

// Time complexity -> O(log n)
// Space complexity -> O(1)
