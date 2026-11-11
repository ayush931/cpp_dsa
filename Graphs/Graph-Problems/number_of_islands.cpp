/**
 * Number of Islands
 *
 * Given an m x n 2D binary grid which represents a map of '1's (land) and
 * '0's (water), return the number of islands. An island is surrounded by water
 * and is formed by connecting adjacent lands horizontally or vertically.
 *
 * Example:
 * Input:
 *   ["1","1","0","0","0"],
 *   ["1","1","0","0","0"],
 *   ["0","0","1","0","0"],
 *   ["0","0","0","1","1"]
 * Output: 3
 *
 * Constraints: 1 <= m, n <= 300, grid[i][j] is '0' or '1'.
 */

#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
   public:
    int numIslands(vector<vector<char>> &grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }

        int rows = grid.size();
        int cols = grid[0].size();

        int cc = 0;  // number of connected components

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == '0') {
                    continue;  // water
                }

                // New unvisited land piece -> new connected component.
                cc++;
                grid[r][c] = '0';  // mark as visited

                queue<pair<int, int>> qu;
                qu.push({r, c});

                while (!qu.empty()) {
                    auto current = qu.front();
                    qu.pop();

                    int currentRow = current.first;
                    int currentCol = current.second;

                    // up
                    if (currentRow - 1 >= 0 &&
                        grid[currentRow - 1][currentCol] == '1') {
                        qu.push({currentRow - 1, currentCol});
                        grid[currentRow - 1][currentCol] = '0';
                    }

                    // down
                    if (currentRow + 1 < rows &&
                        grid[currentRow + 1][currentCol] == '1') {
                        qu.push({currentRow + 1, currentCol});
                        grid[currentRow + 1][currentCol] = '0';
                    }

                    // left
                    if (currentCol - 1 >= 0 &&
                        grid[currentRow][currentCol - 1] == '1') {
                        qu.push({currentRow, currentCol - 1});
                        grid[currentRow][currentCol - 1] = '0';
                    }

                    // right
                    if (currentCol + 1 < cols &&
                        grid[currentRow][currentCol + 1] == '1') {
                        qu.push({currentRow, currentCol + 1});
                        grid[currentRow][currentCol + 1] = '0';
                    }
                }
            }
        }

        return cc;
    }
};

int main() {
    vector<vector<char>> first = {{'1', '1', '1', '1', '0'},
                                  {'1', '1', '0', '1', '0'},
                                  {'1', '1', '0', '0', '0'},
                                  {'0', '0', '0', '0', '0'}};

    vector<vector<char>> second = {{'1', '1', '0', '0', '0'},
                                   {'1', '1', '0', '0', '0'},
                                   {'0', '0', '1', '0', '0'},
                                   {'0', '0', '0', '1', '1'}};

    vector<vector<char>> allWater = {{'0', '0'}, {'0', '0'}};

    vector<vector<char>> allLand = {{'1', '1'}, {'1', '1'}};

    Solution solution;

    cout << solution.numIslands(first) << endl;     // 1
    cout << solution.numIslands(second) << endl;    // 3
    cout << solution.numIslands(allWater) << endl;  // 0
    cout << solution.numIslands(allLand) << endl;   // 1

    return 0;
}

/**
 * Time complexity -> O(m * n), every cell is visited at most once
 * Space complexity -> O(min(m, n)) for the BFS queue in the worst case
 *
 * Special cases: all water (0 islands), all land (1 island), one row, one
 * column, diagonal land does not connect, an empty grid, a single cell.
 *
 * A DFS or a union-find solution also works. The grid is modified in place to
 * mark visited cells, so the input is destroyed.
 */
