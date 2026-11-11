/**
 * Unique Paths
 *
 * A robot starts at the top-left corner of an m x n grid and wants to reach the
 * bottom-right corner. It can only move down or right. Return the number of
 * unique paths.
 *
 * Example 1:
 * Input: m = 3, n = 7
 * Output: 28
 *
 * Example 2:
 * Input: m = 3, n = 2
 * Output: 3
 *
 * Constraints: 1 <= m, n <= 100, answer <= 2 * 10^9.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: explore both moves from every cell.
// O(2^(m + n)) time, O(m + n) recursion space.
long long uniquePathsBrute(int row, int column, int rows, int columns) {
    if (row >= rows || column >= columns) {
        return 0;
    }
    if (row == rows - 1 && column == columns - 1) {
        return 1;
    }

    return uniquePathsBrute(row + 1, column, rows, columns) +
           uniquePathsBrute(row, column + 1, rows, columns);
}

// Optimized: 1D DP. dp[c] is the number of ways to reach the current row's
// column c. The value from above is already stored there, and dp[c - 1] is the
// value from the left. O(m * n) time, O(n) space.
int uniquePaths(int rows, int columns) {
    vector<long long> dp(columns, 1);

    for (int row = 1; row < rows; row++) {
        for (int column = 1; column < columns; column++) {
            dp[column] += dp[column - 1];
        }
    }

    return dp[columns - 1];
}

int main() {
    cout << uniquePathsBrute(0, 0, 3, 7) << " " << uniquePaths(3, 7) << endl;
    // 28 28

    cout << uniquePaths(3, 2) << endl;    // 3
    cout << uniquePaths(1, 1) << endl;    // 1
    cout << uniquePaths(1, 10) << endl;   // 1
    cout << uniquePaths(10, 1) << endl;   // 1
    cout << uniquePaths(3, 3) << endl;    // 6
    cout << uniquePaths(10, 10) << endl;  // 48620

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^(m + n)), two choices per cell
 *   DP -> O(m * n)
 * Space complexity:
 *   brute -> O(m + n) for the recursion stack
 *   DP -> O(n) for one row of states
 *
 * Special cases: one row (1 path), one column (1 path), a single cell (1), and
 * a square grid. The answer is also the binomial coefficient
 * C(m + n - 2, m - 1), which can be computed in O(min(m, n)) time with a
 * multiplicative formula, but the DP avoids overflow issues and is easier to
 * extend to obstacles (see 01_question.cpp).
 *
 * The first row and first column have exactly one path, which is why dp starts
 * filled with 1 and the loops skip row 0 and column 0.
 */
