/**
 * Rod Cutting, Unbounded Knapsack, and Coin Change (Minimum Coins)
 *
 * Rod cutting:
 *   prices = [1,5,8,9,10,17,17,20], length = 8 -> 22
 *   (cut into lengths 2 and 6: 5 + 17)
 *
 * Unbounded knapsack:
 *   weights = [5,10,15], values = [10,30,20], capacity = 20 -> 60
 *   (take the weight-10 item twice)
 *
 * Coin change, minimum number of coins:
 *   coins = [1,2,5], amount = 11 -> 3   (5 + 5 + 1)
 *   coins = [2],     amount = 3  -> -1  (impossible)
 *
 * All three problems allow an item or coin to be used any number of times, so
 * their DP loops run in ascending order.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force rod cutting: try every possible first cut.
// O(2^(n-1)) time, O(n) recursion space.
long long rodCuttingBrute(const vector<int> &prices, int length) {
    if (length == 0) {
        return 0;
    }

    long long best = 0;

    for (int cut = 1; cut <= length; cut++) {
        best =
            max(best, prices[cut - 1] + rodCuttingBrute(prices, length - cut));
    }

    return best;
}

// Rod cutting DP: best[length] = max over the last cut.
// O(n^2) time, O(n) space.
long long rodCutting(const vector<int> &prices, int length) {
    vector<long long> best(length + 1, 0);

    for (int len = 1; len <= length; len++) {
        for (int cut = 1; cut <= len; cut++) {
            best[len] = max(best[len], prices[cut - 1] + best[len - cut]);
        }
    }

    return best[length];
}

// Unbounded knapsack: capacity loop ascending, so an item can be reused.
// O(n * capacity) time, O(capacity) space.
long long unboundedKnapsack(const vector<int> &weights,
                            const vector<int> &values, int capacity) {
    vector<long long> dp(capacity + 1, 0);

    for (int c = 1; c <= capacity; c++) {
        for (int i = 0; i < (int)weights.size(); i++) {
            if (weights[i] <= c) {
                dp[c] = max(dp[c], dp[c - weights[i]] + values[i]);
            }
        }
    }

    return dp[capacity];
}

// Coin change with the minimum number of coins, or -1 if it is impossible.
// O(coins * amount) time, O(amount) space.
int coinChange(const vector<int> &coins, int amount) {
    const int IMPOSSIBLE = amount + 1;
    vector<int> dp(amount + 1, IMPOSSIBLE);
    dp[0] = 0;

    for (int coin : coins) {
        for (int value = coin; value <= amount; value++) {
            dp[value] = min(dp[value], dp[value - coin] + 1);
        }
    }

    return dp[amount] == IMPOSSIBLE ? -1 : dp[amount];
}

int main() {
    vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20};

    cout << rodCuttingBrute(prices, 8) << " " << rodCutting(prices, 8) << endl;
    // 22 22

    vector<int> weights = {5, 10, 15};
    vector<int> values = {10, 30, 20};
    cout << unboundedKnapsack(weights, values, 20) << endl;  // 60

    vector<int> coins = {1, 2, 5};
    cout << coinChange(coins, 11) << endl;  // 3
    cout << coinChange({2}, 3) << endl;     // -1
    cout << coinChange({1}, 0) << endl;     // 0

    cout << rodCutting(prices, 0) << endl;                  // 0
    cout << unboundedKnapsack(weights, values, 0) << endl;  // 0
    cout << coinChange({5}, 3) << endl;  // -1 (coin too large)

    return 0;
}

/**
 * Complexity summary:
 *   rod cutting brute -> O(2^(n-1)) time, O(n) recursion space
 *   rod cutting DP    -> O(n^2) time, O(n) space
 *   unbounded knapsack -> O(n * capacity) time, O(capacity) space
 *   coin change       -> O(coins * amount) time, O(amount) space
 *
 * Edge cases: length 0, capacity 0, amount 0, a coin larger than the amount,
 * no coins, and an amount that cannot be formed.
 *
 * Ascending loops are correct here because reusing the current item is
 * allowed. In 0/1 knapsack the same loops must run descending.
 */
