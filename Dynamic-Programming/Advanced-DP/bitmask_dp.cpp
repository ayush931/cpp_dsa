/**
 * Bitmask DP -> use the bits of an integer as a state
 * Used when n is small (usually n <= 20) and every element can be used once
 * State -> dp[mask][last], where mask stores the visited elements
 *
 * Travelling Salesman Problem (TSP)
 * Find the minimum cost tour that visits every city once and returns to the
 * start.
 *
 * Brute force -> try all permutations, O(n!)
 * Optimized -> bitmask DP, O(2^n * n^2)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int tspBrute(vector<vector<int>> &dist) {
    int n = dist.size();
    vector<int> cities;

    for (int i = 1; i < n; i++) {
        cities.push_back(i);
    }

    int minimum = INT_MAX;

    do {
        int cost = 0;
        int current = 0;

        for (int city : cities) {
            cost += dist[current][city];
            current = city;
        }

        cost += dist[current][0];
        minimum = min(minimum, cost);
    } while (next_permutation(cities.begin(), cities.end()));

    return minimum;
};

int tsp(vector<vector<int>> &dist) {
    int n = dist.size();
    if (n <= 1) {
        return 0;
    }

    vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));
    dp[1][0] = 0;  // start at city 0, only city 0 is visited

    for (int mask = 1; mask < (1 << n); mask++) {
        for (int last = 0; last < n; last++) {
            if (!(mask & (1 << last)) || dp[mask][last] == INT_MAX) {
                continue;
            }

            for (int next = 0; next < n; next++) {
                if (mask & (1 << next)) {
                    continue;
                }

                int nextMask = mask | (1 << next);
                dp[nextMask][next] =
                    min(dp[nextMask][next], dp[mask][last] + dist[last][next]);
            }
        }
    }

    int fullMask = (1 << n) - 1;
    int minimum = INT_MAX;

    for (int last = 1; last < n; last++) {
        if (dp[fullMask][last] != INT_MAX) {
            minimum = min(minimum, dp[fullMask][last] + dist[last][0]);
        }
    }

    return minimum;
};

int main() {
    vector<vector<int>> dist = {
        {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

    cout << tspBrute(dist) << endl;  // 80
    cout << tsp(dist) << endl;       // 80

    return 0;
}

// Special cases: n = 0 or 1, disconnected graph, asymmetric distances
// Bitmask DP is not feasible for large n because 2^n grows very fast
