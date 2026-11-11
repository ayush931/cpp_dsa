/**
 * Is Graph Bipartite?
 *
 * There is an undirected graph with n nodes, given as an adjacency list. A
 * graph is bipartite if its nodes can be split into two sets such that every
 * edge connects a node in one set to a node in the other.
 *
 * Example 1:
 * Input: [[1,2,3],[0,2],[0,1,3],[0,2]]
 * Output: false
 *
 * Example 2:
 * Input: [[1,3],[0,2],[1,3],[0,2]]
 * Output: true
 *
 * Constraints: 1 <= n <= 100, no self-edges, no parallel edges.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Brute force: try every possible 2-coloring with a bitmask.
// O(2^n * E) time, O(1) extra space. Only practical for small n.
bool isBipartiteBrute(vector<vector<int>> &graph) {
    int n = graph.size();

    for (int mask = 0; mask < (1 << n); mask++) {
        bool valid = true;

        for (int node = 0; node < n && valid; node++) {
            for (int neighbour : graph[node]) {
                bool nodeColor = (mask >> node) & 1;
                bool neighbourColor = (mask >> neighbour) & 1;

                if (nodeColor == neighbourColor) {
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
}

// Optimized: BFS coloring. Every edge must connect opposite colors. If a
// neighbour already has the same color, the graph is not bipartite.
// O(V + E) time, O(V) space.
bool bfsColoring(vector<vector<int>> &graph, int source, vector<int> &color) {
    queue<int> pending;
    pending.push(source);
    color[source] = 0;

    while (!pending.empty()) {
        int node = pending.front();
        pending.pop();

        for (int neighbour : graph[node]) {
            if (color[neighbour] == -1) {
                color[neighbour] = 1 - color[node];
                pending.push(neighbour);
            } else if (color[neighbour] == color[node]) {
                return false;
            }
        }
    }

    return true;
}

bool isBipartite(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> color(n, -1);  // -1 = uncolored, 0 and 1 are the two sets

    for (int node = 0; node < n; node++) {
        if (color[node] == -1) {
            if (!bfsColoring(graph, node, color)) {
                return false;
            }
        }
    }

    return true;
}

void check(vector<vector<int>> &graph) {
    cout << isBipartiteBrute(graph) << " " << isBipartite(graph) << endl;
}

int main() {
    vector<vector<int>> oddCycle = {{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}};
    vector<vector<int>> evenCycle = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};

    vector<vector<int>> single = {{}};
    vector<vector<int>> edge = {{1}, {0}};
    vector<vector<int>> triangle = {{1, 2}, {0, 2}, {0, 1}};
    vector<vector<int>> disconnected = {{1}, {0}, {3}, {2}};
    vector<vector<int>> path = {{1}, {0, 2}, {1, 3}, {2}};

    check(oddCycle);      // 0 0
    check(evenCycle);     // 1 1
    check(single);        // 1 1
    check(edge);          // 1 1
    check(triangle);      // 0 0
    check(disconnected);  // 1 1
    check(path);          // 1 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^n * E), every coloring and every edge
 *   BFS -> O(V + E), each node and edge is visited once
 * Space complexity:
 *   brute -> O(1) extra, apart from the graph
 *   BFS -> O(V) for the color array and the queue
 *
 * Special cases: a single node (bipartite), an isolated node (bipartite), a
 * triangle or any odd cycle (not bipartite), an even cycle (bipartite), and a
 * disconnected graph (every component must be bipartite).
 *
 * A graph is bipartite exactly when it has no odd-length cycle. The BFS
 * coloring detects that because an edge between two nodes of the same color
 * closes an odd cycle. A DFS with the same color rule is an equally good
 * O(V + E) alternative.
 */
