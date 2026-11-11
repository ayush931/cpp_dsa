/**
 * Johnson's All-Pairs Shortest Paths Algorithm
 *
 * Johnson's algorithm handles sparse graphs with negative edge weights (but no
 * negative cycles) in O(V * E + V * E log V) time:
 *
 *   1. Add a virtual source connected to every vertex with weight 0.
 *   2. Run Bellman-Ford from that source to compute potentials h[v].
 *   3. Reweight every edge (u, v) to w(u, v) + h[u] - h[v] >= 0.
 *   4. Run Dijkstra from every vertex on the reweighted graph.
 *   5. Convert distances back with d(u, v) = d'(u, v) - h[u] + h[v].
 *
 * If Bellman-Ford can still relax an edge, the graph contains a negative cycle.
 *
 * Floyd-Warshall is included as the O(V^3) reference implementation.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

struct Edge {
    int from;
    int to;
    long long weight;
};

struct JohnsonResult {
    bool hasNegativeCycle;
    vector<vector<long long>> distance;
};

JohnsonResult johnson(int vertices, const vector<Edge> &edges) {
    vector<long long> potential(vertices, 0);

    // Bellman-Ford from a virtual source with a zero-weight edge to every node.
    for (int iteration = 0; iteration < vertices; iteration++) {
        bool changed = false;

        for (const Edge &edge : edges) {
            if (potential[edge.from] == INF) {
                continue;
            }

            if (potential[edge.to] > potential[edge.from] + edge.weight) {
                potential[edge.to] = potential[edge.from] + edge.weight;
                changed = true;
            }
        }

        if (!changed) {
            break;
        }
    }

    for (const Edge &edge : edges) {
        if (potential[edge.from] != INF &&
            potential[edge.to] > potential[edge.from] + edge.weight) {
            return {true, {}};
        }
    }

    vector<vector<pair<int, long long>>> graph(vertices);

    for (const Edge &edge : edges) {
        if (potential[edge.from] == INF || potential[edge.to] == INF) {
            continue;
        }

        long long reweighted =
            edge.weight + potential[edge.from] - potential[edge.to];
        graph[edge.from].push_back({edge.to, reweighted});
    }

    vector<vector<long long>> distance(vertices,
                                       vector<long long>(vertices, INF));

    for (int source = 0; source < vertices; source++) {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                       greater<pair<long long, int>>>
            minHeap;

        distance[source][source] = 0;
        minHeap.push({0, source});

        while (!minHeap.empty()) {
            long long currentDistance = minHeap.top().first;
            int node = minHeap.top().second;
            minHeap.pop();

            if (currentDistance != distance[source][node]) {
                continue;
            }

            for (const pair<int, long long> &next : graph[node]) {
                int neighbor = next.first;
                long long candidate = currentDistance + next.second;

                if (candidate < distance[source][neighbor]) {
                    distance[source][neighbor] = candidate;
                    minHeap.push({candidate, neighbor});
                }
            }
        }

        for (int target = 0; target < vertices; target++) {
            if (distance[source][target] == INF) {
                continue;
            }

            distance[source][target] = distance[source][target] -
                                       potential[source] + potential[target];
        }
    }

    return {false, distance};
}

vector<vector<long long>> floydWarshall(int vertices,
                                        const vector<Edge> &edges) {
    vector<vector<long long>> distance(vertices,
                                       vector<long long>(vertices, INF));

    for (int i = 0; i < vertices; i++) {
        distance[i][i] = 0;
    }

    for (const Edge &edge : edges) {
        distance[edge.from][edge.to] =
            min(distance[edge.from][edge.to], edge.weight);
    }

    for (int middle = 0; middle < vertices; middle++) {
        for (int from = 0; from < vertices; from++) {
            if (distance[from][middle] == INF) {
                continue;
            }

            for (int to = 0; to < vertices; to++) {
                if (distance[middle][to] == INF) {
                    continue;
                }

                distance[from][to] =
                    min(distance[from][to],
                        distance[from][middle] + distance[middle][to]);
            }
        }
    }

    return distance;
}

void printDistances(const vector<vector<long long>> &distance) {
    for (int i = 0; i < static_cast<int>(distance.size()); i++) {
        for (int j = 0; j < static_cast<int>(distance.size()); j++) {
            if (distance[i][j] == INF) {
                cout << "INF";
            } else {
                cout << distance[i][j];
            }

            if (j + 1 < static_cast<int>(distance.size())) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

bool sameDistances(const vector<vector<long long>> &a,
                   const vector<vector<long long>> &b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        for (int j = 0; j < static_cast<int>(a.size()); j++) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    int vertices = 5;
    vector<Edge> edges = {{0, 1, 3}, {0, 2, 8},  {1, 2, -4}, {1, 3, 1},
                          {2, 3, 7}, {3, 4, -2}, {4, 0, 2}};

    JohnsonResult result = johnson(vertices, edges);

    cout << "negative cycle: " << (result.hasNegativeCycle ? "yes" : "no")
         << endl;
    cout << "Johnson distances:" << endl;
    printDistances(result.distance);

    vector<vector<long long>> reference = floydWarshall(vertices, edges);
    cout << "matches Floyd-Warshall: "
         << (sameDistances(result.distance, reference) ? "yes" : "no") << endl;

    vector<Edge> negativeCycle = {{0, 1, -1}, {1, 2, -1}, {2, 0, -1}};
    cout << "negative cycle detection: "
         << (johnson(3, negativeCycle).hasNegativeCycle ? "yes" : "no") << endl;

    vector<Edge> disconnected = {{0, 1, 4}, {2, 3, 5}};
    JohnsonResult disconnectedResult = johnson(4, disconnected);
    cout << "disconnected graph distances:" << endl;
    printDistances(disconnectedResult.distance);
    cout << "matches Floyd-Warshall: "
         << (sameDistances(disconnectedResult.distance,
                           floydWarshall(4, disconnected))
                 ? "yes"
                 : "no")
         << endl;

    // Random DAGs with negative edges but no negative cycles.
    mt19937 generator(20261111);
    uniform_int_distribution<int> weightDistribution(-8, 12);

    bool randomTestsPassed = true;

    for (int test = 0; test < 100; test++) {
        int n = 7;
        vector<Edge> randomEdges;

        for (int from = 0; from < n; from++) {
            for (int to = from + 1; to < n; to++) {
                if (generator() % 3 == 0) {
                    randomEdges.push_back(
                        {from, to, weightDistribution(generator)});
                }
            }
        }

        JohnsonResult randomResult = johnson(n, randomEdges);
        vector<vector<long long>> randomReference =
            floydWarshall(n, randomEdges);

        if (randomResult.hasNegativeCycle ||
            !sameDistances(randomResult.distance, randomReference)) {
            randomTestsPassed = false;
            break;
        }
    }

    cout << "100 random DAG comparisons: "
         << (randomTestsPassed ? "passed" : "failed") << endl;

    return 0;
}

// Expected output (abridged):
// negative cycle: no
// Johnson distances:
// 0 3 -1 4 2
// 1 0 -4 1 -1
// 7 10 0 7 5
// 0 3 -1 0 -2
// 2 5 1 6 0
// matches Floyd-Warshall: yes
// negative cycle detection: yes
// disconnected graph distances:
// 0 4 INF INF
// INF 0 INF INF
// INF INF 0 5
// INF INF INF 0
// matches Floyd-Warshall: yes
// 100 random DAG comparisons: passed
//
// Special cases: negative edges without a negative cycle, negative cycle
// detection, disconnected graphs with unreachable pairs, a single vertex, and
// sparse graphs where Johnson's algorithm is preferable to Floyd-Warshall.
