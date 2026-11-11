/**
 * Longest Substring Without Repeating Characters
 * Given a string s, find the length of the longest substring without repeating
 * characters.
 *
 * Input: s = "abcabcbb"
 * Output: 3 ("abc")
 *
 * Brute force -> check every substring, O(n^3)
 * Optimized -> sliding window with a set, O(n)
 */

#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

bool allUnique(string &s, int start, int end) {
    unordered_set<char> seen;

    for (int i = start; i <= end; i++) {
        if (seen.count(s[i])) {
            return false;
        }
        seen.insert(s[i]);
    }

    return true;
};

int lengthOfLongestSubstringBrute(string s) {
    int n = s.size();
    int maxLength = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (allUnique(s, i, j)) {
                maxLength = max(maxLength, j - i + 1);
            }
        }
    }

    return maxLength;
};

int lengthOfLongestSubstring(string s) {
    unordered_set<char> window;
    int left = 0;
    int maxLength = 0;

    for (int right = 0; right < s.size(); right++) {
        while (window.count(s[right])) {
            window.erase(s[left]);
            left++;
        }

        window.insert(s[right]);
        maxLength = max(maxLength, right - left + 1);
    }

    return maxLength;
};

int main() {
    cout << lengthOfLongestSubstringBrute("abcabcbb") << endl;  // 3
    cout << lengthOfLongestSubstring("abcabcbb") << endl;       // 3
    cout << lengthOfLongestSubstring("bbbbb") << endl;          // 1
    cout << lengthOfLongestSubstring("pwwkew") << endl;         // 3

    return 0;
}
