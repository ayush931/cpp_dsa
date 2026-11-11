/**
 * Count Unreachable Pairs of Nodes in an Undirected Graph
 *
 * You are given an integer n. There is an undirected graph with n nodes
 * numbered from 0 to n - 1, and edges[i] = [ai, bi] is an edge between ai and
 * bi. Return the number of pairs of different nodes that are unreachable from
 * each other.
 *
 * Example 1:
 * Input: n = 3, edges = [[0,1],[0,2],[1,2]]
 * Output: 0
 *
 * Example 2:
 * Input: n = 7, edges = [[0,2],[0,5],[2,4],[1,6],[5,4]]
 * Output: 14
 *
 * Constraints: 1 <= n <= 10^5, 0 <= edges.length <= 2 * 10^5,
 * 0 <= ai, bi < n, ai != bi, no repeated edges.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Approach 1: BFS every connected component. When a new component of size k is
// found, it is unreachable from every node seen before, adding k * seen pairs.
// O(n + E) time, O(n + E) space.
long long countPairsBFS(int n, vector<vector<int>> &edges) {
    vector<vector<int>> graph(n);

    for (vector<int> &edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }

    vector<bool> visited(n, false);
    long long result = 0;
    long long seen = 0;

    for (int start = 0; start < n; start++) {
        if (visited[start]) {
            continue;
        }

        long long componentSize = 0;
        queue<int> pending;
        pending.push(start);
        visited[start] = true;

        while (!pending.empty()) {
            int node = pending.front();
            pending.pop();
            componentSize++;

            for (int neighbour : graph[node]) {
                if (!visited[neighbour]) {
                    visited[neighbour] = true;
                    pending.push(neighbour);
                }
            }
        }

        result += componentSize * seen;
        seen += componentSize;
    }

    return result;
}

// Approach 2: union-find. After all unions, count the size of every component
// and add size * (n - size) / 2, which counts each unreachable pair once.
// O(n + E * alpha(n)) expected time, O(n) space.
class DSU {
    vector<int> parent;
    vector<int> size;

   public:
    DSU(int n) {
        parent.resize(n);
        size.assign(n, 1);

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

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return;
        }

        if (size[a] < size[b]) {
            int temporary = a;
            a = b;
            b = temporary;
        }

        parent[b] = a;
        size[a] += size[b];
    }

    int componentSize(int node) {
        return size[find(node)];
    }
};

long long countPairsDSU(int n, vector<vector<int>> &edges) {
    DSU dsu(n);

    for (vector<int> &edge : edges) {
        dsu.unite(edge[0], edge[1]);
    }

    long long result = 0;

    for (int i = 0; i < n; i++) {
        if (dsu.find(i) == i) {
            long long componentSize = dsu.componentSize(i);
            result += componentSize * (n - componentSize);
        }
    }

    // Every unordered pair was counted twice, once from each side.
    return result / 2;
}

int main() {
    vector<vector<int>> triangle = {{0, 1}, {0, 2}, {1, 2}};
    vector<vector<int>> second = {{0, 2}, {0, 5}, {2, 4}, {1, 6}, {5, 4}};
    vector<vector<int>> none;
    vector<vector<int>> oneEdge = {{0, 1}};

    cout << countPairsBFS(3, triangle) << " " << countPairsDSU(3, triangle)
         << endl;
    // 0 0

    cout << countPairsBFS(7, second) << " " << countPairsDSU(7, second) << endl;
    // 14 14

    cout << countPairsDSU(1, none) << endl;     // 0
    cout << countPairsDSU(5, none) << endl;     // 10
    cout << countPairsDSU(2, oneEdge) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   BFS -> O(n + E)
 *   DSU -> O(n + E * alpha(n)) expected
 * Space complexity -> O(n + E) for the BFS adjacency list, O(n) for the DSU
 *
 * Special cases: a fully connected graph (0), no edges (n * (n - 1) / 2), one
 * node (0), two nodes with an edge (0), two nodes without an edge (1), and
 * many small components.
 *
 * Use long long: with n = 10^5 the number of pairs is about 5 * 10^9, which
 * does not fit in a 32-bit int. A component of size k is unreachable from the
 * other n - k nodes, giving k * (n - k) ordered pairs; divide by 2 because
 * every unordered pair is counted from both ends.
 */
