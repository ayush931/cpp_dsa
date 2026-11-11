/**
 * House Robber II (Circular Street)
 *
 * The houses are arranged in a circle, so the first and last houses are
 * adjacent. Return the maximum amount that can be robbed without robbing two
 * adjacent houses.
 *
 * Examples:
 *   [2,3,2]     -> 3   (rob the middle house only)
 *   [1,2,3,1]   -> 4   (rob houses 0 and 2)
 *   [1,2,3]     -> 3   (rob house 2)
 *   [5]         -> 5
 *   []          -> 0
 *
 * The circle is handled by solving two linear ranges:
 *   - houses 0 .. n-2 (exclude the last house)
 *   - houses 1 .. n-1 (exclude the first house)
 * and taking the larger answer. A brute force over all subsets is included
 * for comparison on small inputs.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every subset and reject subsets containing two adjacent
// houses, including the wrap-around pair (0, n - 1).
// O(2^n) time, O(1) space.
long long robCircularBrute(const vector<int> &money) {
    int n = money.size();
    long long best = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        if (n > 1 && (mask & 1) && (mask & (1 << (n - 1)))) {
            continue;
        }

        bool valid = true;
        long long total = 0;

        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) {
                if (i > 0 && ((mask >> (i - 1)) & 1)) {
                    valid = false;
                    break;
                }
                total += money[i];
            }
        }

        if (valid) {
            best = max(best, total);
        }
    }

    return best;
}

// Linear house robber on the inclusive range [left, right].
// O(length) time, O(1) space.
long long robLinear(const vector<int> &money, int left, int right) {
    long long previousTwo = 0;
    long long previousOne = 0;

    for (int i = left; i <= right; i++) {
        long long current = max(previousOne, previousTwo + money[i]);
        previousTwo = previousOne;
        previousOne = current;
    }

    return previousOne;
}

// Optimized: split the circle into two linear ranges.
// O(n) time, O(1) space.
long long robCircular(const vector<int> &money) {
    int n = money.size();
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return money[0];
    }

    return max(robLinear(money, 0, n - 2), robLinear(money, 1, n - 1));
}

int main() {
    vector<int> first = {2, 3, 2};
    vector<int> second = {1, 2, 3, 1};
    vector<int> third = {1, 2, 3};

    cout << robCircularBrute(first) << " " << robCircular(first) << endl;
    // 3 3

    cout << robCircularBrute(second) << " " << robCircular(second) << endl;
    // 4 4

    cout << robCircular(third) << endl;  // 3

    vector<int> single = {5};
    cout << robCircular(single) << endl;  // 5

    vector<int> empty;
    cout << robCircular(empty) << endl;  // 0

    vector<int> two = {2, 7};
    cout << robCircular(two) << endl;  // 7 (they are adjacent, pick one)

    vector<int> allZero = {0, 0, 0, 0};
    cout << robCircular(allZero) << endl;  // 0

    return 0;
}

/**
 * Complexity:
 *   brute   -> O(2^n) time, O(1) extra space
 *   optimized -> O(n) time, O(1) space
 *
 * Edge cases: empty street, one house, two adjacent houses, all zero values,
 * and a street where the best answer uses neither endpoint.
 *
 * Why two ranges are enough: in any valid selection, at least one of the first
 * and last houses is not robbed. Solving both possibilities covers every valid
 * selection exactly once.
 */
