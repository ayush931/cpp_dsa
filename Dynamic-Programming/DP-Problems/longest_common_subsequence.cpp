/**
 * Given two strings text1 and text2, return the length of their longest common
 * subsequence. If there is no common subsequence, return 0. A subsequence of a
 * string is a new string generated from the original string with some
 * characters (can be none) deleted without changing the relative order of the
 * remaining characters.
 *
 * Example 1: text1 = "abcde", text2 = "ace" -> 3 ("ace")
 * Example 2: text1 = "abc", text2 = "abc" -> 3
 * Example 3: text1 = "abc", text2 = "def" -> 0
 *
 * Constraints: 1 <= text1.length, text2.length <= 1000
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force recursion -> O(2^(n + m))
int lcsBrute(string &a, string &b, int i, int j) {
    if (i == a.size() || j == b.size()) {
        return 0;
    }

    if (a[i] == b[j]) {
        return 1 + lcsBrute(a, b, i + 1, j + 1);
    }

    return max(lcsBrute(a, b, i + 1, j), lcsBrute(a, b, i, j + 1));
};

// Bottom-up DP -> O(n * m) time and space
int longestCommonSubsequence(string text1, string text2) {
    int n = text1.size();
    int m = text2.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (text1[i] == text2[j]) {
                dp[i][j] = 1 + dp[i + 1][j + 1];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }

    return dp[0][0];
};

// Space optimized -> O(m) extra space
int longestCommonSubsequenceOptimized(string text1, string text2) {
    int n = text1.size();
    int m = text2.size();

    vector<int> next(m + 1, 0);
    vector<int> current(m + 1, 0);

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (text1[i] == text2[j]) {
                current[j] = 1 + next[j + 1];
            } else {
                current[j] = max(next[j], current[j + 1]);
            }
        }

        next = current;
    }

    return next[0];
};

int main() {
    string first = "abcde";
    string second = "ace";

    cout << lcsBrute(first, second, 0, 0) << endl;                     // 3
    cout << longestCommonSubsequence(first, second) << endl;           // 3
    cout << longestCommonSubsequenceOptimized(first, second) << endl;  // 3

    cout << longestCommonSubsequence("abc", "def") << endl;  // 0
    cout << longestCommonSubsequence("abc", "abc") << endl;  // 3
    cout << longestCommonSubsequence("", "abc") << endl;     // 0

    return 0;
}

// Special cases: one empty string, both empty, no common characters,
// identical strings, repeated characters, case sensitivity
