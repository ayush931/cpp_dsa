/**
 * Min Cost to Connect All Points
 *
 * You are given points where points[i] = [xi, yi]. The cost of connecting two
 * points is their Manhattan distance: |xi - xj| + |yi - yj|.
 *
 * Return the minimum cost to connect all points so that there is exactly one
 * simple path between any two points.
 *
 * Example 1:
 * Input: points = [[0,0],[2,2],[3,10],[5,2],[7,0]]
 * Output: 20
 *
 * Example 2:
 * Input: points = [[3,12],[-2,5],[-4,1]]
 * Output: 18
 *
 * Constraints: 1 <= points.length <= 1000,
 * -10^6 <= xi, yi <= 10^6, all pairs are distinct.
 */

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

int manhattanDistance(vector<int> &a, vector<int> &b) {
    return abs(a[0] - b[0]) + abs(a[1] - b[1]);
}

// Approach 1: Kruskal on the complete graph.
// O(n^2 log n) time and O(n^2) space for the edge list.
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

struct Edge {
    int from;
    int to;
    int cost;
};

int minCostConnectPointsKruskal(vector<vector<int>> &points) {
    int n = points.size();

    if (n <= 1) {
        return 0;
    }

    vector<Edge> edges;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges.push_back({i, j, manhattanDistance(points[i], points[j])});
        }
    }

    sort(edges.begin(), edges.end(),
         [](Edge &a, Edge &b) { return a.cost < b.cost; });

    DSU dsu(n);
    int total = 0;
    int used = 0;

    for (Edge &edge : edges) {
        if (dsu.unite(edge.from, edge.to)) {
            total += edge.cost;
            used++;

            if (used == n - 1) {
                break;
            }
        }
    }

    return total;
}

// Approach 2: Prim on the complete graph without storing the edges.
// O(n^2) time, O(n) space. This is the better choice for a dense graph.
int minCostConnectPointsPrim(vector<vector<int>> &points) {
    int n = points.size();

    if (n <= 1) {
        return 0;
    }

    vector<int> minCost(n, INT_MAX);
    vector<bool> inTree(n, false);

    minCost[0] = 0;
    int total = 0;

    for (int step = 0; step < n; step++) {
        int point = -1;

        for (int i = 0; i < n; i++) {
            if (!inTree[i] && (point == -1 || minCost[i] < minCost[point])) {
                point = i;
            }
        }

        inTree[point] = true;
        total += minCost[point];

        for (int next = 0; next < n; next++) {
            if (inTree[next]) {
                continue;
            }

            int cost = manhattanDistance(points[point], points[next]);

            if (cost < minCost[next]) {
                minCost[next] = cost;
            }
        }
    }

    return total;
}

int main() {
    vector<vector<int>> first = {{0, 0}, {2, 2}, {3, 10}, {5, 2}, {7, 0}};
    vector<vector<int>> second = {{3, 12}, {-2, 5}, {-4, 1}};
    vector<vector<int>> single = {{0, 0}};
    vector<vector<int>> two = {{0, 0}, {3, 4}};
    vector<vector<int>> line = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};

    cout << minCostConnectPointsKruskal(first) << " "
         << minCostConnectPointsPrim(first) << endl;
    // 20 20

    cout << minCostConnectPointsKruskal(second) << " "
         << minCostConnectPointsPrim(second) << endl;
    // 18 18

    cout << minCostConnectPointsPrim(single) << endl;   // 0
    cout << minCostConnectPointsPrim(two) << endl;      // 7
    cout << minCostConnectPointsKruskal(line) << endl;  // 3
    cout << minCostConnectPointsPrim(line) << endl;     // 3

    return 0;
}

/**
 * Time complexity:
 *   Kruskal -> O(n^2 log n): O(n^2) edges, sorting dominates
 *   Prim -> O(n^2): n rounds, each scanning n distances
 * Space complexity:
 *   Kruskal -> O(n^2) for the edge list
 *   Prim -> O(n) for the distance array
 *
 * Special cases: one point (0), two points (their distance), collinear points,
 * duplicate coordinates (excluded by the constraints), negative coordinates,
 * and a large coordinate difference (use int; the maximum Manhattan distance
 * is 4 * 10^6, which fits comfortably).
 *
 * The graph is complete, so it is always possible to connect all points. The
 * answer is the weight of a minimum spanning tree. For n = 1000, Prim's O(n^2)
 * version avoids materializing about 500,000 edges.
 */
