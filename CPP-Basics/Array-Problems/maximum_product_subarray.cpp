// Maximum product subarray
//
// Find the contiguous subarray with the largest product.
//
// Example:
//   input  {2, 3, -2, 4}
//   output 6        (subarray {2, 3})
//
//   input  {-2, 0, -1}
//   output 0        (subarray {0})
//
//   input  {-2, 3, -4}
//   output 24       (subarray {-2, 3, -4})
//
// Brute force: try every start and extend the product to every end.
// O(n^2) time, O(1) space.
//
// Optimized: keep both the maximum and minimum product ending at the current
// position. A negative number turns the smallest product into the largest, so
// both values are needed. At each step, either start a new subarray at the
// current value or extend one of the two products.
//
// Edge cases:
//   - empty array: no subarray exists, return 0 by convention
//   - one element: return that element, even if it is negative
//   - zeros split the array into independent parts
//   - an even number of negatives can make a large positive product
//   - an odd number of negatives may force the best product to be negative
//   - products can overflow int, so use long long
//
// Complexity:
//   brute     -> O(n^2) time, O(1) space
//   optimized -> O(n) time, O(1) space

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

long long maxProductBrute(const vector<int> &values) {
    if (values.empty()) {
        return 0;
    }

    long long best = LLONG_MIN;

    for (int start = 0; start < static_cast<int>(values.size()); start++) {
        long long product = 1;

        for (int end = start; end < static_cast<int>(values.size()); end++) {
            product *= values[end];
            best = max(best, product);
        }
    }

    return best;
}

long long maxProduct(const vector<int> &values) {
    if (values.empty()) {
        return 0;
    }

    long long best = values[0];
    long long maxEnding = values[0];
    long long minEnding = values[0];

    for (int i = 1; i < static_cast<int>(values.size()); i++) {
        if (values[i] < 0) {
            swap(maxEnding, minEnding);
        }

        maxEnding =
            max(static_cast<long long>(values[i]), maxEnding * values[i]);
        minEnding =
            min(static_cast<long long>(values[i]), minEnding * values[i]);

        best = max(best, maxEnding);
    }

    return best;
}

int main() {
    vector<vector<int>> tests = {{2, 3, -2, 4}, {-2, 0, -1}, {-2, 3, -4},
                                 {0, 2},        {-2},        {}};

    for (const vector<int> &values : tests) {
        cout << maxProductBrute(values) << " " << maxProduct(values) << endl;
    }

    return 0;
}

// Output:
// 6 6
// 0 0
// 24 24
// 2 2
// -2 -2
// 0 0
//
// The maximum/minimum-ending idea is the same as Kadane's algorithm, extended
// with a second state for negative products. Use long long when the values or
// the length can make the product exceed the int range.
