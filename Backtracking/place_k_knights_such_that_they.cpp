/**
 * Place K knights such that they do not attack each other

 * Given integers M, N and K, the task is to place K knights on an M * N
 chessboard that they do not attack each other. The knights are expected to be
 placed on the different squares on the board. A knight can move two squares
 vertically and one square horizontally or two squares horizontally and one
 square vertically. The knight attack each other if one of them can reach the
 other in single move. There are multiple ways of placing the knights on a M * N
 chessboard or sometimes, no way of placing them. We are expected out to list
 out all the possible solutions.

 * Input: M = 3, N = 3, K = 5
 * Output: K A K A K A K A K A K A K K K A K A
 * Total number of solutions: 2
 */

#include <bits/stdc++.h>
using namespace std;

bool canWePlace(vector<vector<char>> &grid, int i, int j) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Only the two rows above the current row can already contain a knight.
    if (i - 1 >= 0 && j - 2 >= 0 && grid[i - 1][j - 2] == 'K') {
        return false;
    }
    if (i - 2 >= 0 && j - 1 >= 0 && grid[i - 2][j - 1] == 'K') {
        return false;
    }
    if (i - 1 >= 0 && j + 2 < cols && grid[i - 1][j + 2] == 'K') {
        return false;
    }
    if (i - 2 >= 0 && j + 1 < cols && grid[i - 2][j + 1] == 'K') {
        return false;
    }

    return i >= 0 && i < rows && j >= 0 && j < cols;
};

void knights(int m, int n, vector<vector<char>> &grid, int k, int i, int j) {
    if (k == 0) {
        // all knights are placed
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << "***\n";
        return;
    }

    if (j == n) {
        return knights(m, n, grid, k, i + 1, 0);
    }

    if (i == m) {
        return;
    }

    for (int row = i; row < m; row++) {
        for (int col = (row == i ? j : 0); col < n; col++) {
            if (canWePlace(grid, row, col)) {
                grid[row][col] = 'K';
                knights(m, n, grid, k - 1, row, col + 1);
                grid[row][col] = ' ';
            }
        }
    }
};

int main() {
    int n = 5, m = 5, k = 2;
    vector<vector<char>> grid(n, vector<char>(m, ' '));
    knights(m, n, grid, k, 0, 0);
    return 0;
}