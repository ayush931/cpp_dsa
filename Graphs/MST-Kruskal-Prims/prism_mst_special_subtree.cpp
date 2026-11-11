/**
 * Prism (MST): Special SubTree
 * Given a graph which consists of several edges connecting its nodes, find a
 subgraph of the given graph with the following properties:
 * The subgaph contains all the nodes present in the original graph.
 * The subgraph is minimum overall weight (sum of all edges) among all the
 subgraph.
 * It is also required that there is exactly one, exclusive path between any two
 nodes of the subgraph.
 * One specific node S is fixed as the starting point of the subgraph using
 Prim's Algorithm.
 * Find the total weight or the sum of all the edges in the subgraph.

 * Example:
 * n = 3
 * edge = [[1, 2, 2], [2, 3, 2], [1, 3, 3]]
 * start = 1
 */

/**
 * Prim's Algorithm:- In prims we start with a source node and then try to
 * discover other nodes. At the start of the algo, the discovery weight of every
 * node except source is infinity, because we did not discovered anything
 */

/**
 * DS - visited set, priority_queue<pair>, unordered_map
 * insert the pair of <1,src> in the pq.
 * one by one remove the root element of the pq.
 * if the root element is already visited, then we will just continue
 * we mark the root visited
 * we store the weight of the pair in the ans
 * update the mapping
 * go to every neighbour of the current element, and only add those which are
 * non visited and have a better weight proposition.
 */

#include <bits/stdc++.h>
#define ll long long int
#define pp pair<int, int>
using namespace std;

vector<list<pp>> gr;

void add_edge(int u, int v, int wt, bool bidir = true) {
    gr[u].push_back({v, wt});
    if (bidir) {
        gr[v].push_back({u, wt});
    }
};

ll prims(int src, int n) {
    priority_queue<pp, vector<pp>, greater<pp>> pq;  // {wt, node}
    unordered_set<int> vis;
    vector<int> par(n + 1);
    unordered_map<int, int> mp;

    for (int i = 1; i <= n; i++) {
        mp[i] = INT_MAX;
    }

    pq.push({0, src});
    mp[src] = 0;
    int total_count = 0;  // 0 -> n - 1 edges
    int result = 0;       // sum of wts

    while (total_count < n and !pq.empty()) {
        pp curr = pq.top();
        if (vis.count(curr.second)) {
            pq.pop();
            continue;
        }
        vis.insert(curr.second);
        total_count++;
        result += curr.first;
        pq.pop();

        for (auto neighbour : gr[curr.second]) {
            if (!vis.count(neighbour.first) and
                mp[neighbour.first] > neighbour.second) {
                pq.push({neighbour.second, neighbour.first});
                par[neighbour.first] = curr.second;
                mp[neighbour.first] = neighbour.second;
            }
        }
    }

    // The graph may be disconnected; in that case Prim's algorithm cannot
    // reach every vertex and the partial result is not a spanning tree.
    if (total_count != n) {
        return -1;
    }

    return result;
};

int main() {
    int n, m;
    cin >> n >> m;

    gr.resize(n + 1, list<pp>());

    while (m--) {
        int u, v, wt;
        cin >> u >> v >> wt;
        add_edge(u, v, wt);
    }

    int src;
    cin >> src;

    cout << prims(src, n) << "\n";

    return 0;
}