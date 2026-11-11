/**
 * Floyd-Warshall -> all pairs shortest paths
 * Try every node k as an intermediate node between i and j
 * Time -> O(V^3), Space -> O(V^2)
 * Works with negative edges, but not with negative cycles
 * A negative cycle exists if distance[i][i] < 0 for any i
 */

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

const int INF = 1e9;

vector<vector<int>> floydWarshall(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<vector<int>> distance = graph;

    for (int i = 0; i < n; i++) {
        distance[i][i] = 0;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distance[i][k] != INF && distance[k][j] != INF) {
                    distance[i][j] =
                        min(distance[i][j], distance[i][k] + distance[k][j]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (distance[i][i] < 0) {
            cout << "Negative weight cycle detected" << endl;
            return {};
        }
    }

    return distance;
};

int main() {
    int n = 4;
    vector<vector<int>> graph(n, vector<int>(n, INF));

    graph[0][1] = 3;
    graph[0][2] = 6;
    graph[1][0] = 3;
    graph[1][2] = 2;
    graph[1][3] = 1;
    graph[2][0] = 6;
    graph[2][1] = 2;
    graph[2][3] = 4;
    graph[3][1] = 1;
    graph[3][2] = 4;

    vector<vector<int>> distance = floydWarshall(graph);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (distance[i][j] == INF) {
                cout << "INF ";
            } else {
                cout << distance[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}

// Special cases: disconnected graph (INF), negative edges, negative cycle,
// large weights (use long long), self loops
