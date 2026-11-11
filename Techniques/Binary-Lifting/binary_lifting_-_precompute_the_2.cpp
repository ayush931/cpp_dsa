/**
 * Binary lifting -> precompute the 2^j-th ancestor of every node
 * up[node][j] = 2^j-th ancestor of node
 * up[node][0] = parent[node]
 * up[node][j] = up[up[node][j - 1]][j - 1]
 *
 * Build -> O(n log n)
 * K-th ancestor query -> O(log k)
 * Used for LCA, path queries and jumping on trees
 */

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

int kthAncestor(int node, int k) {
    for (int j = 0; j < LOG; j++) {
        if (node == -1) {
            return -1;
        }
        if (k & (1 << j)) {
            node = up[node][j];
        }
    }

    return node;
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

    cout << kthAncestor(6, 1) << endl;  // 2
    cout << kthAncestor(6, 2) << endl;  // 0
    cout << kthAncestor(6, 3) << endl;  // -1

    return 0;
}

// Special cases: k = 0, k greater than depth, root node,
// forest (run build from every root), node values not 0-based
