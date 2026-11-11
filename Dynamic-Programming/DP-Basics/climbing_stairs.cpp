/**
 * Climbing Stairs
 *
 * You are climbing a staircase with n steps. Each time you can climb either
 * 1 or 2 steps. In how many distinct ways can you climb to the top?
 *
 * Example:
 * Input:  n = 3
 * Output: 3  (1+1+1, 1+2, 2+1)
 *
 * Constraints: 1 <= n <= 45.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every choice -> O(2^n)
int climbStairsBrute(int n) {
    if (n <= 1) {
        return 1;
    }

    return climbStairsBrute(n - 1) + climbStairsBrute(n - 2);
}

// Memoized recursion -> O(n) time, O(n) space
int climbMemo(int n, vector<int> &dp) {
    if (n <= 1) {
        return 1;
    }
    if (dp[n] != -1) {
        return dp[n];
    }

    return dp[n] = climbMemo(n - 1, dp) + climbMemo(n - 2, dp);
}

// Bottom-up DP -> O(n) time, O(n) space
int climbStairsDP(int n) {
    if (n <= 1) {
        return 1;
    }

    vector<int> dp(n + 1);
    dp[0] = 1;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    return dp[n];
}

// Optimized: only the last two values are needed -> O(n) time, O(1) space
int climbStairs(int n) {
    if (n <= 1) {
        return 1;
    }

    int previousTwo = 1;  // ways to reach step 0
    int previousOne = 1;  // ways to reach step 1

    for (int step = 2; step <= n; step++) {
        int current = previousOne + previousTwo;
        previousTwo = previousOne;
        previousOne = current;
    }

    return previousOne;
}

int main() {
    cout << climbStairsBrute(5) << endl;  // 8
    cout << climbStairsDP(5) << endl;     // 8
    cout << climbStairs(5) << endl;       // 8

    vector<int> dp(6, -1);
    cout << climbMemo(5, dp) << endl;  // 8

    cout << climbStairs(1) << endl;   // 1
    cout << climbStairs(2) << endl;   // 2
    cout << climbStairs(45) << endl;  // 1836311903

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^n)
 *   memo / bottom-up -> O(n) time, O(n) space
 *   optimized -> O(n) time, O(1) space
 *
 * Special cases: n = 1 (one way), n = 2 (two ways), large n (the answer fits
 * in 32-bit int up to n = 45), negative n (not allowed by the constraints).
 *
 * This is the Fibonacci sequence shifted by one position.
 */
