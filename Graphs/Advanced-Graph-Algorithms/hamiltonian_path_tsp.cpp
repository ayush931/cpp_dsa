/**
 * Hamiltonian Path and the Travelling Salesperson Problem (TSP)
 *
 * Hamiltonian path: a path that visits every vertex exactly once.
 * TSP: a minimum-weight Hamiltonian cycle that returns to the start.
 *
 * Both problems are NP-hard in general. For small n they can be solved exactly
 * with bitmask dynamic programming:
 *
 *   dp[mask][last] = best cost of a path that visits exactly `mask` and ends
 *                    at vertex `last`
 *
 * There are 2^n * n states and n transitions per state, so the running time is
 * O(n^2 * 2^n). A brute-force permutation search is included for n <= 8 and is
 * used to verify the DP.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

bool hamiltonianPath(int vertices, const vector<vector<bool>> &adjacent,
                     vector<int> &path) {
    path.clear();
    if (vertices == 0) {
        return false;
    }

    int fullMask = (1 << vertices) - 1;
    vector<vector<char>> reachable(1 << vertices, vector<char>(vertices, 0));
    vector<vector<int>> parent(1 << vertices, vector<int>(vertices, -1));

    for (int vertex = 0; vertex < vertices; vertex++) {
        reachable[1 << vertex][vertex] = 1;
    }

    for (int mask = 1; mask <= fullMask; mask++) {
        for (int last = 0; last < vertices; last++) {
            if (!reachable[mask][last]) {
                continue;
            }

            for (int next = 0; next < vertices; next++) {
                if (mask & (1 << next)) {
                    continue;
                }
                if (!adjacent[last][next]) {
                    continue;
                }

                int nextMask = mask | (1 << next);
                if (!reachable[nextMask][next]) {
                    reachable[nextMask][next] = 1;
                    parent[nextMask][next] = last;
                }
            }
        }
    }

    int last = -1;
    for (int vertex = 0; vertex < vertices; vertex++) {
        if (reachable[fullMask][vertex]) {
            last = vertex;
            break;
        }
    }

    if (last == -1) {
        return false;
    }

    int mask = fullMask;
    while (last != -1) {
        path.push_back(last);
        int previous = parent[mask][last];
        mask ^= (1 << last);
        last = previous;
    }

    reverse(path.begin(), path.end());
    return true;
}

long long travellingSalesperson(int vertices,
                                const vector<vector<long long>> &cost,
                                vector<int> &tour) {
    tour.clear();
    if (vertices == 0) {
        return 0;
    }
    if (vertices == 1) {
        tour = {0};
        return 0;
    }

    int fullMask = (1 << vertices) - 1;
    vector<vector<long long>> dp(1 << vertices,
                                 vector<long long>(vertices, INF));
    vector<vector<int>> parent(1 << vertices, vector<int>(vertices, -1));

    dp[1][0] = 0;

    for (int mask = 1; mask <= fullMask; mask++) {
        for (int last = 0; last < vertices; last++) {
            if (dp[mask][last] == INF) {
                continue;
            }

            for (int next = 0; next < vertices; next++) {
                if (mask & (1 << next)) {
                    continue;
                }
                if (cost[last][next] == INF) {
                    continue;
                }

                int nextMask = mask | (1 << next);
                long long candidate = dp[mask][last] + cost[last][next];

                if (candidate < dp[nextMask][next]) {
                    dp[nextMask][next] = candidate;
                    parent[nextMask][next] = last;
                }
            }
        }
    }

    long long best = INF;
    int last = -1;

    for (int vertex = 1; vertex < vertices; vertex++) {
        if (dp[fullMask][vertex] == INF) {
            continue;
        }
        if (cost[vertex][0] == INF) {
            continue;
        }

        long long candidate = dp[fullMask][vertex] + cost[vertex][0];
        if (candidate < best) {
            best = candidate;
            last = vertex;
        }
    }

    if (last == -1) {
        return INF;
    }

    int mask = fullMask;
    while (last != -1) {
        tour.push_back(last);
        int previous = parent[mask][last];
        mask ^= (1 << last);
        last = previous;
    }

    reverse(tour.begin(), tour.end());
    tour.push_back(0);
    return best;
}

bool bruteHamiltonianPath(int vertices, const vector<vector<bool>> &adjacent) {
    if (vertices == 0) {
        return false;
    }

    vector<int> order(vertices);
    iota(order.begin(), order.end(), 0);

    do {
        bool valid = true;
        for (int i = 0; i + 1 < vertices; i++) {
            if (!adjacent[order[i]][order[i + 1]]) {
                valid = false;
                break;
            }
        }
        if (valid) {
            return true;
        }
    } while (next_permutation(order.begin(), order.end()));

    return false;
}

long long bruteTsp(int vertices, const vector<vector<long long>> &cost) {
    if (vertices <= 1) {
        return 0;
    }

    vector<int> order(vertices - 1);
    iota(order.begin(), order.end(), 1);

    long long best = INF;

    do {
        long long total = 0;
        int previous = 0;
        bool valid = true;

        for (int vertex : order) {
            if (cost[previous][vertex] == INF) {
                valid = false;
                break;
            }
            total += cost[previous][vertex];
            previous = vertex;
        }

        if (valid && cost[previous][0] != INF) {
            best = min(best, total + cost[previous][0]);
        }
    } while (next_permutation(order.begin(), order.end()));

    return best;
}

void printPath(const vector<int> &path) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(path.size()); i++) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << path[i];
    }

    cout << "]";
}

int main() {
    int vertices = 4;
    vector<vector<bool>> adjacent(vertices, vector<bool>(vertices, false));

    adjacent[0][1] = adjacent[1][0] = true;
    adjacent[1][2] = adjacent[2][1] = true;
    adjacent[2][3] = adjacent[3][2] = true;

    vector<int> path;
    bool hasPath = hamiltonianPath(vertices, adjacent, path);

    cout << "path graph has Hamiltonian path: " << (hasPath ? "yes" : "no")
         << endl;
    cout << "path: ";
    printPath(path);
    cout << endl;
    cout << "brute force agrees: "
         << (hasPath == bruteHamiltonianPath(vertices, adjacent) ? "yes" : "no")
         << endl;

    vector<vector<bool>> disconnected(4, vector<bool>(4, false));
    disconnected[0][1] = disconnected[1][0] = true;
    disconnected[2][3] = disconnected[3][2] = true;

    vector<int> noPath;
    bool disconnectedHasPath = hamiltonianPath(4, disconnected, noPath);
    cout << "disconnected graph has Hamiltonian path: "
         << (disconnectedHasPath ? "yes" : "no") << endl;
    cout << "brute force agrees: "
         << (disconnectedHasPath == bruteHamiltonianPath(4, disconnected)
                 ? "yes"
                 : "no")
         << endl;

    vector<vector<long long>> cost = {
        {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

    vector<int> tour;
    long long bestTour = travellingSalesperson(4, cost, tour);

    cout << "TSP cost: " << bestTour << endl;
    cout << "tour: ";
    printPath(tour);
    cout << endl;
    cout << "brute force cost: " << bruteTsp(4, cost) << endl;
    cout << "matches: " << (bestTour == bruteTsp(4, cost) ? "yes" : "no")
         << endl;

    vector<vector<long long>> impossible = {
        {0, 1, INF}, {1, 0, INF}, {INF, INF, 0}};
    vector<int> impossibleTour;
    long long impossibleCost =
        travellingSalesperson(3, impossible, impossibleTour);
    cout << "disconnected TSP returns INF: "
         << (impossibleCost == INF ? "yes" : "no") << endl;

    vector<int> singleTour;
    cout << "single city TSP cost: "
         << travellingSalesperson(1, {{0}}, singleTour) << ", tour: ";
    printPath(singleTour);
    cout << endl;

    return 0;
}

// Expected output (abridged):
// path graph has Hamiltonian path: yes
// path: [3 -> 2 -> 1 -> 0]
// brute force agrees: yes
// disconnected graph has Hamiltonian path: no
// brute force agrees: yes
// TSP cost: 80
// tour: [0 -> 2 -> 3 -> 1 -> 0]
// brute force cost: 80
// matches: yes
// disconnected TSP returns INF: yes
// single city TSP cost: 0, tour: [0]
//
// Special cases: a single vertex, disconnected graphs, graphs with no
// Hamiltonian path, a complete weighted graph, and a TSP instance with no
// feasible tour.
