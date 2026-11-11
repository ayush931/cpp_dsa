/**
 * Aho-Corasick -> multi-pattern string matching
 * Builds a trie of all patterns, adds failure links (like KMP on a trie)
 * and output links for patterns that end at a node
 * Build -> O(total pattern length)
 * Search -> O(text length + number of matches)
 */

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

const int ALPHABET = 26;

class AhoCorasick {
    struct Node {
        int next[ALPHABET];
        int failure;
        vector<int> output;

        Node() {
            fill(next, next + ALPHABET, -1);
            failure = 0;
        }
    };

    vector<Node> trie;

   public:
    AhoCorasick() {
        trie.push_back(Node());
    }

    void insert(string &pattern, int index) {
        int node = 0;

        for (char ch : pattern) {
            int c = ch - 'a';

            if (trie[node].next[c] == -1) {
                trie[node].next[c] = trie.size();
                trie.push_back(Node());
            }

            node = trie[node].next[c];
        }

        trie[node].output.push_back(index);
    }

    void build() {
        queue<int> q;

        for (int c = 0; c < ALPHABET; c++) {
            if (trie[0].next[c] != -1) {
                int child = trie[0].next[c];
                trie[child].failure = 0;
                q.push(child);
            }
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int c = 0; c < ALPHABET; c++) {
                int child = trie[node].next[c];

                if (child == -1) {
                    continue;
                }

                int failure = trie[node].failure;

                while (failure != 0 && trie[failure].next[c] == -1) {
                    failure = trie[failure].failure;
                }

                if (trie[failure].next[c] != -1 &&
                    trie[failure].next[c] != child) {
                    failure = trie[failure].next[c];
                }

                trie[child].failure = failure;

                for (int output : trie[failure].output) {
                    trie[child].output.push_back(output);
                }

                q.push(child);
            }
        }
    }

    vector<pair<int, int>> search(string &text) {
        vector<pair<int, int>> matches;  // {pattern index, end position}
        int node = 0;

        for (int i = 0; i < text.size(); i++) {
            int c = text[i] - 'a';

            while (node != 0 && trie[node].next[c] == -1) {
                node = trie[node].failure;
            }

            if (trie[node].next[c] != -1) {
                node = trie[node].next[c];
            }

            for (int patternIndex : trie[node].output) {
                matches.push_back({patternIndex, i});
            }
        }

        return matches;
    }
};

int main() {
    vector<string> patterns = {"he", "she", "his", "hers"};
    AhoCorasick ac;

    for (int i = 0; i < patterns.size(); i++) {
        ac.insert(patterns[i], i);
    }

    ac.build();

    string text = "ushers";
    vector<pair<int, int>> matches = ac.search(text);

    for (auto &match : matches) {
        cout << patterns[match.first] << " ends at " << match.second << endl;
    }

    return 0;
}

// Special cases: empty patterns, duplicate patterns, patterns that are
// suffixes of other patterns, uppercase letters, no matches
