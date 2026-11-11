/**
 * Check whether two strings are anagram of each other. Return true if they are
 else return false. An anagram of a string is another string that contains the
 same characters, only the order of characters can be different. For example,
 "abcd" and "dabc" are an anagram of each other.

 * Input 1: triangle, integral
 * Output 1: True

 * Input 2:- anagram, grams
 * Output 2: False
 */

#include <bits/stdc++.h>
using namespace std;

bool checkAnagram(string s1, string s2) {
    unordered_map<char, int> m;

    if (s1.length() != s2.length()) {
        return false;
    }

    for (auto c1 : s1) {
        m[c1]++;  // storing characters and their frequency in s1
    }

    for (auto c2 : s2) {
        if (m.find(c2) == m.end()) {
            return false;
        } else {
            m[c2]--;
        }
    }

    for (auto ele : m) {
        if (ele.second != 0) {
            return false;
        }
    }

    return true;
};

int main() {
    string s1, s2;
    cin >> s1 >> s2;

    cout << (checkAnagram(s1, s2) ? "Anagram" : "Not Anagram") << endl;

    return 0;
}

// Time:- O(N) -> sum of length of s1 and s2
// Space:- O(n)