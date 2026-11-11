/**
 * Valid Palindrome
 *
 * A phrase is a palindrome if, after converting all uppercase letters to
 * lowercase and removing all non-alphanumeric characters, it reads the same
 * forward and backward.
 *
 * Example:
 * Input:  "A man, a plan, a canal: Panama"
 * Output: true
 *
 * Input:  "race a car"
 * Output: false
 *
 * Constraints: 1 <= s.length <= 2 * 10^5, s consists of printable ASCII.
 */

#include <cctype>
#include <iostream>
#include <string>
using namespace std;

// Brute force: build the cleaned string, then compare with its reverse
bool isPalindromeBrute(string s) {
    string cleaned = "";

    for (char ch : s) {
        if (isalnum((unsigned char)ch)) {
            cleaned += tolower((unsigned char)ch);
        }
    }

    string reversed(cleaned.rbegin(), cleaned.rend());

    return cleaned == reversed;
}

// Optimized: two pointers skip non-alphanumeric characters -> O(1) space
bool isPalindrome(string s) {
    int left = 0;
    int right = s.size() - 1;

    while (left < right) {
        while (left < right && !isalnum((unsigned char)s[left])) {
            left++;
        }
        while (left < right && !isalnum((unsigned char)s[right])) {
            right--;
        }

        if (tolower((unsigned char)s[left]) !=
            tolower((unsigned char)s[right])) {
            return false;
        }

        left++;
        right--;
    }

    return true;
}

int main() {
    string first = "A man, a plan, a canal: Panama";
    string second = "race a car";
    string third = " ";
    string fourth = "0P";
    string fifth = "";

    cout << isPalindromeBrute(first) << " " << isPalindrome(first) << endl;
    // 1 1

    cout << isPalindromeBrute(second) << " " << isPalindrome(second) << endl;
    // 0 0

    cout << isPalindrome(third) << endl;   // 1 (empty after cleaning)
    cout << isPalindrome(fourth) << endl;  // 0 ('0' != 'p')
    cout << isPalindrome(fifth) << endl;   // 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) time, O(n) space for the cleaned string
 *   two pointers -> O(n) time, O(1) space
 *
 * Special cases: empty string, only punctuation/spaces, one character,
 * mixed case, digits, a string that differs only in case, non-ASCII input
 * (the problem limits input to printable ASCII).
 */
