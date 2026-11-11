/**
 * Tree -> graph having no cycle
 * Spanning -> subgraph having all the vertices but minimum number of edges
 * availabe to keep it connected
 */

/**
 * MST(minimum spanning tree):-
 * Tree -> no cycle
 * includes all the nodes of the graph
 * sum of the edge weight is minimum
 */

/**
 * Kruskal's Algorithm:- One by one keep a picky edge with minimum weight.
 * If choosing an edge forms a cycle avoid it, else use it.
 */

/**
 * Given an undirected weighted connected graph, find the Really Special SubTree
 in it. The Really Special SubTree is defined as a subgraph consisting of all
 the nodes in the graph and:
 * There is only one exclusive path from a node to every other node.
 * The subgraph is of minimum overall weight(sum of all edges) among all such
 subgraph.
 * No cycles are formed.

 * To create a Really Special SubTree, always pick the edge with smallest
 weight. Determine if including it will create a cycle. If so, ignore the edge.
 If there are edges of equal weight available:
 * Choose the edge that minimize the sum u + v + wt where u and v are vertices
 and wt is the edge weight.
 * If there is still a collision, choose any of them.
 * Print the overall weight of the tree formed using the rules.
 */

#include <bits/stdc++.h>
#define ll long long int
using namespace std;

int find(vector<int> &parent, int a) {
    return parent[a] = ((parent[a] == a) ? a : find(parent, parent[a]));
};

void Union(vector<int> &par, vector<int> &rank, int a, int b) {
    a = find(par, a);
    b = find(par, b);

    if (a == b) {
        return;
    }

    if (rank[a] >= rank[b]) {
        rank[a]++;
        par[b] = a;
    } else {
        rank[b]++;
        par[a] = b;
    }
};

struct Edge {
    int src;
    int dest;
    int wt;
};

bool cmp(Edge e1, Edge e2) {
    return e1.wt < e2.wt;
};

ll kruskals(vector<Edge> &input, int n, int e) {
    sort(input.begin(), input.end(), cmp);
    vector<int> parent(n + 1);
    vector<int> rank(n + 1, 1);

    for (int i = 0; i <= n; i++) {
        parent[i] = i;
    }

    int edgeCount = 0;  // n - 1
    int i = 0;
    ll ans = 0;

    while (edgeCount < n - 1 and i < input.size()) {
        Edge curr =
            input[i];  // because input is stored so we will get min wt edge
        int srcPar = find(parent, curr.src);
        int destPar = find(parent, curr.dest);

        if (srcPar != destPar) {
            // include edge as this will not make cycle
            Union(parent, rank, srcPar, destPar);
            ans += curr.wt;
            edgeCount++;
        }

        i++;  // does not matter u picked the last edge or not, we still need to
              // go to next edge
    }

    // A spanning tree needs exactly n - 1 edges. If fewer were selected, the
    // input graph is disconnected and no spanning tree exists.
    if (edgeCount != n - 1) {
        return -1;
    }

    return ans;
};

int main() {
    int n, e;
    cin >> n >> e;
    vector<Edge> v(e);

    for (int i = 0; i < e; i++) {
        cin >> v[i].src >> v[i].dest >> v[i].wt;
    }

    cout << kruskals(v, n, e) << endl;
    return 0;
}