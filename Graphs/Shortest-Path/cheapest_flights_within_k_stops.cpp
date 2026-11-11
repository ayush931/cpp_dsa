/**
 * Cheapest Flights Within K Stops
 * Given n cities, flights (from, to, price), src, dst and k, return the
 * cheapest price from src to dst with at most k stops. Return -1 if impossible.
 *
 * Input: n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]],
 *        src = 0, dst = 3, k = 1
 * Output: 700
 *
 * Brute force -> Bellman-Ford style DP for k + 1 rounds, O(k * E)
 * Optimized -> Dijkstra with stops in the state, O(E * k log(V * k))
 */

#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int findCheapestPriceBellman(int n, vector<vector<int>> &flights, int src,
                             int dst, int k) {
    vector<int> distance(n, INT_MAX);
    distance[src] = 0;

    for (int i = 0; i <= k; i++) {
        vector<int> nextDistance = distance;

        for (auto &flight : flights) {
            int u = flight[0], v = flight[1], price = flight[2];

            if (distance[u] != INT_MAX &&
                distance[u] + price < nextDistance[v]) {
                nextDistance[v] = distance[u] + price;
            }
        }

        distance = nextDistance;
    }

    return (distance[dst] == INT_MAX) ? -1 : distance[dst];
};

int findCheapestPriceDijkstra(int n, vector<vector<int>> &flights, int src,
                              int dst, int k) {
    vector<vector<pair<int, int>>> graph(n);

    for (auto &flight : flights) {
        graph[flight[0]].push_back({flight[1], flight[2]});
    }

    // {cost, node, stops}
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    pq.push({0, src, 0});

    vector<vector<int>> best(n, vector<int>(k + 2, INT_MAX));

    while (!pq.empty()) {
        vector<int> current = pq.top();
        pq.pop();

        int cost = current[0];
        int node = current[1];
        int stops = current[2];

        if (node == dst) {
            return cost;
        }
        if (stops > k) {
            continue;
        }

        for (auto [neighbour, price] : graph[node]) {
            int newCost = cost + price;

            if (newCost < best[neighbour][stops + 1]) {
                best[neighbour][stops + 1] = newCost;
                pq.push({newCost, neighbour, stops + 1});
            }
        }
    }

    return -1;
};

int main() {
    vector<vector<int>> flights = {
        {0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}};

    cout << findCheapestPriceBellman(4, flights, 0, 3, 1) << endl;   // 700
    cout << findCheapestPriceDijkstra(4, flights, 0, 3, 1) << endl;  // 700

    return 0;
}

// Special cases: src == dst, k = 0, no path, cycles, negative prices (not
// allowed)
