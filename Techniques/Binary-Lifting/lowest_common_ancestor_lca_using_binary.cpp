/**
 * Lowest Common Ancestor (LCA) using binary lifting
 * LCA -> deepest node that is an ancestor of both u and v
 *
 * Brute force -> store the path from root to both nodes and compare, O(n)
 * Optimized -> lift the deeper node, then lift both together, O(log n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int LOG = 20;

vector<vector<int>> up;
vector<int> depth;

void build(vector<vector<int>> &tree, int node, int parent) {
    up[node][0] = parent;
    depth[node] = (parent == -1) ? 0 : depth[parent] + 1;

    for (int j = 1; j < LOG; j++) {
        int middle = up[node][j - 1];
        up[node][j] = (middle == -1) ? -1 : up[middle][j - 1];
    }

    for (int child : tree[node]) {
        if (child != parent) {
            build(tree, child, node);
        }
    }
};

int lcaBrute(int u, int v, vector<int> &parent) {
    vector<bool> ancestors(100, false);

    while (u != -1) {
        ancestors[u] = true;
        u = parent[u];
    }

    while (!ancestors[v]) {
        v = parent[v];
    }

    return v;
};

int lca(int u, int v) {
    if (depth[u] < depth[v]) {
        swap(u, v);
    }

    int difference = depth[u] - depth[v];

    for (int j = 0; j < LOG; j++) {
        if (difference & (1 << j)) {
            u = up[u][j];
        }
    }

    if (u == v) {
        return u;
    }

    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }

    return up[u][0];
};

int main() {
    int n = 7;
    vector<vector<int>> tree(n);

    tree[0] = {1, 2};
    tree[1] = {0, 3, 4};
    tree[2] = {0, 5, 6};
    tree[3] = {1};
    tree[4] = {1};
    tree[5] = {2};
    tree[6] = {2};

    up.assign(n, vector<int>(LOG, -1));
    depth.assign(n, 0);

    build(tree, 0, -1);

    vector<int> parent = {-1, 0, 0, 1, 1, 2, 2};

    cout << lcaBrute(3, 4, parent) << endl;  // 1
    cout << lca(3, 4) << endl;               // 1
    cout << lca(3, 6) << endl;               // 0

    return 0;
}

// Special cases: u == v, one node is the ancestor of the other,
// root as one of the nodes, forest with multiple roots
