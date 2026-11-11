/**
 * Gomory-Hu tree
 *
 * A Gomory-Hu tree represents all pairwise minimum s-t cuts of an undirected
 * graph with n - 1 weighted edges. The minimum cut between any two vertices is
 * the minimum edge weight on the path between them in the tree.
 *
 * This implementation uses Gusfield's simplified algorithm:
 *   - for every vertex s from 1 to n - 1, let t = parent[s]
 *   - compute a minimum s-t cut with Dinic
 *   - update the parent pointers of vertices on s's side of the cut
 *
 * Time -> O(n) max-flow computations
 * Space -> O(V + E) per flow
 *
 * Edge cases: one vertex (no edges), disconnected graph (cuts of 0), parallel
 * edges (capacities add), self-loops (ignored), and zero-capacity edges.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Dinic {
    struct Edge {
        int to;
        int capacity;
        int flow;
    };

    int n;
    vector<Edge> edges;
    vector<vector<int>> adjacency;
    vector<int> level;
    vector<int> nextEdge;

    bool bfs(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        level[source] = 0;

        queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int edgeIndex : adjacency[node]) {
                Edge &edge = edges[edgeIndex];

                if (edge.capacity - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[node] + 1;
                    q.push(edge.to);
                }
            }
        }

        return level[sink] != -1;
    }

    int dfs(int node, int sink, int pushed) {
        if (node == sink) {
            return pushed;
        }

        for (int &i = nextEdge[node]; i < (int)adjacency[node].size(); i++) {
            int edgeIndex = adjacency[node][i];
            Edge &edge = edges[edgeIndex];

            if (edge.capacity - edge.flow <= 0) {
                continue;
            }
            if (level[edge.to] != level[node] + 1) {
                continue;
            }

            int flow =
                dfs(edge.to, sink, min(pushed, edge.capacity - edge.flow));
            if (flow == 0) {
                continue;
            }

            edge.flow += flow;
            edges[edgeIndex ^ 1].flow -= flow;
            return flow;
        }

        return 0;
    }

   public:
    Dinic(int size) : n(size), adjacency(size), level(size), nextEdge(size) {}

    void addEdge(int from, int to, int capacity) {
        if (from == to || capacity <= 0) {
            return;
        }

        adjacency[from].push_back(edges.size());
        edges.push_back({to, capacity, 0});

        adjacency[to].push_back(edges.size());
        edges.push_back({from, 0, 0});
    }

    int maxFlow(int source, int sink) {
        int total = 0;

        while (bfs(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);

            while (int pushed = dfs(source, sink, INT_MAX)) {
                total += pushed;
            }
        }

        return total;
    }

    vector<bool> reachableFrom(int source) {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int edgeIndex : adjacency[node]) {
                Edge &edge = edges[edgeIndex];

                if (edge.capacity - edge.flow > 0 && !visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        return visited;
    }
};

struct GomoryHuEdge {
    int from;
    int to;
    int capacity;
};

vector<GomoryHuEdge> gomoryHu(int n, vector<vector<int>> &capacity) {
    vector<GomoryHuEdge> tree;
    if (n <= 1) {
        return tree;
    }

    vector<int> parent(n, 0);
    vector<int> cutValue(n, 0);

    for (int source = 1; source < n; source++) {
        int sink = parent[source];

        Dinic flow(n);

        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                if (capacity[u][v] > 0) {
                    // An undirected edge is two directed edges with the same
                    // capacity.
                    flow.addEdge(u, v, capacity[u][v]);
                    flow.addEdge(v, u, capacity[u][v]);
                }
            }
        }

        int value = flow.maxFlow(source, sink);
        vector<bool> reachable = flow.reachableFrom(source);

        for (int node = source + 1; node < n; node++) {
            if (parent[node] == sink && reachable[node]) {
                parent[node] = source;
            }
        }

        if (sink != 0 && reachable[parent[sink]]) {
            parent[source] = parent[sink];
            parent[sink] = source;
            cutValue[source] = cutValue[sink];
            cutValue[sink] = value;
        } else {
            cutValue[source] = value;
        }
    }

    for (int node = 1; node < n; node++) {
        tree.push_back({node, parent[node], cutValue[node]});
    }

    return tree;
}

int main() {
    // Triangle: 0-1 capacity 1, 1-2 capacity 2, 0-2 capacity 3.
    vector<vector<int>> capacity = {{0, 1, 3}, {1, 0, 2}, {3, 2, 0}};

    vector<GomoryHuEdge> tree = gomoryHu(3, capacity);

    for (GomoryHuEdge &edge : tree) {
        cout << edge.from << " " << edge.to << " " << edge.capacity << endl;
    }
    // 1 0 3
    // 2 0 4

    // Disconnected graph: 0-1 capacity 5, 2 isolated.
    vector<vector<int>> disconnected = {{0, 5, 0}, {5, 0, 0}, {0, 0, 0}};

    vector<GomoryHuEdge> second = gomoryHu(3, disconnected);
    for (GomoryHuEdge &edge : second) {
        cout << edge.from << " " << edge.to << " " << edge.capacity << endl;
    }
    // 1 0 5
    // 2 0 0

    vector<GomoryHuEdge> single = gomoryHu(1, capacity);
    cout << single.size() << endl;  // 0

    return 0;
}

// Special cases:
// - one vertex -> no tree edges
// - disconnected graph -> some tree edges have capacity 0
// - parallel edges are summed in the capacity matrix before the flow
// - self-loops are ignored because they never cross a cut
// - the minimum cut between u and v is the minimum edge on the tree path
