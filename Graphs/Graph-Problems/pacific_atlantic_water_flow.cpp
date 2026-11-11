/**
 * Pacific Atlantic Water Flow
 *
 * There is an m x n rectangular island that borders both the Pacific Ocean and
 * the Atlantic Ocean. The Pacific touches the left and top edges, the Atlantic
 * touches the right and bottom edges. Rain water can flow from a cell to a
 * neighbor if the neighbor's height is less than or equal to the cell's
 * height. Return every cell from which water can reach both oceans.
 *
 * Example:
 * Input: heights = [
 *   [1, 2, 2, 3, 5],
 *   [3, 2, 3, 4, 4],
 *   [2, 4, 5, 3, 1],
 *   [6, 7, 1, 4, 5],
 *   [5, 1, 1, 2, 4]
 * ]
 * Output: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]
 *
 * Constraints: 1 <= m, n <= 200, 0 <= heights[r][c] <= 10^5.
 */

#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
   public:
    vector<vector<int>> dir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int rows;
    int cols;
    vector<vector<int>> h;

    vector<vector<int>> pacificAtlantic(vector<vector<int>> &heights) {
        if (heights.empty() || heights[0].empty()) {
            return {};
        }

        rows = heights.size();
        cols = heights[0].size();
        h = heights;

        queue<pair<int, int>> pacificBfs;
        queue<pair<int, int>> atlanticBfs;

        // Multi-source BFS: every cell touching an ocean is a source.
        for (int i = 0; i < rows; i++) {
            pacificBfs.push({i, 0});
            atlanticBfs.push({i, cols - 1});
        }

        for (int j = 1; j < cols; j++) {
            pacificBfs.push({0, j});
        }

        for (int j = 0; j < cols - 1; j++) {
            atlanticBfs.push({rows - 1, j});
        }

        vector<vector<bool>> pacific = bfs(pacificBfs);
        vector<vector<bool>> atlantic = bfs(atlanticBfs);

        vector<vector<int>> result;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pacific[i][j] && atlantic[i][j]) {
                    result.push_back({i, j});
                }
            }
        }

        return result;
    }

    // Reverse BFS: flow from the ocean inward, only to equal or higher cells.
    vector<vector<bool>> bfs(queue<pair<int, int>> &qu) {
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

        while (!qu.empty()) {
            auto cell = qu.front();
            qu.pop();

            int i = cell.first;
            int j = cell.second;

            if (visited[i][j]) {
                continue;
            }
            visited[i][j] = true;

            for (int d = 0; d < 4; d++) {
                int newRow = i + dir[d][0];
                int newCol = j + dir[d][1];

                if (newRow < 0 || newCol < 0 || newRow >= rows ||
                    newCol >= cols) {
                    continue;  // left the grid
                }

                if (visited[newRow][newCol]) {
                    continue;
                }

                // Water flows downhill, so walking backward we may only climb.
                if (h[newRow][newCol] < h[i][j]) {
                    continue;
                }

                qu.push({newRow, newCol});
            }
        }

        return visited;
    }
};

void print(vector<vector<int>> &cells) {
    for (vector<int> &cell : cells) {
        cout << "[" << cell[0] << "," << cell[1] << "] ";
    }
    cout << endl;
}

int main() {
    vector<vector<int>> heights = {{1, 2, 2, 3, 5},
                                   {3, 2, 3, 4, 4},
                                   {2, 4, 5, 3, 1},
                                   {6, 7, 1, 4, 5},
                                   {5, 1, 1, 2, 4}};

    vector<vector<int>> single = {{1}};

    vector<vector<int>> flat = {{1, 1}, {1, 1}};

    Solution solution;

    vector<vector<int>> first = solution.pacificAtlantic(heights);
    print(first);
    // [0,4] [1,3] [1,4] [2,2] [3,0] [3,1] [4,0]

    vector<vector<int>> one = solution.pacificAtlantic(single);
    print(one);  // [0,0]

    vector<vector<int>> all = solution.pacificAtlantic(flat);
    print(all);  // [0,0] [0,1] [1,0] [1,1]

    return 0;
}

/**
 * Time complexity -> O(m * n), each ocean BFS visits every cell at most once
 * Space complexity -> O(m * n) for the two visited matrices and the queues
 *
 * Special cases: a single cell (both oceans), one row, one column, all equal
 * heights (every cell reaches both), a strictly increasing matrix, a strictly
 * decreasing matrix, an empty grid.
 *
 * The key trick is to search backward from the oceans instead of forward from
 * every cell. A forward DFS from every cell would be O((m * n)^2).
 */
