/**
 * BFS (Breadth First Search) -> visits nodes level by level
 * Uses a queue
 * Time -> O(V + E), Space -> O(V)
 * Finds the shortest path in an unweighted graph
 * Does not work directly for weighted graphs
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<int> bfs(vector<vector<int>> &graph, int source) {
    int n = graph.size();
    vector<int> distance(n, -1);

    queue<int> q;
    q.push(source);
    distance[source] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbour : graph[node]) {
            if (distance[neighbour] == -1) {
                distance[neighbour] = distance[node] + 1;
                q.push(neighbour);
            }
        }
    }

    return distance;
};

int main() {
    vector<vector<int>> graph = {{1, 2}, {0, 3, 4}, {0, 4},
                                 {1, 5}, {1, 2, 5}, {3, 4}};

    vector<int> distance = bfs(graph, 0);

    for (int i = 0; i < distance.size(); i++) {
        cout << i << " -> " << distance[i] << endl;
    }

    return 0;
}

// Special cases: disconnected graph, source out of range, self loops, parallel
// edges For multiple components, run BFS from every unvisited node
