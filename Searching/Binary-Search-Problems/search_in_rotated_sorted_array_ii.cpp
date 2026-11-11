/**
 * Search in Rotated Sorted Array II
 *
 * There is an integer array nums sorted in non-decreasing order (it may contain
 * duplicates), then rotated at some pivot. Given nums and a target, return 1
 * if target exists, otherwise return 0.
 *
 * Example:
 * Input:  nums = [2, 5, 6, 0, 0, 1, 2], target = 0
 * Output: 1
 *
 * Input:  nums = [2, 5, 6, 0, 0, 1, 2], target = 3
 * Output: 0
 *
 * Constraints: 1 <= nums.length <= 5000, -10^4 <= nums[i], target <= 10^4.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: scan every element -> O(n)
bool searchBrute(vector<int> &nums, int target) {
    for (int value : nums) {
        if (value == target) {
            return true;
        }
    }

    return false;
}

// Optimized: binary search with duplicate handling.
// Average O(log n); O(n) in the worst case when duplicates hide the pivot.
bool search(vector<int> &nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return true;
        }

        // nums[left] == nums[mid] == nums[right]: we cannot tell which half is
        // sorted, so shrink both ends.
        if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
            left++;
            right--;
        } else if (nums[left] <= nums[mid]) {
            // The left half is sorted.
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            // The right half is sorted.
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    return false;
}

int main() {
    vector<int> first = {2, 5, 6, 0, 0, 1, 2};
    vector<int> second = {1, 0, 1, 1, 1};
    vector<int> third = {1, 3, 1, 1, 1};
    vector<int> allEqual = {1, 1, 1, 1};
    vector<int> single = {1};

    cout << searchBrute(first, 0) << " " << search(first, 0) << endl;  // 1 1
    cout << search(first, 3) << endl;                                  // 0

    cout << search(second, 0) << endl;    // 1
    cout << search(third, 3) << endl;     // 1
    cout << search(allEqual, 2) << endl;  // 0
    cout << search(single, 1) << endl;    // 1
    cout << search(single, 0) << endl;    // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n)
 *   binary search -> O(log n) average, O(n) worst case with many duplicates
 * Space complexity -> O(1)
 *
 * Special cases: no rotation (already sorted), one element, all elements equal,
 * the target is the pivot, duplicates at both ends, the target is smaller than
 * every element, and an empty array.
 *
 * The duplicate case is what separates this problem from the distinct version:
 * when nums[left] == nums[mid] == nums[right], there is no way to know which
 * half is sorted, so we must discard one element from each end.
 */
