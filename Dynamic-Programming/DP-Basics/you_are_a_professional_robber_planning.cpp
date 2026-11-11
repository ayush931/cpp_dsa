/**
 * You are a professional robber planning to rob houses along a street. Each
 * house has a certain amount of money stashed, the only constraint stopping you
 * from robbing each of them is that adjacent houses have security systems
 * connected and it will automatically contact the police if two adjacent houses
 * were broken into on the same night.
 *
 * Given an integer array nums representing the amount of money of each house,
 * return the maximum amount of money you can rob tonight without alerting the
 * police.
 *
 * Example 1:
 * Input: nums = [1,2,3,1]
 * Output: 4
 * Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
 * Total amount you can rob = 1 + 3 = 4.
 *
 * Example 2:
 * Input: nums = [2,7,9,3,1]
 * Output: 12
 * Explanation: Rob house 1 (money = 2), rob house 3 (money = 9) and rob house 5
 * (money = 1). Total amount you can rob = 2 + 9 + 1 = 12.
 *
 * Constraints:
 * 1 <= nums.length <= 100
 * 0 <= nums[i] <= 400
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try robbing or skipping every house -> O(2^n)
int robBrute(vector<int> &nums, int index) {
    if (index >= nums.size()) {
        return 0;
    }

    int robCurrent = nums[index] + robBrute(nums, index + 2);
    int skipCurrent = robBrute(nums, index + 1);

    return max(robCurrent, skipCurrent);
};

// Memoized recursion -> O(n) time, O(n) space
int robMemo(vector<int> &nums, int index, vector<int> &dp) {
    if (index >= nums.size()) {
        return 0;
    }
    if (dp[index] != -1) {
        return dp[index];
    }

    int robCurrent = nums[index] + robMemo(nums, index + 2, dp);
    int skipCurrent = robMemo(nums, index + 1, dp);

    return dp[index] = max(robCurrent, skipCurrent);
};

// Bottom-up DP -> O(n) time, O(n) space
int robBottomUp(vector<int> &nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return nums[0];
    }

    vector<int> dp(n, 0);
    dp[0] = nums[0];
    dp[1] = max(nums[0], nums[1]);

    for (int i = 2; i < n; i++) {
        dp[i] = max(nums[i] + dp[i - 2], dp[i - 1]);
    }

    return dp[n - 1];
};

// Optimized -> O(n) time, O(1) space
int rob(vector<int> &nums) {
    int previousTwo = 0;
    int previousOne = 0;

    for (int money : nums) {
        int current = max(previousOne, previousTwo + money);
        previousTwo = previousOne;
        previousOne = current;
    }

    return previousOne;
};

int main() {
    vector<int> first = {1, 2, 3, 1};
    vector<int> second = {2, 7, 9, 3, 1};

    vector<int> dp(first.size(), -1);
    cout << robBrute(first, 0) << endl;     // 4
    cout << robMemo(first, 0, dp) << endl;  // 4
    cout << robBottomUp(first) << endl;     // 4
    cout << rob(first) << endl;             // 4

    cout << rob(second) << endl;  // 12

    vector<int> single = {5};
    cout << rob(single) << endl;  // 5

    vector<int> empty;
    cout << rob(empty) << endl;  // 0

    return 0;
}

// Special cases: empty array, one house, all houses have equal money,
// zero money houses, two adjacent houses (pick the larger)
