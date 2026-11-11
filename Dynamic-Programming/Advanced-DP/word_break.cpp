/**
 * Word Break
 * Given a string s and a dictionary of words, return true if s can be
 * segmented into a space-separated sequence of dictionary words.
 *
 * Input: s = "leetcode", dict = ["leet", "code"]
 * Output: true
 *
 * Brute force -> recursion, exponential
 * Optimized -> DP, O(n^2 * word length)
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

bool wordBreakBrute(const string &s, const unordered_set<string> &dict,
                    int start) {
    if (start == s.size()) {
        return true;
    }

    for (int end = start + 1; end <= s.size(); end++) {
        string word = s.substr(start, end - start);

        if (dict.count(word) && wordBreakBrute(s, dict, end)) {
            return true;
        }
    }

    return false;
};

bool wordBreak(const string &s, const vector<string> &wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();

    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }

    return dp[n];
};

int main() {
    vector<string> dict = {"leet", "code"};
    unordered_set<string> dictSet(dict.begin(), dict.end());

    cout << wordBreakBrute("leetcode", dictSet, 0) << endl;  // 1
    cout << wordBreak("leetcode", dict) << endl;             // 1
    cout << wordBreak("catsandog", {"cats", "dog", "sand", "and", "cat"})
         << endl;  // 0

    return 0;
}

// Special cases: empty string, empty dictionary, repeated words, overlapping
// words
