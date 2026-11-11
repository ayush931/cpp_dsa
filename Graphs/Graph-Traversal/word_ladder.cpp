/**
 * Word Ladder
 * Given beginWord, endWord and a dictionary, return the length of the
 * shortest transformation sequence from beginWord to endWord.
 * Each step changes exactly one letter and every intermediate word must be in
 * the dictionary.
 *
 * Input: begin = "hit", end = "cog", dict = [hot, dot, dog, lot, log, cog]
 * Output: 5 (hit -> hot -> dot -> dog -> cog)
 *
 * Brute force -> BFS checking every dictionary word for a one-letter
 * difference, O(N^2 * L) Optimized -> BFS with pattern buckets, O(N * L^2)
 */

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

bool differByOne(string &a, string &b) {
    int differences = 0;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            differences++;
        }
        if (differences > 1) {
            return false;
        }
    }

    return differences == 1;
};

int ladderLengthBrute(string beginWord, string endWord,
                      vector<string> &wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) {
        return 0;
    }

    queue<pair<string, int>> q;
    unordered_set<string> visited;
    q.push({beginWord, 1});
    visited.insert(beginWord);

    while (!q.empty()) {
        auto [word, steps] = q.front();
        q.pop();

        if (word == endWord) {
            return steps;
        }

        for (string candidate : wordList) {
            if (!visited.count(candidate) && differByOne(word, candidate)) {
                visited.insert(candidate);
                q.push({candidate, steps + 1});
            }
        }
    }

    return 0;
};

int ladderLength(string beginWord, string endWord, vector<string> &wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) {
        return 0;
    }

    unordered_map<string, vector<string>> patternMap;

    for (string word : wordList) {
        for (int i = 0; i < word.size(); i++) {
            string pattern = word;
            pattern[i] = '*';
            patternMap[pattern].push_back(word);
        }
    }

    queue<pair<string, int>> q;
    unordered_set<string> visited;
    q.push({beginWord, 1});
    visited.insert(beginWord);

    while (!q.empty()) {
        auto [word, steps] = q.front();
        q.pop();

        if (word == endWord) {
            return steps;
        }

        for (int i = 0; i < word.size(); i++) {
            string pattern = word;
            pattern[i] = '*';

            for (string neighbour : patternMap[pattern]) {
                if (!visited.count(neighbour)) {
                    visited.insert(neighbour);
                    q.push({neighbour, steps + 1});
                }
            }
        }
    }

    return 0;
};

int main() {
    vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};

    cout << ladderLengthBrute("hit", "cog", wordList) << endl;  // 5
    cout << ladderLength("hit", "cog", wordList) << endl;       // 5

    return 0;
}

// Special cases: endWord not in dictionary, beginWord equals endWord,
// words of different lengths, no path
