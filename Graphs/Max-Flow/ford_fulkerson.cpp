/**
 * Maximum flow -> maximum amount of flow that can be sent from source to sink
 * Residual graph -> capacity left after sending flow, plus reverse edges to
 * undo flow
 *
 * Ford-Fulkerson -> repeatedly find an augmenting path with DFS
 * Time -> O(E * maxFlow), which can be slow for large capacities
 *
 * Edmonds-Karp -> uses BFS to find the shortest augmenting path
 * Time -> O(V * E^2)
 */

#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

bool dfs(vector<vector<int>> &capacity, int node, int sink, vector<int> &parent,
         vector<bool> &visited) {
    if (node == sink) {
        return true;
    }

    visited[node] = true;

    for (int next = 0; next < capacity.size(); next++) {
        if (!visited[next] && capacity[node][next] > 0) {
            parent[next] = node;

            if (dfs(capacity, next, sink, parent, visited)) {
                return true;
            }
        }
    }

    return false;
};

int fordFulkerson(vector<vector<int>> capacity, int source, int sink) {
    if (source == sink) {
        return 0;  // no path with a positive number of edges
    }

    int n = capacity.size();
    vector<int> parent(n, -1);
    int maxFlow = 0;

    while (true) {
        vector<bool> visited(n, false);
        fill(parent.begin(), parent.end(), -1);

        if (!dfs(capacity, source, sink, parent, visited)) {
            break;
        }

        int pathFlow = INT_MAX;

        for (int node = sink; node != source; node = parent[node]) {
            pathFlow = min(pathFlow, capacity[parent[node]][node]);
        }

        for (int node = sink; node != source; node = parent[node]) {
            capacity[parent[node]][node] -= pathFlow;
            capacity[node][parent[node]] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
};

bool bfs(vector<vector<int>> &capacity, int source, int sink,
         vector<int> &parent) {
    int n = capacity.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int next = 0; next < n; next++) {
            if (!visited[next] && capacity[node][next] > 0) {
                visited[next] = true;
                parent[next] = node;
                q.push(next);
            }
        }
    }

    return visited[sink];
};

int edmondsKarp(vector<vector<int>> capacity, int source, int sink) {
    if (source == sink) {
        return 0;
    }

    int n = capacity.size();
    vector<int> parent(n);
    int maxFlow = 0;

    while (bfs(capacity, source, sink, parent)) {
        int pathFlow = INT_MAX;

        for (int node = sink; node != source; node = parent[node]) {
            pathFlow = min(pathFlow, capacity[parent[node]][node]);
        }

        for (int node = sink; node != source; node = parent[node]) {
            capacity[parent[node]][node] -= pathFlow;
            capacity[node][parent[node]] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
};

int main() {
    int n = 6;
    vector<vector<int>> capacity(n, vector<int>(n, 0));

    capacity[0][1] = 16;
    capacity[0][2] = 13;
    capacity[1][2] = 10;
    capacity[2][1] = 4;
    capacity[1][3] = 12;
    capacity[3][2] = 9;
    capacity[2][4] = 14;
    capacity[4][3] = 7;
    capacity[3][5] = 20;
    capacity[4][5] = 4;

    cout << fordFulkerson(capacity, 0, 5) << endl;  // 23
    cout << edmondsKarp(capacity, 0, 5) << endl;    // 23
    cout << fordFulkerson(capacity, 0, 0) << endl;  // 0 (source == sink)
    cout << edmondsKarp(capacity, 0, 0) << endl;    // 0

    return 0;
}

// Special cases: source == sink, disconnected source and sink, zero capacity
// edges, undirected edges (add capacity in both directions), large capacities
// (use long long)
