/**
 * Stoer-Wagner Global Minimum Cut
 *
 * The global minimum cut of an undirected weighted graph is the cheapest set
 * of edges whose removal disconnects the graph. Stoer-Wagner finds it without
 * fixing a source or sink.
 *
 * Each phase grows a set A from an arbitrary start vertex. At every step the
 * vertex with the largest total connection weight to A is added. The last two
 * vertices added are s and t. The cut of the phase is the weight from t to A,
 * and then s and t are merged. Repeating this V - 1 times gives the global
 * minimum cut in O(V^3) time.
 *
 * A brute-force subset enumeration is included for small graphs.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

long long stoerWagner(int vertices, vector<vector<long long>> graph) {
    if (vertices <= 1) {
        return 0;
    }

    long long bestCut = INF;
    int n = vertices;

    while (n > 1) {
        vector<long long> connectionWeight(n, 0);
        vector<bool> added(n, false);
        vector<int> previous(n, -1);
        int last = -1;

        for (int step = 0; step < n; step++) {
            int selected = -1;

            for (int vertex = 0; vertex < n; vertex++) {
                if (added[vertex]) {
                    continue;
                }
                if (selected == -1 ||
                    connectionWeight[vertex] > connectionWeight[selected]) {
                    selected = vertex;
                }
            }

            if (step == n - 1) {
                bestCut = min(bestCut, connectionWeight[selected]);

                int mergeInto = last;

                for (int vertex = 0; vertex < n; vertex++) {
                    graph[mergeInto][vertex] += graph[selected][vertex];
                    graph[vertex][mergeInto] += graph[vertex][selected];
                }

                // Remove the selected vertex by swapping it with the last
                // index.
                for (int vertex = 0; vertex < n; vertex++) {
                    swap(graph[selected][vertex], graph[n - 1][vertex]);
                    swap(graph[vertex][selected], graph[vertex][n - 1]);
                }

                n--;
                break;
            }

            added[selected] = true;
            previous[selected] = last;
            last = selected;

            for (int vertex = 0; vertex < n; vertex++) {
                if (!added[vertex]) {
                    connectionWeight[vertex] += graph[selected][vertex];
                }
            }
        }
    }

    return bestCut;
}

long long bruteForceMinCut(int vertices,
                           const vector<vector<long long>> &graph) {
    if (vertices <= 1) {
        return 0;
    }

    long long best = INF;

    for (int mask = 1; mask < (1 << vertices) - 1; mask++) {
        long long cut = 0;

        for (int from = 0; from < vertices; from++) {
            if (!(mask & (1 << from))) {
                continue;
            }

            for (int to = 0; to < vertices; to++) {
                if (mask & (1 << to)) {
                    continue;
                }
                cut += graph[from][to];
            }
        }

        best = min(best, cut);
    }

    return best;
}

void runTest(int vertices, const vector<vector<long long>> &graph) {
    long long actual = stoerWagner(vertices, graph);
    long long expected = bruteForceMinCut(vertices, graph);

    cout << "Stoer-Wagner: " << actual << ", brute force: " << expected
         << ", matches: " << (actual == expected ? "yes" : "no") << endl;
}

int main() {
    vector<vector<long long>> graph = {{0, 2, 3, 0, 0},
                                       {2, 0, 4, 1, 0},
                                       {3, 4, 0, 5, 6},
                                       {0, 1, 5, 0, 7},
                                       {0, 0, 6, 7, 0}};

    runTest(5, graph);

    vector<vector<long long>> disconnected = {
        {0, 5, 0, 0}, {5, 0, 0, 0}, {0, 0, 0, 3}, {0, 0, 3, 0}};
    runTest(4, disconnected);

    vector<vector<long long>> triangle = {{0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
    runTest(3, triangle);

    runTest(1, {{0}});
    runTest(0, {});

    vector<vector<long long>> two = {{0, 9}, {9, 0}};
    runTest(2, two);

    return 0;
}

// Expected output (abridged):
// Stoer-Wagner: 5, brute force: 5, matches: yes
// Stoer-Wagner: 0, brute force: 0, matches: yes
// Stoer-Wagner: 2, brute force: 2, matches: yes
// Stoer-Wagner: 0, brute force: 0, matches: yes
// Stoer-Wagner: 0, brute force: 0, matches: yes
// Stoer-Wagner: 9, brute force: 9, matches: yes
//
// Special cases: disconnected graph (cut 0), single vertex, two vertices,
// triangle with unit weights, and an empty graph.
