/**
 * Dinic algorithm -> maximum flow in O(V^2 * E)
 * 1. Build a level graph with BFS
 * 2. Send blocking flow with DFS, only moving from level i to level i + 1
 * 3. Repeat until the sink is unreachable
 * Faster than Edmonds-Karp for dense graphs and unit networks
 */

#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int reverseIndex;
    int capacity;
};

class Dinic {
    int n;
    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> nextEdge;

    bool buildLevelGraph(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;

        level[source] = 0;
        q.push(source);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (Edge &edge : graph[node]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[node] + 1;
                    q.push(edge.to);
                }
            }
        }

        return level[sink] != -1;
    }

    int sendFlow(int node, int sink, int flow) {
        if (node == sink) {
            return flow;
        }

        for (int &i = nextEdge[node]; i < graph[node].size(); i++) {
            Edge &edge = graph[node][i];

            if (edge.capacity > 0 && level[edge.to] == level[node] + 1) {
                int pushed = sendFlow(edge.to, sink, min(flow, edge.capacity));

                if (pushed > 0) {
                    edge.capacity -= pushed;
                    graph[edge.to][edge.reverseIndex].capacity += pushed;
                    return pushed;
                }
            }
        }

        return 0;
    }

   public:
    Dinic(int size) {
        n = size;
        graph.resize(n);
        level.resize(n);
        nextEdge.resize(n);
    }

    void addEdge(int from, int to, int capacity) {
        Edge forward = {to, (int)graph[to].size(), capacity};
        Edge backward = {from, (int)graph[from].size(), 0};

        graph[from].push_back(forward);
        graph[to].push_back(backward);
    }

    int maxFlow(int source, int sink) {
        if (source == sink) {
            return 0;
        }

        int flow = 0;

        while (buildLevelGraph(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);

            while (int pushed = sendFlow(source, sink, INT_MAX)) {
                flow += pushed;
            }
        }

        return flow;
    }
};

int main() {
    Dinic dinic(6);

    dinic.addEdge(0, 1, 16);
    dinic.addEdge(0, 2, 13);
    dinic.addEdge(1, 2, 10);
    dinic.addEdge(2, 1, 4);
    dinic.addEdge(1, 3, 12);
    dinic.addEdge(3, 2, 9);
    dinic.addEdge(2, 4, 14);
    dinic.addEdge(4, 3, 7);
    dinic.addEdge(3, 5, 20);
    dinic.addEdge(4, 5, 4);

    cout << dinic.maxFlow(0, 5) << endl;  // 23
    cout << dinic.maxFlow(0, 0) << endl;  // 0 (source == sink)

    return 0;
}

// Special cases: source == sink, disconnected graph, zero capacity edges,
// parallel edges, undirected edges (add two directed edges with capacity)
