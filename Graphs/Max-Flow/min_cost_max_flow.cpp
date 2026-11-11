/**
 * Minimum Cost Maximum Flow (MCMF)
 *
 * Problem Definition:
 *   Given a directed flow network with source 's' and sink 't', where each
 *   edge (u, v) has both a capacity cap(u, v) and a unit cost cost(u, v).
 *   Find a flow of maximum magnitude from s to t that incurs the minimum
 *   possible total cost (sum of flow(e) * cost(e)).
 *
 * Algorithm — Successive Shortest Path using SPFA:
 *   1. Residual graph edges:
 *      - Forward edge: residual capacity = cap - flow, cost = +cost
 *      - Reverse edge: residual capacity = 0, cost = -cost
 *   2. In each iteration, find the shortest path from s to t with respect
 *      to edge costs in the residual graph using SPFA (handles negative costs).
 *   3. If no path from s to t exists with positive residual capacity,
 * terminate.
 *   4. Augment flow along the shortest path by the bottleneck capacity.
 *   5. Total cost increases by bottleneck * path_cost.
 *
 * Applications:
 *   - Bipartite matching with weights (Assignment Problem).
 *   - Transportation problem / minimum-cost supply-chain routing.
 *   - Network routing with link delay minimization.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class MinCostMaxFlow {
   public:
    struct Edge {
        int to;
        long long cap;
        long long flow;
        long long cost;
        int rev;  // index of reverse edge in adj[to]
    };

   private:
    vector<vector<Edge>> adj;
    vector<long long> dist;
    vector<int> parent_edge;
    vector<int> parent_node;
    vector<bool> in_queue;

    const long long INF = 1e15;

    // Shortest Path Faster Algorithm (SPFA) on residual costs
    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), INF);
        fill(in_queue.begin(), in_queue.end(), false);
        fill(parent_edge.begin(), parent_edge.end(), -1);
        fill(parent_node.begin(), parent_node.end(), -1);

        queue<int> q;
        dist[s] = 0;
        q.push(s);
        in_queue[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;

            for (size_t i = 0; i < adj[u].size(); ++i) {
                const Edge &e = adj[u][i];
                if (e.cap - e.flow > 0 && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    parent_node[e.to] = u;
                    parent_edge[e.to] = static_cast<int>(i);

                    if (!in_queue[e.to]) {
                        q.push(e.to);
                        in_queue[e.to] = true;
                    }
                }
            }
        }

        return dist[t] < INF;
    }

   public:
    MinCostMaxFlow(int vertices)
        : adj(vertices),
          dist(vertices),
          parent_edge(vertices),
          parent_node(vertices),
          in_queue(vertices) {}

    void addEdge(int from, int to, long long cap, long long cost) {
        Edge forward{to, cap, 0, cost, static_cast<int>(adj[to].size())};
        Edge backward{from, 0, 0, -cost, static_cast<int>(adj[from].size())};
        adj[from].push_back(forward);
        adj[to].push_back(backward);
    }

    // Returns pair of {max_flow, min_cost}
    pair<long long, long long> computeMCMF(int s, int t) {
        long long flow = 0;
        long long cost = 0;

        while (spfa(s, t)) {
            // Find bottleneck capacity along the shortest path
            long long bottleneck = INF;
            for (int curr = t; curr != s; curr = parent_node[curr]) {
                int p = parent_node[curr];
                int idx = parent_edge[curr];
                bottleneck =
                    min(bottleneck, adj[p][idx].cap - adj[p][idx].flow);
            }

            // Augment flow and accumulate cost
            for (int curr = t; curr != s; curr = parent_node[curr]) {
                int p = parent_node[curr];
                int idx = parent_edge[curr];
                adj[p][idx].flow += bottleneck;
                int rev_idx = adj[p][idx].rev;
                adj[curr][rev_idx].flow -= bottleneck;
            }

            flow += bottleneck;
            cost += bottleneck * dist[t];
        }

        return {flow, cost};
    }
};

int main() {
    cout << "=== Minimum Cost Maximum Flow (MCMF) Demo ===" << endl;

    // Example Network:
    // Source: 0, Sink: 3
    // Nodes: 0 (s), 1, 2, 3 (t)
    //
    // Edges (from, to, capacity, unit_cost):
    // 0 -> 1: cap 3, cost 1
    // 0 -> 2: cap 3, cost 4
    // 1 -> 2: cap 2, cost 1
    // 1 -> 3: cap 2, cost 6
    // 2 -> 3: cap 4, cost 2
    int n = 4;
    int s = 0, t = 3;
    MinCostMaxFlow mcmf(n);

    mcmf.addEdge(0, 1, 3, 1);
    mcmf.addEdge(0, 2, 3, 4);
    mcmf.addEdge(1, 2, 2, 1);
    mcmf.addEdge(1, 3, 2, 6);
    mcmf.addEdge(2, 3, 4, 2);

    auto [max_flow, min_cost] = mcmf.computeMCMF(s, t);

    cout << "Max Flow: " << max_flow << endl;
    cout << "Min Cost: " << min_cost << endl;

    // Breakdown of augmentations:
    // Augmentation 1: 0 -> 1 -> 2 -> 3 (cap 2, unit cost: 1 + 1 + 2 = 4) -> 2 *
    // 4 = 8 Augmentation 2: 0 -> 2 -> 3      (cap 2, unit cost: 4 + 2 = 6) -> 2
    // * 6 = 12 Augmentation 3: 0 -> 1 -> 3      (cap 1, unit cost: 1 + 6 = 7)
    // -> 1 * 7 = 7 Augmentation 4: 0 -> 2 -> 1 -> 3 (residual cancel, cost 4 -
    // 1 + 6 = 9) -> 1 * 9 = 9 Total flow = 2 + 2 + 1 + 1 = 6 Total cost = 8 +
    // 12 + 7 + 9 = 36
    cout << "Expected Flow: 6, Expected Cost: 36" << endl;

    return 0;
}

/**
 * Complexity:
 *   - Time Complexity: O(F * E * V) where F is max flow, V is vertices, E is
 * edges using SPFA. (Can be optimized to O(F * E log V) with Johnson's
 * potentials + Dijkstra).
 *   - Space Complexity: O(V + E) for residual graph and distance structures.
 */
