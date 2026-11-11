/**
 * Subsets (Power Set)
 *
 * Given an integer array nums of unique elements, return all possible subsets
 * (the power set). The solution set must not contain duplicate subsets.
 *
 * Example:
 * Input:  [1, 2, 3]
 * Output: [[], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]]
 *
 * Constraints: 1 <= nums.length <= 10, -10 <= nums[i] <= 10,
 * all elements are unique.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: use the bits of a mask -> O(n * 2^n)
vector<vector<int>> subsetsBrute(vector<int> &nums) {
    int total = 1 << nums.size();
    vector<vector<int>> result;

    for (int mask = 0; mask < total; mask++) {
        vector<int> subset;

        for (int bit = 0; bit < nums.size(); bit++) {
            if (mask & (1 << bit)) {
                subset.push_back(nums[bit]);
            }
        }

        result.push_back(subset);
    }

    return result;
}

void generate(vector<int> &nums, int index, vector<int> &current,
              vector<vector<int>> &result) {
    if (index == nums.size()) {
        result.push_back(current);
        return;
    }

    // Exclude nums[index]
    generate(nums, index + 1, current, result);

    // Include nums[index]
    current.push_back(nums[index]);
    generate(nums, index + 1, current, result);
    current.pop_back();
}

// Backtracking: include/exclude every element -> O(n * 2^n)
vector<vector<int>> subsets(vector<int> &nums) {
    vector<vector<int>> result;
    vector<int> current;

    generate(nums, 0, current, result);

    return result;
}

void print(vector<vector<int>> &subsets) {
    for (vector<int> &subset : subsets) {
        cout << "[ ";

        for (int value : subset) {
            cout << value << " ";
        }

        cout << "]" << endl;
    }
}

int main() {
    vector<int> first = {1, 2, 3};
    vector<int> second = {0};
    vector<int> third = {};

    vector<vector<int>> brute = subsetsBrute(first);
    print(brute);  // 8 subsets

    vector<vector<int>> optimized = subsets(first);
    print(optimized);  // 8 subsets

    vector<vector<int>> one = subsets(second);
    print(one);  // [ ] and [ 0 ]

    vector<vector<int>> empty = subsets(third);
    print(empty);  // [ ]

    return 0;
}

/**
 * Time complexity:
 *   bitmask -> O(n * 2^n)
 *   backtracking -> O(n * 2^n)
 * Space -> O(n) recursion depth, plus O(n * 2^n) for the result
 *
 * Special cases: empty array (one empty subset), one element, all elements
 * included, duplicate values (sort first and skip duplicates at the same
 * level to avoid duplicate subsets), negative values.
 */
