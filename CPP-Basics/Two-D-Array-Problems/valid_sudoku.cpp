// Valid Sudoku
//
// A partially filled 9 x 9 Sudoku board is valid when:
//   - each row has no repeated digit from '1' to '9'
//   - each column has no repeated digit
//   - each of the nine 3 x 3 boxes has no repeated digit
//   - empty cells ('.') are ignored
//
// The board does not need to be solvable; only the filled cells are checked.
//
// Example:
//   5 3 . | . 7 . | . . .
//   6 . . | 1 9 5 | . . .
//   . 9 8 | . . . | . 6 .
//   ------+-------+------
//   8 . . | . 6 . | . . 3
//   4 . . | 8 . 3 | . . 1
//   7 . . | . 2 . | . . 6
//   ------+-------+------
//   . 6 . | . . . | 2 8 .
//   . . . | 4 1 9 | . . 5
//   . . . | . 8 . | . 7 9
//
// This board is valid.
//
// Brute force: for every filled cell, scan its row, column, and box.
// O(81 * 27) time, which is O(1) because the board size is fixed.
//
// Optimized: one pass with three arrays of nine bitmasks, one for rows, one
// for columns, and one for boxes. For a digit d, the bit is 1 << (d - '1').
// If the bit is already set in any of the three masks, the board is invalid.
// O(81) time and O(1) space.
//
// Edge cases:
//   - an all-empty board is valid
//   - a board with a duplicate in one row but not in a column or box
//   - a duplicate in a box that is not in the same row or column
//   - a character outside '1'..'9' and '.' violates the input contract
//   - a board with the wrong dimensions is not a Sudoku board
//
// Complexity:
//   brute     -> O(1) for a fixed 9 x 9 board (27 checks per cell)
//   optimized -> O(1) for a fixed 9 x 9 board (one check per cell)

#include <iostream>
#include <vector>
using namespace std;

bool isValidSudokuBrute(const vector<vector<char>> &board) {
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            char digit = board[row][column];

            if (digit == '.') {
                continue;
            }

            for (int other = 0; other < 9; other++) {
                if (other != column && board[row][other] == digit) {
                    return false;
                }

                if (other != row && board[other][column] == digit) {
                    return false;
                }
            }

            int boxRow = (row / 3) * 3;
            int boxColumn = (column / 3) * 3;

            for (int r = boxRow; r < boxRow + 3; r++) {
                for (int c = boxColumn; c < boxColumn + 3; c++) {
                    if ((r != row || c != column) && board[r][c] == digit) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool isValidSudoku(const vector<vector<char>> &board) {
    int rows[9] = {0};
    int columns[9] = {0};
    int boxes[9] = {0};

    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            char digit = board[row][column];

            if (digit == '.') {
                continue;
            }

            int bit = 1 << (digit - '1');
            int box = (row / 3) * 3 + (column / 3);

            if ((rows[row] & bit) != 0 || (columns[column] & bit) != 0 ||
                (boxes[box] & bit) != 0) {
                return false;
            }

            rows[row] |= bit;
            columns[column] |= bit;
            boxes[box] |= bit;
        }
    }

    return true;
}

vector<vector<char>> validBoard() {
    return {{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
            {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
            {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
            {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
            {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
            {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
            {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
            {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
            {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};
}

void report(const string &label, const vector<vector<char>> &board) {
    cout << label << " brute: " << isValidSudokuBrute(board)
         << " fast: " << isValidSudoku(board) << endl;
}

int main() {
    vector<vector<char>> valid = validBoard();
    report("valid", valid);

    vector<vector<char>> rowDuplicate = valid;
    rowDuplicate[0][1] = '5';  // row 0 already has 5 at column 0
    report("row duplicate", rowDuplicate);

    vector<vector<char>> columnDuplicate = valid;
    columnDuplicate[0][2] = '8';  // column 2 already has 8 at row 2
    report("column duplicate", columnDuplicate);

    vector<vector<char>> boxDuplicate = valid;
    boxDuplicate[0][2] = '6';  // top-left box already has 6 at row 1
    report("box duplicate", boxDuplicate);

    vector<vector<char>> empty(9, vector<char>(9, '.'));
    report("empty", empty);

    return 0;
}

// Output:
// valid brute: 1 fast: 1
// row duplicate brute: 0 fast: 0
// column duplicate brute: 0 fast: 0
// box duplicate brute: 0 fast: 0
// empty brute: 1 fast: 1
//
// The bitmask version replaces 27 comparisons per cell with three bit tests.
// Both versions are O(1) for a fixed 9 x 9 board, but the bitmask version has
// a smaller constant and is easier to extend to 16 x 16 Sudoku with 16-bit
// masks.
