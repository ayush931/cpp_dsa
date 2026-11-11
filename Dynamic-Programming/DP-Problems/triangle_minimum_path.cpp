/**
 * Given a triangle array, return the minimum path sum from top to bottom.
 * For each step, you may move to an adjacent number of the row below.
 * If you are on index i on the current row, you may move to index i or i + 1.
 *
 * Example 1:
 * Input: triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
 * Output: 11 (2 + 3 + 5 + 1)
 *
 * Example 2:
 * Input: triangle = [[-10]]
 * Output: -10
 *
 * Constraints: 1 <= triangle.length <= 200, -10^4 <= triangle[i][j] <= 10^4
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force recursion -> O(2^n)
int minimumTotalBrute(vector<vector<int>> &triangle, int row, int col) {
    if (row == triangle.size() - 1) {
        return triangle[row][col];
    }

    int down = minimumTotalBrute(triangle, row + 1, col);
    int diagonal = minimumTotalBrute(triangle, row + 1, col + 1);

    return triangle[row][col] + min(down, diagonal);
};

// Memoized recursion -> O(n^2)
int minimumTotalMemo(vector<vector<int>> &triangle, int row, int col,
                     vector<vector<int>> &dp) {
    if (row == triangle.size() - 1) {
        return triangle[row][col];
    }
    if (dp[row][col] != INT_MAX) {
        return dp[row][col];
    }

    int down = minimumTotalMemo(triangle, row + 1, col, dp);
    int diagonal = minimumTotalMemo(triangle, row + 1, col + 1, dp);

    return dp[row][col] = triangle[row][col] + min(down, diagonal);
};

// Bottom-up DP -> O(n^2) time, O(n) extra space
int minimumTotal(vector<vector<int>> &triangle) {
    int rows = triangle.size();
    vector<int> dp = triangle[rows - 1];

    for (int row = rows - 2; row >= 0; row--) {
        for (int col = 0; col <= row; col++) {
            dp[col] = triangle[row][col] + min(dp[col], dp[col + 1]);
        }
    }

    return dp[0];
};

int main() {
    vector<vector<int>> triangle = {{2}, {3, 4}, {6, 5, 7}, {4, 1, 8, 3}};

    vector<vector<int>> dp(triangle.size(),
                           vector<int>(triangle.size(), INT_MAX));

    cout << minimumTotalBrute(triangle, 0, 0) << endl;     // 11
    cout << minimumTotalMemo(triangle, 0, 0, dp) << endl;  // 11
    cout << minimumTotal(triangle) << endl;                // 11

    vector<vector<int>> single = {{-10}};
    cout << minimumTotal(single) << endl;  // -10

    return 0;
}

// Special cases: one row, all negative values, all positive values,
// duplicate values, a large triangle (use the O(n) space version)
