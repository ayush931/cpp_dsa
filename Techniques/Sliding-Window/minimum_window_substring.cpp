/**
 * Minimum Window Substring
 * Given strings s and t, return the smallest substring of s that contains
 * every character of t (including duplicates). Return "" if none exists.
 *
 * Input: s = "ADOBECODEBANC", t = "ABC"
 * Output: "BANC"
 *
 * Brute force -> check every substring, O(n^3 + n * m) worst case
 * Optimized -> sliding window with frequency maps, O(n + m)
 */

#include <climits>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

bool containsAll(string &window, unordered_map<char, int> &need) {
    unordered_map<char, int> have;

    for (char ch : window) {
        have[ch]++;
    }

    for (auto &entry : need) {
        if (have[entry.first] < entry.second) {
            return false;
        }
    }

    return true;
};

string minWindowBrute(string s, string t) {
    string result = "";
    int minLength = INT_MAX;

    unordered_map<char, int> need;
    for (char ch : t) {
        need[ch]++;
    }

    for (int i = 0; i < s.size(); i++) {
        for (int j = i; j < s.size(); j++) {
            string window = s.substr(i, j - i + 1);

            if (containsAll(window, need) && (int)window.size() < minLength) {
                minLength = window.size();
                result = window;
            }
        }
    }

    return result;
};

string minWindow(string s, string t) {
    if (t.empty() || s.size() < t.size()) {
        return "";
    }

    unordered_map<char, int> need;
    for (char ch : t) {
        need[ch]++;
    }

    int required = need.size();
    int formed = 0;

    unordered_map<char, int> windowCount;
    int left = 0;
    int minLength = INT_MAX;
    int start = 0;

    for (int right = 0; right < s.size(); right++) {
        char ch = s[right];
        windowCount[ch]++;

        if (need.count(ch) && windowCount[ch] == need[ch]) {
            formed++;
        }

        while (left <= right && formed == required) {
            if (right - left + 1 < minLength) {
                minLength = right - left + 1;
                start = left;
            }

            char leftChar = s[left];
            windowCount[leftChar]--;

            if (need.count(leftChar) &&
                windowCount[leftChar] < need[leftChar]) {
                formed--;
            }

            left++;
        }
    }

    return (minLength == INT_MAX) ? "" : s.substr(start, minLength);
};

int main() {
    cout << minWindowBrute("ADOBECODEBANC", "ABC") << endl;  // BANC
    cout << minWindow("ADOBECODEBANC", "ABC") << endl;       // BANC
    cout << minWindow("a", "aa") << endl;                    // empty

    return 0;
}

// Special cases: t longer than s, t has duplicates, no valid window, empty
// strings
