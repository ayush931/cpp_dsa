/**
 * Palindromic Substrings
 *
 * Given a string s, return the number of palindromic substrings in it. A
 * substring is a contiguous sequence of characters. Substrings with the same
 * content but different positions count separately.
 *
 * Example:
 * Input:  "abc"
 * Output: 3   ("a", "b", "c")
 *
 * Input:  "aaa"
 * Output: 6   ("a", "a", "a", "aa", "aa", "aaa")
 *
 * Constraints: 1 <= s.length <= 1000, s consists of lowercase letters.
 */

#include <iostream>
#include <string>
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

// Brute force: check every substring -> O(n^3) time, O(1) space
int countSubstringsBrute(const string &s) {
    int count = 0;

    for (int start = 0; start < s.size(); start++) {
        for (int end = start; end < s.size(); end++) {
            if (isPalindrome(s, start, end)) {
                count++;
            }
        }
    }

    return count;
}

// Expand around every center: 2n - 1 centers (odd and even) -> O(n^2)
int expandAroundCenter(const string &s, int left, int right) {
    int count = 0;

    while (left >= 0 && right < s.size() && s[left] == s[right]) {
        count++;
        left--;
        right++;
    }

    return count;
}

int countSubstrings(const string &s) {
    int count = 0;

    for (int center = 0; center < s.size(); center++) {
        // Odd-length palindromes: one center character.
        count += expandAroundCenter(s, center, center);

        // Even-length palindromes: two center characters.
        count += expandAroundCenter(s, center, center + 1);
    }

    return count;
}

int main() {
    cout << countSubstringsBrute("abc") << " " << countSubstrings("abc")
         << endl;
    // 3 3

    cout << countSubstringsBrute("aaa") << " " << countSubstrings("aaa")
         << endl;
    // 6 6

    cout << countSubstrings("abba") << endl;     // 6
    cout << countSubstrings("racecar") << endl;  // 10
    cout << countSubstrings("a") << endl;        // 1
    cout << countSubstrings("ab") << endl;       // 2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^3): O(n^2) substrings, O(n) palindrome check each
 *   expand around center -> O(n^2) time, O(1) space
 *   Manacher -> O(n) time, O(n) space
 * Space complexity -> O(1) extra for the center expansion, O(n) for Manacher
 *
 * Special cases: one character (1), all characters equal (n * (n + 1) / 2),
 * all characters distinct (n), even-length palindromes, a palindrome with an
 * odd center, and an empty string (0, though the constraints say n >= 1).
 *
 * The same expansion counts the longest palindromic substring by tracking the
 * maximum length instead of a counter.
 */
