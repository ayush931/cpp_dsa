/**
 * Bellman-Ford -> single source shortest path that supports negative edge
 * weights Relax every edge V - 1 times Time -> O(V * E), Space -> O(V) Detects
 * a negative weight cycle if an edge can still be relaxed after V - 1 rounds
 * Dijkstra does not work with negative edges; Bellman-Ford does
 */

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int source;
    int destination;
    int weight;
};

vector<int> bellmanFord(int n, vector<Edge> &edges, int source) {
    vector<int> distance(n, INT_MAX);
    distance[source] = 0;

    for (int i = 0; i < n - 1; i++) {
        bool updated = false;

        for (Edge &edge : edges) {
            if (distance[edge.source] != INT_MAX &&
                distance[edge.source] + edge.weight <
                    distance[edge.destination]) {
                distance[edge.destination] =
                    distance[edge.source] + edge.weight;
                updated = true;
            }
        }

        if (!updated) {
            break;  // no further relaxation possible
        }
    }

    // check for a negative cycle
    for (Edge &edge : edges) {
        if (distance[edge.source] != INT_MAX &&
            distance[edge.source] + edge.weight < distance[edge.destination]) {
            cout << "Negative weight cycle detected" << endl;
            return {};
        }
    }

    return distance;
};

int main() {
    int n = 5;
    vector<Edge> edges = {{0, 1, 6}, {0, 2, 7},  {1, 2, 8},
                          {1, 3, 5}, {1, 4, -4}, {2, 3, -3},
                          {2, 4, 9}, {3, 1, -2}, {4, 3, 7}};

    vector<int> distance = bellmanFord(n, edges, 0);

    for (int i = 0; i < distance.size(); i++) {
        cout << i << " -> " << distance[i] << endl;
    }

    return 0;
}

// Special cases: unreachable nodes (INT_MAX), negative cycles, source out of
// range, multiple edges between the same pair, self loops
