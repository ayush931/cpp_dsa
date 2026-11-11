/**
 * Most Stones Removed with Same Row or Column
 *
 * On a 2D plane, n stones are placed at integer coordinates. Each coordinate
 * holds at most one stone. A stone can be removed if it shares a row or a
 * column with another stone that has not been removed.
 *
 * Return the largest possible number of stones that can be removed.
 *
 * Example 1:
 * Input: stones = [[0,0],[0,1],[1,0],[1,2],[2,1],[2,2]]
 * Output: 5
 *
 * Example 2:
 * Input: stones = [[0,0],[0,2],[1,1],[2,0],[2,2]]
 * Output: 3
 *
 * Example 3:
 * Input: stones = [[0,0]]
 * Output: 0
 *
 * Constraints: 1 <= stones.length <= 1000, 0 <= xi, yi <= 10^4.
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

// Approach 1: build a graph where two stones are adjacent when they share a
// row or a column. Every connected component with k stones can lose k - 1
// stones and keep one. O(n^2) time, O(n^2) space.
void dfs(int stone, vector<vector<int>> &graph, vector<bool> &visited) {
    visited[stone] = true;

    for (int neighbour : graph[stone]) {
        if (!visited[neighbour]) {
            dfs(neighbour, graph, visited);
        }
    }
}

int removeStonesDFS(vector<vector<int>> &stones) {
    int n = stones.size();

    vector<vector<int>> graph(n);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (stones[i][0] == stones[j][0] || stones[i][1] == stones[j][1]) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }

    vector<bool> visited(n, false);
    int components = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            components++;
            dfs(i, graph, visited);
        }
    }

    return n - components;
}

// Approach 2: union-find over the stones. Two stones are in the same set when
// they share a row or a column. O(n * alpha(n)) expected time, O(n) space.
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
            parent[node] = find(parent[node]);  // path compression
        }

        return parent[node];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return;
        }

        if (rank[a] >= rank[b]) {
            rank[a] += rank[b];
            parent[b] = a;
        } else {
            rank[b] += rank[a];
            parent[a] = b;
        }
    }
};

int removeStonesDSU(vector<vector<int>> &stones) {
    int n = stones.size();
    DSU dsu(n);

    unordered_map<int, int> firstStoneInRow;
    unordered_map<int, int> firstStoneInColumn;

    for (int i = 0; i < n; i++) {
        int row = stones[i][0];
        int column = stones[i][1];

        if (firstStoneInRow.count(row)) {
            dsu.unite(i, firstStoneInRow[row]);
        } else {
            firstStoneInRow[row] = i;
        }

        if (firstStoneInColumn.count(column)) {
            dsu.unite(i, firstStoneInColumn[column]);
        } else {
            firstStoneInColumn[column] = i;
        }
    }

    unordered_set<int> roots;

    for (int i = 0; i < n; i++) {
        roots.insert(dsu.find(i));
    }

    return n - roots.size();
}

int main() {
    vector<vector<int>> first = {{0, 0}, {0, 1}, {1, 0},
                                 {1, 2}, {2, 1}, {2, 2}};

    vector<vector<int>> second = {{0, 0}, {0, 2}, {1, 1}, {2, 0}, {2, 2}};

    vector<vector<int>> single = {{0, 0}};

    vector<vector<int>> sameRow = {{0, 0}, {0, 1}, {0, 2}};

    vector<vector<int>> disconnected = {{0, 0}, {1, 1}, {2, 2}};

    cout << removeStonesDFS(first) << " " << removeStonesDSU(first) << endl;
    // 5 5

    cout << removeStonesDFS(second) << " " << removeStonesDSU(second) << endl;
    // 3 3

    cout << removeStonesDSU(single) << endl;        // 0
    cout << removeStonesDSU(sameRow) << endl;       // 2
    cout << removeStonesDSU(disconnected) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   DFS graph -> O(n^2) to build the graph, O(n^2) space
 *   DSU -> O(n * alpha(n)) expected, O(n) space
 * Space complexity -> O(n^2) for the graph, O(n) for the DSU
 *
 * Special cases: one stone (0), all stones in one row (n - 1), all stones in
 * one column (n - 1), isolated stones (0), and several independent components
 * (n - number of components).
 *
 * The answer is always n minus the number of connected components. In each
 * component one stone must remain, and every other stone can be removed in a
 * leaf-to-root order. Coordinates can be large, so the DSU maps rows and
 * columns instead of allocating an array over the coordinate range.
 */
