/**
 * Maximum XOR Pair and Wildcard Search
 *
 * Maximum XOR pair:
 *   Build the answer bit by bit from the most significant bit. At each step,
 *   keep the prefixes of all numbers and test whether two prefixes can form
 *   the candidate answer. This is O(bits * n) expected time.
 *   A binary trie gives the same result in O(bits * n) worst case (see
 *   97_Bit_Manipulation/07_max_xor_trie_single_ii.cpp).
 *
 * Wildcard search:
 *   A trie supports '.' as "match any one character". For a '.' the search
 *   branches into every child; for a normal character it follows one link.
 *
 * Example:
 *   nums = {3, 10, 5, 25, 2, 8}
 *   maximum XOR pair -> 28   (5 ^ 25)
 *
 *   words = {"bad", "dad", "mad"}
 *   search("pad") -> false
 *   search(".ad") -> true
 *   search("b..") -> true
 *
 * Time complexity:
 *   maximum XOR pair -> O(31 * n) expected
 *   wildcard search  -> O(26^dots * length) worst case
 * Space complexity -> O(n) for the prefix set, O(total characters) for the trie
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

int maximumXORBrute(vector<int> &nums) {
    int best = 0;

    for (int i = 0; i < (int)nums.size(); i++) {
        for (int j = i + 1; j < (int)nums.size(); j++) {
            best = max(best, nums[i] ^ nums[j]);
        }
    }

    return best;
}

int maximumXOR(vector<int> &nums) {
    int result = 0;
    int mask = 0;

    for (int bit = 30; bit >= 0; bit--) {
        mask |= (1 << bit);

        unordered_set<int> prefixes;
        for (int value : nums) {
            prefixes.insert(value & mask);
        }

        int candidate = result | (1 << bit);

        for (int prefix : prefixes) {
            if (prefixes.count(prefix ^ candidate)) {
                result = candidate;
                break;
            }
        }
    }

    return result;
}

class TrieNode {
   public:
    TrieNode *children[26];
    bool isEnd;

    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        isEnd = false;
    }
};

void deleteTrie(TrieNode *node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 26; i++) {
        deleteTrie(node->children[i]);
    }

    delete node;
}

class Trie {
    TrieNode *root;

    bool searchHelper(TrieNode *node, const string &word, int index) {
        if (node == NULL) {
            return false;
        }
        if (index == (int)word.size()) {
            return node->isEnd;
        }

        char ch = word[index];

        if (ch == '.') {
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != NULL &&
                    searchHelper(node->children[i], word, index + 1)) {
                    return true;
                }
            }

            return false;
        }

        int child = ch - 'a';
        return searchHelper(node->children[child], word, index + 1);
    }

   public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        deleteTrie(root);
    }

    void insert(const string &word) {
        TrieNode *current = root;

        for (char ch : word) {
            int index = ch - 'a';

            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }

            current = current->children[index];
        }

        current->isEnd = true;
    }

    bool search(const string &word) {
        return searchHelper(root, word, 0);
    }
};

int main() {
    vector<int> nums = {3, 10, 5, 25, 2, 8};

    cout << maximumXORBrute(nums) << endl;  // 28
    cout << maximumXOR(nums) << endl;       // 28

    vector<int> single = {7};
    cout << maximumXOR(single) << endl;  // 0

    Trie trie;
    trie.insert("bad");
    trie.insert("dad");
    trie.insert("mad");

    cout << trie.search("pad") << endl;   // 0
    cout << trie.search(".ad") << endl;   // 1
    cout << trie.search("b..") << endl;   // 1
    cout << trie.search("bad") << endl;   // 1
    cout << trie.search("....") << endl;  // 0
    cout << trie.search("...") << endl;   // 1

    return 0;
}
