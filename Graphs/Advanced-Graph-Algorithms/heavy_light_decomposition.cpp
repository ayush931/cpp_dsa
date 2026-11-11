/**
 * Heavy-Light Decomposition (HLD)
 *
 * A tree can be split into disjoint chains so that any root-to-node path
 * crosses at most O(log n) chains. Each chain is stored contiguously in an
 * array, so a path query becomes O(log n) range queries on a segment tree.
 *
 * Terms:
 *   subtree[v] -> number of nodes in v's subtree
 *   heavy[v]   -> the child with the largest subtree (or -1)
 *   head[v]    -> the top node of the chain containing v
 *   position[v]-> index of v in the flattened array
 *
 * Build:
 *   1. DFS computes parent, depth, subtree size, and the heavy child
 *   2. a second DFS walks heavy children first, assigning contiguous positions
 *   3. a segment tree is built over the flattened values
 *
 * Path query(u, v):
 *   while u and v are on different chains:
 *     move the deeper chain head up, querying its whole chain segment
 *   then query the final segment between u and v on the same chain
 *
 * Complexity:
 *   build -> O(n)
 *   path query -> O(log^2 n)
 *   point update -> O(log n)
 *   LCA -> O(log n)
 *
 * HLD is the standard tool for path queries and point updates on a static
 * tree; with a lazy segment tree it also supports path updates. Link-cut trees
 * handle dynamic trees, and Euler-tour trees handle subtree updates, but HLD is
 * simpler and faster in practice for static trees.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class HeavyLight {
    int n;
    vector<vector<int>> tree;
    vector<int> parent;
    vector<int> depth;
    vector<int> subtree;
    vector<int> heavy;
    vector<int> head;
    vector<int> position;
    vector<int> values;
    vector<int> base;           // values in chain order
    vector<long long> segment;  // segment tree of sums

    int dfsSize(int node, int previous, int currentDepth) {
        parent[node] = previous;
        depth[node] = currentDepth;
        subtree[node] = 1;

        int bestSize = 0;

        for (int next : tree[node]) {
            if (next == previous) {
                continue;
            }

            subtree[node] += dfsSize(next, node, currentDepth + 1);

            if (subtree[next] > bestSize) {
                bestSize = subtree[next];
                heavy[node] = next;
            }
        }

        return subtree[node];
    }

    void decompose(int node, int chainHead) {
        head[node] = chainHead;
        position[node] = base.size();
        base.push_back(values[node]);

        // The heavy child continues the same chain.
        if (heavy[node] != -1) {
            decompose(heavy[node], chainHead);
        }

        // Every light child starts a new chain.
        for (int next : tree[node]) {
            if (next != parent[node] && next != heavy[node]) {
                decompose(next, next);
            }
        }
    }

    void buildSegment(int index, int left, int right) {
        if (left == right) {
            segment[index] = base[left];
            return;
        }

        int middle = (left + right) / 2;

        buildSegment(2 * index, left, middle);
        buildSegment(2 * index + 1, middle + 1, right);

        segment[index] = segment[2 * index] + segment[2 * index + 1];
    }

    long long querySegment(int index, int left, int right, int queryLeft,
                           int queryRight) {
        if (queryLeft > right || queryRight < left) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return segment[index];
        }

        int middle = (left + right) / 2;

        return querySegment(2 * index, left, middle, queryLeft, queryRight) +
               querySegment(2 * index + 1, middle + 1, right, queryLeft,
                            queryRight);
    }

    void updateSegment(int index, int left, int right, int target,
                       int newValue) {
        if (left == right) {
            segment[index] = newValue;
            return;
        }

        int middle = (left + right) / 2;

        if (target <= middle) {
            updateSegment(2 * index, left, middle, target, newValue);
        } else {
            updateSegment(2 * index + 1, middle + 1, right, target, newValue);
        }

        segment[index] = segment[2 * index] + segment[2 * index + 1];
    }

   public:
    HeavyLight(vector<vector<int>> &graph, vector<int> &nodeValues) {
        tree = graph;
        values = nodeValues;
        n = tree.size();

        parent.assign(n, -1);
        depth.assign(n, 0);
        subtree.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, 0);
        position.assign(n, 0);
        segment.assign(4 * n, 0);

        dfsSize(0, -1, 0);
        decompose(0, 0);
        buildSegment(1, 0, n - 1);
    }

    long long pathSum(int u, int v) {
        long long result = 0;

        while (head[u] != head[v]) {
            // Move the deeper chain head up, so the path does not jump over it.
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }

            result += querySegment(1, 0, n - 1, position[head[u]], position[u]);
            u = parent[head[u]];
        }

        // Same chain now: the remaining segment is contiguous.
        if (depth[u] > depth[v]) {
            swap(u, v);
        }

        result += querySegment(1, 0, n - 1, position[u], position[v]);
        return result;
    }

    void update(int node, int newValue) {
        values[node] = newValue;
        updateSegment(1, 0, n - 1, position[node], newValue);
    }

    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }
            u = parent[head[u]];
        }

        return depth[u] < depth[v] ? u : v;
    }

    int distance(int u, int v) {
        int ancestor = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[ancestor];
    }

    int chainCount() const {
        int chains = 0;

        for (int node = 0; node < n; node++) {
            if (head[node] == node) {
                chains++;
            }
        }

        return chains;
    }
};

void printPath(HeavyLight &hld, int u, int v) {
    cout << hld.pathSum(u, v) << " " << hld.lca(u, v) << " "
         << hld.distance(u, v) << endl;
}

int main() {
    //          0
    //        / | \
    //       1  2  3
    //      / \    |
    //     4   5   6
    //         |
    //         7
    vector<vector<int>> tree = {{1, 2, 3}, {0, 4, 5}, {0}, {0, 6},
                                {1},       {1, 7},    {3}, {5}};

    vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8};

    HeavyLight hld(tree, values);

    printPath(hld, 7, 6);  // 28 0 5  (8+6+2+1+4+7)
    printPath(hld, 4, 5);  // 13 1 2  (5+2+6)
    printPath(hld, 4, 4);  // 5 4 0
    printPath(hld, 2, 7);  // 20 0 4  (3+1+2+6+8)

    // Point update: node 5 becomes 60.
    hld.update(5, 60);
    printPath(hld, 4, 7);  // 75 1 3  (5+2+60+8)
    printPath(hld, 7, 6);  // 82 0 5  (8+60+2+1+4+7)

    cout << hld.chainCount() << endl;  // 4 chains: 0-1-5-7, 2, 3-6, 4

    // A path graph is one chain.
    vector<vector<int>> path = {{1}, {0, 2}, {1, 3}, {2}};
    vector<int> pathValues = {10, 20, 30, 40};
    HeavyLight pathHld(path, pathValues);
    cout << pathHld.chainCount() << endl;    // 1
    cout << pathHld.pathSum(0, 3) << endl;   // 100
    cout << pathHld.distance(0, 3) << endl;  // 3

    // A star has one heavy chain of length 2 and one chain per other child.
    vector<vector<int>> star = {{1, 2, 3, 4}, {0}, {0}, {0}, {0}};
    vector<int> starValues = {1, 2, 3, 4, 5};
    HeavyLight starHld(star, starValues);
    cout << starHld.chainCount() << endl;   // 4
    cout << starHld.pathSum(1, 4) << endl;  // 8 (2+1+5)
    cout << starHld.lca(1, 4) << endl;      // 0

    // Single node.
    vector<vector<int>> single = {{}};
    vector<int> singleValue = {42};
    HeavyLight singleHld(single, singleValue);
    cout << singleHld.pathSum(0, 0) << " " << singleHld.distance(0, 0) << endl;
    // 42 0

    return 0;
}

/**
 * Time complexity:
 *   build -> O(n) for the two DFS passes and the segment tree
 *   path query -> O(log^2 n): O(log n) chains, each range query O(log n)
 *   point update -> O(log n)
 *   LCA -> O(log n), at most O(log n) chain jumps
 * Space complexity -> O(n) for the tree, the arrays, and the segment tree
 *
 * Special cases:
 *   - a single node (no edges, query returns its value)
 *   - an empty tree is not supported; the constructor expects at least one node
 *   - a path graph (one chain, so a path query is one range query)
 *   - a star (many chains, but each path crosses at most three)
 *   - u == v (the final segment has length one)
 *   - a node value update (the segment tree and the values array stay in sync)
 *   - recursion depth: the DFS is O(height); a degenerate tree of 10^5 nodes
 *     needs an iterative build or a larger stack
 *
 * Why at most O(log n) chains: every time a path leaves a chain through a
 * light edge, the subtree size at least doubles, so there can be only
 * O(log n) light edges on any root-to-node path.
 *
 * HLD also supports path minimum/maximum, path assignment, subtree queries
 * (a subtree is a contiguous range because positions are assigned in DFS
 * order), and LCA. For subtree-only queries, an Euler tour with a Fenwick tree
 * is simpler; HLD is the choice when both paths and subtrees matter.
 */
