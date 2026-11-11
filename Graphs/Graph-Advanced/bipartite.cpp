/**
 * Bipartite graph -> vertices can be divided into two sets such that every
 * edge connects a vertex from one set to the other
 * A graph is bipartite if and only if it has no odd length cycle
 *
 * Brute force -> try every 2-coloring, O(2^V * E)
 * Optimized -> BFS/DFS coloring, O(V + E)
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

bool isBipartiteBrute(vector<vector<int>> &graph) {
    int n = graph.size();

    for (int mask = 0; mask < (1 << n); mask++) {
        bool valid = true;

        for (int u = 0; u < n && valid; u++) {
            for (int v : graph[u]) {
                if (((mask >> u) & 1) == ((mask >> v) & 1)) {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            return true;
        }
    }

    return false;
};

bool isBipartite(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> color(n, -1);

    for (int start = 0; start < n; start++) {
        if (color[start] != -1) {
            continue;
        }

        queue<int> q;
        q.push(start);
        color[start] = 0;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbour : graph[node]) {
                if (color[neighbour] == -1) {
                    color[neighbour] = 1 - color[node];
                    q.push(neighbour);
                } else if (color[neighbour] == color[node]) {
                    return false;
                }
            }
        }
    }

    return true;
};

int main() {
    vector<vector<int>> bipartite = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
    vector<vector<int>> oddCycle = {{1, 2}, {0, 2}, {0, 1}};

    cout << isBipartiteBrute(bipartite) << endl;  // 1
    cout << isBipartite(bipartite) << endl;       // 1
    cout << isBipartite(oddCycle) << endl;        // 0

    return 0;
}

// Special cases: disconnected graph, self loop (never bipartite),
// parallel edges, empty graph (bipartite)
