/**
 * 0-1 BFS
 *
 * Given a graph where every edge weight is either 0 or 1, find the shortest
 * distance from a source node to every other node.
 *
 * A normal Dijkstra uses a priority queue -> O((V + E) log V).
 * 0-1 BFS uses a deque:
 *   - weight 0 -> push to the front
 *   - weight 1 -> push to the back
 * This keeps the deque sorted by distance -> O(V + E).
 *
 * Example:
 * 0 --1--> 1
 * 0 --0--> 2
 * 2 --0--> 1
 * 1 --1--> 3
 * 2 --1--> 3
 * 3 --0--> 4
 *
 * Distances from 0: 0 0 0 1 1
 */

#include <climits>
#include <deque>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Edge {
   public:
    int to;
    int weight;

    Edge(int destination, int w) {
        to = destination;
        weight = w;
    }
};

vector<int> dijkstra(vector<vector<Edge>> &graph, int source) {
    int n = graph.size();
    vector<int> distance(n, INT_MAX);

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        minHeap;

    distance[source] = 0;
    minHeap.push({0, source});

    while (!minHeap.empty()) {
        int currentDistance = minHeap.top().first;
        int node = minHeap.top().second;
        minHeap.pop();

        if (currentDistance > distance[node]) {
            continue;
        }

        for (Edge &edge : graph[node]) {
            int nextDistance = currentDistance + edge.weight;

            if (nextDistance < distance[edge.to]) {
                distance[edge.to] = nextDistance;
                minHeap.push({nextDistance, edge.to});
            }
        }
    }

    return distance;
}

vector<int> zeroOneBfs(vector<vector<Edge>> &graph, int source) {
    int n = graph.size();
    vector<int> distance(n, INT_MAX);
    deque<int> dq;

    distance[source] = 0;
    dq.push_front(source);

    while (!dq.empty()) {
        int node = dq.front();
        dq.pop_front();

        for (Edge &edge : graph[node]) {
            int nextDistance = distance[node] + edge.weight;

            if (nextDistance >= distance[edge.to]) {
                continue;
            }

            distance[edge.to] = nextDistance;

            if (edge.weight == 0) {
                dq.push_front(edge.to);  // same distance, process first
            } else {
                dq.push_back(edge.to);  // one more distance, process later
            }
        }
    }

    return distance;
}

void print(const vector<int> &distance) {
    for (int value : distance) {
        if (value == INT_MAX) {
            cout << "INF ";
        } else {
            cout << value << " ";
        }
    }
    cout << endl;
}

int main() {
    vector<vector<Edge>> graph(5);

    graph[0].push_back(Edge(1, 1));
    graph[0].push_back(Edge(2, 0));
    graph[2].push_back(Edge(1, 0));
    graph[1].push_back(Edge(3, 1));
    graph[2].push_back(Edge(3, 1));
    graph[3].push_back(Edge(4, 0));

    vector<int> dijkstraDistance = dijkstra(graph, 0);
    vector<int> bfsDistance = zeroOneBfs(graph, 0);

    print(dijkstraDistance);  // 0 0 0 1 1
    print(bfsDistance);       // 0 0 0 1 1

    // Unreachable node
    vector<vector<Edge>> disconnected(3);
    disconnected[0].push_back(Edge(1, 0));
    print(zeroOneBfs(disconnected, 0));  // 0 0 INF

    return 0;
}

/**
 * Time complexity:
 *   Dijkstra -> O((V + E) log V)
 *   0-1 BFS -> O(V + E) time, O(V) space
 *
 * Special cases: source isolated (all other distances INF), self loops, a
 * zero-weight cycle (0-1 BFS still terminates because a node is relaxed only
 * when its distance improves), parallel edges, a graph with only weight-1
 * edges (behaves like BFS), a graph with only weight-0 edges.
 *
 * 0-1 BFS does not work for general non-negative weights; use Dijkstra then.
 */
