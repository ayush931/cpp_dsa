/**
 * 0-1 BFS on a Grid
 *
 * Given an m x n grid, each cell contains a sign:
 *   1 -> go right, 2 -> go left, 3 -> go down, 4 -> go up.
 *
 * Start at (0, 0) and reach (m - 1, n - 1). You may change the sign of any cell
 * at cost 1. Return the minimum total cost.
 *
 * Example 1:
 * Input: [[1,1,1,1],[2,2,2,2],[1,1,1,1],[2,2,2,2]]
 * Output: 3
 *
 * Example 2:
 * Input: [[1,1,3],[3,2,2],[1,1,4]]
 * Output: 0
 *
 * Example 3:
 * Input: [[1,2],[4,3]]
 * Output: 1
 *
 * Constraints: 1 <= m, n <= 100, grid[i][j] is 1..4.
 *
 * A normal Dijkstra on this grid is O(m * n * log(m * n)). Every edge weight
 * is 0 (follow the existing sign) or 1 (change the sign), so 0-1 BFS with a
 * deque solves it in O(m * n).
 */

#include <climits>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

int minCost(vector<vector<int>> &grid) {
    int rows = grid.size();
    int columns = grid[0].size();

    vector<vector<int>> distance(rows, vector<int>(columns, INT_MAX));
    deque<pair<int, int>> pending;

    // Direction order must match the sign values: right, left, down, up.
    int rowChange[4] = {0, 0, 1, -1};
    int columnChange[4] = {1, -1, 0, 0};

    distance[0][0] = 0;
    pending.push_front({0, 0});

    while (!pending.empty()) {
        int row = pending.front().first;
        int column = pending.front().second;
        pending.pop_front();

        for (int direction = 0; direction < 4; direction++) {
            int nextRow = row + rowChange[direction];
            int nextColumn = column + columnChange[direction];

            if (nextRow < 0 || nextColumn < 0 || nextRow >= rows ||
                nextColumn >= columns) {
                continue;
            }

            // Following the existing sign costs 0; any other direction costs 1.
            int weight = (direction + 1 == grid[row][column]) ? 0 : 1;
            int nextDistance = distance[row][column] + weight;

            if (nextDistance >= distance[nextRow][nextColumn]) {
                continue;
            }

            distance[nextRow][nextColumn] = nextDistance;

            if (weight == 0) {
                pending.push_front({nextRow, nextColumn});
            } else {
                pending.push_back({nextRow, nextColumn});
            }
        }
    }

    return distance[rows - 1][columns - 1];
}

int main() {
    vector<vector<int>> first = {
        {1, 1, 1, 1}, {2, 2, 2, 2}, {1, 1, 1, 1}, {2, 2, 2, 2}};

    vector<vector<int>> second = {{1, 1, 3}, {3, 2, 2}, {1, 1, 4}};

    vector<vector<int>> third = {{1, 2}, {4, 3}};
    vector<vector<int>> single = {{1}};

    vector<vector<int>> allRight = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    cout << minCost(first) << endl;   // 3
    cout << minCost(second) << endl;  // 0
    cout << minCost(third) << endl;   // 1
    cout << minCost(single) << endl;  // 0
    cout << minCost(allRight)
         << endl;  // 2 (right twice, then change down twice)

    return 0;
}

/**
 * Time complexity -> O(m * n), because every cell is relaxed a constant number
 * of times and each relaxation is O(1)
 * Space complexity -> O(m * n) for the distance matrix and the deque
 *
 * Special cases: a single cell (0), a grid that already follows a valid path
 * (0), a grid where every move needs a change, and a grid with multiple paths
 * of the same cost.
 *
 * Why the deque works: a weight-0 edge goes to the front, so it is processed
 * before any weight-1 edge. The deque therefore always holds nodes in
 * non-decreasing distance order, which is the property Dijkstra needs. Using a
 * priority queue would also be correct but slower.
 */
