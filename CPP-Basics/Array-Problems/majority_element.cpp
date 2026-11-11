/**
 * Majority Element
 *
 * Given an array nums of size n, return the majority element. The majority
 * element is the element that appears more than n / 2 times. The problem
 * guarantees that a majority element exists.
 *
 * Example:
 * Input:  [2, 2, 1, 1, 1, 2, 2]
 * Output: 2
 *
 * Constraints: 1 <= n <= 5 * 10^4, -10^9 <= nums[i] <= 10^9.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// Brute force: count every candidate -> O(n^2) time, O(1) space
int majorityElementBrute(vector<int> &nums) {
    int n = nums.size();

    for (int i = 0; i < n; i++) {
        int count = 0;

        for (int j = 0; j < n; j++) {
            if (nums[j] == nums[i]) {
                count++;
            }
        }

        if (count > n / 2) {
            return nums[i];
        }
    }

    return -1;
}

// Hash map: store frequencies -> O(n) time, O(n) space
int majorityElementHash(vector<int> &nums) {
    unordered_map<int, int> frequency;

    for (int value : nums) {
        frequency[value]++;

        if (frequency[value] > nums.size() / 2) {
            return value;
        }
    }

    return -1;
}

// Boyer-Moore voting: cancel pairs of different elements -> O(n), O(1) space
int majorityElement(vector<int> &nums) {
    int candidate = nums[0];
    int balance = 0;

    for (int value : nums) {
        if (balance == 0) {
            candidate = value;
            balance = 1;
        } else if (value == candidate) {
            balance++;
        } else {
            balance--;
        }
    }

    // The problem guarantees a majority, but verify to be safe.
    int count = 0;

    for (int value : nums) {
        if (value == candidate) {
            count++;
        }
    }

    return (count > nums.size() / 2) ? candidate : -1;
}

int main() {
    vector<int> first = {2, 2, 1, 1, 1, 2, 2};
    vector<int> second = {3, 2, 3};
    vector<int> third = {1};
    vector<int> noMajority = {1, 2, 3, 4};

    cout << majorityElementBrute(first) << endl;  // 2
    cout << majorityElementHash(first) << endl;   // 2
    cout << majorityElement(first) << endl;       // 2

    cout << majorityElement(second) << endl;      // 3
    cout << majorityElement(third) << endl;       // 1
    cout << majorityElement(noMajority) << endl;  // -1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   hash map -> O(n) time, O(n) space
 *   Boyer-Moore -> O(n) time, O(1) space
 *
 * Special cases: single element, all elements equal, no majority (-1 after
 * verification), negative values, large n (use the O(1) space version).
 */
