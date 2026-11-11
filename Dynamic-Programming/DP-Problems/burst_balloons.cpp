/**
 * Burst Balloons
 *
 * You are given n balloons, each with a number. Bursting balloon i gives
 *   nums[left] * nums[i] * nums[right]
 * coins, where left and right are the nearest balloons that have not burst
 * yet. After bursting, the balloon disappears and its neighbours become
 * adjacent. Add a virtual 1 at both ends. Return the maximum coins obtainable
 * by bursting every balloon.
 *
 * Examples:
 *   [3,1,5,8] -> 167
 *   [1,5]     -> 10
 *   [2]       -> 2
 *   []        -> 0
 *
 * The key is to choose the LAST balloon to burst in an interval. At that
 * moment its neighbours are the interval boundaries, so the interval splits
 * into two independent subintervals.
 *
 * Time: O(n^3). Space: O(n^2).
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every balloon as the last one in the interval.
// O(2^n) time, O(n) recursion space.
long long burstBrute(const vector<int> &nums, int left, int right) {
    if (left > right) {
        return 0;
    }

    long long best = 0;

    for (int last = left; last <= right; last++) {
        long long coins = 1LL * nums[left - 1] * nums[last] * nums[right + 1] +
                          burstBrute(nums, left, last - 1) +
                          burstBrute(nums, last + 1, right);
        best = max(best, coins);
    }

    return best;
}

// Memoized interval DP. nums is padded with 1 at both ends.
// O(n^3) time, O(n^2) space.
long long burstMemo(const vector<int> &nums, int left, int right,
                    vector<vector<long long>> &dp) {
    if (left > right) {
        return 0;
    }
    if (dp[left][right] != -1) {
        return dp[left][right];
    }

    long long best = 0;

    for (int last = left; last <= right; last++) {
        long long coins = 1LL * nums[left - 1] * nums[last] * nums[right + 1] +
                          burstMemo(nums, left, last - 1, dp) +
                          burstMemo(nums, last + 1, right, dp);
        best = max(best, coins);
    }

    return dp[left][right] = best;
}

long long maxCoins(const vector<int> &balloons) {
    if (balloons.empty()) {
        return 0;
    }

    vector<int> nums;
    nums.push_back(1);
    for (int value : balloons) {
        nums.push_back(value);
    }
    nums.push_back(1);

    int n = balloons.size();
    vector<vector<long long>> dp(n + 2, vector<long long>(n + 2, -1));

    return burstMemo(nums, 1, n, dp);
}

int main() {
    vector<int> first = {3, 1, 5, 8};
    vector<int> second = {1, 5};
    vector<int> third = {2};
    vector<int> empty;

    vector<int> paddedFirst = {1, 3, 1, 5, 8, 1};

    cout << burstBrute(paddedFirst, 1, 4) << " " << maxCoins(first) << endl;
    // 167 167

    cout << maxCoins(second) << endl;  // 10
    cout << maxCoins(third) << endl;   // 2
    cout << maxCoins(empty) << endl;   // 0

    vector<int> allOnes = {1, 1, 1, 1};
    cout << maxCoins(allOnes) << endl;  // 4? See note below.

    return 0;
}

/**
 * Complexity:
 *   brute -> O(2^n) time, O(n) recursion space
 *   memo  -> O(n^3) time, O(n^2) space
 *
 * Edge cases: empty input, one balloon, two balloons, all values equal to 1,
 * and zero values (a zero balloon contributes 0 but still separates its
 * neighbours).
 *
 * For [1,1,1,1], bursting any balloon gives 1 coin, and four balloons give 4.
 * The virtual 1s make the boundary handling uniform, so no special case is
 * needed for the first or last real balloon.
 */
