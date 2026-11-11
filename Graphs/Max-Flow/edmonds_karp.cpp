/**
 * Edmonds-Karp Maximum Flow
 *
 * Edmonds-Karp is the Ford-Fulkerson method with one crucial rule: always
 * augment along a shortest augmenting path, found by BFS in the residual graph.
 * With adjacency lists this gives O(V * E^2) time. This teaching version
 * scans a capacity matrix during BFS: O(V^2) per search, with at most O(V*E)
 * augmentations, giving O(V^3*E) time and O(V^2) space.
 *
 * The residual graph contains:
 *   - forward edges with remaining capacity
 *   - backward edges with the amount of flow currently sent
 *
 * When no augmenting path exists, the set of vertices reachable from the source
 * in the residual graph defines a minimum cut. Max-flow equals min-cut.
 *
 * A brute-force minimum-cut enumeration is included for small graphs.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

struct FlowResult {
    long long maxFlow;
    long long minCut;
    vector<bool> sourceSide;
    vector<vector<long long>> residual;
};

FlowResult edmondsKarp(int vertices, const vector<vector<long long>> &capacity,
                       int source, int sink) {
    FlowResult result{0, 0, vector<bool>(vertices, false), capacity};

    if (vertices == 0 || source < 0 || sink < 0 || source >= vertices ||
        sink >= vertices || source == sink) {
        return result;
    }

    while (true) {
        vector<int> parent(vertices, -1);
        queue<int> bfs;
        parent[source] = source;
        bfs.push(source);

        while (!bfs.empty() && parent[sink] == -1) {
            int node = bfs.front();
            bfs.pop();

            for (int next = 0; next < vertices; next++) {
                if (parent[next] != -1) {
                    continue;
                }
                if (result.residual[node][next] <= 0) {
                    continue;
                }

                parent[next] = node;
                bfs.push(next);
            }
        }

        if (parent[sink] == -1) {
            break;
        }

        long long bottleneck = INF;

        for (int node = sink; node != source; node = parent[node]) {
            bottleneck = min(bottleneck, result.residual[parent[node]][node]);
        }

        for (int node = sink; node != source; node = parent[node]) {
            int previous = parent[node];
            result.residual[previous][node] -= bottleneck;
            result.residual[node][previous] += bottleneck;
        }

        result.maxFlow += bottleneck;
    }

    // Vertices reachable from the source in the final residual graph.
    queue<int> reachable;
    result.sourceSide[source] = true;
    reachable.push(source);

    while (!reachable.empty()) {
        int node = reachable.front();
        reachable.pop();

        for (int next = 0; next < vertices; next++) {
            if (!result.sourceSide[next] && result.residual[node][next] > 0) {
                result.sourceSide[next] = true;
                reachable.push(next);
            }
        }
    }

    for (int from = 0; from < vertices; from++) {
        if (!result.sourceSide[from]) {
            continue;
        }

        for (int to = 0; to < vertices; to++) {
            if (!result.sourceSide[to]) {
                result.minCut += capacity[from][to];
            }
        }
    }

    return result;
}

long long bruteForceMinCut(int vertices,
                           const vector<vector<long long>> &capacity,
                           int source, int sink) {
    if (vertices == 0 || source == sink) {
        return 0;
    }

    long long best = INF;

    for (int mask = 0; mask < (1 << vertices); mask++) {
        if (!(mask & (1 << source))) {
            continue;
        }
        if (mask & (1 << sink)) {
            continue;
        }

        long long cut = 0;

        for (int from = 0; from < vertices; from++) {
            if (!(mask & (1 << from))) {
                continue;
            }

            for (int to = 0; to < vertices; to++) {
                if (mask & (1 << to)) {
                    continue;
                }
                cut += capacity[from][to];
            }
        }

        best = min(best, cut);
    }

    return best;
}

void printPartition(const vector<bool> &sourceSide) {
    cout << "source side: [";

    bool first = true;
    for (int i = 0; i < static_cast<int>(sourceSide.size()); i++) {
        if (!sourceSide[i]) {
            continue;
        }
        if (!first) {
            cout << ", ";
        }
        cout << i;
        first = false;
    }

    cout << "]" << endl;
}

int main() {
    int vertices = 6;
    vector<vector<long long>> capacity(vertices,
                                       vector<long long>(vertices, 0));

    capacity[0][1] = 16;
    capacity[0][2] = 13;
    capacity[1][2] = 10;
    capacity[2][1] = 4;
    capacity[1][3] = 12;
    capacity[3][2] = 9;
    capacity[2][4] = 14;
    capacity[4][3] = 7;
    capacity[3][5] = 20;
    capacity[4][5] = 4;

    FlowResult result = edmondsKarp(vertices, capacity, 0, 5);

    cout << "max flow: " << result.maxFlow << endl;
    cout << "min cut: " << result.minCut << endl;
    printPartition(result.sourceSide);
    cout << "brute-force min cut: "
         << bruteForceMinCut(vertices, capacity, 0, 5) << endl;
    cout << "max-flow equals min-cut: "
         << (result.maxFlow == result.minCut &&
                     result.minCut == bruteForceMinCut(vertices, capacity, 0, 5)
                 ? "yes"
                 : "no")
         << endl;

    vector<vector<long long>> disconnected(4, vector<long long>(4, 0));
    disconnected[0][1] = 5;
    disconnected[2][3] = 7;

    FlowResult noFlow = edmondsKarp(4, disconnected, 0, 3);
    cout << "disconnected max flow: " << noFlow.maxFlow
         << ", min cut: " << noFlow.minCut << endl;

    vector<vector<long long>> single(2, vector<long long>(2, 0));
    single[0][1] = 42;
    cout << "single edge max flow: " << edmondsKarp(2, single, 0, 1).maxFlow
         << endl;

    FlowResult sameNode = edmondsKarp(2, single, 0, 0);
    cout << "source equals sink max flow: " << sameNode.maxFlow << endl;

    return 0;
}

// Expected output:
// max flow: 23
// min cut: 23
// source side: [0, 1, 2, 4]
// brute-force min cut: 23
// max-flow equals min-cut: yes
// disconnected max flow: 0, min cut: 0
// single edge max flow: 42
// source equals sink max flow: 0
//
// Special cases: disconnected source and sink, a single edge, source equal to
// sink, zero-capacity edges, backward residual edges, and multiple edges whose
// capacities are combined in the capacity matrix.
