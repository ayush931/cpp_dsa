/**
 * Kosaraju's Strongly Connected Components Algorithm
 *
 * A strongly connected component (SCC) is a maximal set of vertices in a
 * directed graph where every vertex can reach every other vertex in the set.
 *
 * Kosaraju's algorithm uses two DFS passes:
 *   1. DFS on the original graph and push vertices onto a stack when their
 *      exploration finishes.
 *   2. Pop vertices from the stack and DFS on the reversed graph. Each DFS
 *      tree is one SCC.
 *
 * Time:  O(V + E) for SCC discovery, plus O(V log V) to sort the output here.
 * Space: O(V + E) for the reversed graph and the DFS stack.
 *
 * A brute-force reachability implementation is included for small graphs.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void dfsFinishOrder(const vector<vector<int>> &graph, int node,
                    vector<bool> &visited, vector<int> &order) {
    visited[node] = true;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsFinishOrder(graph, neighbor, visited, order);
        }
    }

    order.push_back(node);
}

void dfsCollect(const vector<vector<int>> &graph, int node,
                vector<bool> &visited, vector<int> &component) {
    visited[node] = true;
    component.push_back(node);

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfsCollect(graph, neighbor, visited, component);
        }
    }
}

vector<vector<int>> kosaraju(const vector<vector<int>> &graph) {
    int vertices = static_cast<int>(graph.size());
    vector<bool> visited(vertices, false);
    vector<int> order;
    order.reserve(vertices);

    for (int vertex = 0; vertex < vertices; vertex++) {
        if (!visited[vertex]) {
            dfsFinishOrder(graph, vertex, visited, order);
        }
    }

    vector<vector<int>> reversed(vertices);
    for (int from = 0; from < vertices; from++) {
        for (int to : graph[from]) {
            reversed[to].push_back(from);
        }
    }

    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> components;

    for (int i = vertices - 1; i >= 0; i--) {
        int vertex = order[i];
        if (visited[vertex]) {
            continue;
        }

        vector<int> component;
        dfsCollect(reversed, vertex, visited, component);
        sort(component.begin(), component.end());
        components.push_back(component);
    }

    sort(components.begin(), components.end());
    return components;
}

void reachabilityDfs(const vector<vector<int>> &graph, int node,
                     vector<bool> &reachable) {
    reachable[node] = true;

    for (int neighbor : graph[node]) {
        if (!reachable[neighbor]) {
            reachabilityDfs(graph, neighbor, reachable);
        }
    }
}

vector<vector<int>> bruteForceScc(const vector<vector<int>> &graph) {
    int vertices = static_cast<int>(graph.size());
    vector<vector<bool>> reachable(vertices, vector<bool>(vertices, false));

    for (int source = 0; source < vertices; source++) {
        reachabilityDfs(graph, source, reachable[source]);
    }

    vector<bool> assigned(vertices, false);
    vector<vector<int>> components;

    for (int vertex = 0; vertex < vertices; vertex++) {
        if (assigned[vertex]) {
            continue;
        }

        vector<int> component;
        for (int other = 0; other < vertices; other++) {
            if (!assigned[other] && reachable[vertex][other] &&
                reachable[other][vertex]) {
                component.push_back(other);
            }
        }

        for (int member : component) {
            assigned[member] = true;
        }
        sort(component.begin(), component.end());
        components.push_back(component);
    }

    sort(components.begin(), components.end());
    return components;
}

void printComponents(const vector<vector<int>> &components) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(components.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "[";

        for (int j = 0; j < static_cast<int>(components[i].size()); j++) {
            if (j > 0) {
                cout << ", ";
            }
            cout << components[i][j];
        }

        cout << "]";
    }

    cout << "]";
}

void runTest(const vector<vector<int>> &graph) {
    vector<vector<int>> actual = kosaraju(graph);
    vector<vector<int>> expected = bruteForceScc(graph);

    cout << "SCCs: ";
    printComponents(actual);
    cout << "  matches brute force: " << (actual == expected ? "yes" : "no")
         << endl;
}

int main() {
    vector<vector<int>> graph = {
        {1},     // 0
        {2},     // 1
        {0, 3},  // 2
        {4},     // 3
        {3},     // 4
        {}       // 5
    };

    runTest(graph);

    vector<vector<int>> dag = {{1, 2}, {2}, {3}, {}};
    runTest(dag);

    vector<vector<int>> cycle = {{1}, {2}, {3}, {0}};
    runTest(cycle);

    vector<vector<int>> selfLoop = {{0}, {1}};
    runTest(selfLoop);

    runTest({});
    runTest({{}});

    return 0;
}

// Expected output:
// SCCs: [[0, 1, 2], [3, 4], [5]]  matches brute force: yes
// SCCs: [[0], [1], [2], [3]]  matches brute force: yes
// SCCs: [[0, 1, 2, 3]]  matches brute force: yes
// SCCs: [[0], [1]]  matches brute force: yes
// SCCs: []  matches brute force: yes
// SCCs: [[0]]  matches brute force: yes
//
// Special cases: empty graph, a single vertex, a DAG where every vertex is its
// own SCC, one large directed cycle, self-loops, and disconnected components.
