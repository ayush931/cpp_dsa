/**
 * Contains Duplicate
 *
 * Given an integer array nums, return true if any value appears at least
 * twice, and false if every element is distinct.
 *
 * Example:
 * Input:  [1, 2, 3, 1]
 * Output: true
 *
 * Constraints: 1 <= nums.length <= 10^5, -10^9 <= nums[i] <= 10^9.
 */

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

// Brute force: compare every pair -> O(n^2) time, O(1) space
bool containsDuplicateBrute(vector<int> &nums) {
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            if (nums[i] == nums[j]) {
                return true;
            }
        }
    }

    return false;
}

// Sorting: duplicates become adjacent -> O(n log n) time, O(1) extra space
bool containsDuplicateSort(vector<int> nums) {
    sort(nums.begin(), nums.end());

    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] == nums[i - 1]) {
            return true;
        }
    }

    return false;
}

// Hash set: remember what has been seen -> O(n) time, O(n) space
bool containsDuplicate(vector<int> &nums) {
    unordered_set<int> seen;

    for (int value : nums) {
        if (seen.count(value)) {
            return true;
        }
        seen.insert(value);
    }

    return false;
}

int main() {
    vector<int> first = {1, 2, 3, 1};
    vector<int> second = {1, 2, 3, 4};
    vector<int> third = {1, 1, 1, 3, 3, 4, 3, 2, 4, 2};
    vector<int> single = {7};

    cout << containsDuplicateBrute(first) << endl;  // 1
    cout << containsDuplicateSort(first) << endl;   // 1
    cout << containsDuplicate(first) << endl;       // 1

    cout << containsDuplicate(second) << endl;  // 0
    cout << containsDuplicate(third) << endl;   // 1
    cout << containsDuplicate(single) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   sorting -> O(n log n) time, O(1) extra space (the copy costs O(n))
 *   hash set -> O(n) average time, O(n) space
 *
 * Special cases: one element, all equal, no duplicates, negative values,
 * values outside the 32-bit range (use long long if needed), very large n
 * (prefer the hash set).
 */
