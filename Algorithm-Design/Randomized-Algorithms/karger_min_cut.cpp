/**
 * Karger's randomized global minimum cut
 *
 * A cut partitions the vertices into two non-empty sets. The size of the cut
 * is the number of edges crossing it. Karger's algorithm repeatedly contracts
 * a random edge until two super-nodes remain; the edges between them form a
 * cut.
 *
 * One trial succeeds with probability at least 2 / (n * (n - 1)).
 * Repeating O(n^2 log n) trials gives a high success probability.
 *
 * Time per trial -> O(n^2) with an adjacency matrix
 * Total -> O(n^4 log n) in the simple version, fine for small n
 *
 * Edge cases: n <= 1 (no cut), disconnected graph (cut 0), parallel edges,
 * self-loops (ignored), and a graph with a unique minimum cut.
 */

#include <climits>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

class KargerMinCut {
    int n;
    vector<vector<int>> capacity;
    mt19937 generator;

    int contractOnce() {
        vector<vector<int>> graph = capacity;
        vector<bool> alive(n, true);
        int vertices = n;

        while (vertices > 2) {
            vector<pair<int, int>> edges;

            for (int u = 0; u < n; u++) {
                if (!alive[u]) {
                    continue;
                }
                for (int v = u + 1; v < n; v++) {
                    if (!alive[v]) {
                        continue;
                    }
                    if (graph[u][v] > 0) {
                        edges.push_back({u, v});
                    }
                }
            }

            if (edges.empty()) {
                return 0;
            }

            uniform_int_distribution<int> distribution(0,
                                                       (int)edges.size() - 1);
            auto [u, v] = edges[distribution(generator)];

            // Contract v into u.
            for (int w = 0; w < n; w++) {
                if (!alive[w] || w == u || w == v) {
                    continue;
                }
                graph[u][w] += graph[v][w];
                graph[w][u] += graph[w][v];
                graph[v][w] = 0;
                graph[w][v] = 0;
            }

            graph[u][v] = 0;
            graph[v][u] = 0;
            alive[v] = false;
            vertices--;
        }

        int first = -1;
        int second = -1;

        for (int i = 0; i < n; i++) {
            if (!alive[i]) {
                continue;
            }
            if (first == -1) {
                first = i;
            } else {
                second = i;
            }
        }

        return graph[first][second];
    }

   public:
    KargerMinCut(vector<vector<int>> &input)
        : capacity(input), generator(2024) {
        n = capacity.size();
    }

    int minimumCut(int trials = 100) {
        if (n <= 1) {
            return 0;
        }

        int best = INT_MAX;

        for (int trial = 0; trial < trials; trial++) {
            best = min(best, contractOnce());
        }

        return best;
    }
};

int main() {
    // Triangle with a tail:
    // 0 - 1, 1 - 2, 0 - 2, 2 - 3
    // The minimum cut is 1 (edge 2-3).
    vector<vector<int>> graph = {
        {0, 1, 1, 0}, {1, 0, 1, 0}, {1, 1, 0, 1}, {0, 0, 1, 0}};

    KargerMinCut solver(graph);
    cout << solver.minimumCut(200) << endl;  // 1

    // Two disconnected triangles -> minimum cut 0.
    vector<vector<int>> disconnected = {{0, 1, 1, 0, 0, 0}, {1, 0, 1, 0, 0, 0},
                                        {1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1},
                                        {0, 0, 0, 1, 0, 1}, {0, 0, 0, 1, 1, 0}};

    KargerMinCut second(disconnected);
    cout << second.minimumCut(200) << endl;  // 0

    // A single vertex has no cut.
    vector<vector<int>> single = {{0}};
    KargerMinCut third(single);
    cout << third.minimumCut(10) << endl;  // 0

    return 0;
}

// Special cases:
// - self-loops are never chosen because only u < v edges are considered
// - parallel edges are represented by capacities greater than 1
// - a disconnected graph has a cut of 0
// - a fixed seed makes the demo reproducible, but the result is probabilistic
