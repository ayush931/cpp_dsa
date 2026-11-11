/**
 * Given an integer array nums, return the length of the longest strictly
 * increasing subsequence.
 *
 * Example 1: nums = [10,9,2,5,3,7,101,18] -> 4 ([2,3,7,101])
 * Example 2: nums = [0,1,0,3,2,3] -> 4
 * Example 3: nums = [7,7,7,7,7,7,7] -> 1
 *
 * Constraints: 1 <= nums.length <= 2500, -10^4 <= nums[i] <= 10^4
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force recursion -> O(2^n)
int lisBrute(vector<int> &nums, int index, int previousIndex) {
    if (index == nums.size()) {
        return 0;
    }

    int skip = lisBrute(nums, index + 1, previousIndex);
    int take = 0;

    if (previousIndex == -1 || nums[index] > nums[previousIndex]) {
        take = 1 + lisBrute(nums, index + 1, index);
    }

    return max(skip, take);
};

// DP -> O(n^2) time, O(n) space
int lengthOfLIS(vector<int> &nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 1);
    int answer = 1;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }

        answer = max(answer, dp[i]);
    }

    return answer;
};

// Patience sorting + binary search -> O(n log n)
int lengthOfLISOptimized(vector<int> &nums) {
    vector<int> tails;

    for (int value : nums) {
        // lower_bound keeps the sequence strictly increasing
        auto it = lower_bound(tails.begin(), tails.end(), value);

        if (it == tails.end()) {
            tails.push_back(value);
        } else {
            *it = value;
        }
    }

    return tails.size();
};

int main() {
    vector<int> first = {10, 9, 2, 5, 3, 7, 101, 18};
    vector<int> second = {0, 1, 0, 3, 2, 3};
    vector<int> third = {7, 7, 7, 7};

    cout << lisBrute(first, 0, -1) << endl;       // 4
    cout << lengthOfLIS(first) << endl;           // 4
    cout << lengthOfLISOptimized(first) << endl;  // 4

    cout << lengthOfLIS(second) << endl;           // 4
    cout << lengthOfLISOptimized(second) << endl;  // 4

    cout << lengthOfLIS(third) << endl;           // 1
    cout << lengthOfLISOptimized(third) << endl;  // 1

    return 0;
}

// Special cases: empty array, one element, strictly decreasing array,
// all equal elements, negative values, duplicates (strictly increasing means
// duplicates do not extend the subsequence)
