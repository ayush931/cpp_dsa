/**
 * 0/1 Knapsack, Subset Sum, and Partition Equal Subset Sum
 *
 * 0/1 knapsack:
 *   weights = [1,3,4,5], values = [1,4,5,7], capacity = 7 -> 9
 *   (take weights 3 and 4, values 4 + 5)
 *
 * Subset sum:
 *   nums = [3,34,4,12,5,2], target = 9  -> true  (4 + 5)
 *   nums = [3,34,4,12,5,2], target = 30 -> false
 *
 * Partition equal subset sum:
 *   [1,5,11,5] -> true  (11 and 1 + 5 + 5)
 *   [1,2,3,5]  -> false (the total is odd)
 *
 * Every item may be used at most once. The DP iterates the capacity in
 * descending order so an item is not reused.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// Brute force: include or skip every item.
// O(2^n) time, O(n) recursion space.
int knapsackBrute(const vector<int> &weights, const vector<int> &values,
                  int index, int capacity) {
    if (index == (int)weights.size()) {
        return 0;
    }

    int skip = knapsackBrute(weights, values, index + 1, capacity);
    int take = 0;

    if (weights[index] <= capacity) {
        take = values[index] + knapsackBrute(weights, values, index + 1,
                                             capacity - weights[index]);
    }

    return max(skip, take);
}

// 0/1 knapsack with a one-dimensional DP.
// O(n * capacity) time, O(capacity) space.
int knapsack01(const vector<int> &weights, const vector<int> &values,
               int capacity) {
    vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < (int)weights.size(); i++) {
        // Descending order: dp[c - weight] still belongs to the previous item
        // set.
        for (int c = capacity; c >= weights[i]; c--) {
            dp[c] = max(dp[c], dp[c - weights[i]] + values[i]);
        }
    }

    return dp[capacity];
}

// Subset sum: can some subset add up to target?
// O(n * target) time, O(target) space.
bool subsetSum(const vector<int> &nums, int target) {
    if (target < 0) {
        return false;
    }

    vector<char> reachable(target + 1, false);
    reachable[0] = true;

    for (int value : nums) {
        for (int sum = target; sum >= value; sum--) {
            if (reachable[sum - value]) {
                reachable[sum] = true;
            }
        }
    }

    return reachable[target];
}

// Partition into two subsets with equal sums. The total must be even, and one
// subset must reach total / 2.
// O(n * total) time, O(total) space.
bool canPartition(const vector<int> &nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);

    if (total % 2 != 0) {
        return false;
    }

    return subsetSum(nums, total / 2);
}

int main() {
    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};

    cout << knapsackBrute(weights, values, 0, 7) << " "
         << knapsack01(weights, values, 7) << endl;
    // 9 9

    vector<int> nums = {3, 34, 4, 12, 5, 2};
    cout << subsetSum(nums, 9) << " " << subsetSum(nums, 30) << endl;
    // 1 0

    vector<int> firstPartition = {1, 5, 11, 5};
    vector<int> secondPartition = {1, 2, 3, 5};
    vector<int> thirdPartition = {2, 2};

    cout << canPartition(firstPartition) << " " << canPartition(secondPartition)
         << " " << canPartition(thirdPartition) << endl;
    // 1 0 1

    cout << knapsack01(weights, values, 0) << endl;  // 0 (zero capacity)
    cout << knapsack01({}, {}, 10) << endl;          // 0 (no items)
    cout << subsetSum(nums, 0) << endl;              // 1 (empty subset)
    cout << subsetSum(nums, 100) << endl;            // 0 (too large)

    return 0;
}

/**
 * Complexity summary:
 *   knapsack brute -> O(2^n) time, O(n) recursion space
 *   knapsack DP    -> O(n * capacity) time, O(capacity) space
 *   subset sum     -> O(n * target) time, O(target) space
 *   partition      -> O(n * total) time, O(total) space
 *
 * Edge cases: zero capacity, no items, an item heavier than the capacity,
 * target 0, an odd total for partition, and negative target.
 *
 * The descending capacity loop is the key 0/1 detail. Ascending order would
 * allow the same item to be selected several times (that is the unbounded
 * knapsack recurrence, shown in the next file).
 */
