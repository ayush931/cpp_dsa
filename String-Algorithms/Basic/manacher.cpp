/**
 * Manacher algorithm -> longest palindromic substring in O(n)
 * Insert '#' between characters so odd and even length palindromes
 * are handled in the same way
 * p[i] stores the radius of the palindrome centered at i
 */

#include <iostream>
#include <vector>
using namespace std;

string longestPalindrome(string s) {
    string t = "^#";

    for (char ch : s) {
        t += ch;
        t += '#';
    }

    t += '$';

    int n = t.size();
    vector<int> p(n, 0);

    int center = 0;
    int right = 0;

    for (int i = 1; i < n - 1; i++) {
        int mirror = 2 * center - i;

        if (i < right) {
            p[i] = min(right - i, p[mirror]);
        }

        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
            p[i]++;
        }

        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
    }

    int maxLength = 0;
    int centerIndex = 0;

    for (int i = 1; i < n - 1; i++) {
        if (p[i] > maxLength) {
            maxLength = p[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLength) / 2;

    return s.substr(start, maxLength);
};

int main() {
    cout << longestPalindrome("babad") << endl;  // bab
    cout << longestPalindrome("cbbd") << endl;   // bb
    cout << longestPalindrome("forgeeksskeegfor") << endl;

    return 0;
}

// Time complexity -> O(n)
// Space complexity -> O(n)
