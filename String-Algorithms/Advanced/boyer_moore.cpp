/**
 * Boyer-Moore String Matching
 *
 * Boyer-Moore compares a pattern against the text from right to left. On a
 * mismatch it uses two heuristics to skip characters:
 *
 *   - Bad-character rule: align the mismatching text character with its
 *     rightmost occurrence in the pattern.
 *   - Good-suffix rule: if a suffix of the pattern already matched, shift so
 *     that another occurrence of that suffix aligns with the text.
 *
 * The algorithm runs in O(n / m) best case and O(n * m) worst case, with
 * O(m^2 + 256) preprocessing in this readable suffix-table implementation.
 * A linear-time suffix preprocessor is possible. In practice it can skip
 * many text positions for long patterns.
 *
 * A naive matcher is included to verify every reported match.
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

vector<int> buildGoodSuffixTable(const string &pattern) {
    int m = static_cast<int>(pattern.size());
    if (m == 0) {
        return {0};
    }
    vector<int> suffix(m, 0);
    vector<int> goodSuffix(m + 1, m);

    suffix[m - 1] = m;

    for (int i = m - 2; i >= 0; i--) {
        int j = i;
        while (j >= 0 && pattern[j] == pattern[m - 1 - i + j]) {
            j--;
        }
        suffix[i] = i - j;
    }

    int j = 0;
    for (int i = m - 1; i >= 0; i--) {
        if (suffix[i] == i + 1) {
            for (; j < m - 1 - i; j++) {
                if (goodSuffix[j] == m) {
                    goodSuffix[j] = m - 1 - i;
                }
            }
        }
    }

    for (int i = 0; i <= m - 2; i++) {
        goodSuffix[m - 1 - suffix[i]] = m - 1 - i;
    }

    return goodSuffix;
}

vector<int> boyerMoore(const string &text, const string &pattern) {
    vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    if (m == 0 || n < m) {
        return matches;
    }

    array<int, 256> badCharacter;
    badCharacter.fill(-1);

    // Store the index of the rightmost occurrence of each character.
    for (int i = 0; i < m; i++) {
        badCharacter[static_cast<unsigned char>(pattern[i])] = i;
    }

    vector<int> goodSuffix = buildGoodSuffixTable(pattern);
    int position = 0;

    while (position <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[position + j]) {
            j--;
        }

        if (j < 0) {
            matches.push_back(position);
            position += goodSuffix[0];
        } else {
            int badCharacterShift =
                j -
                badCharacter[static_cast<unsigned char>(text[position + j])];
            int shift = max(goodSuffix[j], badCharacterShift);
            position += max(1, shift);
        }
    }

    return matches;
}

vector<int> naiveSearch(const string &text, const string &pattern) {
    vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    if (m == 0 || n < m) {
        return matches;
    }

    for (int i = 0; i <= n - m; i++) {
        bool matched = true;

        for (int j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                matched = false;
                break;
            }
        }

        if (matched) {
            matches.push_back(i);
        }
    }

    return matches;
}

void printMatches(const vector<int> &matches) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(matches.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << matches[i];
    }

    cout << "]";
}

void runTest(const string &text, const string &pattern) {
    vector<int> actual = boyerMoore(text, pattern);
    vector<int> expected = naiveSearch(text, pattern);

    cout << "pattern \"" << pattern << "\" in \"" << text << "\": ";
    printMatches(actual);
    cout << "  matches naive: " << (actual == expected ? "yes" : "no") << endl;
}

int main() {
    string text = "abababababcabcabc";

    runTest(text, "abc");
    runTest(text, "abab");
    runTest(text, "aba");
    runTest(text, "a");
    runTest(text, "xyz");
    runTest(text, "");

    runTest("aaaaaaaaaa", "aaa");
    runTest("aaaaaaaaaa", "aaaa");
    runTest("the quick brown fox jumps over the lazy dog", "the");
    runTest("the quick brown fox jumps over the lazy dog", "quick");
    runTest("abc", "abcd");

    mt19937 generator(20261111);
    uniform_int_distribution<int> characterDistribution(0, 2);
    bool randomTestsPassed = true;

    for (int test = 0; test < 200; test++) {
        int textLength = 5 + generator() % 40;
        int patternLength = 1 + generator() % 8;

        string randomText;
        string randomPattern;

        for (int i = 0; i < textLength; i++) {
            randomText.push_back(
                static_cast<char>('a' + characterDistribution(generator)));
        }

        for (int i = 0; i < patternLength; i++) {
            randomPattern.push_back(
                static_cast<char>('a' + characterDistribution(generator)));
        }

        if (boyerMoore(randomText, randomPattern) !=
            naiveSearch(randomText, randomPattern)) {
            randomTestsPassed = false;
            break;
        }
    }

    cout << "200 random comparisons: "
         << (randomTestsPassed ? "passed" : "failed") << endl;

    return 0;
}

// Expected output (abridged):
// pattern "abc" in "abababababcabcabc": [8, 11, 14]  matches naive: yes
// pattern "abab" in "abababababcabcabc": [0, 2, 4, 6]  matches naive: yes
// pattern "aba" in "abababababcabcabc": [0, 2, 4, 6]  matches naive: yes
// pattern "" in "...": []  matches naive: yes
// 200 random comparisons: passed
//
// Special cases: empty pattern, pattern longer than text, no matches,
// overlapping matches, highly repetitive text, and random alphabets.
