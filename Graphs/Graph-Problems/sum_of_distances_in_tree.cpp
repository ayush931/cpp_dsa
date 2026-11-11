/**
 * Sum of Distances in Tree (Rerooting DP)
 *
 * Return the sum of edge distances from every vertex to all other vertices.
 * Contract: an undirected tree on vertices 0..n-1; n=0 returns an empty list.
 *
 * First root at 0 and compute subtree sizes and the root's distance sum.
 * Moving the root from parent to child makes size[child] nodes one step
 * closer and all other nodes one step farther:
 *   answer[child] = answer[parent] + n - 2 * size[child].
 *
 * Two passes over an iterative traversal order avoid a deep recursion stack.
 * Time: O(n). Space: O(n). Prerequisites: tree traversal and subtree DP.
 */

#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

vector<long long> sumOfDistances(int n, const vector<array<int, 2>> &edges) {
    if (n < 0 || edges.size() != static_cast<size_t>(n > 0 ? n - 1 : 0)) {
        throw invalid_argument("a tree requires n vertices and n-1 edges");
    }
    if (n == 0) {
        return {};
    }

    vector<vector<int>> adjacent(n);
    for (auto [from, to] : edges) {
        if (from < 0 || from >= n || to < 0 || to >= n || from == to) {
            throw invalid_argument("invalid tree edge");
        }
        adjacent[from].push_back(to);
        adjacent[to].push_back(from);
    }

    vector<int> parent(n, -1), depth(n), order{0};
    parent[0] = 0;
    for (size_t index = 0; index < order.size(); ++index) {
        int node = order[index];
        for (int next : adjacent[node]) {
            if (parent[next] == -1) {
                parent[next] = node;
                depth[next] = depth[node] + 1;
                order.push_back(next);
            }
        }
    }
    if (static_cast<int>(order.size()) != n) {
        throw invalid_argument("tree must be connected");
    }

    vector<long long> subtreeSize(n, 1), answer(n);
    for (int node : order) {
        answer[0] += depth[node];
    }
    // Reverse traversal order guarantees children are processed before parents.
    for (int index = n - 1; index > 0; --index) {
        int node = order[index];
        subtreeSize[parent[node]] += subtreeSize[node];
    }
    for (int index = 1; index < n; ++index) {
        int node = order[index];
        answer[node] = answer[parent[node]] + n - 2 * subtreeSize[node];
    }
    return answer;
}

int main() {
    auto answer = sumOfDistances(6, {{0, 1}, {0, 2}, {2, 3}, {2, 4}, {2, 5}});
    assert((answer == vector<long long>{8, 12, 6, 10, 10, 10}));
    assert((sumOfDistances(1, {}) == vector<long long>{0}));
    assert((sumOfDistances(3, {{0, 1}, {1, 2}}) == vector<long long>{3, 2, 3}));
    assert(sumOfDistances(0, {}).empty());
    for (long long distance : answer) {
        cout << distance << ' ';
    }
    cout << '\n';  // 8 12 6 10 10 10
}
