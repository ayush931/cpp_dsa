/**
 * Minimum Path Sum
 *
 * Given an m x n grid filled with non-negative numbers, find a path from the
 * top-left to the bottom-right that minimizes the sum of all numbers along the
 * path. You can only move down or right.
 *
 * Example 1:
 * Input: [[1,3,1],[1,5,1],[4,2,1]]
 * Output: 7  (1 -> 3 -> 1 -> 1 -> 1)
 *
 * Example 2:
 * Input: [[1,2,3],[4,5,6]]
 * Output: 12
 *
 * Constraints: 1 <= m, n <= 200, 0 <= grid[i][j] <= 200.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: explore both moves from every cell.
// O(2^(m + n)) time, O(m + n) recursion space.
int minPathSumBrute(vector<vector<int>> &grid, int row, int column) {
    int rows = grid.size();
    int columns = grid[0].size();

    if (row >= rows || column >= columns) {
        return INT_MAX;
    }
    if (row == rows - 1 && column == columns - 1) {
        return grid[row][column];
    }

    int down = minPathSumBrute(grid, row + 1, column);
    int right = minPathSumBrute(grid, row, column + 1);

    return grid[row][column] + min(down, right);
}

// Optimized: 1D DP. dp[c] holds the best sum to reach column c in the current
// row. It already contains the value from above, so we only compare with the
// value from the left. O(m * n) time, O(n) space.
int minPathSum(vector<vector<int>> &grid) {
    int rows = grid.size();
    int columns = grid[0].size();

    vector<int> dp(columns, 0);

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (row == 0 && column == 0) {
                dp[column] = grid[row][column];
            } else if (row == 0) {
                dp[column] = dp[column - 1] + grid[row][column];
            } else if (column == 0) {
                dp[column] = dp[column] + grid[row][column];
            } else {
                dp[column] =
                    min(dp[column], dp[column - 1]) + grid[row][column];
            }
        }
    }

    return dp[columns - 1];
}

int main() {
    vector<vector<int>> first = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    vector<vector<int>> second = {{1, 2, 3}, {4, 5, 6}};
    vector<vector<int>> single = {{7}};
    vector<vector<int>> oneRow = {{1, 2, 3, 4}};
    vector<vector<int>> oneColumn = {{1}, {2}, {3}};
    vector<vector<int>> zeros = {{0, 0}, {0, 0}};

    cout << minPathSumBrute(first, 0, 0) << " " << minPathSum(first) << endl;
    // 7 7

    cout << minPathSum(second) << endl;     // 12
    cout << minPathSum(single) << endl;     // 7
    cout << minPathSum(oneRow) << endl;     // 10
    cout << minPathSum(oneColumn) << endl;  // 6
    cout << minPathSum(zeros) << endl;      // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^(m + n))
 *   DP -> O(m * n)
 * Space complexity:
 *   brute -> O(m + n) for the recursion stack
 *   DP -> O(n) for one row of states
 *
 * Special cases: a single cell (its value), one row (only right moves), one
 * column (only down moves), all zeros (0), and large values (the maximum sum is
 * about 200 * 400 = 80,000, so int is enough).
 *
 * The recurrence is:
 *   dp[r][c] = grid[r][c] + min(dp[r - 1][c], dp[r][c - 1])
 * with the first row and first column taking their only possible predecessor.
 */
