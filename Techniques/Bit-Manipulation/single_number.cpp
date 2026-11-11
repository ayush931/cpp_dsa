/**
 * Single Number
 *
 * Given a non-empty array of integers nums, every element appears twice except
 * for one. Find that single element. The solution must use O(1) extra space.
 *
 * Example:
 * Input:  [4, 1, 2, 1, 2]
 * Output: 4
 *
 * Constraints: 1 <= nums.length <= 3 * 10^4,
 * -3 * 10^4 <= nums[i] <= 3 * 10^4, every element appears twice except one.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// Brute force: count frequencies -> O(n) time, O(n) space
int singleNumberBrute(vector<int> &nums) {
    unordered_map<int, int> frequency;

    for (int value : nums) {
        frequency[value]++;
    }

    for (auto &entry : frequency) {
        if (entry.second == 1) {
            return entry.first;
        }
    }

    return 0;
}

// XOR: x ^ x = 0 and x ^ 0 = x, so pairs cancel out -> O(n), O(1)
int singleNumber(vector<int> &nums) {
    int result = 0;

    for (int value : nums) {
        result ^= value;
    }

    return result;
}

int main() {
    vector<int> first = {2, 2, 1};
    vector<int> second = {4, 1, 2, 1, 2};
    vector<int> third = {1};
    vector<int> fourth = {-1, -1, -2};

    cout << singleNumberBrute(first) << " " << singleNumber(first) << endl;
    // 1 1

    cout << singleNumber(second) << endl;  // 4
    cout << singleNumber(third) << endl;   // 1
    cout << singleNumber(fourth) << endl;  // -2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) time, O(n) space
 *   XOR -> O(n) time, O(1) space
 *
 * Special cases: one element, negative values, the single element is the
 * first or the last, duplicate values cancel regardless of order.
 *
 * Variants: if every element appears three times except one, count the bits
 * modulo 3; if two elements appear once, XOR everything and split by the
 * lowest set bit.
 */
