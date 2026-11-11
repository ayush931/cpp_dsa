// Find the floor of the square root of a non-negative integer x.
//
// Example:
//   x = 100 -> 10
//   x = 8   -> 2   (2 * 2 <= 8 < 3 * 3)
//   x = 0   -> 0
//
// Binary search the answer in [0, x]. Use long long for mid * mid so the
// comparison does not overflow when x is close to INT_MAX.

#include <bits/stdc++.h>
using namespace std;

int floorSqrt(int x) {
    if (x < 2) {
        return x;
    }

    int lo = 1;
    int hi = x;
    int ans = 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        long long square = 1LL * mid * mid;

        if (square <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return ans;
}

int main() {
    cout << floorSqrt(100) << endl;         // 10
    cout << floorSqrt(8) << endl;           // 2
    cout << floorSqrt(0) << endl;           // 0
    cout << floorSqrt(1) << endl;           // 1
    cout << floorSqrt(2147483647) << endl;  // 46340

    return 0;
}

// Time complexity -> O(logn)
// Space complexity -> O(1)