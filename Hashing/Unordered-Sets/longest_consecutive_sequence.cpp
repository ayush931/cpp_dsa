/**
 * Longest Consecutive Sequence
 *
 * Given an unsorted array of integers nums, return the length of the longest
 * consecutive elements sequence. The algorithm must run in O(n) time.
 *
 * Example:
 * Input:  [100, 4, 200, 1, 3, 2]
 * Output: 4  (the sequence 1, 2, 3, 4)
 *
 * Constraints: 0 <= nums.length <= 10^5, -10^9 <= nums[i] <= 10^9.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

// Brute force: sort, then scan runs -> O(n log n) time
int longestConsecutiveSort(vector<int> nums) {
    if (nums.empty()) {
        return 0;
    }

    sort(nums.begin(), nums.end());

    int best = 1;
    int current = 1;

    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] == nums[i - 1]) {
            continue;  // duplicates do not extend the run
        } else if (nums[i] == nums[i - 1] + 1) {
            current++;
        } else {
            current = 1;
        }

        best = max(best, current);
    }

    return best;
}

// Optimized: only start counting at the beginning of a run -> O(n) average
int longestConsecutive(vector<int> &nums) {
    unordered_set<int> values(nums.begin(), nums.end());
    int best = 0;

    for (int value : values) {
        // Start only if value - 1 is absent, otherwise this is the middle of a
        // run. The INT_MIN / INT_MAX guards avoid signed overflow at the
        // boundaries.
        if (value != INT_MIN && values.count(value - 1)) {
            continue;
        }

        int current = value;
        int length = 1;

        while (current != INT_MAX && values.count(current + 1)) {
            current++;
            length++;
        }

        best = max(best, length);
    }

    return best;
}

int main() {
    vector<int> first = {100, 4, 200, 1, 3, 2};
    vector<int> second = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    vector<int> third = {};
    vector<int> fourth = {1, 2, 0, 1};
    vector<int> fifth = {9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6};

    cout << longestConsecutiveSort(first) << endl;  // 4
    cout << longestConsecutive(first) << endl;      // 4

    cout << longestConsecutive(second) << endl;  // 9
    cout << longestConsecutive(third) << endl;   // 0
    cout << longestConsecutive(fourth) << endl;  // 3
    cout << longestConsecutive(fifth) << endl;   // 7 (3, 4, 5, 6, 7, 8, 9)

    return 0;
}

/**
 * Time complexity:
 *   sorting -> O(n log n)
 *   hash set -> O(n) average time, O(n) space
 *
 * Special cases: empty array, one element, duplicates, negative values,
 * already consecutive input, values that are far apart, and runs touching
 * INT_MIN / INT_MAX (guarded against signed overflow).
 */
