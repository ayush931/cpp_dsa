/**
 * Broken Profile DP (plug DP)
 *
 * Count the number of ways to tile a rows-by-columns board with 1x2 and 2x1
 * dominoes. The board has no holes and every cell must be covered exactly
 * once.
 *
 * A full profile DP keeps one bit per column. The broken profile processes one
 * cell at a time and keeps a mask with one bit per column:
 *
 *   for columns before the current cell -> the bit says whether the cell in
 *   the next row is already covered by a vertical domino
 *
 *   for columns at or after the current cell -> the bit says whether the cell
 *   in the current row is already covered, either by a horizontal domino in
 *   this row or by a vertical domino from the row above
 *
 * When the cell is already covered, clear its bit and move on. Otherwise:
 *   - place a horizontal domino -> set the bit of the next column
 *   - place a vertical domino   -> set the current bit, which now refers to
 *     the cell directly below
 *
 * The mask is "broken" at the current column: the left part already describes
 * the next row while the right part still describes the current row.
 *
 * Complexity -> O(rows * columns * 2^min(rows, columns)). The mask uses the
 * smaller dimension, so the board is transposed internally when needed.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

long long countTilings(int rows, int columns) {
    if (rows <= 0 || columns <= 0) {
        return 0;
    }
    if ((rows * columns) % 2 != 0) {
        return 0;  // odd area cannot be tiled
    }

    // The profile width is the smaller dimension.
    int width = min(rows, columns);
    int height = max(rows, columns);

    int states = 1 << width;
    vector<long long> dp(states, 0);
    vector<long long> next(states, 0);

    dp[0] = 1;

    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            fill(next.begin(), next.end(), 0);

            for (int mask = 0; mask < states; mask++) {
                if (dp[mask] == 0) {
                    continue;
                }

                if (mask & (1 << column)) {
                    // The cell is already covered. After advancing, the bit
                    // describes the cell below and that cell is not covered
                    // yet.
                    next[mask ^ (1 << column)] += dp[mask];
                    continue;
                }

                // Place a horizontal domino over (row, column) and (row, column
                // + 1).
                if (column + 1 < width && !(mask & (1 << (column + 1)))) {
                    next[mask | (1 << (column + 1))] += dp[mask];
                }

                // Place a vertical domino over (row, column) and (row + 1,
                // column).
                if (row + 1 < height) {
                    next[mask | (1 << column)] += dp[mask];
                }
            }

            dp.swap(next);
        }
    }

    return dp[0];  // no domino may stick out below the last row
}

long long bruteHelper(vector<vector<bool>> &used, int rows, int columns,
                      int cell) {
    if (cell == rows * columns) {
        return 1;
    }

    int row = cell / columns;
    int column = cell % columns;

    if (used[row][column]) {
        return bruteHelper(used, rows, columns, cell + 1);
    }

    long long total = 0;

    if (column + 1 < columns && !used[row][column + 1]) {
        used[row][column] = true;
        used[row][column + 1] = true;

        total += bruteHelper(used, rows, columns, cell + 1);

        used[row][column] = false;
        used[row][column + 1] = false;
    }

    if (row + 1 < rows && !used[row + 1][column]) {
        used[row][column] = true;
        used[row + 1][column] = true;

        total += bruteHelper(used, rows, columns, cell + 1);

        used[row][column] = false;
        used[row + 1][column] = false;
    }

    return total;
}

long long countTilingsBrute(int rows, int columns) {
    if (rows <= 0 || columns <= 0) {
        return 0;
    }
    if ((rows * columns) % 2 != 0) {
        return 0;
    }

    vector<vector<bool>> used(rows, vector<bool>(columns, false));
    return bruteHelper(used, rows, columns, 0);
}

int main() {
    cout << countTilings(2, 2) << " " << countTilingsBrute(2, 2) << endl;
    // 2 2

    cout << countTilings(2, 3) << " " << countTilingsBrute(2, 3) << endl;
    // 3 3

    cout << countTilings(3, 2) << " " << countTilingsBrute(3, 2) << endl;
    // 3 3

    cout << countTilings(3, 3) << endl;  // 0 (odd area)
    cout << countTilings(1, 1) << endl;  // 0
    cout << countTilings(1, 2) << endl;  // 1
    cout << countTilings(2, 1) << endl;  // 1
    cout << countTilings(1, 4) << endl;  // 1
    cout << countTilings(4, 1) << endl;  // 1
    cout << countTilings(4, 4) << endl;  // 36
    cout << countTilings(4, 6) << endl;  // 281
    cout << countTilings(0, 5) << endl;  // 0
    cout << countTilings(2, 6) << endl;  // 13 (Fibonacci F7)

    // 2 x n tilings follow the Fibonacci numbers: 1, 2, 3, 5, 8, 13, ...
    for (int n = 1; n <= 8; n++) {
        cout << countTilings(2, n) << " ";
    }
    cout << endl;
    // 1 2 3 5 8 13 21 34

    return 0;
}

/**
 * Time complexity:
 *   broken profile -> O(rows * columns * 2^min(rows, columns))
 *   brute -> exponential in the number of cells; only useful for tiny boards
 * Space complexity -> O(2^min(rows, columns)) for the two DP arrays, plus
 * O(rows * columns) recursion for the brute force
 *
 * Special cases:
 *   - odd area -> 0, because every domino covers two cells
 *   - one row or one column -> 1 if the length is even, else 0
 *   - 1x1 -> 0
 *   - 0 rows or 0 columns -> 0
 *   - a board with a hole is not supported by this recurrence; it needs the
 *     hole marked as blocked and the transition must forbid covering it
 *
 * Why the final answer is dp[0]: after the last cell, every bit describes a
 * cell below the board. A vertical domino cannot be placed there, so a valid
 * tiling leaves all bits zero.
 *
 * The same broken-profile idea solves:
 *   - tiling with L-trominoes or arbitrary polyominoes
 *   - counting Hamiltonian paths and cycles on a grid (plug DP)
 *   - matching problems on a narrow grid
 *   - connectivity states for plugs (the "plug DP" generalization)
 */
