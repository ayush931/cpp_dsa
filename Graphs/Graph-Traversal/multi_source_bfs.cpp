/**
 * Multi-source BFS
 *
 * Start BFS from many sources at once by pushing all sources into the queue
 * with distance 0. The first time a cell is reached is the shortest distance
 * to any source.
 *
 * Used for:
 *   - nearest exit / nearest zero
 *   - rotting oranges
 *   - fire spread
 *   - distance to the nearest facility
 *
 * Time -> O(rows * cols), Space -> O(rows * cols)
 *
 * Edge cases: no sources, all cells blocked, source on the target, empty grid,
 * and unreachable cells (distance stays -1).
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<vector<int>> nearestSourceDistance(vector<vector<int>> &grid) {
    int rows = grid.size();
    if (rows == 0) {
        return {};
    }

    int cols = grid[0].size();
    vector<vector<int>> distance(rows, vector<int>(cols, -1));
    queue<pair<int, int>> q;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 1) {
                distance[r][c] = 0;
                q.push({r, c});
            }
        }
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int direction = 0; direction < 4; direction++) {
            int nr = r + dr[direction];
            int nc = c + dc[direction];

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                continue;
            }
            if (grid[nr][nc] == -1) {
                continue;
            }
            if (distance[nr][nc] != -1) {
                continue;
            }

            distance[nr][nc] = distance[r][c] + 1;
            q.push({nr, nc});
        }
    }

    return distance;
}

int rottingOranges(vector<vector<int>> &grid) {
    int rows = grid.size();
    if (rows == 0) {
        return 0;
    }

    int cols = grid[0].size();
    queue<pair<int, int>> q;
    int fresh = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 2) {
                q.push({r, c});
            } else if (grid[r][c] == 1) {
                fresh++;
            }
        }
    }

    if (fresh == 0) {
        return 0;
    }

    int minutes = 0;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty() && fresh > 0) {
        int size = q.size();
        minutes++;

        for (int i = 0; i < size; i++) {
            auto [r, c] = q.front();
            q.pop();

            for (int direction = 0; direction < 4; direction++) {
                int nr = r + dr[direction];
                int nc = c + dc[direction];

                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                    continue;
                }
                if (grid[nr][nc] != 1) {
                    continue;
                }

                grid[nr][nc] = 2;
                fresh--;
                q.push({nr, nc});
            }
        }
    }

    return fresh == 0 ? minutes : -1;
}

int main() {
    // 1 = source, 0 = open, -1 = blocked
    vector<vector<int>> grid = {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}};

    vector<vector<int>> distance = nearestSourceDistance(grid);
    for (auto &row : distance) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
    // 0 1 2
    // 1 -1 1
    // 2 1 0

    vector<vector<int>> oranges = {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}};
    cout << rottingOranges(oranges) << endl;  // 4

    vector<vector<int>> noSource = {{0, 0}, {0, 0}};
    vector<vector<int>> unreachable = nearestSourceDistance(noSource);
    cout << unreachable[0][0] << endl;  // -1

    vector<vector<int>> empty;
    cout << rottingOranges(empty) << endl;  // 0

    return 0;
}

// Special cases:
// - no sources -> every distance is -1
// - blocked cells stay -1 and are never entered
// - a source cell has distance 0
// - rotten oranges with no fresh oranges -> 0 minutes
// - fresh oranges that cannot be reached -> -1
