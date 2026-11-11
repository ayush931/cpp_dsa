/**
 * Offline dynamic connectivity
 *
 * Edges are active only during an interval of time. Queries ask whether two
 * vertices are connected at a specific time.
 *
 * Algorithm:
 *   1. Add every edge to a segment tree over the time axis, covering its active
 *      interval.
 *   2. DFS the segment tree. At each node, union the edges stored there.
 *   3. At a leaf, answer the query for that time.
 *   4. Roll back the unions when leaving the node.
 *
 * A rollback DSU uses union by size without path compression, so every union
 * can be undone in O(1).
 *
 * Time -> O((E + Q) log Q log n)
 * Space -> O(E log Q + n)
 *
 * Edge cases: an edge active for one time unit, repeated add/remove of the same
 * edge, self-loops, parallel edges, queries before any edge, and disconnected
 * vertices.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class RollbackDSU {
    vector<int> parent;
    vector<int> size;

    struct Change {
        int child;
        int root;
        int rootSize;
    };

    vector<Change> history;

   public:
    RollbackDSU(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {
        while (parent[node] != node) {
            node = parent[node];
        }
        return node;
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            history.push_back({-1, -1, -1});
            return;
        }

        if (size[a] < size[b]) {
            swap(a, b);
        }

        history.push_back({b, a, size[a]});
        parent[b] = a;
        size[a] += size[b];
    }

    int snapshot() const {
        return history.size();
    }

    void rollback(int snapshot) {
        while ((int)history.size() > snapshot) {
            Change change = history.back();
            history.pop_back();

            if (change.child == -1) {
                continue;
            }

            parent[change.child] = change.child;
            size[change.root] = change.rootSize;
        }
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};

struct Edge {
    int u;
    int v;
    int left;
    int right;
};

struct Query {
    int u;
    int v;
};

class OfflineDynamicConnectivity {
    int n;
    int timeCount;
    vector<Edge> edges;
    vector<Query> queries;
    vector<vector<pair<int, int>>> segmentTree;
    vector<bool> answers;
    RollbackDSU dsu;

    void addEdge(int node, int left, int right, int queryLeft, int queryRight,
                 int u, int v) {
        if (queryLeft > right || queryRight < left) {
            return;
        }

        if (queryLeft <= left && right <= queryRight) {
            segmentTree[node].push_back({u, v});
            return;
        }

        int middle = left + (right - left) / 2;
        addEdge(node * 2, left, middle, queryLeft, queryRight, u, v);
        addEdge(node * 2 + 1, middle + 1, right, queryLeft, queryRight, u, v);
    }

    void dfs(int node, int left, int right) {
        int snapshot = dsu.snapshot();

        for (auto &[u, v] : segmentTree[node]) {
            dsu.unite(u, v);
        }

        if (left == right) {
            if (left < (int)queries.size() && queries[left].u != -1) {
                answers[left] = dsu.connected(queries[left].u, queries[left].v);
            }
        } else {
            int middle = left + (right - left) / 2;
            dfs(node * 2, left, middle);
            dfs(node * 2 + 1, middle + 1, right);
        }

        dsu.rollback(snapshot);
    }

   public:
    OfflineDynamicConnectivity(int vertices, vector<Edge> &inputEdges,
                               vector<Query> &inputQueries)
        : n(vertices), edges(inputEdges), queries(inputQueries), dsu(vertices) {
        timeCount = max(1, (int)queries.size());
        segmentTree.assign(4 * timeCount, {});
        answers.assign(timeCount, false);

        for (Edge &edge : edges) {
            int left = max(0, edge.left);
            int right = min(timeCount - 1, edge.right);

            if (left <= right) {
                addEdge(1, 0, timeCount - 1, left, right, edge.u, edge.v);
            }
        }
    }

    vector<bool> solve() {
        dfs(1, 0, timeCount - 1);
        return answers;
    }
};

int main() {
    // Time 0..3.
    // 0-1 is active at times 0,1,2.
    // 2-3 is active at times 1,2,3.
    // 1-2 is active only at time 2.
    vector<Edge> edges = {{0, 1, 0, 2}, {2, 3, 1, 3}, {1, 2, 2, 2}};

    vector<Query> queries = {{0, 1}, {0, 3}, {0, 3}, {0, 3}};

    OfflineDynamicConnectivity solver(4, edges, queries);
    vector<bool> answers = solver.solve();

    for (bool answer : answers) {
        cout << answer << " ";
    }
    cout << endl;  // 1 0 1 0

    // No queries -> empty answer.
    vector<Query> none;
    OfflineDynamicConnectivity empty(2, edges, none);
    cout << empty.solve().size() << endl;  // 1 (one default time slot)

    // Self-loop and parallel edges.
    vector<Edge> parallel = {{0, 0, 0, 1}, {0, 1, 0, 1}, {0, 1, 0, 1}};
    vector<Query> parallelQueries = {{0, 1}, {1, 1}};

    OfflineDynamicConnectivity second(2, parallel, parallelQueries);
    vector<bool> secondAnswers = second.solve();
    for (bool answer : secondAnswers) {
        cout << answer << " ";
    }
    cout << endl;  // 1 1

    return 0;
}

// Special cases:
// - an edge active for a single time unit is stored in one leaf path
// - self-loops never change connectivity
// - parallel edges are harmless; the DSU simply sees already connected nodes
// - a query at a time with no edges returns false unless u == v
// - the segment tree is built over the number of query times
