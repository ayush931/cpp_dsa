/**
 * Partition Equal Subset Sum
 * Given an array of positive integers, check if it can be partitioned into
 * two subsets with equal sum.
 *
 * Input: nums = [1, 5, 11, 5]
 * Output: true (1 + 5 + 5 = 11)
 *
 * Brute force -> try every subset, O(2^n)
 * Optimized -> subset sum DP, O(n * target)
 */

#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

bool canPartitionBrute(vector<int> &nums, int index, int sum) {
    if (sum == 0) {
        return true;
    }
    if (index == nums.size() || sum < 0) {
        return false;
    }

    return canPartitionBrute(nums, index + 1, sum - nums[index]) ||
           canPartitionBrute(nums, index + 1, sum);
};

bool canPartition(vector<int> &nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);

    if (total % 2 != 0) {
        return false;
    }

    int target = total / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int sum = target; sum >= num; sum--) {
            dp[sum] = dp[sum] || dp[sum - num];
        }
    }

    return dp[target];
};

int main() {
    vector<int> nums = {1, 5, 11, 5};

    cout << canPartitionBrute(nums, 0, 11) << endl;  // 1
    cout << canPartition(nums) << endl;              // 1

    vector<int> odd = {1, 2, 3, 5};
    cout << canPartition(odd) << endl;  // 0

    return 0;
}

// Special cases: odd total, empty array, single element, zeros in the array
