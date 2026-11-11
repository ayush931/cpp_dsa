/**
 * Permutation in String
 * Given strings s1 and s2, return true if s2 contains a permutation of s1.
 *
 * Input: s1 = "ab", s2 = "eidbaooo"
 * Output: true ("ba" is a permutation of "ab")
 *
 * Brute force -> sort every window of size s1 and compare, O(n * k log k)
 * Optimized -> compare frequency arrays of the window, O(n)
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool checkInclusionBrute(string s1, string s2) {
    int k = s1.size();
    string sortedS1 = s1;
    sort(sortedS1.begin(), sortedS1.end());

    for (int i = 0; i + k <= s2.size(); i++) {
        string window = s2.substr(i, k);
        sort(window.begin(), window.end());

        if (window == sortedS1) {
            return true;
        }
    }

    return false;
};

bool checkInclusion(string s1, string s2) {
    int k = s1.size();
    if (k > s2.size()) {
        return false;
    }

    vector<int> need(26, 0), window(26, 0);

    for (char ch : s1) {
        need[ch - 'a']++;
    }

    for (int i = 0; i < k; i++) {
        window[s2[i] - 'a']++;
    }

    if (need == window) {
        return true;
    }

    for (int i = k; i < s2.size(); i++) {
        window[s2[i] - 'a']++;
        window[s2[i - k] - 'a']--;

        if (need == window) {
            return true;
        }
    }

    return false;
};

int main() {
    cout << checkInclusionBrute("ab", "eidbaooo") << endl;  // 1
    cout << checkInclusion("ab", "eidbaooo") << endl;       // 1
    cout << checkInclusion("ab", "eidboaoo") << endl;       // 0

    return 0;
}

// Special cases: s1 longer than s2, repeated characters, empty s1
