/**
 * Suffix array -> sorted array of all suffix starting indices
 * Brute force -> sort suffixes by comparing strings, O(n^2 log n)
 * Doubling -> sort by rank of 2^k length prefixes, O(n log^2 n)
 * LCP array (Kasai) -> longest common prefix of adjacent suffixes, O(n)
 * Used for pattern search, distinct substrings and longest repeated substring
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> buildSuffixArrayBrute(string s) {
    int n = s.size();
    vector<int> suffixes(n);

    for (int i = 0; i < n; i++) {
        suffixes[i] = i;
    }

    sort(suffixes.begin(), suffixes.end(),
         [&](int a, int b) { return s.substr(a) < s.substr(b); });

    return suffixes;
};

vector<int> buildSuffixArray(string s) {
    s += '$';
    int n = s.size();

    vector<int> suffixArray(n);
    vector<int> rank(n);
    vector<int> temporaryRank(n);

    for (int i = 0; i < n; i++) {
        suffixArray[i] = i;
        rank[i] = s[i];
    }

    for (int length = 1; length < n; length *= 2) {
        auto compare = [&](int a, int b) {
            if (rank[a] != rank[b]) {
                return rank[a] < rank[b];
            }

            int rankA = (a + length < n) ? rank[a + length] : -1;
            int rankB = (b + length < n) ? rank[b + length] : -1;

            return rankA < rankB;
        };

        sort(suffixArray.begin(), suffixArray.end(), compare);

        temporaryRank[suffixArray[0]] = 0;

        for (int i = 1; i < n; i++) {
            temporaryRank[suffixArray[i]] =
                temporaryRank[suffixArray[i - 1]] +
                (compare(suffixArray[i - 1], suffixArray[i]) ? 1 : 0);
        }

        rank = temporaryRank;
    }

    suffixArray.erase(suffixArray.begin());  // remove the '$' suffix
    return suffixArray;
};

vector<int> buildLCP(string s, vector<int> &suffixArray) {
    int n = s.size();
    if (n == 0) {
        return {};
    }

    vector<int> rank(n);
    vector<int> lcp(n - 1, 0);

    for (int i = 0; i < n; i++) {
        rank[suffixArray[i]] = i;
    }

    int k = 0;

    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }

        int j = suffixArray[rank[i] + 1];

        while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
            k++;
        }

        lcp[rank[i]] = k;

        if (k > 0) {
            k--;
        }
    }

    return lcp;
};

int main() {
    string s = "banana";

    vector<int> brute = buildSuffixArrayBrute(s);
    vector<int> optimized = buildSuffixArray(s);

    for (int index : brute) {
        cout << index << " ";
    }
    cout << endl;

    for (int index : optimized) {
        cout << index << " ";
    }
    cout << endl;

    vector<int> lcp = buildLCP(s, optimized);
    for (int value : lcp) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: empty string, repeated characters, all distinct characters,
// sentinel character must not appear in the input
