/**
 * Strongly connected components (SCC)
 * An SCC is a maximal set of vertices where every vertex can reach every other
 * vertex
 *
 * Brute force -> for every pair check reachability, O(V * (V + E))
 * Kosaraju -> two DFS passes, O(V + E)
 * Tarjan -> one DFS pass with discovery and low values, O(V + E)
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void dfsOrder(vector<vector<int>> &graph, int node, vector<bool> &visited,
              stack<int> &order) {
    visited[node] = true;

    for (int neighbour : graph[node]) {
        if (!visited[neighbour]) {
            dfsOrder(graph, neighbour, visited, order);
        }
    }

    order.push(node);
};

void dfsComponent(vector<vector<int>> &reverseGraph, int node,
                  vector<bool> &visited) {
    visited[node] = true;
    cout << node << " ";

    for (int neighbour : reverseGraph[node]) {
        if (!visited[neighbour]) {
            dfsComponent(reverseGraph, neighbour, visited);
        }
    }
};

void kosaraju(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> order;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfsOrder(graph, i, visited, order);
        }
    }

    vector<vector<int>> reverseGraph(n);
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            reverseGraph[v].push_back(u);
        }
    }

    fill(visited.begin(), visited.end(), false);

    while (!order.empty()) {
        int node = order.top();
        order.pop();

        if (!visited[node]) {
            dfsComponent(reverseGraph, node, visited);
            cout << endl;
        }
    }
};

void tarjanDFS(vector<vector<int>> &graph, int node, int &timer,
               vector<int> &discovery, vector<int> &low, stack<int> &st,
               vector<bool> &inStack) {
    discovery[node] = low[node] = timer++;
    st.push(node);
    inStack[node] = true;

    for (int neighbour : graph[node]) {
        if (discovery[neighbour] == -1) {
            tarjanDFS(graph, neighbour, timer, discovery, low, st, inStack);
            low[node] = min(low[node], low[neighbour]);
        } else if (inStack[neighbour]) {
            low[node] = min(low[node], discovery[neighbour]);
        }
    }

    if (low[node] == discovery[node]) {
        while (true) {
            int current = st.top();
            st.pop();
            inStack[current] = false;
            cout << current << " ";
            if (current == node) {
                break;
            }
        }
        cout << endl;
    }
};

int main() {
    vector<vector<int>> graph = {{1}, {2}, {0, 3}, {4}, {3}};

    kosaraju(graph);
    cout << "---" << endl;

    int n = graph.size();
    vector<int> discovery(n, -1), low(n, -1);
    vector<bool> inStack(n, false);
    stack<int> st;
    int timer = 0;

    for (int i = 0; i < n; i++) {
        if (discovery[i] == -1) {
            tarjanDFS(graph, i, timer, discovery, low, st, inStack);
        }
    }

    return 0;
}

// Special cases: single vertex, disconnected graph, self loop,
// graph that is already one SCC, DAG (every vertex is its own SCC)
