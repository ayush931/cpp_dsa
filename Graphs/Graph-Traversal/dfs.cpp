/**
 * DFS (Depth First Search) -> visits a node and goes as deep as possible
 * before backtracking
 * Recursive DFS uses the call stack, iterative DFS uses an explicit stack
 * Time -> O(V + E), Space -> O(V)
 * Used for paths, cycles, topological sort, SCC and backtracking
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void dfsRecursive(vector<vector<int>> &graph, int node, vector<bool> &visited) {
    visited[node] = true;
    cout << node << " ";

    for (int neighbour : graph[node]) {
        if (!visited[neighbour]) {
            dfsRecursive(graph, neighbour, visited);
        }
    }
};

void dfsIterative(vector<vector<int>> &graph, int source) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> st;

    st.push(source);

    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (visited[node]) {
            continue;
        }

        visited[node] = true;
        cout << node << " ";

        for (int i = graph[node].size() - 1; i >= 0; i--) {
            int neighbour = graph[node][i];
            if (!visited[neighbour]) {
                st.push(neighbour);
            }
        }
    }
};

int main() {
    vector<vector<int>> graph = {{1, 2}, {0, 3, 4}, {0, 4},
                                 {1, 5}, {1, 2, 5}, {3, 4}};

    vector<bool> visited(graph.size(), false);

    dfsRecursive(graph, 0, visited);
    cout << endl;

    dfsIterative(graph, 0);
    cout << endl;

    return 0;
}

// Special cases: disconnected graph, deep recursion (stack overflow), cycles
// Iterative DFS avoids recursion depth limits
