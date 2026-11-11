/**
 * Network Delay Time
 * Given n nodes, directed edges (u, v, w) and a source k, return the time
 * for all nodes to receive a signal. Return -1 if not all nodes are reachable.
 *
 * Input: times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
 * Output: 2
 *
 * Brute force -> Bellman-Ford, O(V * E)
 * Optimized -> Dijkstra with a priority queue, O((V + E) log V)
 */

#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int networkDelayBellman(int n, vector<vector<int>> &times, int k) {
    vector<int> distance(n + 1, INT_MAX);
    distance[k] = 0;

    for (int i = 0; i < n - 1; i++) {
        bool updated = false;

        for (auto &edge : times) {
            int u = edge[0], v = edge[1], w = edge[2];

            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                updated = true;
            }
        }

        if (!updated) {
            break;
        }
    }

    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (distance[i] == INT_MAX) {
            return -1;
        }
        answer = max(answer, distance[i]);
    }

    return answer;
};

int networkDelayDijkstra(int n, vector<vector<int>> &times, int k) {
    vector<vector<pair<int, int>>> graph(n + 1);

    for (auto &edge : times) {
        graph[edge[0]].push_back({edge[1], edge[2]});
    }

    vector<int> distance(n + 1, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    distance[k] = 0;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [currentDistance, node] = pq.top();
        pq.pop();

        if (currentDistance > distance[node]) {
            continue;
        }

        for (auto [neighbour, weight] : graph[node]) {
            if (distance[node] + weight < distance[neighbour]) {
                distance[neighbour] = distance[node] + weight;
                pq.push({distance[neighbour], neighbour});
            }
        }
    }

    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (distance[i] == INT_MAX) {
            return -1;
        }
        answer = max(answer, distance[i]);
    }

    return answer;
};

int main() {
    vector<vector<int>> times = {{2, 1, 1}, {2, 3, 1}, {3, 4, 1}};

    cout << networkDelayBellman(4, times, 2) << endl;   // 2
    cout << networkDelayDijkstra(4, times, 2) << endl;  // 2

    return 0;
}

// Special cases: source isolated, disconnected graph, n = 1, duplicate edges
