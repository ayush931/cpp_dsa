/**
 * Word Search II
 *
 * Given an m x n board of characters and a list of strings words, return all
 * words that can be formed by adjacent cells (horizontally or vertically).
 * The same cell may not be used more than once in a word.
 *
 * Example:
 * board = [
 *   ['o','a','a','n'],
 *   ['e','t','a','e'],
 *   ['i','h','k','r'],
 *   ['i','f','l','v']
 * ]
 * words = ["oath", "pea", "eat", "rain"]
 * Output: ["eat", "oath"]
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool dfsBoard(vector<vector<char>> &board, string &word, int row, int col,
              int index) {
    if (index == word.size()) {
        return true;
    }

    if (row < 0 || col < 0 || row >= board.size() || col >= board[0].size()) {
        return false;
    }

    if (board[row][col] != word[index]) {
        return false;
    }

    char saved = board[row][col];
    board[row][col] = '#';

    bool found = dfsBoard(board, word, row + 1, col, index + 1) ||
                 dfsBoard(board, word, row - 1, col, index + 1) ||
                 dfsBoard(board, word, row, col + 1, index + 1) ||
                 dfsBoard(board, word, row, col - 1, index + 1);

    board[row][col] = saved;

    return found;
}

// Brute force: search the board separately for every word -> O(W * m * n * 4^L)
vector<string> findWordsBrute(vector<vector<char>> board,
                              vector<string> &words) {
    vector<string> result;

    for (string &word : words) {
        bool found = false;

        for (int row = 0; row < board.size() && !found; row++) {
            for (int col = 0; col < board[0].size() && !found; col++) {
                if (dfsBoard(board, word, row, col, 0)) {
                    found = true;
                }
            }
        }

        if (found) {
            result.push_back(word);
        }
    }

    return result;
}

class TrieNode {
   public:
    TrieNode *children[26];
    string word;  // non-empty only at the end of a word

    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
        word = "";
    }
};

class Trie {
   public:
    TrieNode *root;

    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode *current = root;

        for (char ch : word) {
            int index = ch - 'a';

            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }

            current = current->children[index];
        }

        current->word = word;
    }
};

void dfsTrie(vector<vector<char>> &board, int row, int col, TrieNode *node,
             vector<string> &result) {
    if (row < 0 || col < 0 || row >= board.size() || col >= board[0].size()) {
        return;
    }

    char ch = board[row][col];

    if (ch == '#' || node->children[ch - 'a'] == NULL) {
        return;
    }

    node = node->children[ch - 'a'];

    if (!node->word.empty()) {
        result.push_back(node->word);
        node->word = "";  // avoid adding the same word twice
    }

    board[row][col] = '#';

    dfsTrie(board, row + 1, col, node, result);
    dfsTrie(board, row - 1, col, node, result);
    dfsTrie(board, row, col + 1, node, result);
    dfsTrie(board, row, col - 1, node, result);

    board[row][col] = ch;
}

// Optimized: one trie-guided DFS over the board -> O(m * n * 4^L)
vector<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    Trie trie;

    for (string &word : words) {
        trie.insert(word);
    }

    vector<string> result;

    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[0].size(); col++) {
            dfsTrie(board, row, col, trie.root, result);
        }
    }

    return result;
}

void print(vector<string> &words) {
    for (string &word : words) {
        cout << word << " ";
    }
    cout << endl;
}

int main() {
    vector<vector<char>> board = {{'o', 'a', 'a', 'n'},
                                  {'e', 't', 'a', 'e'},
                                  {'i', 'h', 'k', 'r'},
                                  {'i', 'f', 'l', 'v'}};

    vector<string> words = {"oath", "pea", "eat", "rain"};

    vector<string> brute = findWordsBrute(board, words);
    print(brute);  // oath eat

    vector<string> optimized = findWords(board, words);
    print(optimized);  // oath eat

    vector<string> duplicates = {"oath", "oath"};
    vector<string> unique = findWords(board, duplicates);
    print(unique);  // oath

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(W * m * n * 4^L), where W is the number of words
 *   trie + DFS -> O(m * n * 4^L) in the worst case, shared prefixes prune it
 * Space -> O(total characters in words) for the trie
 *
 * Special cases: empty word list, empty board, one-letter words, duplicate
 * words (return each once), words longer than the number of cells, words that
 * share prefixes, a board with repeated characters.
 */
