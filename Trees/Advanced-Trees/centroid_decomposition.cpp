/**
 * Centroid Decomposition of a Tree
 *
 * Centroid Decomposition is a divide-and-conquer technique on trees.
 *
 * Definition of a Centroid:
 *   A node 'c' in a tree of size N is a centroid if removing 'c' leaves no
 *   connected component with size strictly greater than N / 2.
 *   Every tree has at least one and at most two centroids.
 *
 * Centroid Tree (Decomposition Tree):
 *   1. Find the centroid 'c' of the current tree.
 *   2. Remove 'c' (mark as removed/decomposed).
 *   3. Recursively decompose each resulting subtree and make their centroids
 *      children of 'c' in the "Centroid Tree".
 *   4. The height of the centroid tree is guaranteed to be at most O(log N).
 *
 * Applications:
 *   - Path counting / queries: Count paths with length == K or <= K.
 *   - Nearest marked node: Update node color/state, query distance to closest
 * marked node.
 *   - Offline tree queries that depend on paths between all pairs of nodes.
 *
 * This implementation demonstrates:
 *   1. Finding tree centroids.
 *   2. Building the Centroid Tree with parent pointers.
 *   3. Counting the number of simple paths of length exactly K in the tree.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class CentroidDecomposition {
   private:
    vector<vector<int>> adj;
    vector<int> sub_size;
    vector<bool> removed;
    vector<int> centroid_parent;
    int centroid_root;

    // DFS to calculate sizes of subtrees in the remaining component
    void dfs_size(int u, int p) {
        sub_size[u] = 1;
        for (int v : adj[u]) {
            if (v == p || removed[v]) {
                continue;
            }
            dfs_size(v, u);
            sub_size[u] += sub_size[v];
        }
    }

    // Find the centroid of the component containing node u
    int get_centroid(int u, int p, int total_nodes) {
        for (int v : adj[u]) {
            if (v == p || removed[v]) {
                continue;
            }
            if (sub_size[v] > total_nodes / 2) {
                return get_centroid(v, u, total_nodes);
            }
        }
        return u;
    }

    // Recursive decomposition building the centroid tree
    int decompose(int u) {
        dfs_size(u, -1);
        int total_nodes = sub_size[u];
        int c = get_centroid(u, -1, total_nodes);
        removed[c] = true;

        for (int v : adj[c]) {
            if (!removed[v]) {
                int child_c = decompose(v);
                centroid_parent[child_c] = c;
            }
        }
        return c;
    }

    // Helper DFS to collect path lengths from the centroid
    void get_distances(int u, int p, int depth, vector<int> &depths) {
        depths.push_back(depth);
        for (int v : adj[u]) {
            if (v == p || removed[v]) {
                continue;
            }
            get_distances(v, u, depth + 1, depths);
        }
    }

    // Count paths of length K passing through the centroid c
    long long count_paths_through(int c, int k) {
        long long paths = 0;
        vector<int> depth_count(k + 1, 0);
        depth_count[0] = 1;  // path starting at c itself

        for (int v : adj[c]) {
            if (removed[v]) {
                continue;
            }
            vector<int> subtree_depths;
            get_distances(v, c, 1, subtree_depths);

            // Check pairs between this subtree and previously processed
            // subtrees
            for (int d : subtree_depths) {
                if (k - d >= 0 && k - d <= k) {
                    paths += depth_count[k - d];
                }
            }
            // Add current subtree depths into depth_count
            for (int d : subtree_depths) {
                if (d <= k) {
                    depth_count[d]++;
                }
            }
        }
        return paths;
    }

    // Recursive counting function
    long long solve_paths_of_length_k(int u, int k) {
        dfs_size(u, -1);
        int c = get_centroid(u, -1, sub_size[u]);

        long long ans = count_paths_through(c, k);
        removed[c] = true;

        for (int v : adj[c]) {
            if (!removed[v]) {
                ans += solve_paths_of_length_k(v, k);
            }
        }
        return ans;
    }

   public:
    CentroidDecomposition(int vertices)
        : adj(vertices),
          sub_size(vertices, 0),
          removed(vertices, false),
          centroid_parent(vertices, -1),
          centroid_root(-1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void buildCentroidTree() {
        fill(removed.begin(), removed.end(), false);
        fill(centroid_parent.begin(), centroid_parent.end(), -1);
        centroid_root = decompose(0);
    }

    int getCentroidRoot() const {
        return centroid_root;
    }

    int getCentroidParent(int u) const {
        return centroid_parent[u];
    }

    // Problem: Count total pairs (u, v) with u < v having distance exactly K
    long long countPairsWithDistanceK(int k) {
        fill(removed.begin(), removed.end(), false);
        return solve_paths_of_length_k(0, k);
    }
};

int main() {
    cout << "=== Centroid Decomposition Demo ===" << endl;

    // Build a tree with 7 nodes (0 to 6):
    //        0
    //       / \
    //      1   2
    //     / \   \
    //    3   4   5
    //             \
    //              6
    int n = 7;
    CentroidDecomposition cd(n);
    cd.addEdge(0, 1);
    cd.addEdge(0, 2);
    cd.addEdge(1, 3);
    cd.addEdge(1, 4);
    cd.addEdge(2, 5);
    cd.addEdge(5, 6);

    cd.buildCentroidTree();

    cout << "Centroid tree root: " << cd.getCentroidRoot() << endl;
    cout << "Centroid tree parent links:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "  Node " << i
             << " -> parent in centroid tree: " << cd.getCentroidParent(i)
             << endl;
    }

    // Count paths of length K
    for (int k = 1; k <= 4; ++k) {
        long long count = cd.countPairsWithDistanceK(k);
        cout << "Number of simple paths of length " << k << ": " << count
             << endl;
    }

    return 0;
}

/**
 * Complexity:
 *   - Tree height in Centroid Tree: O(log N)
 *   - Building Centroid Tree: O(N log N)
 *   - Path counting / queries: O(N log N) or O(N log^2 N) depending on inner
 * query
 *   - Space Complexity: O(N) auxiliary arrays
 */
