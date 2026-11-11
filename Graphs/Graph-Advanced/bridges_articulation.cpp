/**
 * Bridges and articulation points
 * Bridge -> an edge whose removal increases the number of connected components
 * Articulation point -> a vertex whose removal increases the number of
 * connected components
 *
 * Brute force -> remove every edge/vertex and count components, O(E * (V + E))
 * Optimized -> Tarjan DFS with discovery time and low value, O(V + E)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Edges are identified by index. Skipping the parent *edge* instead of the
// parent *vertex* is what makes parallel edges work: the second copy is a
// back edge, so neither copy is a bridge.
void dfsBridges(vector<vector<pair<int, int>>> &graph, int node, int parentEdge,
                int &timer, vector<int> &discovery, vector<int> &low,
                vector<pair<int, int>> &bridges, vector<bool> &isArticulation) {
    discovery[node] = low[node] = timer++;
    int children = 0;

    for (auto &[neighbour, edgeId] : graph[node]) {
        if (edgeId == parentEdge) {
            continue;
        }

        if (discovery[neighbour] == -1) {
            children++;
            dfsBridges(graph, neighbour, edgeId, timer, discovery, low, bridges,
                       isArticulation);

            low[node] = min(low[node], low[neighbour]);

            if (low[neighbour] > discovery[node]) {
                bridges.push_back({node, neighbour});
            }

            if (parentEdge != -1 && low[neighbour] >= discovery[node]) {
                isArticulation[node] = true;
            }
        } else {
            low[node] = min(low[node], discovery[neighbour]);
        }
    }

    if (parentEdge == -1 && children > 1) {
        isArticulation[node] = true;
    }
};

int main() {
    int n = 5;
    vector<pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 2}, {2, 3}, {3, 4}};
    vector<vector<pair<int, int>>> graph(n);

    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        graph[u].push_back({v, i});
        graph[v].push_back({u, i});
    }

    vector<int> discovery(n, -1);
    vector<int> low(n, -1);
    vector<pair<int, int>> bridges;
    vector<bool> isArticulation(n, false);
    int timer = 0;

    for (int i = 0; i < n; i++) {
        if (discovery[i] == -1) {
            dfsBridges(graph, i, -1, timer, discovery, low, bridges,
                       isArticulation);
        }
    }

    for (auto &bridge : bridges) {
        cout << bridge.first << " - " << bridge.second << endl;
    }

    for (int i = 0; i < n; i++) {
        if (isArticulation[i]) {
            cout << "Articulation: " << i << endl;
        }
    }

    // Two parallel edges between 0 and 1 are not a bridge. The only bridge in
    // this graph is 1 - 2.
    vector<vector<pair<int, int>>> parallel(3);
    parallel[0].push_back({1, 0});
    parallel[1].push_back({0, 0});
    parallel[0].push_back({1, 1});
    parallel[1].push_back({0, 1});
    parallel[1].push_back({2, 2});
    parallel[2].push_back({1, 2});

    discovery.assign(3, -1);
    low.assign(3, -1);
    bridges.clear();
    isArticulation.assign(3, false);
    timer = 0;

    dfsBridges(parallel, 0, -1, timer, discovery, low, bridges, isArticulation);

    cout << "Parallel graph bridges:";
    for (auto &bridge : bridges) {
        cout << " " << bridge.first << "-" << bridge.second;
    }
    cout << endl;  // Parallel graph bridges: 1-2

    return 0;
}

// Special cases: disconnected graph, single vertex, tree (every edge is a
// bridge), parallel edges, self loops
