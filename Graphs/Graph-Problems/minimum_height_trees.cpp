/**
 * Minimum Height Trees: roots minimizing tree height are its one or two
 * centers. Repeatedly peel off all current leaves until <=2 nodes remain.
 * O(n) time and space. Contract: a simple connected UNDIRECTED tree on
 * vertices 0..n-1 (not an arbitrary graph); n=0 returns no roots.
 * Tree center minimizes maximum distance; tree centroid balances component
 * sizes. These are different concepts.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>
using namespace std;

vector<int> minimumHeightRoots(int n, const vector<array<int, 2>> &edges) {
    if (n < 0) {
        throw invalid_argument("negative vertex count");
    }
    if (edges.size() != static_cast<size_t>(n > 0 ? n - 1 : 0)) {
        throw invalid_argument("tree requires n-1 edges");
    }
    if (n == 0) {
        return {};
    }
    vector<vector<int>> adjacent(n);
    vector<int> degree(n);
    for (auto [a, b] : edges) {
        if (a < 0 || a >= n || b < 0 || b >= n || a == b) {
            throw invalid_argument("invalid tree edge");
        }
        adjacent[a].push_back(b);
        adjacent[b].push_back(a);
        ++degree[a];
        ++degree[b];
    }
    // Connectivity plus n-1 undirected edges verifies the tree contract.
    vector<bool> seen(n);
    queue<int> traversal;
    traversal.push(0);
    seen[0] = true;
    int visited = 0;
    while (!traversal.empty()) {
        int node = traversal.front();
        traversal.pop();
        ++visited;
        for (int next : adjacent[node]) {
            if (!seen[next]) {
                seen[next] = true;
                traversal.push(next);
            }
        }
    }
    if (visited != n) {
        throw invalid_argument("tree must be connected");
    }
    if (n == 1) {
        return {0};
    }
    queue<int> leaves;
    for (int node = 0; node < n; ++node) {
        if (degree[node] == 1) {
            leaves.push(node);
        }
    }
    int remaining = n;
    while (remaining > 2) {
        int layerSize = static_cast<int>(leaves.size());
        remaining -= layerSize;
        while (layerSize--) {
            int leaf = leaves.front();
            leaves.pop();
            for (int next : adjacent[leaf]) {
                if (--degree[next] == 1) {
                    leaves.push(next);
                }
            }
        }
    }
    vector<int> roots;
    while (!leaves.empty()) {
        roots.push_back(leaves.front());
        leaves.pop();
    }
    sort(roots.begin(), roots.end());
    return roots;
}

int main() {
    assert((minimumHeightRoots(4, {{1, 0}, {1, 2}, {1, 3}}) == vector<int>{1}));
    assert(
        (minimumHeightRoots(4, {{0, 1}, {1, 2}, {2, 3}}) == vector<int>{1, 2}));
    assert((minimumHeightRoots(1, {}) == vector<int>{0}));
    assert(minimumHeightRoots(0, {}).empty());
    cout << minimumHeightRoots(4, {{1, 0}, {1, 2}, {1, 3}})[0] << '\n';  // 1
}
