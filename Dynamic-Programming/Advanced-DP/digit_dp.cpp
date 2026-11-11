/**
 * Digit DP -> counts numbers in a range that satisfy a digit based property
 * State -> dp[position][state][tight]
 * tight tells whether the current prefix is equal to the prefix of the upper
 * bound
 *
 * Problem: count numbers from 0 to N whose digit sum is exactly S.
 *
 * Brute force -> check every number, O(N * digits)
 * Optimized -> digit DP, O(digits * S * 2 * 10)
 */

#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int countBrute(int n, int sum) {
    int count = 0;

    for (int i = 0; i <= n; i++) {
        int current = i;
        int digitSum = 0;

        while (current > 0) {
            digitSum += current % 10;
            current /= 10;
        }

        if (digitSum == sum) {
            count++;
        }
    }

    return count;
};

int countDigitSum(int n, int sum) {
    if (n < 0 || sum < 0) {
        return 0;
    }

    string digits = to_string(n);
    int length = digits.size();

    // No number with this many digits can reach a larger digit sum.
    if (sum > 9 * length) {
        return 0;
    }

    vector<vector<vector<int>>> dp(
        length, vector<vector<int>>(sum + 1, vector<int>(2, -1)));

    function<int(int, int, bool)> solve = [&](int position, int remaining,
                                              bool tight) {
        if (remaining < 0) {
            return 0;
        }
        if (position == length) {
            return (remaining == 0) ? 1 : 0;
        }

        if (dp[position][remaining][tight] != -1) {
            return dp[position][remaining][tight];
        }

        int limit = tight ? digits[position] - '0' : 9;
        int count = 0;

        for (int digit = 0; digit <= limit; digit++) {
            count +=
                solve(position + 1, remaining - digit, tight && digit == limit);
        }

        return dp[position][remaining][tight] = count;
    };

    return solve(0, sum, true);
};

int main() {
    cout << countBrute(100, 5) << endl;     // 6
    cout << countDigitSum(100, 5) << endl;  // 6

    cout << countDigitSum(1000, 0) << endl;    // 1 (only 0)
    cout << countDigitSum(100, -1) << endl;    // 0 (negative sum)
    cout << countDigitSum(100, 1000) << endl;  // 0 (larger than 9 * digits)

    return 0;
}

// Special cases: N < 0, sum = 0, sum > 9 * digits, leading zeros
// Range [L, R] -> countDigitSum(R) - countDigitSum(L - 1)
