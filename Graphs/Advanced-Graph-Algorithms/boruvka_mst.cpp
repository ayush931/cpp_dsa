/**
 * Boruvka's Minimum Spanning Tree Algorithm
 *
 * Boruvka's algorithm builds an MST in rounds. In every round, each current
 * component chooses its cheapest outgoing edge. All chosen edges are then
 * added, merging components. The number of components at least halves each
 * round, so there are O(log V) rounds.
 *
 * Time: O(E log V) with a disjoint-set structure.
 *
 * The algorithm also produces a minimum spanning forest when the graph is
 * disconnected. Kruskal's algorithm is included as a reference implementation.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct Edge {
    int id;
    int from;
    int to;
    int weight;
};

class DisjointSet {
   private:
    vector<int> parent;
    vector<int> rank;
    int components;

   public:
    explicit DisjointSet(int n) : parent(n), rank(n, 0), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int node) {
        if (parent[node] == node) {
            return node;
        }
        return parent[node] = find(parent[node]);
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) {
            return false;
        }

        if (rank[rootA] < rank[rootB]) {
            swap(rootA, rootB);
        }
        parent[rootB] = rootA;
        if (rank[rootA] == rank[rootB]) {
            rank[rootA]++;
        }

        components--;
        return true;
    }

    int count() const {
        return components;
    }
};

bool cheaper(const Edge &a, const Edge &b) {
    if (a.weight != b.weight) {
        return a.weight < b.weight;
    }
    return a.id < b.id;
}

struct MSTResult {
    long long weight;
    vector<Edge> edges;
    bool connected;
};

MSTResult boruvka(int vertices, const vector<Edge> &edges) {
    DisjointSet dsu(vertices);
    MSTResult result{0, {}, false};

    while (dsu.count() > 1) {
        vector<int> cheapest(vertices, -1);

        for (int i = 0; i < static_cast<int>(edges.size()); i++) {
            const Edge &edge = edges[i];
            int rootFrom = dsu.find(edge.from);
            int rootTo = dsu.find(edge.to);

            if (rootFrom == rootTo) {
                continue;
            }

            if (cheapest[rootFrom] == -1 ||
                cheaper(edge, edges[cheapest[rootFrom]])) {
                cheapest[rootFrom] = i;
            }

            if (cheapest[rootTo] == -1 ||
                cheaper(edge, edges[cheapest[rootTo]])) {
                cheapest[rootTo] = i;
            }
        }

        bool added = false;

        for (int component = 0; component < vertices; component++) {
            if (cheapest[component] == -1) {
                continue;
            }

            const Edge &edge = edges[cheapest[component]];

            if (dsu.unite(edge.from, edge.to)) {
                result.weight += edge.weight;
                result.edges.push_back(edge);
                added = true;
            }
        }

        if (!added) {
            break;
        }
    }

    result.connected = (dsu.count() == 1);
    sort(result.edges.begin(), result.edges.end(),
         [](const Edge &a, const Edge &b) { return a.id < b.id; });
    return result;
}

MSTResult kruskal(int vertices, vector<Edge> edges) {
    sort(edges.begin(), edges.end(), cheaper);

    DisjointSet dsu(vertices);
    MSTResult result{0, {}, false};

    for (const Edge &edge : edges) {
        if (dsu.unite(edge.from, edge.to)) {
            result.weight += edge.weight;
            result.edges.push_back(edge);
        }
    }

    result.connected = (dsu.count() == 1);
    return result;
}

void printEdges(const vector<Edge> &edges) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(edges.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << edges[i].id << "(" << edges[i].from << "-" << edges[i].to << ":"
             << edges[i].weight << ")";
    }

    cout << "]";
}

void runTest(int vertices, const vector<Edge> &edges) {
    MSTResult boruvkaResult = boruvka(vertices, edges);
    MSTResult kruskalResult = kruskal(vertices, edges);

    cout << "vertices: " << vertices << ", edges: " << edges.size() << endl;
    cout << "Boruvka weight: " << boruvkaResult.weight << "  edges: ";
    printEdges(boruvkaResult.edges);
    cout << endl;
    cout << "Kruskal weight: " << kruskalResult.weight
         << "  connected: " << (kruskalResult.connected ? "yes" : "no") << endl;
    cout << "same weight: "
         << (boruvkaResult.weight == kruskalResult.weight ? "yes" : "no")
         << endl;
    cout << "---" << endl;
}

int main() {
    vector<Edge> connected = {{0, 0, 1, 4}, {1, 0, 2, 3}, {2, 1, 2, 1},
                              {3, 1, 3, 2}, {4, 2, 3, 4}, {5, 3, 4, 2},
                              {6, 2, 4, 5}};

    vector<Edge> disconnected = {{0, 0, 1, 1}, {1, 1, 2, 2}, {2, 3, 4, 3}};

    vector<Edge> duplicateEdges = {
        {0, 0, 1, 5}, {1, 0, 1, 5}, {2, 1, 2, 5}, {3, 0, 2, 9}};

    vector<Edge> negativeWeights = {{0, 0, 1, -2}, {1, 1, 2, -3}, {2, 0, 2, 4}};

    runTest(5, connected);
    runTest(5, disconnected);
    runTest(3, duplicateEdges);
    runTest(3, negativeWeights);
    runTest(1, {});

    return 0;
}

// Expected output (abridged):
// vertices: 5, edges: 7
// Boruvka weight: 8  edges: [1(0-2:3), 2(1-2:1), 3(1-3:2), 5(3-4:2)]
// Kruskal weight: 8  connected: yes
// same weight: yes
// ---
// vertices: 5, edges: 3
// Boruvka weight: 6  edges: [0(0-1:1), 1(1-2:2), 2(3-4:3)]
// Kruskal weight: 6  connected: no
// same weight: yes
// ---
//
// Special cases: disconnected graph (minimum spanning forest), single vertex,
// duplicate edges with equal weights, negative edge weights, and parallel
// edges between the same pair of vertices.
