/**
 * Given an integer array of coins[] of size N representing different types of
 * currency and an integer sum. The task is to find the number of ways to make
 * sum by using different combinations from coins[]. Assume that you have an
 * infinite supply of each type of coin. Input1: sum = 4, coins[] = {1, 2, 3}
 * Output1: 4
 * Explanation: there are four solutions: {1, 1, 1, 1}, {1, 1, 2}, {2, 2}, {1,
 * 3}.
 */

#include <bits/stdc++.h>
using namespace std;

int fbu(vector<int> &coins, int x, int n) {
    vector<int> dp(1000005, 0);
    int MOD = 10000007;
    dp[0] = 1;
    for (int j = 0; j < n; j++) {
        // go to each coin
        for (int i = 1; i <= x; i++) {
            if (i - coins[j] < 0) {
                continue;
            }
            dp[i] = (dp[i] % MOD + dp[i - coins[j]] % MOD) % MOD;
        }
    }

    return dp[x];
};

int main() {
    int x, n;
    cin >> x >> n;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    cout << fbu(coins, x, n) << endl;

    return 0;
}
