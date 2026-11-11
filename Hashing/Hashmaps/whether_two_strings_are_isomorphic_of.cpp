/**
 * Check whether two strings are isomorphic of each other. Return true if they
 are else return false.

 * Two strings are isomorphic of each other if there is a one-to-one mapping
 possible for every character of the first string to every character of second
 string and all occurances of every character in first string maps to the same
 character in the second string.

 * Input 1: aab xxy
 * Output 1: True

 * Input 2: abcdec viouog
 * Output 2: False
 */

#include <bits/stdc++.h>
using namespace std;

bool checkNoOneToManyMapping(string s1, string s2) {
    unordered_map<char, char> m;
    for (int i = 0; i < s1.length(); i++) {
        if (m.find(s1[i]) != m.end()) {
            if (m[s1[i]] != s2[i]) {
                return false;
            }
        } else {
            m[s1[i]] = s2[i];
        }
    }

    return true;
}

bool checkIsomorphic(string s1, string s2) {
    if (s1.length() != s2.length()) {
        return false;
    }

    // checking one-to-many mapping from s1 -> s2
    bool s1s2 = checkNoOneToManyMapping(s1, s2);

    // checking one-to-many mapping from s2 -> s1
    bool s2s1 = checkNoOneToManyMapping(s2, s1);

    return s1s2 && s2s1;
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;

    cout << (checkIsomorphic(s1, s2) ? "Is isomorphic" : "Is not isomorphic")
         << endl;
    // "aab xxy" -> Is isomorphic
    // "abcdec viouog" -> Is not isomorphic

    return 0;
}

// Time:- O(N) -> sum of length of s1 and s2
// Space:- O(n) -> unique character of s1 and s2