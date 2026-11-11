/**
 * BFS on a state graph with keys and doors
 *
 * A cell can be visited in different key states, so the BFS state is
 * (row, column, keyMask). Two visits to the same cell with different keys are
 * different states and must both be explored.
 *
 * Grid symbols:
 *   S -> start
 *   E -> end
 *   . -> open
 *   # -> wall
 *   a-f -> keys
 *   A-F -> doors that need the matching key
 *
 * Time -> O(rows * cols * 2^k), where k is the number of key types
 * Space -> O(rows * cols * 2^k)
 *
 * Edge cases: no path, start == end, a door without its key, multiple keys of
 * the same type, and a key behind its own door.
 */

#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct State {
    int row;
    int col;
    int mask;
};

int shortestPathWithKeys(vector<string> &grid) {
    int rows = grid.size();
    if (rows == 0) {
        return -1;
    }

    int cols = grid[0].size();
    int startRow = -1;
    int startCol = -1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 'S') {
                startRow = r;
                startCol = c;
            }
        }
    }

    if (startRow == -1) {
        return -1;
    }

    const int KEY_COUNT = 6;
    int stateCount = 1 << KEY_COUNT;

    vector<vector<vector<int>>> distance(
        rows, vector<vector<int>>(cols, vector<int>(stateCount, -1)));

    queue<State> q;
    q.push({startRow, startCol, 0});
    distance[startRow][startCol][0] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        State current = q.front();
        q.pop();

        int r = current.row;
        int c = current.col;
        int mask = current.mask;

        if (grid[r][c] == 'E') {
            return distance[r][c][mask];
        }

        for (int direction = 0; direction < 4; direction++) {
            int nr = r + dr[direction];
            int nc = c + dc[direction];

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                continue;
            }
            if (grid[nr][nc] == '#') {
                continue;
            }

            int nextMask = mask;
            char cell = grid[nr][nc];

            if (cell >= 'a' && cell <= 'f') {
                nextMask |= 1 << (cell - 'a');
            } else if (cell >= 'A' && cell <= 'F' && cell != 'E') {
                // 'E' is the end cell, so it is not treated as a door.
                int key = cell - 'A';
                if ((mask & (1 << key)) == 0) {
                    continue;
                }
            }

            if (distance[nr][nc][nextMask] != -1) {
                continue;
            }

            distance[nr][nc][nextMask] = distance[r][c][mask] + 1;
            q.push({nr, nc, nextMask});
        }
    }

    return -1;
}

int main() {
    vector<string> grid = {"S..#.", "##.#.", "..a..", ".#A..", "....E"};

    cout << shortestPathWithKeys(grid) << endl;  // 8

    vector<string> noKey = {"S.#.", "..##", "..AE", "...#"};
    cout << shortestPathWithKeys(noKey) << endl;  // -1

    vector<string> immediate = {"SE"};
    cout << shortestPathWithKeys(immediate) << endl;  // 1

    vector<string> noStart = {"...", "..."};
    cout << shortestPathWithKeys(noStart) << endl;  // -1

    return 0;
}

// Special cases:
// - start == end -> 0 (the start cell is checked before expanding)
// - a door with no matching key is never entered
// - multiple keys of the same type set the same bit
// - a key behind its own door makes the level unsolvable
// - the mask has 6 bits because the grid uses keys a-f
