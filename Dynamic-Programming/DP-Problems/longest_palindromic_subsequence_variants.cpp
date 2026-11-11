/**
 * LCS Variants: Longest Palindromic Subsequence and Shortest Common
 * Supersequence
 *
 * Longest palindromic subsequence (LPS):
 *   "bbbab" -> 4   ("bbbb")
 *   "cbbd"  -> 2   ("bb")
 *   "abcde" -> 1
 *
 * Shortest common supersequence (SCS):
 *   "abac" and "cab" -> length 5, for example "cabac"
 *   "abc"  and "abc" -> length 3, "abc"
 *   "abc"  and "def" -> length 6, "abcdef"
 *
 * Both problems reduce to the longest common subsequence:
 *   LPS(s) = LCS(s, reverse(s))
 *   |SCS(a, b)| = |a| + |b| - LCS(a, b)
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force LCS: at every position either skip a character or match one.
// O(2^(n + m)) time, O(n + m) recursion space.
int lcsBrute(const string &a, const string &b, int i, int j) {
    if (i == (int)a.size() || j == (int)b.size()) {
        return 0;
    }

    if (a[i] == b[j]) {
        return 1 + lcsBrute(a, b, i + 1, j + 1);
    }

    return max(lcsBrute(a, b, i + 1, j), lcsBrute(a, b, i, j + 1));
}

// Standard O(n * m) LCS.
int lcsLength(const string &a, const string &b) {
    int n = a.size();
    int m = b.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (a[i] == b[j]) {
                dp[i][j] = 1 + dp[i + 1][j + 1];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }

    return dp[0][0];
}

// LPS = LCS with the reversed string.
int longestPalindromicSubsequence(const string &s) {
    string reversed = s;
    reverse(reversed.begin(), reversed.end());
    return lcsLength(s, reversed);
}

// The SCS length follows directly from the LCS length.
int shortestCommonSupersequenceLength(const string &a, const string &b) {
    return a.size() + b.size() - lcsLength(a, b);
}

// Build one shortest common supersequence. The LCS table decides which
// character can be emitted while preserving both strings as subsequences.
string shortestCommonSupersequence(const string &a, const string &b) {
    int n = a.size();
    int m = b.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (a[i] == b[j]) {
                dp[i][j] = 1 + dp[i + 1][j + 1];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }

    string result;
    int i = 0;
    int j = 0;

    while (i < n && j < m) {
        if (a[i] == b[j]) {
            result += a[i];
            i++;
            j++;
        } else if (dp[i + 1][j] >= dp[i][j + 1]) {
            result += a[i];
            i++;
        } else {
            result += b[j];
            j++;
        }
    }

    while (i < n) {
        result += a[i++];
    }
    while (j < m) {
        result += b[j++];
    }

    return result;
}

// Checks that every character of "part" appears in "whole" in order.
bool isSubsequence(const string &part, const string &whole) {
    int index = 0;

    for (char c : whole) {
        if (index < (int)part.size() && part[index] == c) {
            index++;
        }
    }

    return index == (int)part.size();
}

int main() {
    string first = "abcde";
    string second = "ace";

    cout << lcsBrute(first, second, 0, 0) << " " << lcsLength(first, second)
         << endl;
    // 3 3

    cout << longestPalindromicSubsequence("bbbab") << endl;  // 4
    cout << longestPalindromicSubsequence("cbbd") << endl;   // 2
    cout << longestPalindromicSubsequence("abcde") << endl;  // 1
    cout << longestPalindromicSubsequence("") << endl;       // 0

    string a = "abac";
    string b = "cab";
    string scs = shortestCommonSupersequence(a, b);

    cout << shortestCommonSupersequenceLength(a, b) << " " << scs << endl;
    // 5 cabac

    cout << isSubsequence(a, scs) << isSubsequence(b, scs) << endl;
    // 11

    cout << shortestCommonSupersequenceLength("abc", "abc") << " "
         << shortestCommonSupersequence("abc", "abc") << endl;
    // 3 abc

    cout << shortestCommonSupersequenceLength("abc", "def") << " "
         << shortestCommonSupersequence("abc", "def") << endl;
    // 6 abcdef

    return 0;
}

/**
 * Complexity:
 *   brute LCS -> O(2^(n + m)) time, O(n + m) recursion space
 *   DP LCS    -> O(n * m) time and space
 *   LPS       -> O(n^2) time and space
 *   SCS       -> O(n * m) time and space
 *
 * Edge cases: one empty string, both empty, identical strings, no common
 * characters, repeated characters, and a single character.
 *
 * LPS uses the fact that a palindromic subsequence reads the same forwards and
 * backwards, so it is a common subsequence of s and reverse(s). The SCS
 * reconstruction emits one character per step and appends the unused suffix.
 */
