/**
 * Missing Number
 *
 * Given an array nums containing n distinct numbers in the range [0, n],
 * return the only number in the range that is missing from the array.
 *
 * Example:
 * Input:  [3, 0, 1]
 * Output: 2
 *
 * Input:  [9, 6, 4, 2, 3, 5, 7, 0, 1]
 * Output: 8
 *
 * Constraints: 1 <= n <= 10^4, all numbers are distinct.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: check every value in [0, n] with a linear scan -> O(n^2)
int missingNumberBrute(vector<int> &nums) {
    int n = nums.size();

    for (int candidate = 0; candidate <= n; candidate++) {
        bool found = false;

        for (int value : nums) {
            if (value == candidate) {
                found = true;
                break;
            }
        }

        if (!found) {
            return candidate;
        }
    }

    return -1;
}

// Sum formula: expected sum - actual sum -> O(n) time, O(1) space
int missingNumberSum(vector<int> &nums) {
    long long n = nums.size();
    long long expected = n * (n + 1) / 2;
    long long actual = 0;

    for (int value : nums) {
        actual += value;
    }

    return (int)(expected - actual);
}

// XOR: index ^ value cancels every present number -> O(n), O(1)
int missingNumber(vector<int> &nums) {
    int result = nums.size();

    for (int i = 0; i < nums.size(); i++) {
        result ^= i;
        result ^= nums[i];
    }

    return result;
}

int main() {
    vector<int> first = {3, 0, 1};
    vector<int> second = {0, 1};
    vector<int> third = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    vector<int> fourth = {0};

    cout << missingNumberBrute(first) << " " << missingNumberSum(first) << " "
         << missingNumber(first) << endl;  // 2 2 2

    cout << missingNumber(second) << endl;  // 2
    cout << missingNumber(third) << endl;   // 8
    cout << missingNumber(fourth) << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   sum -> O(n) time, O(1) space
 *   XOR -> O(n) time, O(1) space
 *
 * Special cases: the missing number is 0, the missing number is n, one
 * element, the array is sorted or unsorted, large n (use long long for the
 * sum to avoid overflow), duplicate values (the problem guarantees distinct).
 */
