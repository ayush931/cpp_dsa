/**
 * Consider a 9 * 9 2D array grid number that is partially filled with numbers
 * from 1 to 9. The Sudoku solver problem is to fill remaining blocks with the
 * number 1 to 9 so that every row, column and subgrid (3 * 3) contains exactly
 * one instance of digits (1 to 9).
 */

#include <iostream>
#include <vector>
using namespace std;

class Solution {
   public:
    bool canWePlace(vector<vector<char>> &board, int row, int col, char num) {
        for (int j = 0; j < 9; j++) {
            if (board[row][j] == num) {
                return false;
            }
        }

        for (int i = 0; i < 9; i++) {
            if (board[i][col] == num) {
                return false;
            }
        }

        int r = (row / 3) * 3;
        int c = (col / 3) * 3;

        for (int i = r; i < r + 3; i++) {
            for (int j = c; j < c + 3; j++) {
                if (board[i][j] == num) {
                    return false;
                }
            }
        }

        return true;
    };

    bool sudoku(vector<vector<char>> &board, int row, int col) {
        if (col == 9) {
            return sudoku(board, row + 1, 0);
        }
        if (row == 9) {
            return true;
        }

        if (board[row][col] == '.') {
            for (int num = 1; num <= 9; num++) {
                if (canWePlace(board, row, col, '0' + num)) {
                    board[row][col] = '0' + num;
                    bool solved = sudoku(board, row, col + 1);
                    if (solved) {
                        return true;
                    }
                    board[row][col] = '.';  // backtrack
                }
            }
            return false;
        }

        return sudoku(board, row, col + 1);
    };

    void solveSudoku(vector<vector<char>> &board) {
        sudoku(board, 0, 0);
    }
};

int main() {
    vector<vector<char>> board = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};

    Solution solution;
    solution.solveSudoku(board);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

// Time -> O(9^(empty cells)) in the worst case, much faster with pruning
// Space -> O(empty cells) for the recursion
// Special cases: an already solved board, an unsolvable board, an empty
// board, a board with conflicting givens (returns without a solution)
