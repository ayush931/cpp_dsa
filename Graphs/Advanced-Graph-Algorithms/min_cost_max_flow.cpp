/**
 * Minimum Cost Maximum Flow
 *
 * A flow network has edges with a capacity and a cost per unit of flow. The
 * goal is to send a given amount of flow from source to sink at minimum cost.
 *
 * Successive shortest augmenting paths:
 *   1. find the cheapest path from source to sink in the residual graph
 *   2. push as much flow as possible along it
 *   3. repeat until the requested flow is sent or no path remains
 *
 * With non-negative costs, Dijkstra finds the cheapest path. Residual edges
 * have negative costs, so the implementation uses Johnson potentials:
 *   reducedCost(u, v) = cost(u, v) + potential[u] - potential[v]
 * The reduced costs stay non-negative, so Dijkstra remains valid.
 *
 * Complexity -> O(F * E log V), where F is the flow. For unit-capacity
 * bipartite matching, that is O(V * E log V); a specialized matching algorithm
 * is faster.
 *
 * This implementation assumes non-negative edge costs. If the graph has
 * negative-cost edges, initialize the potentials with Bellman-Ford before the
 * first Dijkstra; that initialization is not included here. A negative cycle
 * reachable from the source would make the minimum cost unbounded.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int reverse;
    int capacity;
    int cost;
};

class MinCostMaxFlow {
    int nodeCount;
    vector<vector<Edge>> graph;

   public:
    explicit MinCostMaxFlow(int n) {
        nodeCount = n;
        graph.resize(n);
    }

    void addEdge(int from, int to, int capacity, int cost) {
        Edge forward{to, (int)graph[to].size(), capacity, cost};
        Edge backward{from, (int)graph[from].size(), 0, -cost};

        graph[from].push_back(forward);
        graph[to].push_back(backward);
    }

    // Returns {flow, cost}.
    pair<int, int> run(int source, int sink, int requestedFlow) {
        int flow = 0;
        int cost = 0;

        vector<int> potential(nodeCount, 0);

        while (flow < requestedFlow) {
            vector<int> distance(nodeCount, INT_MAX);
            vector<int> previousNode(nodeCount, -1);
            vector<int> previousEdge(nodeCount, -1);

            priority_queue<pair<int, int>, vector<pair<int, int>>,
                           greater<pair<int, int>>>
                pending;

            distance[source] = 0;
            pending.push({0, source});

            while (!pending.empty()) {
                int currentDistance = pending.top().first;
                int node = pending.top().second;
                pending.pop();

                if (currentDistance > distance[node]) {
                    continue;
                }

                for (int index = 0; index < graph[node].size(); index++) {
                    Edge &edge = graph[node][index];

                    if (edge.capacity == 0) {
                        continue;
                    }

                    int reducedCost =
                        edge.cost + potential[node] - potential[edge.to];
                    int nextDistance = currentDistance + reducedCost;

                    if (nextDistance < distance[edge.to]) {
                        distance[edge.to] = nextDistance;
                        previousNode[edge.to] = node;
                        previousEdge[edge.to] = index;
                        pending.push({nextDistance, edge.to});
                    }
                }
            }

            if (distance[sink] == INT_MAX) {
                break;  // no augmenting path
            }

            for (int node = 0; node < nodeCount; node++) {
                if (distance[node] != INT_MAX) {
                    potential[node] += distance[node];
                }
            }

            int add = requestedFlow - flow;

            for (int node = sink; node != source; node = previousNode[node]) {
                Edge &edge = graph[previousNode[node]][previousEdge[node]];
                add = min(add, edge.capacity);
            }

            for (int node = sink; node != source; node = previousNode[node]) {
                Edge &edge = graph[previousNode[node]][previousEdge[node]];

                edge.capacity -= add;
                graph[node][edge.reverse].capacity += add;
                cost += add * edge.cost;
            }

            flow += add;
        }

        return {flow, cost};
    }
};

int main() {
    // Assignment: 3 workers, 3 jobs. cost[i][j] is the cost of worker i doing
    // job j. A perfect assignment has flow 3.
    vector<vector<int>> cost = {{4, 1, 3}, {2, 0, 5}, {3, 2, 2}};

    int workers = 3;
    int source = 0;
    int leftStart = 1;
    int rightStart = leftStart + workers;
    int sink = rightStart + workers;

    MinCostMaxFlow assignment(sink + 1);

    for (int worker = 0; worker < workers; worker++) {
        assignment.addEdge(source, leftStart + worker, 1, 0);

        for (int job = 0; job < workers; job++) {
            assignment.addEdge(leftStart + worker, rightStart + job, 1,
                               cost[worker][job]);
        }
    }

    for (int job = 0; job < workers; job++) {
        assignment.addEdge(rightStart + job, sink, 1, 0);
    }

    pair<int, int> result = assignment.run(source, sink, workers);
    cout << result.first << " " << result.second << endl;
    // 3 5 (worker 0 -> job 1, worker 1 -> job 0, worker 2 -> job 2)

    // A small network with capacities and costs.
    // s=0, a=1, b=2, t=3
    MinCostMaxFlow network(4);
    network.addEdge(0, 1, 2, 1);
    network.addEdge(0, 2, 1, 2);
    network.addEdge(1, 3, 1, 3);
    network.addEdge(2, 3, 2, 1);

    pair<int, int> second = network.run(0, 3, 10);
    cout << second.first << " " << second.second << endl;
    // 2 7 (one unit through a, one through b)

    // Request more flow than the network can carry.
    MinCostMaxFlow limited(2);
    limited.addEdge(0, 1, 1, 5);

    pair<int, int> third = limited.run(0, 1, 10);
    cout << third.first << " " << third.second << endl;
    // 1 5

    // No path.
    MinCostMaxFlow disconnected(2);
    pair<int, int> fourth = disconnected.run(0, 1, 1);
    cout << fourth.first << " " << fourth.second << endl;
    // 0 0

    return 0;
}

/**
 * Time complexity -> O(F * E log V), where F is the amount of flow
 * Space complexity -> O(V + E) for the residual graph and the arrays
 *
 * Special cases:
 *   - no path from source to sink (flow 0)
 *   - requested flow larger than the maximum flow (returns the maximum)
 *   - zero-capacity edges (ignored)
 *   - zero-cost edges (allowed)
 *   - negative-cost edges are not supported; a Bellman-Ford initialization
 *     would be needed before the first Dijkstra
 *   - negative cycles (the minimum cost is unbounded)
 *   - parallel edges (the residual graph handles them)
 *
 * Applications: assignment, transportation, scheduling with costs, min-cost
 * bipartite matching, and network design. For unit-capacity bipartite
 * matching, Hopcroft-Karp is faster; min-cost flow is the tool when costs
 * matter.
 */
