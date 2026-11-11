/**
 * Group Anagrams
 *
 * Given an array of strings strs, group the anagrams together. An anagram is
 * a word formed by rearranging the letters of another word, using all the
 * original letters exactly once.
 *
 * Example:
 * Input:  ["eat", "tea", "tan", "ate", "nat", "bat"]
 * Output: [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]
 *
 * Constraints: 1 <= strs.length <= 10^4, 0 <= strs[i].length <= 100,
 * strs[i] consists of lowercase English letters.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

string sortedKey(string word) {
    sort(word.begin(), word.end());
    return word;
}

// Brute force: compare every pair of words -> O(n^2 * k log k)
vector<vector<string>> groupAnagramsBrute(vector<string> &strs) {
    vector<vector<string>> groups;
    vector<bool> used(strs.size(), false);

    for (int i = 0; i < strs.size(); i++) {
        if (used[i]) {
            continue;
        }

        vector<string> group;
        string key = sortedKey(strs[i]);

        for (int j = i; j < strs.size(); j++) {
            if (!used[j] && sortedKey(strs[j]) == key) {
                used[j] = true;
                group.push_back(strs[j]);
            }
        }

        groups.push_back(group);
    }

    return groups;
}

// Optimized: hash map from sorted word to its group -> O(n * k log k)
vector<vector<string>> groupAnagrams(vector<string> &strs) {
    unordered_map<string, vector<string>> groups;

    for (string &word : strs) {
        groups[sortedKey(word)].push_back(word);
    }

    vector<vector<string>> result;

    for (auto &entry : groups) {
        result.push_back(entry.second);
    }

    return result;
}

void print(vector<vector<string>> &groups) {
    for (vector<string> &group : groups) {
        cout << "[ ";

        for (string &word : group) {
            cout << word << " ";
        }

        cout << "]" << endl;
    }
}

int main() {
    vector<string> first = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<string> second = {""};
    vector<string> third = {"a"};

    vector<vector<string>> brute = groupAnagramsBrute(first);
    print(brute);

    vector<vector<string>> optimized = groupAnagrams(first);
    print(optimized);

    vector<vector<string>> empty = groupAnagrams(second);
    print(empty);  // [ ]

    vector<vector<string>> single = groupAnagrams(third);
    print(single);  // [ a ]

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2 * k log k), where k is the maximum word length
 *   hash map -> O(n * k log k) time, O(n * k) space
 *
 * Special cases: empty strings (all group together), one word, words with
 * repeated letters, no anagrams, all words are anagrams, very long words.
 *
 * A counting-sort key (26 frequencies) can reduce the key cost to O(k + 26)
 * when the alphabet is small.
 */
