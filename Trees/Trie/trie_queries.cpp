/**
 * Trie Queries: Prefix Count, Word Delete, LCP, Distinct Substrings
 *
 * A trie stores a set of lowercase words. Each node counts how many words
 * pass through it (prefixCount) and how many end there (endCount). Those two
 * counters answer prefix queries and make deletion safe.
 *
 * Distinct substrings: insert every suffix of a string into a fresh trie.
 * Every newly created node is one distinct substring, so the answer is the
 * number of nodes created (the root is not a substring).
 *
 * Example:
 *   words = {"apple", "app", "apricot", "bat"}
 *   countWords("app")     -> 1
 *   countPrefix("ap")     -> 3
 *   countPrefix("app")    -> 2
 *   erase("app")          -> countWords("app") = 0, countPrefix("app") = 1
 *
 *   longest common prefix of {"flower", "flow", "flight"} -> "fl"
 *   distinct substrings of "ababa" -> 9
 *
 * Time complexity  -> O(length) per insert, search, delete, or prefix query
 * Space complexity -> O(total characters stored)
 */

#include <iostream>
#include <string>
using namespace std;

class TrieNode {
   public:
    TrieNode *children[26];
    int prefixCount;
    int endCount;

    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        prefixCount = 0;
        endCount = 0;
    }
};

void deleteSubtree(TrieNode *node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 26; i++) {
        deleteSubtree(node->children[i]);
    }

    delete node;
}

class Trie {
    TrieNode *root;

    bool eraseHelper(TrieNode *node, const string &word, int depth) {
        if (depth == (int)word.size()) {
            if (node->endCount == 0) {
                return false;
            }

            node->endCount--;
            return true;
        }

        int index = word[depth] - 'a';

        if (node->children[index] == NULL) {
            return false;
        }

        bool removed = eraseHelper(node->children[index], word, depth + 1);

        if (removed) {
            node->children[index]->prefixCount--;

            if (node->children[index]->prefixCount == 0) {
                deleteSubtree(node->children[index]);
                node->children[index] = NULL;
            }
        }

        return removed;
    }

   public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        deleteSubtree(root);
    }

    void insert(const string &word) {
        TrieNode *current = root;

        for (char ch : word) {
            int index = ch - 'a';

            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }

            current = current->children[index];
            current->prefixCount++;
        }

        current->endCount++;
    }

    bool erase(const string &word) {
        return eraseHelper(root, word, 0);
    }

    int countWords(const string &word) {
        TrieNode *current = root;

        for (char ch : word) {
            int index = ch - 'a';

            if (current->children[index] == NULL) {
                return 0;
            }
            current = current->children[index];
        }

        return current->endCount;
    }

    int countPrefix(const string &prefix) {
        TrieNode *current = root;

        for (char ch : prefix) {
            int index = ch - 'a';

            if (current->children[index] == NULL) {
                return 0;
            }
            current = current->children[index];
        }

        return current->prefixCount;
    }

    string longestCommonPrefix() {
        string result;
        TrieNode *current = root;

        while (true) {
            int childCount = 0;
            int onlyChild = -1;

            for (int i = 0; i < 26; i++) {
                if (current->children[i] != NULL) {
                    childCount++;
                    onlyChild = i;
                }
            }

            if (childCount != 1 || current->endCount > 0) {
                break;
            }

            result += (char)('a' + onlyChild);
            current = current->children[onlyChild];
        }

        return result;
    }
};

long long countDistinctSubstrings(const string &s) {
    TrieNode *root = new TrieNode();
    long long created = 0;

    for (int start = 0; start < (int)s.size(); start++) {
        TrieNode *current = root;

        for (int i = start; i < (int)s.size(); i++) {
            int index = s[i] - 'a';

            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
                created++;
            }

            current = current->children[index];
        }
    }

    deleteSubtree(root);
    return created;
}

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("apricot");
    trie.insert("bat");

    cout << trie.countWords("app") << endl;   // 1
    cout << trie.countPrefix("ap") << endl;   // 3
    cout << trie.countPrefix("app") << endl;  // 2
    cout << trie.countPrefix("b") << endl;    // 1
    cout << trie.countWords("ba") << endl;    // 0

    cout << trie.erase("app") << endl;         // 1
    cout << trie.countWords("app") << endl;    // 0
    cout << trie.countWords("apple") << endl;  // 1
    cout << trie.countPrefix("app") << endl;   // 1
    cout << trie.countPrefix("ap") << endl;    // 2

    cout << trie.erase("zzz") << endl;  // 0

    cout << trie.erase("apple") << endl;      // 1
    cout << trie.countPrefix("app") << endl;  // 0
    cout << trie.countPrefix("ap") << endl;   // 1

    Trie lcpTrie;
    lcpTrie.insert("flower");
    lcpTrie.insert("flow");
    lcpTrie.insert("flight");

    cout << lcpTrie.longestCommonPrefix() << endl;  // fl

    cout << countDistinctSubstrings("ababa") << endl;  // 9
    cout << countDistinctSubstrings("aaa") << endl;    // 3
    cout << countDistinctSubstrings("abc") << endl;    // 6

    return 0;
}
