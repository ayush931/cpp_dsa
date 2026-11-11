/**
 * Euler path and Euler circuit
 * Euler path -> visits every edge exactly once
 * Euler circuit -> Euler path that starts and ends at the same vertex
 * A connected undirected graph has an Euler circuit if every vertex has even
 * degree It has an Euler path if exactly 0 or 2 vertices have odd degree
 *
 * Brute force -> try all edge orders with backtracking, exponential
 * Hierholzer -> O(V + E)
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

void hierholzer(vector<vector<int>> &graph, int start, vector<int> &path) {
    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int node = st.top();

        if (!graph[node].empty()) {
            int neighbour = graph[node].back();
            graph[node].pop_back();
            graph[neighbour].erase(
                find(graph[neighbour].begin(), graph[neighbour].end(), node));
            st.push(neighbour);
        } else {
            path.push_back(node);
            st.pop();
        }
    }

    reverse(path.begin(), path.end());
};

bool allEdgesReachable(vector<vector<int>> &graph, int start) {
    vector<bool> visited(graph.size(), false);
    vector<int> stack = {start};
    visited[start] = true;

    while (!stack.empty()) {
        int node = stack.back();
        stack.pop_back();

        for (int neighbour : graph[node]) {
            if (!visited[neighbour]) {
                visited[neighbour] = true;
                stack.push_back(neighbour);
            }
        }
    }

    for (int node = 0; node < (int)graph.size(); node++) {
        if (!graph[node].empty() && !visited[node]) {
            return false;
        }
    }

    return true;
};

int main() {
    // A triangle 0-1-2 with a tail 2-3. Only 2 and 3 have odd degree, so an
    // Euler path starts at 3 and ends at 2.
    int n = 4;
    vector<vector<int>> graph = {{1, 2}, {0, 2}, {0, 1, 3}, {2}};

    int oddDegree = 0;
    int start = 0;

    for (int i = 0; i < n; i++) {
        if (graph[i].size() % 2 != 0) {
            oddDegree++;
            start = i;
        }
    }

    if (oddDegree != 0 && oddDegree != 2) {
        cout << "No Euler path" << endl;
        return 0;
    }

    if (!allEdgesReachable(graph, start)) {
        cout << "No Euler path" << endl;
        return 0;
    }

    vector<int> path;
    hierholzer(graph, start, path);

    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;  // 3 2 0 1 2

    return 0;
}

// Special cases: disconnected graph, isolated vertices, self loops,
// directed graph (compare in-degree and out-degree), multiple edges
