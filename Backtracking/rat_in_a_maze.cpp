/**
 * Rat in a Maze

 * A maze is a N * N binary matrix of blocks where the upper left block is known
 as the source block, and the lower rightmost back is known as the Destination
 block. If we consider the maze, the maze[0][0] is the source, and maze[N - 1][N
 - 1] is the destination. Our main task is to reach the destination from the
 source. We have considered a rat as a character that can move left, right, up
 and down.

 * In the maze matrix, a few dead blocks will be denoted by 0 and active blocks
 will be denoted by 1. A rat can move only in the active blocks.
 */

#include <bits/stdc++.h>
using namespace std;

bool canWeGo(int a, int b, vector<vector<int>> &grid) {
    return (a < grid.size() and b < grid.size() and a >= 0 and b >= 0) and
           grid[a][b] == 1;
};

int f(int i, int j, vector<vector<int>> &grid) {
    int n = grid.size();
    if (i == n - 1 and j == n - 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << "****\n";
        return 1;
    };

    int ans = 0;
    grid[i][j] = 2;

    if (canWeGo(i, j + 1, grid)) {
        ans += f(i, j + 1, grid);  // right
    }
    if (canWeGo(i + 1, j, grid)) {
        ans += f(i + 1, j, grid);  // down
    }
    if (canWeGo(i, j - 1, grid)) {
        ans += f(i, j - 1, grid);  // left
    }
    if (canWeGo(i - 1, j, grid)) {
        ans += f(i - 1, j, grid);  // up
    }

    grid[i][j] = 1;
    return ans;
};

int main() {
    vector<vector<int>> grid = {
        {1, 1, 1, 1}, {0, 1, 0, 1}, {0, 1, 1, 1}, {0, 1, 1, 1}};

    int res = f(0, 0, grid);
    cout << res << "\n";

    return 0;
}