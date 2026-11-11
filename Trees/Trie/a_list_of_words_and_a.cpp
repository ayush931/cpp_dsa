/**
 * Given a list of words and a prefix, print all words that start with that
 * prefix.
 *
 * Input: words = ["apple", "app", "apricot", "bat"], prefix = "ap"
 * Output: app apple apricot   (lexicographic order)
 */

#include <iostream>
#include <map>
#include <vector>
using namespace std;

class TrieNode {
   public:
    map<char, TrieNode *> children;
    bool isEnd;

    TrieNode() {
        isEnd = false;
    }
};

class Trie {
    TrieNode *root;

    void collect(TrieNode *node, string current, vector<string> &result) {
        if (node->isEnd) {
            result.push_back(current);
        }

        for (auto &child : node->children) {
            collect(child.second, current + child.first, result);
        }
    }

   public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode *curr = root;

        for (char ch : word) {
            if (!curr->children.count(ch)) {
                curr->children[ch] = new TrieNode();
            }
            curr = curr->children[ch];
        }

        curr->isEnd = true;
    }

    vector<string> wordsWithPrefix(string prefix) {
        vector<string> result;
        TrieNode *curr = root;

        for (char ch : prefix) {
            if (!curr->children.count(ch)) {
                return result;
            }
            curr = curr->children[ch];
        }

        collect(curr, prefix, result);
        return result;
    }
};

int main() {
    vector<string> words = {"apple", "app", "apricot", "bat"};
    Trie trie;

    for (string word : words) {
        trie.insert(word);
    }

    vector<string> result = trie.wordsWithPrefix("ap");

    for (string word : result) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}
