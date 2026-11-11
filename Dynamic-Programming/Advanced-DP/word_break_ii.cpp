/**
 * Word Break II
 *
 * Enumerate all sentences formed by segmenting a byte string into dictionary
 * words. Dictionary words are non-empty; duplicate words do not duplicate
 * results. An empty input has one empty segmentation, represented by "".
 *
 * Memoize all sentences for each suffix start index. Every recursive step
 * consumes a non-empty word, so recursion terminates. Unlike Word Break I,
 * returning all answers necessarily takes exponential output in some cases.
 *
 * Building/checking substrings costs O(n*L^2) expected before output costs,
 * where L is min(n, maximum word length). Memo storage includes intermediate
 * suffix sentences, which may also be exponential; recursion depth is O(n).
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

vector<string> wordBreakSentences(const string &text,
                                  const vector<string> &words) {
    unordered_set<string> dictionary;
    size_t maximumLength = 0;
    for (const string &word : words) {
        if (!word.empty()) {
            dictionary.insert(word);
            maximumLength = max(maximumLength, word.size());
        }
    }

    vector<vector<string>> memo(text.size() + 1);
    vector<bool> ready(text.size() + 1);
    ready[text.size()] = true;
    memo[text.size()] = {""};

    function<const vector<string> &(size_t)> sentences =
        [&](size_t start) -> const vector<string> & {
        if (ready[start]) {
            return memo[start];
        }
        for (size_t length = 1;
             length <= min(maximumLength, text.size() - start); ++length) {
            string word = text.substr(start, length);
            if (!dictionary.count(word)) {
                continue;
            }
            for (const string &suffix : sentences(start + length)) {
                memo[start].push_back(suffix.empty() ? word
                                                     : word + " " + suffix);
            }
        }
        ready[start] = true;
        return memo[start];
    };
    return sentences(0);
}

int main() {
    auto answer =
        wordBreakSentences("catsanddog", {"cat", "cats", "and", "sand", "dog"});
    sort(answer.begin(), answer.end());
    assert((answer == vector<string>{"cat sand dog", "cats and dog"}));
    assert((wordBreakSentences("a", {"", "a", "a"}) == vector<string>{"a"}));
    assert((wordBreakSentences("", {}) == vector<string>{""}));
    assert(
        wordBreakSentences("catsandog", {"cats", "dog", "sand", "and", "cat"})
            .empty());
    for (const string &sentence : answer) {
        cout << sentence << '\n';
    }
}
