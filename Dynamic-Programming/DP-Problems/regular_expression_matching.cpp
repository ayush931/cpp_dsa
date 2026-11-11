/**
 * Regular Expression Matching
 *
 * Given an input string s and a pattern p, implement matching with support for
 * '.' (any single character) and '*' (zero or more of the preceding element).
 * The match must cover the entire string.
 *
 * Example 1:
 * Input: s = "aa", p = "a"
 * Output: false
 *
 * Example 2:
 * Input: s = "aa", p = "a*"
 * Output: true
 *
 * Example 3:
 * Input: s = "ab", p = ".*"
 * Output: true
 *
 * Constraints: 1 <= s.length <= 20, 1 <= p.length <= 20,
 * s contains lowercase letters, p contains lowercase letters, '.', and '*'.
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force: recursion over the string and the pattern.
// O(2^(n + m)) in the worst case, O(n + m) recursion space.
bool isMatchBrute(const string &s, const string &p, int i, int j) {
    if (j == p.size()) {
        return i == s.size();
    }

    // Does the current pattern character match the current string character?
    bool currentMatches = (i < s.size()) && (p[j] == '.' || p[j] == s[i]);

    if (j + 1 < p.size() && p[j + 1] == '*') {
        // Either use zero copies of p[j], or consume one character and stay on
        // the same pattern position.
        return isMatchBrute(s, p, i, j + 2) ||
               (currentMatches && isMatchBrute(s, p, i + 1, j));
    }

    if (currentMatches) {
        return isMatchBrute(s, p, i + 1, j + 1);
    }

    return false;
}

// Optimized: bottom-up DP. dp[i][j] is true when s[i:] matches p[j:].
// O(n * m) time, O(n * m) space.
bool isMatch(const string &s, const string &p) {
    int n = s.size();
    int m = p.size();

    vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
    dp[n][m] = true;  // empty string matches empty pattern

    for (int i = n; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            bool currentMatches = (i < n) && (p[j] == '.' || p[j] == s[i]);

            if (j + 1 < m && p[j + 1] == '*') {
                dp[i][j] = dp[i][j + 2] || (currentMatches && dp[i + 1][j]);
            } else if (currentMatches) {
                dp[i][j] = dp[i + 1][j + 1];
            }
        }
    }

    return dp[0][0];
}

void check(const string &s, const string &p) {
    cout << "\"" << s << "\" vs \"" << p << "\" -> " << isMatchBrute(s, p, 0, 0)
         << " " << isMatch(s, p) << endl;
}

int main() {
    check("aa", "a");                    // 0 0
    check("aa", "a*");                   // 1 1
    check("ab", ".*");                   // 1 1
    check("aab", "c*a*b");               // 1 1
    check("mississippi", "mis*is*p*.");  // 0 0
    check("", "a*");                     // 1 1
    check("", ".*");                     // 1 1
    check("abc", "a.c");                 // 1 1
    check("abc", "a*");                  // 0 0
    check("aaa", "a*a");                 // 1 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^(n + m)) worst case, because a star can branch at every step
 *   DP -> O(n * m), every state is computed once
 * Space complexity -> O(n * m) for the DP table, O(n + m) recursion for brute
 *
 * Special cases: an empty string with a pattern like "a*" (true, zero copies),
 * an empty string with "." (false), a star at the start (invalid by the
 * constraints), consecutive stars (invalid), a pattern that is longer than the
 * string, and a pattern that matches a prefix but not the whole string.
 *
 * The '*' is handled with two choices:
 *   1. skip "x*" entirely -> move two positions in the pattern
 *   2. use one "x" -> move one position in the string, stay on the star
 * Both choices must be tried because either may lead to a full match.
 */
