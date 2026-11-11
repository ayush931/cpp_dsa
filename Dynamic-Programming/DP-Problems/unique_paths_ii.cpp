/**
 * Unique Paths II
 *
 * A robot starts at the top-left corner of an m x n grid and wants to reach the
 * bottom-right corner. It can only move down or right. Cells marked 1 are
 * obstacles and cannot be entered. Return the number of unique paths.
 *
 * Example 1:
 * Input: [[0,0,0],[0,1,0],[0,0,0]]
 * Output: 2
 *
 * Example 2:
 * Input: [[0,1],[0,0]]
 * Output: 1
 *
 * Constraints: 1 <= m, n <= 100, grid[i][j] is 0 or 1.
 * The answer is guaranteed to be at most 2 * 10^9.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every path with recursion. O(2^(m + n)) time.
long long uniquePathsBrute(vector<vector<int>> &grid, int row, int column) {
    int rows = grid.size();
    int columns = grid[0].size();

    if (row >= rows || column >= columns) {
        return 0;
    }
    if (grid[row][column] == 1) {
        return 0;
    }
    if (row == rows - 1 && column == columns - 1) {
        return 1;
    }

    return uniquePathsBrute(grid, row + 1, column) +
           uniquePathsBrute(grid, row, column + 1);
}

// Optimized: 1D DP. dp[c] is the number of ways to reach the current row's
// column c. It starts as the value from above, then adds the value from the
// left. O(m * n) time, O(n) space.
int uniquePathsWithObstacles(vector<vector<int>> &grid) {
    int rows = grid.size();
    int columns = grid[0].size();

    vector<long long> dp(columns, 0);
    dp[0] = 1;

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (grid[row][column] == 1) {
                dp[column] = 0;
            } else if (column > 0) {
                dp[column] += dp[column - 1];
            }
            // column == 0 keeps the value from above (or the initial 1).
        }
    }

    return dp[columns - 1];
}

int main() {
    vector<vector<int>> first = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    vector<vector<int>> second = {{0, 1}, {0, 0}};
    vector<vector<int>> blockedStart = {{1, 0}, {0, 0}};
    vector<vector<int>> blockedEnd = {{0, 0}, {0, 1}};
    vector<vector<int>> singleFree = {{0}};
    vector<vector<int>> singleBlocked = {{1}};
    vector<vector<int>> oneRow = {{0, 0, 0, 0}};
    vector<vector<int>> rowWithObstacle = {{0, 1, 0, 0}};

    cout << uniquePathsBrute(first, 0, 0) << " "
         << uniquePathsWithObstacles(first) << endl;
    // 2 2

    cout << uniquePathsWithObstacles(second) << endl;           // 1
    cout << uniquePathsWithObstacles(blockedStart) << endl;     // 0
    cout << uniquePathsWithObstacles(blockedEnd) << endl;       // 0
    cout << uniquePathsWithObstacles(singleFree) << endl;       // 1
    cout << uniquePathsWithObstacles(singleBlocked) << endl;    // 0
    cout << uniquePathsWithObstacles(oneRow) << endl;           // 1
    cout << uniquePathsWithObstacles(rowWithObstacle) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^(m + n)), two choices at every cell
 *   DP -> O(m * n), every cell is processed once
 * Space complexity:
 *   brute -> O(m + n) recursion depth
 *   DP -> O(n) for one row of states
 *
 * Special cases: an obstacle at the start (0), an obstacle at the destination
 * (0), a single cell (1 if free, 0 if blocked), one row, one column, and a row
 * where an obstacle cuts off every path after it.
 *
 * The first row and first column have exactly one path, so the DP only needs to
 * add the left value when a column exists. Setting dp[column] = 0 at an
 * obstacle discards all paths through that cell.
 */
