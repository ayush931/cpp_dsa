/**
 * Divide and conquer -> divide the problem into smaller subproblems,
 * solve them recursively, then combine their answers
 * Steps -> divide, conquer, combine
 * Used by merge sort, quick sort, binary search and the maximum subarray
 * problem Time is often written as T(n) = aT(n / b) + f(n), solved by the
 * master theorem
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int maxCrossingSum(vector<int> &arr, int low, int mid, int high) {
    int sum = 0;
    int leftSum = INT_MIN;

    for (int i = mid; i >= low; i--) {
        sum += arr[i];
        leftSum = max(leftSum, sum);
    }

    sum = 0;
    int rightSum = INT_MIN;

    for (int i = mid + 1; i <= high; i++) {
        sum += arr[i];
        rightSum = max(rightSum, sum);
    }

    return leftSum + rightSum;
};

int maxSubarraySum(vector<int> &arr, int low, int high) {
    // base case
    if (low == high) {
        return arr[low];
    }

    int mid = low + (high - low) / 2;

    int leftMax = maxSubarraySum(arr, low, mid);
    int rightMax = maxSubarraySum(arr, mid + 1, high);
    int crossMax = maxCrossingSum(arr, low, mid, high);

    return max({leftMax, rightMax, crossMax});
};

int main() {
    vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    cout << maxSubarraySum(arr, 0, arr.size() - 1) << endl;  // 6

    return 0;
}

// Time complexity -> O(n log n)
// Space complexity -> O(log n) due to recursion
