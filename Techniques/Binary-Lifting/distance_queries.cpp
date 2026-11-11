/**
 * Distance between two nodes using binary lifting
 *
 * Build a jump table:
 *   up[node][0] = parent of node
 *   up[node][j] = 2^j-th ancestor of node
 *
 * LCA(u, v) -> lift the deeper node to the same depth, then lift both nodes
 * together while their ancestors differ.
 *
 * Distance(u, v) = depth[u] + depth[v] - 2 * depth[LCA(u, v)]
 *
 * Build -> O(n log n)
 * Each LCA/distance query -> O(log n)
 * Edge cases: same node, root involved, node not in tree, disconnected forest.
 */

#include <iostream>
#include <vector>
using namespace std;

class BinaryLifting {
    int n;
    int log;
    vector<vector<int>> up;
    vector<int> depth;

   public:
    BinaryLifting(vector<vector<int>> &tree, int root) {
        n = tree.size();
        log = 1;
        while ((1 << log) <= n) {
            log++;
        }

        up.assign(n, vector<int>(log, -1));
        depth.assign(n, 0);

        build(tree, root, -1);
    }

    void build(vector<vector<int>> &tree, int node, int parent) {
        up[node][0] = parent;
        depth[node] = (parent == -1) ? 0 : depth[parent] + 1;

        for (int j = 1; j < log; j++) {
            int middle = up[node][j - 1];
            up[node][j] = (middle == -1) ? -1 : up[middle][j - 1];
        }

        for (int child : tree[node]) {
            if (child != parent) {
                build(tree, child, node);
            }
        }
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

        // Bring both nodes to the same depth.
        u = kthAncestor(u, depth[u] - depth[v]);
        if (u == v) {
            return u;
        }

        // Lift both nodes while their 2^j ancestors are different.
        for (int j = log - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    }

    int distance(int u, int v) {
        int ancestor = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[ancestor];
    }
};

int main() {
    // Tree:
    //        0
    //       / \
    //      1   2
    //     / \   \
    //    3   4   5
    //   /
    //  6
    vector<vector<int>> tree = {{1, 2}, {0, 3, 4}, {0, 5}, {1, 6},
                                {1},    {2},       {3}};

    BinaryLifting lifting(tree, 0);

    cout << lifting.lca(6, 4) << endl;       // 1
    cout << lifting.lca(6, 5) << endl;       // 0
    cout << lifting.lca(3, 3) << endl;       // 3
    cout << lifting.distance(6, 4) << endl;  // 3 (6-3-1-4)
    cout << lifting.distance(6, 5) << endl;  // 5 (6-3-1-0-2-5)

    return 0;
}

// Special cases:
// - u == v -> distance 0
// - one node is an ancestor of the other -> LCA is the ancestor
// - root has no parent -> up[root][j] stays -1
// - a forest needs one BinaryLifting object per component, or a virtual root
