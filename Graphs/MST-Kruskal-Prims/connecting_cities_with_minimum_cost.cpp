/**
 * Connecting Cities With Minimum Cost
 *
 * There are n cities labelled from 1 to n. connections[i] = [xi, yi, costi]
 * means city xi and city yi can be connected at cost costi. Return the minimum
 * cost to connect all cities so that every pair has at least one path between
 * them, or -1 if it is impossible.
 *
 * Example 1:
 * Input: n = 3, connections = [[1,2,5],[1,3,6],[2,3,1]]
 * Output: 6
 *
 * Example 2:
 * Input: n = 4, connections = [[1,2,3],[3,4,4]]
 * Output: -1
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// Approach 1: Prim's algorithm without a heap. Repeatedly pick the unvisited
// city with the smallest connection cost. O(n^2 + E) time, O(n + E) space.
int minimumCostPrim(int n, vector<vector<int>> &connections) {
    if (n <= 1) {
        return 0;
    }

    vector<vector<pair<int, int>>> graph(n + 1);

    for (vector<int> &connection : connections) {
        int from = connection[0];
        int to = connection[1];
        int cost = connection[2];

        graph[from].push_back({to, cost});
        graph[to].push_back({from, cost});
    }

    vector<int> minCost(n + 1, INT_MAX);
    vector<bool> inTree(n + 1, false);

    minCost[1] = 0;
    int total = 0;

    for (int step = 0; step < n; step++) {
        int city = -1;

        for (int i = 1; i <= n; i++) {
            if (!inTree[i] && (city == -1 || minCost[i] < minCost[city])) {
                city = i;
            }
        }

        if (city == -1 || minCost[city] == INT_MAX) {
            return -1;  // disconnected
        }

        inTree[city] = true;
        total += minCost[city];

        for (pair<int, int> &edge : graph[city]) {
            int neighbour = edge.first;
            int cost = edge.second;

            if (!inTree[neighbour] && cost < minCost[neighbour]) {
                minCost[neighbour] = cost;
            }
        }
    }

    return total;
}

// Approach 2: Kruskal's algorithm with union-find.
// O(E log E) time, O(n + E) space.
class DSU {
    vector<int> parent;
    vector<int> rank;

   public:
    DSU(int n) {
        parent.resize(n);
        rank.assign(n, 1);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]);
        }

        return parent[node];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return false;
        }

        if (rank[a] >= rank[b]) {
            rank[a] += rank[b];
            parent[b] = a;
        } else {
            rank[b] += rank[a];
            parent[a] = b;
        }

        return true;
    }
};

int minimumCostKruskal(int n, vector<vector<int>> &connections) {
    if (n <= 1) {
        return 0;
    }

    vector<vector<int>> edges = connections;

    sort(edges.begin(), edges.end(),
         [](vector<int> &a, vector<int> &b) { return a[2] < b[2]; });

    DSU dsu(n + 1);
    int total = 0;
    int usedEdges = 0;

    for (vector<int> &edge : edges) {
        if (dsu.unite(edge[0], edge[1])) {
            total += edge[2];
            usedEdges++;

            if (usedEdges == n - 1) {
                break;  // spanning tree is complete
            }
        }
    }

    return (usedEdges == n - 1) ? total : -1;
}

int main() {
    vector<vector<int>> first = {{1, 2, 5}, {1, 3, 6}, {2, 3, 1}};
    vector<vector<int>> second = {{1, 2, 3}, {3, 4, 4}};
    vector<vector<int>> none;
    vector<vector<int>> duplicate = {
        {1, 2, 10}, {1, 2, 1}, {2, 3, 2}, {1, 3, 100}};

    cout << minimumCostPrim(3, first) << " " << minimumCostKruskal(3, first)
         << endl;
    // 6 6

    cout << minimumCostPrim(4, second) << " " << minimumCostKruskal(4, second)
         << endl;
    // -1 -1

    cout << minimumCostPrim(1, none) << endl;          // 0
    cout << minimumCostKruskal(2, none) << endl;       // -1 (no connection)
    cout << minimumCostKruskal(3, duplicate) << endl;  // 3 (1 + 2)

    return 0;
}

/**
 * Time complexity:
 *   Prim without a heap -> O(n^2 + E), good for dense graphs
 *   Kruskal -> O(E log E) for sorting plus O(E * alpha(n)) for the unions
 * Space complexity -> O(n + E) for the graph and the DSU
 *
 * Special cases: one city (0), no connections and more than one city (-1),
 * duplicate connections with different costs (the cheapest one is used),
 * self-loops (ignored by union-find), and a graph that is already a tree.
 *
 * The result is the weight of a minimum spanning tree. If the graph is not
 * connected, no spanning tree exists, so both algorithms return -1. Prim's
 * heap version is O(E log n); the O(n^2) version above is simpler and faster
 * when E is close to n^2.
 */
