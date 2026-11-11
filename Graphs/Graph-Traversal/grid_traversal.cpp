/**
 * Grid traversal -> treat every cell as a graph node
 * Neighbours are up, down, left and right (sometimes diagonals)
 * BFS gives the shortest path when every move has the same cost
 *
 * Problem: shortest path from (0, 0) to (n - 1, m - 1)
 * 0 -> open cell, 1 -> blocked cell
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int shortestPath(vector<vector<int>> &grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return -1;
    }

    vector<vector<int>> distance(rows, vector<int>(cols, -1));
    queue<pair<int, int>> q;

    q.push({0, 0});
    distance[0][0] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [row, col] = q.front();
        q.pop();

        if (row == rows - 1 && col == cols - 1) {
            return distance[row][col];
        }

        for (int i = 0; i < 4; i++) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            bool inside =
                newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols;

            if (inside && grid[newRow][newCol] == 0 &&
                distance[newRow][newCol] == -1) {
                distance[newRow][newCol] = distance[row][col] + 1;
                q.push({newRow, newCol});
            }
        }
    }

    return -1;
};

int main() {
    vector<vector<int>> grid = {{0, 0, 0}, {1, 1, 0}, {0, 0, 0}};

    cout << shortestPath(grid) << endl;  // 4

    return 0;
}

// Special cases: start is blocked, destination is blocked, no path,
// single row or column, 8-directional movement, weighted cells (use Dijkstra)
