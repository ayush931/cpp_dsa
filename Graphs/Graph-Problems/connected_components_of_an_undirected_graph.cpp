/**
 * Connected Components of an Undirected Graph
 *
 * Input:
 *   v e
 *   e lines, each "u v" for an undirected edge
 *
 * Output: the number of connected components.
 *
 * Example:
 *   4 3
 *   1 0
 *   2 0
 *   3 1
 *   -> 1
 *
 * The DFS starts from every unvisited vertex, so isolated vertices and
 * disconnected components are counted correctly.
 *
 * Time: O(V + E). Space: O(V + E).
 */

#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>
using namespace std;

vector<list<int>> graph;
int v;  // no of vertices

void add_edge(int src, int dest, bool bi_dir = true) {
    graph[src].push_back(dest);

    if (bi_dir) {
        graph[dest].push_back(src);
    }
};

void dfs(int node, unordered_set<int> &visited) {
    visited.insert(node);
    for (auto neighbour : graph[node]) {
        if (not visited.count(neighbour)) {
            dfs(neighbour, visited);
        }
    }
};

int connected_components() {
    int result = 0;
    unordered_set<int> visited;
    for (int i = 0; i < v; i++) {
        // go to every vertex
        // if from a vertex, we can initialize a dfs, we got one more cc
        if (visited.count(i) == 0) {
            result++;
            dfs(i, visited);
        }
    }

    return result;
};

int main() {
    cin >> v;
    graph.resize(v, list<int>());

    int e;
    cin >> e;

    while (e--) {
        int s, d;
        cin >> s >> d;
        add_edge(s, d);  // the graph is undirected
    }

    cout << connected_components() << "\n";

    return 0;
};
