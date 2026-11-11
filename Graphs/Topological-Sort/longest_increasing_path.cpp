/**
 * Longest strictly increasing path in a rectangular integer matrix; move
 * up/down/left/right. Orient edges smaller->larger: strict increase creates
 * a DAG. Count Kahn BFS layers to obtain the longest path length.
 * O(rows*columns) time and space. No recursion stack or visited backtracking.
 * Alternative: memoized DFS with one longest-path value per cell.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace std;

int longestIncreasingPath(const vector<vector<int>> &matrix) {
    if (matrix.empty()) {
        return 0;
    }
    int rows = static_cast<int>(matrix.size()),
        columns = static_cast<int>(matrix[0].size());
    for (const auto &row : matrix) {
        if (static_cast<int>(row.size()) != columns) {
            throw invalid_argument("ragged matrix");
        }
    }
    if (!columns) {
        return 0;
    }
    const array<int, 5> direction{1, 0, -1, 0, 1};
    auto inside = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < columns;
    };
    vector<vector<int>> indegree(rows, vector<int>(columns));
    queue<pair<int, int>> ready;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            for (int d = 0; d < 4; ++d) {
                int nr = r + direction[d], nc = c + direction[d + 1];
                if (inside(nr, nc) && matrix[nr][nc] < matrix[r][c]) {
                    ++indegree[r][c];
                }
            }
            if (indegree[r][c] == 0) {
                ready.emplace(r, c);
            }
        }
    }
    int length = 0;
    while (!ready.empty()) {
        size_t layerSize = ready.size();
        ++length;
        while (layerSize--) {
            auto [r, c] = ready.front();
            ready.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + direction[d], nc = c + direction[d + 1];
                if (inside(nr, nc) && matrix[nr][nc] > matrix[r][c] &&
                    --indegree[nr][nc] == 0) {
                    ready.emplace(nr, nc);
                }
            }
        }
    }
    return length;
}

int main() {
    assert(longestIncreasingPath({{9, 9, 4}, {6, 6, 8}, {2, 1, 1}}) == 4);
    assert(longestIncreasingPath({{7, 7}, {7, 7}}) == 1);
    assert(longestIncreasingPath({}) == 0);
    assert(longestIncreasingPath({{}}) == 0);
    cout << longestIncreasingPath({{9, 9, 4}, {6, 6, 8}, {2, 1, 1}})
         << '\n';  // 4
}
