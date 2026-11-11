/**
 * Trie (prefix tree) -> tree used to store strings
 * Every node represents a character and stores links to its children
 * isEnd marks the end of a valid word
 * Insert, search and delete -> O(length of the string)
 * Used in autocomplete, spell check and prefix queries
 */

#include <iostream>
#include <unordered_map>
using namespace std;

class TrieNode {
   public:
    unordered_map<char, TrieNode *> children;
    bool isEnd;

    TrieNode() {
        isEnd = false;
    }
};

class Trie {
    TrieNode *root;

    bool isEmpty(TrieNode *node) {
        return node->children.empty();
    }

    TrieNode *remove(TrieNode *node, string word, int depth) {
        if (node == NULL) {
            return NULL;
        }

        if (depth == word.size()) {
            if (node->isEnd) {
                node->isEnd = false;
            }
            if (isEmpty(node)) {
                delete node;
                return NULL;
            }
            return node;
        }

        char ch = word[depth];
        if (node->children.count(ch)) {
            node->children[ch] = remove(node->children[ch], word, depth + 1);
            if (node->children[ch] == NULL) {
                node->children.erase(ch);
            }
        }

        if (isEmpty(node) && !node->isEnd) {
            delete node;
            return NULL;
        }

        return node;
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

    bool search(string word) {
        TrieNode *curr = root;

        for (char ch : word) {
            if (!curr->children.count(ch)) {
                return false;
            }
            curr = curr->children[ch];
        }

        return curr->isEnd;
    }

    bool startsWith(string prefix) {
        TrieNode *curr = root;

        for (char ch : prefix) {
            if (!curr->children.count(ch)) {
                return false;
            }
            curr = curr->children[ch];
        }

        return true;
    }

    void erase(string word) {
        root = remove(root, word, 0);
    }
};

int main() {
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("bat");

    cout << trie.search("app") << endl;     // 1
    cout << trie.search("ap") << endl;      // 0
    cout << trie.startsWith("ap") << endl;  // 1

    trie.erase("app");
    cout << trie.search("app") << endl;    // 0
    cout << trie.search("apple") << endl;  // 1

    return 0;
}
