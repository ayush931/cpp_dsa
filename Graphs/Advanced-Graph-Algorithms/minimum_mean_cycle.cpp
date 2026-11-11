/**
 * Minimum mean cycle (Karp's algorithm)
 *
 * The mean of a cycle is its total weight divided by its number of edges.
 * Karp's algorithm finds the cycle with the smallest mean in a directed graph,
 * including graphs with negative edge weights.
 *
 * Let F[k][v] be the minimum weight of a walk of exactly k edges that ends at
 * v. Initialize F[0][v] = 0 for every v (a virtual start can reach every node).
 * Then:
 *
 *   F[k][v] = min over edges (u, v) of F[k - 1][u] + weight(u, v)
 *
 * The minimum cycle mean is:
 *
 *   min over v of max over 0 <= k < n of (F[n][v] - F[k][v]) / (n - k)
 *
 * Time -> O(V * E), Space -> O(V^2)
 *
 * Edge cases: no cycle (returns infinity), self-loop, negative cycle,
 * disconnected graph, and multiple cycles with the same mean.
 */

#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int from;
    int to;
    int weight;
};

double minimumMeanCycle(int n, vector<Edge> &edges) {
    const long long INF = (long long)1e18;
    vector<vector<long long>> dp(n + 1, vector<long long>(n, INF));

    for (int node = 0; node < n; node++) {
        dp[0][node] = 0;
    }

    for (int length = 1; length <= n; length++) {
        for (Edge &edge : edges) {
            if (dp[length - 1][edge.from] == INF) {
                continue;
            }

            long long candidate = dp[length - 1][edge.from] + edge.weight;
            if (candidate < dp[length][edge.to]) {
                dp[length][edge.to] = candidate;
            }
        }
    }

    double answer = 1e100;

    for (int node = 0; node < n; node++) {
        if (dp[n][node] == INF) {
            continue;
        }

        double worst = -1e100;

        for (int length = 0; length < n; length++) {
            if (dp[length][node] == INF) {
                continue;
            }

            double mean =
                (double)(dp[n][node] - dp[length][node]) / (n - length);
            worst = max(worst, mean);
        }

        answer = min(answer, worst);
    }

    return answer;
}

int main() {
    // 0 -> 1 (1), 1 -> 2 (1), 2 -> 0 (1): mean 1
    // 2 -> 2 (-3): mean -3 (the minimum)
    vector<Edge> edges = {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}, {2, 2, -3}};

    cout << minimumMeanCycle(3, edges) << endl;  // -3

    // A graph with no cycle.
    vector<Edge> acyclic = {{0, 1, 5}, {1, 2, 7}};
    double none = minimumMeanCycle(3, acyclic);
    cout << (none > 1e90 ? "infinity" : to_string(none)) << endl;  // infinity

    // Two cycles: 0-1 mean 2, 1-2 mean 1.
    vector<Edge> twoCycles = {{0, 1, 4}, {1, 0, 0}, {1, 2, 2}, {2, 1, 0}};
    cout << minimumMeanCycle(3, twoCycles) << endl;  // 1

    return 0;
}

// Special cases:
// - no cycle -> the answer stays at the infinity sentinel
// - a self-loop is a cycle of length 1
// - negative cycles produce a negative mean
// - F[0][v] = 0 for all v lets the walk start anywhere
