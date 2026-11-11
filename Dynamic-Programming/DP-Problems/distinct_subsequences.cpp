/**
 * Distinct Subsequences
 *
 * Given strings s and t, count how many distinct subsequences of s equal t.
 * A subsequence is formed by deleting zero or more characters without
 * changing the order of the remaining characters. Different index choices
 * count separately, even when the resulting string is the same.
 *
 * Examples:
 *   s = "rabbbit", t = "rabbit" -> 3
 *   s = "babgbag", t = "bag"    -> 5
 *   s = "aaa",     t = "a"      -> 3
 *   s = "abc",     t = ""       -> 1   (the empty subsequence)
 *   s = "abc",     t = "abcd"   -> 0
 *
 * Brute force tries every subsequence. The DP processes s left to right and
 * updates t from right to left so each character of s is used at most once.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force: at each position of s, either skip it or, if it matches the
// next character of t, consume both.
// O(2^n) time, O(n + m) recursion space.
long long distinctBrute(const string &s, const string &t, int i, int j) {
    if (j == (int)t.size()) {
        return 1;
    }
    if (i == (int)s.size()) {
        return 0;
    }

    long long ways = distinctBrute(s, t, i + 1, j);

    if (s[i] == t[j]) {
        ways += distinctBrute(s, t, i + 1, j + 1);
    }

    return ways;
}

// O(n * m) DP, O(m) space. dp[j] is the number of ways to form t[0..j).
long long distinctSubsequences(const string &s, const string &t) {
    int m = t.size();
    vector<long long> dp(m + 1, 0);
    dp[0] = 1;

    for (char c : s) {
        // Descending order prevents using this character of s more than once.
        for (int j = m; j >= 1; j--) {
            if (c == t[j - 1]) {
                dp[j] += dp[j - 1];
            }
        }
    }

    return dp[m];
}

int main() {
    string s1 = "rabbbit";
    string t1 = "rabbit";

    cout << distinctBrute(s1, t1, 0, 0) << " " << distinctSubsequences(s1, t1)
         << endl;
    // 3 3

    string s2 = "babgbag";
    string t2 = "bag";

    cout << distinctBrute(s2, t2, 0, 0) << " " << distinctSubsequences(s2, t2)
         << endl;
    // 5 5

    cout << distinctSubsequences("aaa", "a") << endl;     // 3
    cout << distinctSubsequences("abc", "") << endl;      // 1
    cout << distinctSubsequences("abc", "abcd") << endl;  // 0
    cout << distinctSubsequences("", "") << endl;         // 1
    cout << distinctSubsequences("", "a") << endl;        // 0

    return 0;
}

/**
 * Complexity:
 *   brute -> O(2^n) time, O(n + m) recursion space
 *   DP    -> O(n * m) time, O(m) space
 *
 * Edge cases: empty s, empty t, t longer than s, no matching characters,
 * repeated characters (the source of the multiple counts), and identical
 * strings (the answer is 1).
 *
 * The answer can be exponential in n, so long long is used. For the usual
 * problem constraints the result fits in a 64-bit integer.
 */
