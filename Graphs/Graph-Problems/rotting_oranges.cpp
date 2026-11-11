/**
 * Rotting Oranges
 *
 * You are given an m x n grid where each cell is one of:
 *   0 -> empty, 1 -> fresh orange, 2 -> rotten orange.
 *
 * Every minute, a fresh orange adjacent (4-directionally) to a rotten orange
 * becomes rotten. Return the minimum number of minutes until no fresh orange
 * remains, or -1 if that is impossible.
 *
 * Example:
 * Input:  [[2,1,1],[1,1,0],[0,1,1]]
 * Output: 4
 *
 * Input:  [[2,1,1],[0,1,1],[1,0,1]]
 * Output: -1
 *
 * Input:  [[0,2]]
 * Output: 0
 *
 * Constraints: 1 <= m, n <= 10, grid[i][j] is 0, 1, or 2.
 */

#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
   public:
    int orangesRotting(vector<vector<int>> &grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }

        queue<pair<int, int>> qu;
        int freshOranges = 0;
        int n = grid.size();
        int m = grid[0].size();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1) {
                    freshOranges++;
                } else if (grid[i][j] == 2) {
                    qu.push({i, j});  // multi-source BFS
                }
            }
        }

        if (freshOranges == 0) {
            return 0;
        }

        qu.push({-1, -1});  // level separator

        int minutes = 0;
        vector<vector<int>> dir = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        while (!qu.empty()) {
            auto cell = qu.front();
            qu.pop();

            if (cell.first == -1 && cell.second == -1) {
                minutes++;

                if (!qu.empty()) {
                    qu.push({-1, -1});
                } else {
                    break;
                }
            } else {
                int i = cell.first;
                int j = cell.second;

                for (int d = 0; d < 4; d++) {
                    int newRow = i + dir[d][0];
                    int newCol = j + dir[d][1];

                    if (newCol < 0 || newRow < 0 || newRow >= n ||
                        newCol >= m) {
                        continue;
                    }

                    if (grid[newRow][newCol] == 2 ||
                        grid[newRow][newCol] == 0) {
                        continue;
                    }

                    freshOranges--;
                    grid[newRow][newCol] = 2;
                    qu.push({newRow, newCol});
                }
            }
        }

        // The final separator adds one extra minute that never rotted anything.
        return (freshOranges == 0) ? minutes - 1 : -1;
    }
};

int main() {
    vector<vector<int>> first = {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}};

    vector<vector<int>> second = {{2, 1, 1}, {0, 1, 1}, {1, 0, 1}};

    vector<vector<int>> third = {{0, 2}};

    vector<vector<int>> noFresh = {{0, 2}, {0, 0}};

    vector<vector<int>> noRotten = {{1, 1}, {1, 1}};

    Solution solution;

    cout << solution.orangesRotting(first) << endl;     // 4
    cout << solution.orangesRotting(second) << endl;    // -1
    cout << solution.orangesRotting(third) << endl;     // 0
    cout << solution.orangesRotting(noFresh) << endl;   // 0
    cout << solution.orangesRotting(noRotten) << endl;  // -1

    return 0;
}

/**
 * Time complexity -> O(m * n), every cell enters the queue at most once
 * Space complexity -> O(m * n) for the queue
 *
 * Special cases: no fresh oranges (0), fresh oranges but no rotten source
 * (-1), all oranges rotten (0), an unreachable fresh orange (-1), a single
 * cell, an empty grid.
 *
 * A level-order BFS naturally gives the number of minutes. The separator
 * approach above is equivalent to processing one full queue level at a time.
 */
