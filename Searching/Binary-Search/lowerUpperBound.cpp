// Lower bound and upper bound on a sorted array.
//
// lower_bound(target): first index i such that a[i] >= target, or n.
// upper_bound(target): first index i such that a[i] >  target, or n.
//
// They differ only in the comparison, and together they give the half-open
// range [lower_bound, upper_bound) containing every occurrence of target.
//
// Example:
//   a = [1, 2, 2, 2, 4, 5]
//   lower_bound(2) = 1, upper_bound(2) = 4, count = 3
//   lower_bound(3) = 4, upper_bound(3) = 4, count = 0
//
// Brute force: scan from the left -> O(n). Binary search -> O(log n).

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int lowerBoundBrute(const vector<int> &a, int target) {
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] >= target) {
            return i;
        }
    }
    return a.size();
}

int upperBoundBrute(const vector<int> &a, int target) {
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] > target) {
            return i;
        }
    }
    return a.size();
}

int lowerBound(const vector<int> &a, int target) {
    int lo = 0;
    int hi = a.size();  // half-open range [lo, hi)

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (a[mid] >= target) {
            hi = mid;  // mid is a candidate, keep looking left
        } else {
            lo = mid + 1;  // everything through mid is too small
        }
    }

    return lo;
}

int upperBound(const vector<int> &a, int target) {
    int lo = 0;
    int hi = a.size();

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (a[mid] > target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}

int main() {
    vector<int> a = {1, 2, 2, 2, 4, 5};

    cout << lowerBound(a, 2) << " " << upperBound(a, 2) << endl;
    // 1 4

    cout << lowerBound(a, 3) << " " << upperBound(a, 3) << endl;
    // 4 4

    cout << lowerBound(a, 0) << " " << upperBound(a, 0) << endl;
    // 0 0

    cout << lowerBound(a, 6) << " " << upperBound(a, 6) << endl;
    // 6 6

    cout << lowerBoundBrute(a, 2) << " " << upperBoundBrute(a, 2) << endl;
    // 1 4

    cout << upperBound(a, 2) - lowerBound(a, 2) << endl;
    // 3

    // Cross-check the hand-written versions against the standard algorithms.
    for (int target = 0; target <= 6; target++) {
        cout << (lowerBound(a, target) ==
                 (int)(std::lower_bound(a.begin(), a.end(), target) -
                       a.begin()))
             << (upperBound(a, target) ==
                 (int)(std::upper_bound(a.begin(), a.end(), target) -
                       a.begin()));
    }
    cout << endl;  // fourteen 1s: every result matches std::lower_bound /
                   // std::upper_bound

    vector<int> empty;
    cout << lowerBound(empty, 5) << " " << upperBound(empty, 5) << endl;
    // 0 0

    return 0;
}

// Time complexity -> O(log n) for each bound, O(n) for the brute-force scan
// Space complexity -> O(1)
