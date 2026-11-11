/**
 * Heavy-Light Decomposition (HLD)
 *
 * Heavy-Light Decomposition decomposes a tree into vertex-disjoint paths
 * ("heavy paths") such that any simple path from the root to any node
 * traverses at most O(log N) light edges.
 *
 * Key Properties:
 *   - Heavy child: The child with the largest subtree size.
 *   - Heavy edge: Edge connecting a node to its heavy child.
 *   - Light edge: Edge connecting a node to any other child.
 *   - Heavy path: A maximal path consisting entirely of heavy edges.
 *   - Every node belongs to exactly one heavy path.
 *
 * Tree Flattening & Segment Tree:
 *   - By performing a DFS that visits the heavy child first, all nodes
 *     along the same heavy path receive contiguous positions in the DFS order.
 *   - A standard Segment Tree can then be built on top of this flattened array.
 *   - Path queries (sum, max, etc.) are answered by jumping up heavy path
 *     heads until the two nodes meet at their LCA, requiring at most O(log N)
 *     segment tree queries -> O(log^2 N) overall.
 *   - Subtree queries cover a single contiguous range [pos[u], pos[u] + size[u]
 * - 1] in the segment tree -> O(log N) overall.
 *
 * Operations:
 *   1. updateNode(u, val): Point update in O(log N).
 *   2. queryPath(u, v): Query sum/max along path between u and v in O(log^2 N).
 *   3. querySubtree(u): Query sum/max in subtree of u in O(log N).
 *   4. getLCA(u, v): Lowest Common Ancestor via heavy path heads in O(log N).
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Segment Tree for range sum queries and point updates
class SegmentTree {
   private:
    int n;
    vector<long long> tree;

    void build(int node, int start, int end, const vector<long long> &arr) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(2 * node, start, mid, arr);
        build(2 * node + 1, mid + 1, end, arr);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void update(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return 0;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = start + (end - start) / 2;
        return query(2 * node, start, mid, l, r) +
               query(2 * node + 1, mid + 1, end, l, r);
    }

   public:
    SegmentTree(int size = 0) : n(size), tree(4 * size, 0) {}

    void init(const vector<long long> &arr) {
        n = arr.size();
        tree.assign(4 * n, 0);
        if (n > 0) {
            build(1, 0, n - 1, arr);
        }
    }

    void update(int idx, long long val) {
        update(1, 0, n - 1, idx, val);
    }

    long long query(int l, int r) {
        if (l > r) {
            return 0;
        }
        return query(1, 0, n - 1, l, r);
    }
};

class HeavyLightDecomposition {
   private:
    int n;
    vector<vector<int>> adj;
    vector<int> parent;
    vector<int> depth;
    vector<int> heavy;           // heavy[u] = heavy child of u (-1 if leaf)
    vector<int> head;            // head[u] = head of heavy path containing u
    vector<int> pos;             // pos[u] = position of u in flattened array
    vector<int> sub_size;        // sub_size[u] = size of subtree rooted at u
    vector<long long> node_val;  // initial value of node u
    SegmentTree segTree;
    int cur_pos;

    // DFS 1: compute subtree sizes, depth, parent, and identify heavy children
    int dfs_size(int u, int p, int d) {
        parent[u] = p;
        depth[u] = d;
        sub_size[u] = 1;
        int max_child_size = 0;
        heavy[u] = -1;

        for (int v : adj[u]) {
            if (v == p) {
                continue;
            }
            int c_size = dfs_size(v, u, d + 1);
            sub_size[u] += c_size;
            if (c_size > max_child_size) {
                max_child_size = c_size;
                heavy[u] = v;
            }
        }
        return sub_size[u];
    }

    // DFS 2: decompose into heavy paths, assigning contiguous positions
    void dfs_hld(int u, int h, vector<long long> &flat_arr) {
        head[u] = h;
        pos[u] = cur_pos++;
        flat_arr[pos[u]] = node_val[u];

        // Recurse on heavy child first to keep path contiguous
        if (heavy[u] != -1) {
            dfs_hld(heavy[u], h, flat_arr);
        }

        // Recurse on light children; each starts a new heavy path
        for (int v : adj[u]) {
            if (v == parent[u] || v == heavy[u]) {
                continue;
            }
            dfs_hld(v, v, flat_arr);
        }
    }

   public:
    HeavyLightDecomposition(int vertices)
        : n(vertices),
          adj(vertices),
          parent(vertices, -1),
          depth(vertices, 0),
          heavy(vertices, -1),
          head(vertices, 0),
          pos(vertices, 0),
          sub_size(vertices, 0),
          node_val(vertices, 0),
          cur_pos(0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void setNodeValue(int u, long long val) {
        node_val[u] = val;
    }

    void build(int root = 0) {
        cur_pos = 0;
        dfs_size(root, -1, 0);
        vector<long long> flat_arr(n, 0);
        dfs_hld(root, root, flat_arr);
        segTree.init(flat_arr);
    }

    void updateNode(int u, long long val) {
        node_val[u] = val;
        segTree.update(pos[u], val);
    }

    long long queryPath(int u, int v) {
        long long total = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                swap(u, v);
            }
            // v's head is deeper; query from head[v] to v
            total += segTree.query(pos[head[v]], pos[v]);
            v = parent[head[v]];
        }
        // Now both u and v are on the same heavy path
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        total += segTree.query(pos[u], pos[v]);
        return total;
    }

    long long querySubtree(int u) {
        return segTree.query(pos[u], pos[u] + sub_size[u] - 1);
    }

    int getLCA(int u, int v) {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                swap(u, v);
            }
            v = parent[head[v]];
        }
        return (depth[u] < depth[v]) ? u : v;
    }
};

int main() {
    cout << "=== Heavy-Light Decomposition (HLD) Demo ===" << endl;

    // Build a tree with 9 nodes (0 through 8)
    //
    //            0 (val: 10)
    //          /   \
    //    (val: 20) 1   2 (val: 30)
    //            / | \
    //           3  4  5
    //          /       \
    //         6         7
    //                    \
    //                     8
    int n = 9;
    HeavyLightDecomposition hld(n);

    hld.addEdge(0, 1);
    hld.addEdge(0, 2);
    hld.addEdge(1, 3);
    hld.addEdge(1, 4);
    hld.addEdge(1, 5);
    hld.addEdge(3, 6);
    hld.addEdge(5, 7);
    hld.addEdge(7, 8);

    vector<long long> values = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    for (int i = 0; i < n; ++i) {
        hld.setNodeValue(i, values[i]);
    }

    hld.build(0);

    cout << "\n--- Path Sum Queries ---" << endl;
    cout << "Sum along path 6 -> 8 (6-3-1-5-7-8): " << hld.queryPath(6, 8)
         << " (expected: " << (70 + 40 + 20 + 60 + 80 + 90) << ")" << endl;

    cout << "Sum along path 2 -> 6 (2-0-1-3-6): " << hld.queryPath(2, 6)
         << " (expected: " << (30 + 10 + 20 + 40 + 70) << ")" << endl;

    cout << "\n--- Lowest Common Ancestor (LCA) ---" << endl;
    cout << "LCA(6, 8): " << hld.getLCA(6, 8) << " (expected: 1)" << endl;
    cout << "LCA(2, 7): " << hld.getLCA(2, 7) << " (expected: 0)" << endl;

    cout << "\n--- Subtree Sum Query ---" << endl;
    cout << "Subtree sum of node 1 (1, 3, 4, 5, 6, 7, 8): "
         << hld.querySubtree(1)
         << " (expected: " << (20 + 40 + 50 + 60 + 70 + 80 + 90) << ")" << endl;

    cout << "\n--- Point Update ---" << endl;
    cout << "Updating node 1 value from 20 to 100..." << endl;
    hld.updateNode(1, 100);

    cout << "New sum along path 6 -> 8: " << hld.queryPath(6, 8)
         << " (expected: " << (70 + 40 + 100 + 60 + 80 + 90) << ")" << endl;
    cout << "New subtree sum of node 1: " << hld.querySubtree(1) << endl;

    return 0;
}

/**
 * Time Complexity:
 *   - Preprocessing / Build: O(N) DFS + O(N) Segment Tree build = O(N)
 *   - Path Query: O(log^2 N) - at most O(log N) heavy path jumps, each O(log N)
 * in segment tree
 *   - Subtree Query: O(log N) - contiguous range query in segment tree
 *   - Point Update: O(log N) - single segment tree update
 *   - LCA Query: O(log N) - jumping heavy path heads without segment tree
 * queries
 *
 * Space Complexity:
 *   - O(N) for tree representations, HLD metadata, and Segment Tree
 */
