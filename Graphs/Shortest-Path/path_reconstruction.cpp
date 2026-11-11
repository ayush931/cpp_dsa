/**
 * Shortest path reconstruction
 *
 * Dijkstra computes distances, but it does not remember the path. Store a
 * parent for every node when its distance improves, then walk backwards from
 * the target.
 *
 * Path reconstruction -> O(path length)
 * Dijkstra -> O((V + E) log V)
 *
 * The same idea works with Bellman-Ford and Floyd-Warshall (with a next
 * matrix).
 *
 * Edge cases: source == target, unreachable target, multiple shortest paths,
 * negative weights (Dijkstra is not valid), and a negative cycle
 * (Bellman-Ford must detect it).
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int weight;
};

pair<vector<int>, vector<int>> dijkstraWithPath(int n,
                                                vector<vector<Edge>> &graph,
                                                int source) {
    vector<int> distance(n, INT_MAX);
    vector<int> parent(n, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDistance, node] = pq.top();
        pq.pop();

        if (currentDistance != distance[node]) {
            continue;
        }

        for (Edge &edge : graph[node]) {
            if (distance[node] + edge.weight < distance[edge.to]) {
                distance[edge.to] = distance[node] + edge.weight;
                parent[edge.to] = node;
                pq.push({distance[edge.to], edge.to});
            }
        }
    }

    return {distance, parent};
}

vector<int> buildPath(vector<int> &parent, int source, int target) {
    if (source < 0 || target < 0 || target >= (int)parent.size()) {
        return {};
    }

    vector<int> path;
    int node = target;

    while (node != -1) {
        path.push_back(node);
        if (node == source) {
            break;
        }
        node = parent[node];
    }

    if (path.empty() || path.back() != source) {
        return {};
    }

    reverse(path.begin(), path.end());
    return path;
}

int main() {
    // 0 -> 1 (1), 0 -> 2 (4), 1 -> 2 (2), 1 -> 3 (6), 2 -> 3 (3)
    vector<vector<Edge>> graph = {
        {{1, 1}, {2, 4}}, {{2, 2}, {3, 6}}, {{3, 3}}, {}};

    auto [distance, parent] = dijkstraWithPath(4, graph, 0);

    for (int value : distance) {
        cout << value << " ";
    }
    cout << endl;  // 0 1 3 6

    vector<int> path = buildPath(parent, 0, 3);
    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;  // 0 1 2 3

    vector<int> same = buildPath(parent, 2, 2);
    for (int node : same) {
        cout << node << " ";
    }
    cout << endl;  // 2

    vector<int> unreachable = buildPath(parent, 3, 0);
    cout << unreachable.size() << endl;  // 0

    return 0;
}

// Special cases:
// - source == target -> path [source]
// - unreachable target -> empty path
// - multiple shortest paths -> the parent array stores the last improvement
// - negative edges break Dijkstra; use Bellman-Ford instead
// - a negative cycle has no well-defined shortest path
