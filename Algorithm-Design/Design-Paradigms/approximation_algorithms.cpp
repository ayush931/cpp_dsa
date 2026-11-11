/**
 * Approximation algorithms
 *
 * Some problems are NP-hard, so an exact algorithm may be too slow. An
 * approximation algorithm runs in polynomial time and guarantees a ratio
 * between its answer and the optimum.
 *
 * 1. Vertex cover 2-approximation
 *    Repeatedly pick an uncovered edge and add both endpoints.
 *    The chosen edges are disjoint, so any vertex cover needs at least one
 *    endpoint per chosen edge -> the answer is at most twice the optimum.
 *
 * 2. Metric TSP 2-approximation
 *    Build an MST, walk around it, and shortcut repeated vertices.
 *    The shortcut tour costs at most twice the MST weight, and the MST weight
 *    is at most the optimal tour weight.
 *
 * 3. Greedy set cover
 *    Repeatedly choose the set that covers the most uncovered elements.
 *    Gives an O(log n) approximation.
 *
 * Edge cases: empty graph, one vertex, duplicate edges, disconnected graphs,
 * and non-metric TSP (where the 2-approximation does not apply).
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

vector<pair<int, int>> vertexCoverApprox(int n, vector<pair<int, int>> &edges) {
    vector<bool> covered(n, false);
    vector<pair<int, int>> answer;

    for (auto &[u, v] : edges) {
        if (!covered[u] && !covered[v]) {
            covered[u] = true;
            covered[v] = true;
            answer.push_back({u, v});
        }
    }

    return answer;
}

class DisjointSet {
    vector<int> parent;
    vector<int> rank;

   public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
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
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) {
            return false;
        }

        if (rank[rootA] < rank[rootB]) {
            swap(rootA, rootB);
        }
        parent[rootB] = rootA;
        if (rank[rootA] == rank[rootB]) {
            rank[rootA]++;
        }
        return true;
    }
};

int metricTspApprox(vector<vector<int>> &distance) {
    int n = distance.size();
    if (n <= 1) {
        return 0;
    }

    vector<pair<int, pair<int, int>>> edges;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges.push_back({distance[i][j], {i, j}});
        }
    }

    sort(edges.begin(), edges.end());

    DisjointSet dsu(n);
    vector<vector<int>> tree(n);

    for (auto &[weight, endpoints] : edges) {
        int u = endpoints.first;
        int v = endpoints.second;

        if (dsu.unite(u, v)) {
            tree[u].push_back(v);
            tree[v].push_back(u);
        }
    }

    // Preorder traversal of the MST, then shortcut repeated vertices.
    vector<int> tour;
    vector<bool> visited(n, false);
    vector<int> stack = {0};

    while (!stack.empty()) {
        int node = stack.back();
        stack.pop_back();

        if (visited[node]) {
            continue;
        }
        visited[node] = true;
        tour.push_back(node);

        for (int i = (int)tree[node].size() - 1; i >= 0; i--) {
            if (!visited[tree[node][i]]) {
                stack.push_back(tree[node][i]);
            }
        }
    }

    int cost = 0;
    for (int i = 0; i < n; i++) {
        cost += distance[tour[i]][tour[(i + 1) % n]];
    }

    return cost;
}

int greedySetCover(vector<set<int>> &sets, int universeSize) {
    set<int> uncovered;
    for (int i = 0; i < universeSize; i++) {
        uncovered.insert(i);
    }

    int chosen = 0;

    while (!uncovered.empty()) {
        int bestSet = -1;
        int bestCount = 0;

        for (int i = 0; i < (int)sets.size(); i++) {
            int count = 0;
            for (int element : sets[i]) {
                if (uncovered.count(element)) {
                    count++;
                }
            }
            if (count > bestCount) {
                bestCount = count;
                bestSet = i;
            }
        }

        if (bestSet == -1) {
            break;
        }

        chosen++;
        for (int element : sets[bestSet]) {
            uncovered.erase(element);
        }
    }

    return chosen;
}

int main() {
    vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    vector<pair<int, int>> cover = vertexCoverApprox(5, edges);
    cout << cover.size() * 2 << endl;  // 4 vertices in the approximate cover

    vector<vector<int>> distance = {
        {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};
    cout << metricTspApprox(distance) << endl;  // 95 (MST preorder 0-1-2-3-0)

    vector<set<int>> sets = {{0, 1, 2}, {2, 3, 4}, {4, 5}, {1, 5}};
    cout << greedySetCover(sets, 6) << endl;  // 3

    return 0;
}

// Special cases:
// - an empty graph has an empty vertex cover
// - one vertex has an empty cover and a TSP tour of cost 0
// - a disconnected TSP instance has no finite tour; the approximation assumes
//   a complete metric graph
// - set cover may return fewer than the optimum only if the input is invalid;
//   it always covers every element when the union is the whole universe
