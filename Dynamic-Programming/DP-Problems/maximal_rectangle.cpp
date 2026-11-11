/**
 * Maximal Rectangle
 *
 * Given a rows x cols binary matrix filled with '0's and '1's, find the
 * largest rectangle containing only '1's and return its area.
 *
 * Example 1:
 * Input:
 *   ["1","0","1","0","0"],
 *   ["1","0","1","1","1"],
 *   ["1","1","1","1","1"],
 *   ["1","0","0","1","0"]
 * Output: 6
 *
 * Example 2:
 * Input: [["0"]]
 * Output: 0
 *
 * Example 3:
 * Input: [["1"]]
 * Output: 1
 *
 * Constraints: 1 <= rows, cols <= 200, matrix[i][j] is '0' or '1'.
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

// Brute force: build 2D prefix sums, then test every possible rectangle.
// O(rows^2 * cols^2) time, O(rows * cols) space.
int maximalRectangleBrute(vector<vector<char>> &matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    // prefix[r + 1][c + 1] = number of ones in matrix[0..r][0..c]
    vector<vector<int>> prefix(rows + 1, vector<int>(cols + 1, 0));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            prefix[r + 1][c + 1] = (matrix[r][c] == '1') + prefix[r][c + 1] +
                                   prefix[r + 1][c] - prefix[r][c];
        }
    }

    int best = 0;

    for (int top = 0; top < rows; top++) {
        for (int bottom = top; bottom < rows; bottom++) {
            for (int left = 0; left < cols; left++) {
                for (int right = left; right < cols; right++) {
                    int ones = prefix[bottom + 1][right + 1] -
                               prefix[top][right + 1] -
                               prefix[bottom + 1][left] + prefix[top][left];

                    int area = (bottom - top + 1) * (right - left + 1);

                    if (ones == area) {
                        best = max(best, area);  // all ones
                    }
                }
            }
        }
    }

    return best;
}

// Largest rectangle in a histogram using a monotonic stack.
// O(n) time, O(n) space.
int largestRectangleArea(vector<int> &heights) {
    vector<int> h = heights;
    h.push_back(0);  // sentinel: flush the stack at the end

    stack<int> pending;  // indices with increasing heights
    int best = 0;

    for (int i = 0; i < h.size(); i++) {
        while (!pending.empty() && h[pending.top()] >= h[i]) {
            int height = h[pending.top()];
            pending.pop();

            int left = pending.empty() ? -1 : pending.top();
            int width = i - left - 1;

            best = max(best, height * width);
        }

        pending.push(i);
    }

    return best;
}

// Optimized: treat every row as the base of a histogram.
// heights[c] = number of consecutive ones ending at the current row.
// O(rows * cols) time, O(cols) space.
int maximalRectangle(vector<vector<char>> &matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<int> heights(cols, 0);
    int best = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (matrix[r][c] == '1') {
                heights[c]++;
            } else {
                heights[c] = 0;
            }
        }

        best = max(best, largestRectangleArea(heights));
    }

    return best;
}

int main() {
    vector<vector<char>> first = {{'1', '0', '1', '0', '0'},
                                  {'1', '0', '1', '1', '1'},
                                  {'1', '1', '1', '1', '1'},
                                  {'1', '0', '0', '1', '0'}};

    vector<vector<char>> zero = {{'0'}};
    vector<vector<char>> one = {{'1'}};

    vector<vector<char>> allOnes = {{'1', '1', '1'}, {'1', '1', '1'}};

    vector<vector<char>> checkerboard = {
        {'1', '0', '1'}, {'0', '1', '0'}, {'1', '0', '1'}};

    cout << maximalRectangleBrute(first) << " " << maximalRectangle(first)
         << endl;
    // 6 6

    cout << maximalRectangle(zero) << endl;          // 0
    cout << maximalRectangle(one) << endl;           // 1
    cout << maximalRectangle(allOnes) << endl;       // 6
    cout << maximalRectangle(checkerboard) << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(rows^2 * cols^2), four nested loops
 *   histogram -> O(rows * cols), one monotonic-stack pass per row
 * Space complexity:
 *   brute -> O(rows * cols) for the prefix sums
 *   histogram -> O(cols) for the heights and the stack
 *
 * Special cases: an all-zero matrix (0), an all-one matrix (rows * cols), a
 * single row, a single column, a one-cell matrix, and a matrix with only
 * isolated ones (1).
 *
 * The key reduction: a maximal rectangle ending at row r is a rectangle in the
 * histogram of consecutive ones above each column. The same monotonic-stack
 * idea also solves Largest Rectangle in Histogram.
 */
