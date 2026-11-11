/**
 * Consider an N * N chessboard. N Queen problems is to accomodate N queens on
 * the N * N chessboard such that no 2 queens can attack each other. Input = 4
 * Output:-
 * {0, 1, 0, 0},
 * {0, 0, 0, 1},
 * {1, 0, 0, 0},
 * {0, 0, 1, 0}
 */

#include <bits/stdc++.h>
using namespace std;

bool canPlaceQueen(int row, int col, vector<vector<char>> &grid) {
    // is someone is attacking from vertical up
    for (int i = row - 1; i >= 0; i--) {
        if (grid[i][col] == 'Q') {
            return false;
        }
    }

    for (int i = row - 1, j = col - 1; i >= 0 and j >= 0; i--, j--) {
        if (grid[i][j] == 'Q') {
            return false;
        }
    }

    for (int i = row - 1, j = col + 1; i >= 0 and j < grid.size(); i--, j++) {
        if (grid[i][j] == 'Q') {
            return false;
        }
    }

    return true;
};

void nqueen(int currRow, int n, vector<vector<char>> &grid) {
    if (currRow == n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << grid[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "**\n";
    }

    for (int col = 0; col < n; col++) {
        // we will go to all the columns
        // lets check if we can place the queen at currRow and col
        if (canPlaceQueen(currRow, col, grid)) {
            grid[currRow][col] = 'Q';
            nqueen(currRow + 1, n, grid);
            grid[currRow][col] = '.';
        }
    }
};

int main() {
    int n = 4;
    vector<vector<char>> grid(n, vector<char>(n, '.'));
    nqueen(0, n, grid);

    return 0;
}