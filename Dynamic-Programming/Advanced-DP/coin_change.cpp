/**
 * Coin Change
 * Given coin denominations and an amount, return the minimum number of coins
 * needed to make the amount. Return -1 if it is not possible.
 *
 * Input: coins = [1, 2, 5], amount = 11
 * Output: 3 (5 + 5 + 1)
 *
 * Brute force -> try every coin recursively, O(amount^coins)
 * Optimized -> DP, O(amount * coins)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int coinChangeBrute(const vector<int> &coins, int amount) {
    if (amount == 0) {
        return 0;
    }
    if (amount < 0) {
        return INT_MAX;
    }

    int minimum = INT_MAX;

    for (int coin : coins) {
        int result = coinChangeBrute(coins, amount - coin);

        if (result != INT_MAX) {
            minimum = min(minimum, result + 1);
        }
    }

    return minimum;
};

int coinChange(const vector<int> &coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (coin <= i && dp[i - coin] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }

    return (dp[amount] == INT_MAX) ? -1 : dp[amount];
};

int main() {
    vector<int> coins = {1, 2, 5};

    int brute = coinChangeBrute(coins, 11);
    cout << ((brute == INT_MAX) ? -1 : brute) << endl;  // 3
    cout << coinChange(coins, 11) << endl;              // 3
    cout << coinChange(coins, 3) << endl;               // 2
    cout << coinChange({2}, 3) << endl;                 // -1

    return 0;
}

// Special cases: amount 0, impossible amount, coin value 0, large amount
// Number of ways (not minimum coins) -> dp[i] += dp[i - coin]
