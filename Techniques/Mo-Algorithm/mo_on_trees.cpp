/**
 * Mo's algorithm on trees (path queries)
 *
 * Flatten the tree with an Euler tour of length 2n:
 *   - a node is written when it is entered (tin)
 *   - the same node is written again when it is left (tout)
 *
 * For a path u-v with tin[u] <= tin[v]:
 *   - if LCA(u, v) == u: query [tin[u], tin[v]]
 *   - otherwise: query [tout[u], tin[v]] and add LCA(u, v) separately
 *
 * A node that appears twice in the current range is toggled off; a node that
 * appears once is on the path. Toggling adds or removes its value.
 *
 * Build -> O(n log n)
 * Each query -> O((n + q) * sqrt(n)) total
 *
 * Edge cases: u == v, one node is an ancestor of the other, root, negative
 * values, duplicate values.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class MoOnTree {
    int n;
    int log;
    vector<vector<int>> tree;
    vector<int> value;
    vector<int> depth;
    vector<vector<int>> up;
    vector<int> tin;
    vector<int> tout;
    vector<int> euler;
    int timer;

    struct Query {
        int left;
        int right;
        int extra;
        int index;
    };

    vector<Query> queries;
    vector<long long> answers;
    vector<bool> active;
    long long currentSum;
    int blockSize;
    int currentLeft;
    int currentRight;

    void dfs(int node, int parent) {
        tin[node] = timer;
        euler[timer++] = node;

        up[node][0] = parent;
        depth[node] = (parent == -1) ? 0 : depth[parent] + 1;

        for (int j = 1; j < log; j++) {
            int middle = up[node][j - 1];
            up[node][j] = (middle == -1) ? -1 : up[middle][j - 1];
        }

        for (int child : tree[node]) {
            if (child != parent) {
                dfs(child, node);
            }
        }

        tout[node] = timer;
        euler[timer++] = node;
    }

    int kthAncestor(int node, int k) {
        for (int j = 0; j < log && node != -1; j++) {
            if (k & (1 << j)) {
                node = up[node][j];
            }
        }
        return node;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }

        u = kthAncestor(u, depth[u] - depth[v]);
        if (u == v) {
            return u;
        }

        for (int j = log - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

    void toggle(int position) {
        int node = euler[position];

        if (active[node]) {
            currentSum -= value[node];
            active[node] = false;
        } else {
            currentSum += value[node];
            active[node] = true;
        }
    }

   public:
    MoOnTree(vector<vector<int>> &inputTree, vector<int> &inputValue,
             vector<pair<int, int>> &paths) {
        tree = inputTree;
        value = inputValue;
        n = tree.size();

        log = 1;
        while ((1 << log) <= n) {
            log++;
        }

        depth.assign(n, 0);
        up.assign(n, vector<int>(log, -1));
        tin.assign(n, 0);
        tout.assign(n, 0);
        euler.assign(2 * n, 0);
        timer = 0;

        dfs(0, -1);

        blockSize = max(1, (int)sqrt(2 * n) + 1);

        for (int i = 0; i < (int)paths.size(); i++) {
            int u = paths[i].first;
            int v = paths[i].second;

            if (tin[u] > tin[v]) {
                swap(u, v);
            }

            int ancestor = lca(u, v);
            int left;
            int right;
            int extra = -1;

            if (ancestor == u) {
                left = tin[u];
                right = tin[v];
            } else {
                left = tout[u];
                right = tin[v];
                extra = ancestor;
            }

            queries.push_back({left, right, extra, i});
        }

        sort(queries.begin(), queries.end(), [this](Query &a, Query &b) {
            int blockA = a.left / blockSize;
            int blockB = b.left / blockSize;

            if (blockA != blockB) {
                return blockA < blockB;
            }
            if (blockA % 2 == 0) {
                return a.right < b.right;
            }
            return a.right > b.right;
        });

        answers.assign(paths.size(), 0);
        active.assign(n, false);
        currentSum = 0;
        currentLeft = 0;
        currentRight = -1;
    }

    vector<long long> solve() {
        for (Query &query : queries) {
            while (currentLeft > query.left) {
                toggle(--currentLeft);
            }
            while (currentRight < query.right) {
                toggle(++currentRight);
            }
            while (currentLeft < query.left) {
                toggle(currentLeft++);
            }
            while (currentRight > query.right) {
                toggle(currentRight--);
            }

            long long answer = currentSum;
            if (query.extra != -1) {
                answer += value[query.extra];
            }

            answers[query.index] = answer;
        }

        return answers;
    }
};

int main() {
    //        0(1)
    //       /    \
    //     1(2)   2(3)
    //    /   \      \
    //  3(4)  4(5)   5(6)
    vector<vector<int>> tree = {{1, 2}, {0, 3, 4}, {0, 5}, {1}, {1}, {2}};
    vector<int> value = {1, 2, 3, 4, 5, 6};

    vector<pair<int, int>> paths = {{3, 4},   // 4 + 2 + 5 = 11
                                    {3, 5},   // 4 + 2 + 1 + 3 + 6 = 16
                                    {0, 0},   // 1
                                    {4, 5},   // 5 + 2 + 1 + 3 + 6 = 17
                                    {3, 3}};  // 4

    MoOnTree solver(tree, value, paths);
    vector<long long> answers = solver.solve();

    for (long long answer : answers) {
        cout << answer << endl;
    }
    // 11
    // 16
    // 1
    // 17
    // 4

    return 0;
}

// Special cases:
// - u == v -> only that node's value
// - LCA is u or v -> the path is a single Euler interval
// - otherwise the LCA is outside the interval and is added separately
// - negative values work because the sum is maintained incrementally
