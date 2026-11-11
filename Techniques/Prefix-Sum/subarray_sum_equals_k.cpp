/**
 * Subarray Sum Equals K
 * Given an array of integers and an integer k, return the number of
 * contiguous subarrays whose sum equals k.
 *
 * Input: nums = [1, 1, 1], k = 2
 * Output: 2
 *
 * Brute force -> check every subarray, O(n^2)
 * Optimized -> prefix sum + hash map, O(n)
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int subarraySumBrute(vector<int> &nums, int k) {
    int count = 0;

    for (int i = 0; i < nums.size(); i++) {
        int sum = 0;

        for (int j = i; j < nums.size(); j++) {
            sum += nums[j];
            if (sum == k) {
                count++;
            }
        }
    }

    return count;
};

int subarraySum(vector<int> &nums, int k) {
    unordered_map<long long, int> prefixCount;
    prefixCount[0] = 1;

    long long prefixSum = 0;
    int count = 0;

    for (int num : nums) {
        prefixSum += num;

        // prefixSum - k exists -> the subarray between that prefix and now sums
        // to k
        if (prefixCount.count(prefixSum - k)) {
            count += prefixCount[prefixSum - k];
        }

        prefixCount[prefixSum]++;
    }

    return count;
};

int main() {
    vector<int> nums = {1, 1, 1};
    cout << subarraySumBrute(nums, 2) << endl;  // 2
    cout << subarraySum(nums, 2) << endl;       // 2

    vector<int> negative = {1, -1, 0};
    cout << subarraySum(negative, 0) << endl;  // 3

    return 0;
}

// Special cases: negative numbers, zero values, k = 0, empty array
