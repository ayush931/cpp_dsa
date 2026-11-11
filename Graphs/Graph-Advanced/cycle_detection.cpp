/**
 * Cycle detection
 * Undirected graph -> a cycle exists if an edge connects two vertices
 * that are already in the same disjoint set
 * Directed graph -> a cycle exists if DFS finds a back edge to a node
 * that is still in the recursion stack
 *
 * DSU approach -> O(V + E) with path compression
 * DFS approach -> O(V + E)
 */

#include <iostream>
#include <vector>
using namespace std;

class DSU {
    vector<int> parent;
    vector<int> rank;

   public:
    DSU(int n) {
        parent.resize(n);
        rank.assign(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return false;  // already connected -> cycle
        }

        if (rank[a] < rank[b]) {
            swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }

        return true;
    }
};

bool hasCycleUndirected(int n, vector<pair<int, int>> &edges) {
    DSU dsu(n);

    for (auto &edge : edges) {
        if (!dsu.unite(edge.first, edge.second)) {
            return true;
        }
    }

    return false;
};

bool dfsDirected(vector<vector<int>> &graph, int node, vector<int> &state) {
    state[node] = 1;  // 0 = unvisited, 1 = in stack, 2 = done

    for (int neighbour : graph[node]) {
        if (state[neighbour] == 1) {
            return true;  // back edge
        }
        if (state[neighbour] == 0 && dfsDirected(graph, neighbour, state)) {
            return true;
        }
    }

    state[node] = 2;
    return false;
};

bool hasCycleDirected(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> state(n, 0);

    for (int i = 0; i < n; i++) {
        if (state[i] == 0 && dfsDirected(graph, i, state)) {
            return true;
        }
    }

    return false;
};

int main() {
    vector<pair<int, int>> undirected = {{0, 1}, {1, 2}, {2, 0}};
    cout << hasCycleUndirected(3, undirected) << endl;  // 1

    vector<vector<int>> directed = {{1}, {2}, {0}};
    cout << hasCycleDirected(directed) << endl;  // 1

    vector<vector<int>> dag = {{1, 2}, {2}, {}};
    cout << hasCycleDirected(dag) << endl;  // 0

    return 0;
}

// Special cases: self loop, parallel edges, disconnected graph,
// undirected edge listed twice, directed graph with cross edges
