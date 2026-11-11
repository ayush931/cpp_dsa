/**
 * Word Search
 *
 * Given an m x n board of characters and a string word, return true if word
 * exists in the grid. The word can be constructed from letters of adjacent
 * cells (horizontally or vertically). The same cell may not be used twice.
 *
 * Example:
 * board = [
 *   ['A','B','C','E'],
 *   ['S','F','C','S'],
 *   ['A','D','E','E']
 * ]
 * word = "ABCCED" -> true
 * word = "SEE"    -> true
 * word = "ABCB"   -> false
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool dfsBrute(vector<vector<char>> &board, string &word, int row, int col,
              int index, vector<vector<bool>> &visited) {
    if (index == word.size()) {
        return true;
    }

    if (row < 0 || col < 0 || row >= board.size() || col >= board[0].size()) {
        return false;
    }

    if (visited[row][col] || board[row][col] != word[index]) {
        return false;
    }

    visited[row][col] = true;

    bool found = dfsBrute(board, word, row + 1, col, index + 1, visited) ||
                 dfsBrute(board, word, row - 1, col, index + 1, visited) ||
                 dfsBrute(board, word, row, col + 1, index + 1, visited) ||
                 dfsBrute(board, word, row, col - 1, index + 1, visited);

    visited[row][col] = false;  // backtrack

    return found;
}

// Brute force: separate visited matrix for every path
bool existBrute(vector<vector<char>> &board, string word) {
    int rows = board.size();
    int cols = board[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (dfsBrute(board, word, row, col, 0, visited)) {
                return true;
            }
        }
    }

    return false;
}

bool dfs(vector<vector<char>> &board, string &word, int row, int col,
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
    board[row][col] = '#';  // mark as visited in place

    bool found = dfs(board, word, row + 1, col, index + 1) ||
                 dfs(board, word, row - 1, col, index + 1) ||
                 dfs(board, word, row, col + 1, index + 1) ||
                 dfs(board, word, row, col - 1, index + 1);

    board[row][col] = saved;  // restore

    return found;
}

// Optimized: mark the board in place -> O(1) extra space per path
bool exist(vector<vector<char>> &board, string word) {
    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[0].size(); col++) {
            if (dfs(board, word, row, col, 0)) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    vector<vector<char>> board = {
        {'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}};

    cout << existBrute(board, "ABCCED") << " " << exist(board, "ABCCED")
         << endl;
    // 1 1

    cout << existBrute(board, "SEE") << " " << exist(board, "SEE") << endl;
    // 1 1

    cout << existBrute(board, "ABCB") << " " << exist(board, "ABCB") << endl;
    // 0 0

    cout << exist(board, "A") << endl;  // 1
    cout << exist(board, "") << endl;   // 1 (empty word)

    return 0;
}

/**
 * Time complexity -> O(m * n * 4^L) in the worst case, where L is the word
 * length; the search is heavily pruned by the character check.
 * Space complexity -> O(L) recursion depth (the in-place version uses O(1)
 * extra grid space).
 *
 * Special cases: empty word, word longer than the number of cells, a single
 * cell, repeated letters, the word starts at every cell, the board has one
 * row or one column, the same cell must not be reused.
 */
