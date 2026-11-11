/**
 * Shortest path in a directed acyclic graph (DAG)
 *
 * A DAG has a topological order. Relax edges in that order and every shortest
 * path is found in one pass, even with negative edge weights.
 *
 * Algorithm:
 *   1. Compute the topological order with Kahn's algorithm.
 *   2. Initialize distance[source] = 0.
 *   3. For every node in topological order, relax all outgoing edges.
 *
 * Time -> O(V + E), Space -> O(V)
 * If the graph has a cycle, no topological order exists and the function
 * returns an empty result.
 *
 * Edge cases: source with no outgoing edges, disconnected nodes, negative
 * weights, parallel edges, self-loops (which create a cycle), and a cycle.
 */

#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int weight;
};

vector<int> dagShortestPath(int n, vector<vector<Edge>> &graph, int source) {
    vector<int> indegree(n, 0);

    for (int u = 0; u < n; u++) {
        for (Edge &edge : graph[u]) {
            indegree[edge.to]++;
        }
    }

    queue<int> q;
    for (int node = 0; node < n; node++) {
        if (indegree[node] == 0) {
            q.push(node);
        }
    }

    vector<int> order;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);

        for (Edge &edge : graph[node]) {
            indegree[edge.to]--;
            if (indegree[edge.to] == 0) {
                q.push(edge.to);
            }
        }
    }

    if ((int)order.size() != n) {
        return {};  // cycle
    }

    vector<int> distance(n, INT_MAX);
    distance[source] = 0;

    for (int node : order) {
        if (distance[node] == INT_MAX) {
            continue;
        }

        for (Edge &edge : graph[node]) {
            if (distance[node] + edge.weight < distance[edge.to]) {
                distance[edge.to] = distance[node] + edge.weight;
            }
        }
    }

    return distance;
}

int main() {
    // 0 -> 1 (2), 0 -> 2 (6), 1 -> 2 (3), 1 -> 3 (8), 2 -> 3 (-2)
    vector<vector<Edge>> graph = {
        {{1, 2}, {2, 6}}, {{2, 3}, {3, 8}}, {{3, -2}}, {}};

    vector<int> distance = dagShortestPath(4, graph, 0);
    for (int value : distance) {
        cout << value << " ";
    }
    cout << endl;  // 0 2 5 3

    // A cycle: 0 -> 1 -> 0
    vector<vector<Edge>> cyclic = {{{1, 1}}, {{0, 1}}};
    vector<int> none = dagShortestPath(2, cyclic, 0);
    cout << none.size() << endl;  // 0

    // Unreachable node: 0 -> 1, 2 isolated
    vector<vector<Edge>> disconnected = {{{1, 5}}, {}, {}};
    vector<int> partial = dagShortestPath(3, disconnected, 0);
    for (int value : partial) {
        cout << value << " ";
    }
    cout << endl;  // 0 5 2147483647

    return 0;
}

// Special cases:
// - a cycle means no topological order -> empty vector
// - unreachable nodes keep INT_MAX
// - negative weights are fine because the topological order prevents
//   revisiting a node after it has been relaxed
// - self-loops create a cycle
