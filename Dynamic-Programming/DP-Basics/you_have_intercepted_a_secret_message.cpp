/**
 * You have intercepted a secret message encoded as a string of numbers. The
 * message is decoded via the following mapping: "1" -> 'A' "2" -> 'B' "25" ->
 * 'Y' "26" -> 'Z'
 *
 * However, while decoding the message, you realize that there are many
 * different ways you can decode the message because some codes are contained in
 * other codes ("2" and "5" vs "25").
 *
 * For example, "11106" can be decoded into:
 * "AAJF" with the grouping (1, 1, 10, 6)
 * "KJF" with the grouping (11, 10, 6)
 * The grouping (1, 11, 06) is invalid because "06" is not a valid code (only
 * "6" is valid).
 *
 * Given a string s containing only digits, return the number of ways to decode
 * it. If the entire string cannot be decoded in any valid way, return 0.
 *
 * Example 1: s = "12"  -> 2
 * Example 2: s = "226" -> 3
 * Example 3: s = "06"  -> 0
 *
 * Constraints: 1 <= s.length <= 100, s contains only digits.
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force recursion -> exponential
int decodeBrute(string &s, int index) {
    if (index == s.size()) {
        return 1;
    }
    if (s[index] == '0') {
        return 0;
    }

    int ways = decodeBrute(s, index + 1);

    if (index + 1 < s.size()) {
        int twoDigits = (s[index] - '0') * 10 + (s[index + 1] - '0');

        if (twoDigits >= 10 && twoDigits <= 26) {
            ways += decodeBrute(s, index + 2);
        }
    }

    return ways;
};

// Memoized recursion -> O(n) time, O(n) space
int decodeMemo(string &s, int index, vector<int> &dp) {
    if (index == s.size()) {
        return 1;
    }
    if (s[index] == '0') {
        return 0;
    }
    if (dp[index] != -1) {
        return dp[index];
    }

    int ways = decodeMemo(s, index + 1, dp);

    if (index + 1 < s.size()) {
        int twoDigits = (s[index] - '0') * 10 + (s[index + 1] - '0');

        if (twoDigits >= 10 && twoDigits <= 26) {
            ways += decodeMemo(s, index + 2, dp);
        }
    }

    return dp[index] = ways;
};

// Bottom-up DP -> O(n) time, O(n) space
int numDecodings(string s) {
    int n = s.size();
    if (n == 0 || s[0] == '0') {
        return 0;
    }

    vector<int> dp(n + 1, 0);
    dp[0] = 1;  // empty prefix has one decoding
    dp[1] = 1;  // first character is non-zero here

    for (int i = 2; i <= n; i++) {
        // use s[i - 1] as a single digit
        if (s[i - 1] != '0') {
            dp[i] += dp[i - 1];
        }

        // use s[i - 2] and s[i - 1] as a two digit number
        int twoDigits = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');

        if (twoDigits >= 10 && twoDigits <= 26) {
            dp[i] += dp[i - 2];
        }
    }

    return dp[n];
};

int main() {
    vector<string> inputs = {"12", "226", "06", "10", "100", "11106", "0"};

    for (string &s : inputs) {
        vector<int> dp(s.size() + 1, -1);
        cout << s << " -> " << decodeBrute(s, 0) << " " << decodeMemo(s, 0, dp)
             << " " << numDecodings(s) << endl;
    }

    // 12 -> 2 2 2
    // 226 -> 3 3 3
    // 06 -> 0 0 0
    // 10 -> 1 1 1
    // 100 -> 0 0 0
    // 11106 -> 2 2 2
    // 0 -> 0 0 0

    return 0;
}

// Special cases: leading zero ("06"), a zero in the middle ("100"),
// a zero that can only be part of a two digit code ("10"),
// consecutive zeros ("00"), single character, empty string
