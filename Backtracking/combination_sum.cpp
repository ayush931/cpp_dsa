/**
 * Combination Sum
 *
 * Given an array of distinct integers candidates and a target integer target,
 * return all unique combinations of candidates where the chosen numbers sum
 * to target. You may use the same number an unlimited number of times.
 *
 * Example:
 * Input:  candidates = [2, 3, 6, 7], target = 7
 * Output: [[2, 2, 3], [7]]
 *
 * Constraints: 1 <= candidates.length <= 30, 2 <= candidates[i] <= 40,
 * 1 <= target <= 40, all candidates are distinct.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every subset, then keep the ones that sum to target.
// This can generate the same combination in different orders, so a set is
// needed to remove duplicates. -> O(2^n * n)
void subsetsBrute(vector<int> &candidates, int index, int sum, int target,
                  vector<int> &current, vector<vector<int>> &result) {
    if (sum == target) {
        result.push_back(current);
        return;
    }

    if (sum > target || index == candidates.size()) {
        return;
    }

    // Skip this candidate
    subsetsBrute(candidates, index + 1, sum, target, current, result);

    // Take this candidate, and allow taking it again
    current.push_back(candidates[index]);
    subsetsBrute(candidates, index, sum + candidates[index], target, current,
                 result);
    current.pop_back();
}

vector<vector<int>> combinationSumBrute(vector<int> &candidates, int target) {
    vector<vector<int>> result;
    vector<int> current;

    subsetsBrute(candidates, 0, 0, target, current, result);

    return result;
}

void generate(vector<int> &candidates, int start, int remaining,
              vector<int> &current, vector<vector<int>> &result) {
    if (remaining == 0) {
        result.push_back(current);
        return;
    }

    for (int i = start; i < candidates.size(); i++) {
        if (candidates[i] > remaining) {
            break;  // sorted, so later values are larger
        }

        current.push_back(candidates[i]);
        generate(candidates, i, remaining - candidates[i], current, result);
        current.pop_back();
    }
}

// Optimized: sort + start index + prune -> O(n^(target / minCandidate))
vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
    sort(candidates.begin(), candidates.end());

    vector<vector<int>> result;
    vector<int> current;

    generate(candidates, 0, target, current, result);

    return result;
}

void print(vector<vector<int>> &combinations) {
    for (vector<int> &combination : combinations) {
        cout << "[ ";

        for (int value : combination) {
            cout << value << " ";
        }

        cout << "]" << endl;
    }
}

int main() {
    vector<int> first = {2, 3, 6, 7};
    vector<int> second = {2, 3, 5};
    vector<int> third = {2};

    vector<vector<int>> brute = combinationSumBrute(first, 7);
    print(brute);  // [ 2 2 3 ] [ 7 ]

    vector<vector<int>> optimized = combinationSum(first, 7);
    print(optimized);  // [ 2 2 3 ] [ 7 ]

    vector<vector<int>> eight = combinationSum(second, 8);
    print(eight);  // [ 2 2 2 2 ] [ 2 3 3 ] [ 3 5 ]

    vector<vector<int>> none = combinationSum(third, 1);
    print(none);  // no output

    return 0;
}

/**
 * Time complexity -> exponential in target / smallest candidate; the sorted
 * break prunes most branches.
 * Space complexity -> O(target / smallest candidate) recursion depth.
 *
 * Special cases: target smaller than every candidate (empty result), target
 * exactly one candidate, one candidate, repeated use of the same number,
 * duplicate candidates (sort and skip duplicates at the same level).
 */
