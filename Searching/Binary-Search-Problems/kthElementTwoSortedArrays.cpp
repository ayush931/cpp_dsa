// Kth element of two sorted arrays
//
// Given two sorted arrays a and b and an integer k (1-based), return the kth
// smallest element of the combined arrays.
//
// Example:
//   a = [2, 3, 6, 7, 9], b = [1, 4, 8, 10]
//   k = 1 -> 1
//   k = 5 -> 6
//   k = 9 -> 10
//
// Brute force: merge until the kth element -> O(k) time, O(1) extra space.
// Optimized: binary search a partition of the smaller array -> O(log min(m,
// n)).
//
// The partition puts cut1 elements of a and cut2 = k - cut1 elements of b on
// the left side. A valid partition satisfies
//   max(left side) <= min(right side).

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int kthElementBrute(const vector<int> &a, const vector<int> &b, int k) {
    int m = a.size();
    int n = b.size();
    if (k < 1 || k > m + n) {
        return -1;
    }

    int i = 0;
    int j = 0;

    for (int count = 1; count < k; count++) {
        if (i < m && (j >= n || a[i] <= b[j])) {
            i++;
        } else {
            j++;
        }
    }

    if (i >= m) {
        return b[j];
    }
    if (j >= n) {
        return a[i];
    }
    return min(a[i], b[j]);
}

int kthElement(const vector<int> &a, const vector<int> &b, int k) {
    int m = a.size();
    int n = b.size();
    if (k < 1 || k > m + n) {
        return -1;
    }

    // Binary search the smaller array so the range is as short as possible.
    if (m > n) {
        return kthElement(b, a, k);
    }

    // cut1 elements of a go to the left; cut2 elements of b go to the left.
    int lo = max(0, k - n);
    int hi = min(k, m);

    while (lo <= hi) {
        int cut1 = lo + (hi - lo) / 2;
        int cut2 = k - cut1;

        int left1 = (cut1 == 0) ? INT_MIN : a[cut1 - 1];
        int right1 = (cut1 == m) ? INT_MAX : a[cut1];

        int left2 = (cut2 == 0) ? INT_MIN : b[cut2 - 1];
        int right2 = (cut2 == n) ? INT_MAX : b[cut2];

        if (left1 <= right2 && left2 <= right1) {
            return max(left1, left2);
        } else if (left1 > right2) {
            hi = cut1 - 1;  // too many elements of a on the left
        } else {
            lo = cut1 + 1;  // too few elements of a on the left
        }
    }

    return -1;
}

int main() {
    vector<int> a = {2, 3, 6, 7, 9};
    vector<int> b = {1, 4, 8, 10};

    cout << kthElement(a, b, 1) << endl;       // 1
    cout << kthElement(a, b, 5) << endl;       // 6
    cout << kthElement(a, b, 9) << endl;       // 10
    cout << kthElementBrute(a, b, 5) << endl;  // 6

    vector<int> empty;
    cout << kthElement(empty, b, 3) << endl;  // 8
    cout << kthElement(a, empty, 2) << endl;  // 3

    cout << kthElement(a, b, 0) << endl;   // -1
    cout << kthElement(a, b, 10) << endl;  // -1

    return 0;
}

// Time complexity -> O(log min(m, n)) time, O(1) extra space
// Brute force -> O(k) time, O(1) extra space
