/**
 * Palindrome Partitioning II (Minimum Cuts)
 *
 * Given a string s, partition it into palindromic substrings and return the
 * minimum number of cuts needed. A single palindrome needs 0 cuts.
 *
 * Examples:
 *   "aab"   -> 1   ("aa" | "b")
 *   "a"     -> 0
 *   "ab"    -> 1   ("a" | "b")
 *   "abcba" -> 0
 *   "abcd"  -> 3
 *
 * The solution precomputes isPalindrome[i][j] in O(n^2), then uses
 *   cuts[i] = 0 if s[0..i] is a palindrome,
 *   otherwise 1 + min(cuts[j]) over every j < i where s[j+1..i] is a
 *   palindrome.
 *
 * Time: O(n^2). Space: O(n^2).
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isPalindrome(const string &s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Brute force: try every palindromic prefix and recurse on the rest.
// O(2^n) time in the worst case, O(n) recursion space.
int minCutBrute(const string &s, int start) {
    if (start == (int)s.size()) {
        return -1;  // empty suffix needs no cut
    }

    int best = INT_MAX;

    for (int end = start; end < (int)s.size(); end++) {
        if (isPalindrome(s, start, end)) {
            best = min(best, 1 + minCutBrute(s, end + 1));
        }
    }

    return best;
}

// O(n^2) DP with a palindrome table.
int minCut(const string &s) {
    int n = s.size();
    if (n <= 1) {
        return 0;
    }

    vector<vector<bool>> palindrome(n, vector<bool>(n, false));

    // Build from the end so palindrome[i + 1][j - 1] is already known.
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i; j < n; j++) {
            if (s[i] == s[j] && (j - i < 2 || palindrome[i + 1][j - 1])) {
                palindrome[i][j] = true;
            }
        }
    }

    vector<int> cuts(n, 0);

    for (int end = 0; end < n; end++) {
        if (palindrome[0][end]) {
            cuts[end] = 0;
            continue;
        }

        cuts[end] = end;  // worst case: cut before every character

        for (int start = 0; start < end; start++) {
            if (palindrome[start + 1][end]) {
                cuts[end] = min(cuts[end], cuts[start] + 1);
            }
        }
    }

    return cuts[n - 1];
}

int main() {
    cout << minCutBrute("aab", 0) << " " << minCut("aab") << endl;
    // 1 1

    cout << minCut("a") << endl;      // 0
    cout << minCut("ab") << endl;     // 1
    cout << minCut("abcba") << endl;  // 0
    cout << minCut("abcd") << endl;   // 3

    cout << minCutBrute("ab", 0) << " " << minCut("aa") << endl;
    // 1 0

    cout << minCut("") << endl;  // 0

    return 0;
}

/**
 * Complexity:
 *   brute -> O(2^n) time, O(n) recursion space
 *   DP    -> O(n^2) time, O(n^2) space
 *
 * Edge cases: empty string, one character, two equal characters, two
 * different characters, a string that is already a palindrome, and a string
 * with no palindromic substring longer than one.
 *
 * The palindrome table uses the recurrence
 *   palindrome[i][j] = (s[i] == s[j]) && (j - i < 2 || palindrome[i+1][j-1]).
 * The cuts DP then only needs to know which suffixes are palindromes.
 */
