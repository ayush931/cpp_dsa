/**
 * A graph is a collection of nodes where each node might or might be point to
 other nodes.
 * Two nodes represent real life entities and are connected by edges
 representing relationship between the nodes / entities.

 * Types of Graphs -> undirected and directed
 * Edge Weights -> Weighted graph and Unweighted graph
 */

/**
 * Multigraph:- an undirected graph in which multiple edges are allowed between
 * 2 nodes. Simple graph:- an undirected graph in which both multiple edges and
 * loops are not allowed. Complete graph:- a complete graph is the one in which
 * every node is directly connected to every other node. Connected graph:- In
 * this graph we have a direct or indirect path for every possible pair of
 * vertices. Path:- Path is a graph whose vertices can be arranged in a
 * sequence. Cycle:- a cycle cn is also a graph whose vertices can be arranged
 * in cyclic sequence. Degree:- Degree of a vertex in a graph is the total
 * number of edges incident to it or away from it. Indegree or Outdegree in
 * directed In directed graph, outdegree of a vertex is the total number of
 * outgoing edges & indegree is total number of incoming edges. Trees:- It is a
 * connected graph with no cycles. If we remove all cycles from a graph we get a
 * tree. If we remove an edge from a tree, it no more remains connected and
 * should be called forest. If there is a disconnected graph then the set of
 * vertices which are connected forms a connected component.
 */

/**
 * Facts:-
 * Tree |e| = |v| - 1
 * Forest |e| = |v| - 1 (max)
 * Connected |e| = |v| - 1 (min)
 * Undirected completed:- |E| = |v|C2  (max)
 * Directed completed: |E| = |v| x (|V| - 1)  (max)
 */

#include <bits/stdc++.h>
using namespace std;

vector<list<int>> graph;
int v;  // no of vertices

void add_edge(int src, int dest, bool bi_dir = true) {
    graph[src].push_back(dest);
    if (bi_dir) {
        graph[dest].push_back(src);
    }
};

void display() {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << "-> ";
        for (auto el : graph[i]) {
            cout << el << " , ";
        }
        cout << "\n";
    }
};

int main() {
    cin >> v;

    graph.resize(v, list<int>());

    int e;
    cin >> e;

    while (e--) {
        int s, d;
        cin >> s >> d;
        add_edge(s, d);
    }

    display();

    return 0;
}
