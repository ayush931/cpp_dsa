/**
 * Detect Cycle in a Grid
 *
 * Given an m x n grid of characters, determine whether there is a cycle made
 * of cells with the same value. A cycle is a path of length 4 or more that
 * starts and ends at the same cell. You may move up, down, left, or right to a
 * cell with the same value, but you may not immediately move back to the cell
 * you just came from.
 *
 * Example 1:
 * Input: [["a","a","a","a"],
 *         ["a","b","b","a"],
 *         ["a","b","b","a"],
 *         ["a","a","a","a"]]
 * Output: true
 *
 * Example 2:
 * Input: [["c","c","c","a"],
 *         ["c","d","c","c"],
 *         ["c","c","e","c"],
 *         ["f","c","c","c"]]
 * Output: true
 *
 * Example 3:
 * Input: [["a","b","b"],
 *         ["b","z","b"],
 *         ["b","b","a"]]
 * Output: false
 *
 * Constraints: 1 <= m, n <= 500, grid contains lowercase English letters.
 */

#include <iostream>
#include <utility>
#include <vector>
using namespace std;

vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool inside(vector<vector<char>> &grid, int row, int column) {
    return row >= 0 && column >= 0 && row < grid.size() &&
           column < grid[0].size();
}

// Brute force: run a fresh DFS from every cell, with its own visited matrix.
// O((m * n)^2) time, O(m * n) space.
bool dfsBrute(vector<vector<char>> &grid, int row, int column, int parentRow,
              int parentColumn, vector<vector<bool>> &visited) {
    visited[row][column] = true;

    for (pair<int, int> &direction : directions) {
        int nextRow = row + direction.first;
        int nextColumn = column + direction.second;

        if (!inside(grid, nextRow, nextColumn)) {
            continue;
        }
        if (grid[nextRow][nextColumn] != grid[row][column]) {
            continue;
        }

        if (!visited[nextRow][nextColumn]) {
            if (dfsBrute(grid, nextRow, nextColumn, row, column, visited)) {
                return true;
            }
        } else if (nextRow != parentRow || nextColumn != parentColumn) {
            // A visited neighbour that is not the cell we came from closes a
            // cycle.
            return true;
        }
    }

    return false;
}

bool containsCycleBrute(vector<vector<char>> &grid) {
    int rows = grid.size();
    int columns = grid[0].size();

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            vector<vector<bool>> visited(rows, vector<bool>(columns, false));

            if (dfsBrute(grid, row, column, -1, -1, visited)) {
                return true;
            }
        }
    }

    return false;
}

// Optimized: one global visited matrix. Every cell is visited once, and a back
// edge to a visited neighbour that is not the parent means a cycle.
// O(m * n) time, O(m * n) space.
bool dfsGlobal(vector<vector<char>> &grid, int row, int column, int parentRow,
               int parentColumn, vector<vector<bool>> &visited) {
    visited[row][column] = true;

    for (pair<int, int> &direction : directions) {
        int nextRow = row + direction.first;
        int nextColumn = column + direction.second;

        if (!inside(grid, nextRow, nextColumn)) {
            continue;
        }
        if (grid[nextRow][nextColumn] != grid[row][column]) {
            continue;
        }

        if (!visited[nextRow][nextColumn]) {
            if (dfsGlobal(grid, nextRow, nextColumn, row, column, visited)) {
                return true;
            }
        } else if (nextRow != parentRow || nextColumn != parentColumn) {
            return true;
        }
    }

    return false;
}

bool containsCycle(vector<vector<char>> &grid) {
    int rows = grid.size();
    int columns = grid[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(columns, false));

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (!visited[row][column]) {
                if (dfsGlobal(grid, row, column, -1, -1, visited)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int main() {
    vector<vector<char>> first = {{'a', 'a', 'a', 'a'},
                                  {'a', 'b', 'b', 'a'},
                                  {'a', 'b', 'b', 'a'},
                                  {'a', 'a', 'a', 'a'}};

    vector<vector<char>> second = {{'c', 'c', 'c', 'a'},
                                   {'c', 'd', 'c', 'c'},
                                   {'c', 'c', 'e', 'c'},
                                   {'f', 'c', 'c', 'c'}};

    vector<vector<char>> third = {
        {'a', 'b', 'b'}, {'b', 'z', 'b'}, {'b', 'b', 'a'}};

    vector<vector<char>> square = {{'a', 'a'}, {'a', 'a'}};

    vector<vector<char>> single = {{'a'}};

    vector<vector<char>> line = {{'a', 'a', 'a', 'a'}};

    vector<vector<char>> checkerboard = {
        {'a', 'b', 'a'}, {'b', 'a', 'b'}, {'a', 'b', 'a'}};

    cout << containsCycleBrute(first) << " " << containsCycle(first) << endl;
    // 1 1

    cout << containsCycle(second) << endl;        // 1
    cout << containsCycle(third) << endl;         // 0
    cout << containsCycle(square) << endl;        // 1
    cout << containsCycle(single) << endl;        // 0
    cout << containsCycle(line) << endl;          // 0
    cout << containsCycle(checkerboard) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O((m * n)^2), a fresh O(m * n) DFS from every cell
 *   global visited -> O(m * n), every cell is entered once
 * Space complexity -> O(m * n) for the visited matrix and O(m * n) recursion
 * in the worst case
 *
 * Special cases: one cell (no cycle), a single row or column (no cycle, because
 * a cycle needs at least four cells), a 2x2 block of equal values (cycle), a
 * checkerboard (no equal neighbours), and several disconnected components.
 *
 * A cycle in a grid graph always has length at least 4: grid edges only join
 * horizontal and vertical neighbours, so triangles are impossible. Skipping
 * the parent cell is what rejects the invalid two-cell path A -> B -> A.
 */
