/**
 * Subsets II (Unique Subsets with Duplicates)
 *
 * Given an integer array nums that may contain duplicates, return all possible
 * subsets. The result must not contain duplicate subsets.
 *
 * Example:
 * Input:  [1, 2, 2]
 * Output: [], [1], [1,2], [1,2,2], [2], [2,2]
 *
 * Input:  [0, 0]
 * Output: [], [0], [0,0]
 *
 * Constraints: 1 <= nums.length <= 10, -10 <= nums[i] <= 10.
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// Brute force: generate all 2^n subsets, then let a set remove duplicates.
// O(n * 2^n) time and O(n * 2^n) extra space for the set.
void generateAll(vector<int> &nums, int index, vector<int> &current,
                 set<vector<int>> &uniqueSubsets) {
    if (index == nums.size()) {
        uniqueSubsets.insert(current);
        return;
    }

    generateAll(nums, index + 1, current, uniqueSubsets);  // exclude

    current.push_back(nums[index]);  // include
    generateAll(nums, index + 1, current, uniqueSubsets);
    current.pop_back();
}

vector<vector<int>> subsetsWithDupBrute(vector<int> &nums) {
    set<vector<int>> uniqueSubsets;
    vector<int> current;

    generateAll(nums, 0, current, uniqueSubsets);

    return vector<vector<int>>(uniqueSubsets.begin(), uniqueSubsets.end());
}

// Optimized: sort, then skip a duplicate value at the same recursion level.
// O(n * 2^n) time to produce the output, O(n) recursion space.
void generateUnique(vector<int> &nums, int start, vector<int> &current,
                    vector<vector<int>> &result) {
    result.push_back(current);

    for (int i = start; i < nums.size(); i++) {
        // The first occurrence at this level is enough; later equal values
        // would create the same subset again.
        if (i > start && nums[i] == nums[i - 1]) {
            continue;
        }

        current.push_back(nums[i]);
        generateUnique(nums, i + 1, current, result);
        current.pop_back();
    }
}

vector<vector<int>> subsetsWithDup(vector<int> &nums) {
    sort(nums.begin(), nums.end());

    vector<vector<int>> result;
    vector<int> current;

    generateUnique(nums, 0, current, result);

    return result;
}

void print(const vector<vector<int>> &subsets) {
    for (const vector<int> &subset : subsets) {
        cout << "[";
        for (int i = 0; i < subset.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << subset[i];
        }
        cout << "]\n";
    }
}

int main() {
    vector<int> first = {1, 2, 2};

    vector<vector<int>> brute = subsetsWithDupBrute(first);
    print(brute);
    // [][1][1,2][1,2,2][2][2,2]

    cout << "---\n";

    vector<vector<int>> optimized = subsetsWithDup(first);
    print(optimized);
    // [][1][1,2][1,2,2][2][2,2]

    cout << "---\n";

    vector<int> second = {0, 0};
    print(subsetsWithDup(second));
    // [][0][0,0]

    vector<int> empty;
    print(subsetsWithDup(empty));
    // []

    return 0;
}

/**
 * Time complexity -> O(n * 2^n): at most 2^n subsets, each copied in O(n)
 * Space complexity -> O(n) recursion depth plus the result
 *
 * Special cases: empty input (one empty subset), all values equal, no
 * duplicates, negative values, and duplicate values that are not adjacent
 * before sorting.
 */
