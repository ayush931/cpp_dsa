/**
 * Maximum Subarray (Kadane's Algorithm)
 *
 * Given an integer array nums, find the contiguous subarray with the largest
 * sum and return its sum.
 *
 * Example:
 * Input:  [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 * Output: 6  (subarray [4, -1, 2, 1])
 *
 * Constraints: 1 <= nums.length <= 10^5, -10^4 <= nums[i] <= 10^4.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every subarray -> O(n^2) time, O(1) space
int maxSubarrayBrute(vector<int> &nums) {
    int best = INT_MIN;

    for (int start = 0; start < nums.size(); start++) {
        int sum = 0;

        for (int end = start; end < nums.size(); end++) {
            sum += nums[end];
            best = max(best, sum);
        }
    }

    return best;
}

// Kadane: either extend the previous subarray or start fresh -> O(n), O(1)
int maxSubarray(vector<int> &nums) {
    int currentSum = nums[0];
    int best = nums[0];

    for (int i = 1; i < nums.size(); i++) {
        currentSum = max(nums[i], currentSum + nums[i]);
        best = max(best, currentSum);
    }

    return best;
}

// Variant that also returns the subarray boundaries [left, right]
int maxSubarrayWithIndices(vector<int> &nums, int &left, int &right) {
    int currentSum = 0;
    int best = INT_MIN;
    int start = 0;

    left = 0;
    right = 0;

    for (int i = 0; i < nums.size(); i++) {
        currentSum += nums[i];

        if (currentSum > best) {
            best = currentSum;
            left = start;
            right = i;
        }

        if (currentSum < 0) {
            currentSum = 0;
            start = i + 1;
        }
    }

    return best;
}

int main() {
    vector<int> first = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    vector<int> second = {1};
    vector<int> third = {5, 4, -1, 7, 8};
    vector<int> allNegative = {-8, -3, -6, -2, -5, -4};

    cout << maxSubarrayBrute(first) << endl;  // 6
    cout << maxSubarray(first) << endl;       // 6

    cout << maxSubarray(second) << endl;       // 1
    cout << maxSubarray(third) << endl;        // 23
    cout << maxSubarray(allNegative) << endl;  // -2

    int left, right;
    cout << maxSubarrayWithIndices(first, left, right) << " ";
    cout << left << " " << right << endl;  // 6 3 6

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   Kadane -> O(n) time, O(1) space
 *
 * Special cases: all negative values (return the least negative element),
 * one element, all positive values (return the whole array), zeros, the best
 * subarray at the beginning or the end, very large sums (use long long).
 */
