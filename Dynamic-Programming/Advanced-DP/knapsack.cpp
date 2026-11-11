/**
 * 0/1 Knapsack
 * Given weights, values and capacity, choose items (each at most once)
 * to maximize value without exceeding capacity.
 *
 * Brute force -> try include/exclude every item, O(2^n)
 * Memoization -> O(n * capacity)
 * Tabulation -> O(n * capacity) time, O(capacity) space
 */

#include <iostream>
#include <vector>
using namespace std;

int knapsackBrute(vector<int> &weights, vector<int> &values, int index,
                  int capacity) {
    if (index == weights.size() || capacity == 0) {
        return 0;
    }

    if (weights[index] > capacity) {
        return knapsackBrute(weights, values, index + 1, capacity);
    }

    int include = values[index] + knapsackBrute(weights, values, index + 1,
                                                capacity - weights[index]);
    int exclude = knapsackBrute(weights, values, index + 1, capacity);

    return max(include, exclude);
};

int knapsackMemo(vector<int> &weights, vector<int> &values, int index,
                 int capacity, vector<vector<int>> &dp) {
    if (index == weights.size() || capacity == 0) {
        return 0;
    }

    if (dp[index][capacity] != -1) {
        return dp[index][capacity];
    }

    if (weights[index] > capacity) {
        return dp[index][capacity] =
                   knapsackMemo(weights, values, index + 1, capacity, dp);
    }

    int include = values[index] + knapsackMemo(weights, values, index + 1,
                                               capacity - weights[index], dp);
    int exclude = knapsackMemo(weights, values, index + 1, capacity, dp);

    return dp[index][capacity] = max(include, exclude);
};

int knapsackDP(vector<int> &weights, vector<int> &values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < n; i++) {
        // iterate backwards so each item is used at most once
        for (int w = capacity; w >= weights[i]; w--) {
            dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }

    return dp[capacity];
};

int main() {
    vector<int> weights = {1, 3, 4, 5};
    vector<int> values = {1, 4, 5, 7};
    int capacity = 7;

    cout << knapsackBrute(weights, values, 0, capacity) << endl;  // 9

    vector<vector<int>> dp(weights.size(), vector<int>(capacity + 1, -1));
    cout << knapsackMemo(weights, values, 0, capacity, dp) << endl;  // 9
    cout << knapsackDP(weights, values, capacity) << endl;           // 9

    return 0;
}

// Special cases: capacity 0, zero weight items, empty arrays
// Unbounded knapsack -> iterate w forward instead of backward
