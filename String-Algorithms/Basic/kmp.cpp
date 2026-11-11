/**
 * KMP (Knuth Morris Pratt) -> pattern matching in O(n + m)
 * The LPS array stores the length of the longest proper prefix
 * which is also a suffix for every prefix of the pattern
 * When a mismatch happens, the pattern is shifted using LPS instead of starting
 * again
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> buildLPS(string pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);

    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
};

vector<int> kmpSearch(string text, string pattern) {
    vector<int> result;

    if (pattern.empty()) {
        return result;
    }

    int n = text.size();
    int m = pattern.size();

    vector<int> lps = buildLPS(pattern);

    int i = 0;  // index in text
    int j = 0;  // index in pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            result.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return result;
};

int main() {
    string text = "ababcababcabc";
    string pattern = "abc";

    vector<int> positions = kmpSearch(text, pattern);

    for (int pos : positions) {
        cout << pos << " ";
    }
    cout << endl;

    return 0;
}

// Time complexity -> O(n + m)
// Space complexity -> O(m)
