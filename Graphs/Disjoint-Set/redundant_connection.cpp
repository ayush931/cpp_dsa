/**
 * Redundant Connection: an undirected tree on vertices 1..n plus one extra
 * edge. In input order, the edge whose endpoints are already connected forms
 * the cycle. Return the last such edge (the removable edge latest in input).
 * O(E*alpha(n)) amortized time, O(n) space. No redundant edge -> nullopt.
 * This DSU rule is for UNDIRECTED graphs, not directed cycle detection.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <vector>
using namespace std;

optional<array<int, 2>> redundantConnection(
    int n, const vector<array<int, 2>> &edges) {
    if (n < 0) {
        throw invalid_argument("negative vertex count");
    }
    vector<int> parent(n + 1), size(n + 1, 1);
    iota(parent.begin(), parent.end(), 0);
    auto find = [&](int x) {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];  // path halving
            x = parent[x];
        }
        return x;
    };
    optional<array<int, 2>> answer;
    for (auto edge : edges) {
        if (edge[0] < 1 || edge[0] > n || edge[1] < 1 || edge[1] > n) {
            throw invalid_argument("vertex outside 1..n");
        }
        int a = find(edge[0]), b = find(edge[1]);
        if (a == b) {
            answer = edge;
            continue;
        }
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
    return answer;
}

int main() {
    auto edge = redundantConnection(3, {{1, 2}, {1, 3}, {2, 3}});
    assert((edge == array<int, 2>{2, 3}));
    assert(!redundantConnection(3, {{1, 2}, {2, 3}}));
    assert(!redundantConnection(0, {}));
    cout << (*edge)[0] << ' ' << (*edge)[1] << '\n';  // 2 3
}
