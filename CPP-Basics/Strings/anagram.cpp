// Given two strings s and t, return true if t is an anagram of s, and false
// otherwise. Contraints: String s and t will only contain lowercase
// alphabetical characters.

#include <bits/stdc++.h>
using namespace std;

bool isAnagrams(string s1, string s2) {
    // create frequency array
    vector<int> freq(26, 0);

    // if lengths are different for s1, s2 then return false
    if (s1.length() != s2.length()) {
        return false;
    }

    // store frequency of characters in s1 and s2
    for (int i = 0; i < s1.length(); i++) {
        freq[s1[i] - 'a']++;  // for s1, we are incrementing freq of char
        freq[s2[i] - 'a']--;  // for s2, we are decrementing freq of char
    }

    // checking if freq of every character is 0
    for (int i = 0; i < 26; i++) {
        if (freq[i] != 0) {  // not anagram
            return false;
        }
    }
    return true;
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;

    if (isAnagrams(s1, s2)) {
        cout << "Strings are anagrams" << endl;
    } else {
        cout << "Strings are not anagrams" << endl;
    }

    return 0;
}

// Time complexity -> 0(length of string)
// Space complexity -> 0(1)