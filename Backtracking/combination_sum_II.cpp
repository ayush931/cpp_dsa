/**
 * Given a collection of candidate numbers (candidates) and a target number
 * (target), find all unique combinations in candidates where the candidate
 * numbers sum to target.
 *
 * Each number in candidates may only be used once in the combination.
 * The solution set must not contain duplicate combinations.
 *
 * Example 1:
 * Input: candidates = [10,1,2,7,6,1,5], target = 8
 * Output: [[1,1,6],[1,2,5],[1,7],[2,6]]
 *
 * Example 2:
 * Input: candidates = [2,5,2,1,2], target = 5
 * Output: [[1,2,2],[5]]
 *
 * Constraints: 1 <= candidates.length <= 100, 1 <= candidates[i] <= 50,
 * 1 <= target <= 30
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
   public:
    vector<vector<int>> result;
    vector<int> current;

    void f(vector<int> &candidates, int index, int target) {
        if (target == 0) {
            result.push_back(current);
            return;
        }

        for (int i = index; i < candidates.size(); i++) {
            // skip duplicates at the same recursion level
            if (i > index && candidates[i] == candidates[i - 1]) {
                continue;
            }

            // the array is sorted, so every later value is too large
            if (candidates[i] > target) {
                break;
            }

            current.push_back(candidates[i]);
            f(candidates, i + 1, target - candidates[i]);
            current.pop_back();
        }
    };

    vector<vector<int>> combinationSum2(vector<int> &candidates, int target) {
        result.clear();
        current.clear();
        sort(candidates.begin(), candidates.end());
        f(candidates, 0, target);
        return result;
    }
};

int main() {
    vector<int> first = {10, 1, 2, 7, 6, 1, 5};
    vector<int> second = {2, 5, 2, 1, 2};

    Solution solution;
    vector<vector<int>> result = solution.combinationSum2(first, 8);

    for (vector<int> &combination : result) {
        for (int value : combination) {
            cout << value << " ";
        }
        cout << endl;
    }

    result = solution.combinationSum2(second, 5);

    for (vector<int> &combination : result) {
        for (int value : combination) {
            cout << value << " ";
        }
        cout << endl;
    }

    return 0;
}

// Time -> O(2^n) in the worst case, pruned by sorting and breaking early
// Space -> O(n) for the recursion and current combination
// Special cases: target smaller than every candidate, target 0 (the empty
// combination), duplicate candidates, no valid combination, negative values
// (sorting and break would need adjustment)
