/**
 * K shortest loopless paths (Yen's algorithm)
 *
 * Yen's algorithm finds the K shortest paths from a source to a target without
 * repeating a vertex in a path.
 *
 * 1. Find the shortest path with Dijkstra and put it in A.
 * 2. For each previous path, create spur paths:
 *    - choose a spur node along the previous path
 *    - keep the root path before the spur node
 *    - ban the next edge of every previous path that shares the same root
 *    - ban every root node except the spur node
 *    - run Dijkstra from the spur node to the target
 *    - combine the root and spur paths and add the result to B
 * 3. Move the smallest candidate from B to A.
 *
 * Time -> O(K * V * (E log V)) in this simple implementation
 * Space -> O(K * V)
 *
 * Edge cases: no path, K larger than the number of loopless paths, zero-weight
 * edges, duplicate candidates, and a target equal to the source.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int weight;
};

struct Path {
    vector<int> nodes;
    int cost;

    bool operator<(Path const &other) const {
        if (cost != other.cost) {
            return cost < other.cost;
        }
        return nodes < other.nodes;
    }
};

vector<int> dijkstra(vector<vector<Edge>> &graph, int source, int target,
                     set<int> &bannedNodes, set<pair<int, int>> &bannedEdges) {
    int n = graph.size();
    vector<int> distance(n, INT_MAX);
    vector<int> parent(n, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDistance, node] = pq.top();
        pq.pop();

        if (currentDistance != distance[node]) {
            continue;
        }
        if (node == target) {
            break;
        }

        for (Edge &edge : graph[node]) {
            if (bannedNodes.count(edge.to)) {
                continue;
            }
            if (bannedEdges.count({node, edge.to})) {
                continue;
            }

            int nextDistance = currentDistance + edge.weight;
            if (nextDistance < distance[edge.to]) {
                distance[edge.to] = nextDistance;
                parent[edge.to] = node;
                pq.push({nextDistance, edge.to});
            }
        }
    }

    if (distance[target] == INT_MAX) {
        return {};
    }

    vector<int> path;
    int node = target;

    while (node != -1) {
        path.push_back(node);
        if (node == source) {
            break;
        }
        node = parent[node];
    }

    if (path.back() != source) {
        return {};
    }

    reverse(path.begin(), path.end());
    return path;
}

int pathCost(vector<vector<Edge>> &graph, vector<int> &path) {
    int cost = 0;

    for (int i = 0; i + 1 < (int)path.size(); i++) {
        for (Edge &edge : graph[path[i]]) {
            if (edge.to == path[i + 1]) {
                cost += edge.weight;
                break;
            }
        }
    }

    return cost;
}

vector<Path> kShortestPaths(vector<vector<Edge>> &graph, int source, int target,
                            int k) {
    vector<Path> answer;
    if (k <= 0) {
        return answer;
    }

    set<int> noBannedNodes;
    set<pair<int, int>> noBannedEdges;

    vector<int> first =
        dijkstra(graph, source, target, noBannedNodes, noBannedEdges);
    if (first.empty()) {
        return answer;
    }

    answer.push_back({first, pathCost(graph, first)});
    set<Path> candidates;

    for (int pathIndex = 1; pathIndex < k; pathIndex++) {
        vector<int> &previous = answer[pathIndex - 1].nodes;

        for (int spurIndex = 0; spurIndex + 1 < (int)previous.size();
             spurIndex++) {
            vector<int> root(previous.begin(),
                             previous.begin() + spurIndex + 1);
            int spurNode = previous[spurIndex];

            set<pair<int, int>> bannedEdges;
            set<int> bannedNodes;

            for (Path &path : answer) {
                if (path.nodes.size() <= spurIndex + 1) {
                    continue;
                }

                bool sameRoot = true;
                for (int i = 0; i <= spurIndex; i++) {
                    if (path.nodes[i] != root[i]) {
                        sameRoot = false;
                        break;
                    }
                }

                if (sameRoot) {
                    bannedEdges.insert(
                        {path.nodes[spurIndex], path.nodes[spurIndex + 1]});
                }
            }

            for (int i = 0; i < spurIndex; i++) {
                bannedNodes.insert(root[i]);
            }

            vector<int> spur =
                dijkstra(graph, spurNode, target, bannedNodes, bannedEdges);
            if (spur.empty()) {
                continue;
            }

            vector<int> combined = root;
            combined.insert(combined.end(), spur.begin() + 1, spur.end());

            candidates.insert({combined, pathCost(graph, combined)});
        }

        if (candidates.empty()) {
            break;
        }

        auto best = candidates.begin();
        answer.push_back(*best);
        candidates.erase(best);
    }

    return answer;
}

int main() {
    // 0 -> 1 (1), 0 -> 2 (4), 1 -> 2 (2), 1 -> 3 (6), 2 -> 3 (3)
    vector<vector<Edge>> graph = {
        {{1, 1}, {2, 4}}, {{2, 2}, {3, 6}}, {{3, 3}}, {}};

    vector<Path> paths = kShortestPaths(graph, 0, 3, 3);

    for (Path &path : paths) {
        for (int node : path.nodes) {
            cout << node << " ";
        }
        cout << ": " << path.cost << endl;
    }
    // 0 1 2 3 : 6
    // 0 2 3 : 7
    // 0 1 3 : 7

    vector<Path> none = kShortestPaths(graph, 3, 0, 3);
    cout << none.size() << endl;  // 0

    vector<Path> zero = kShortestPaths(graph, 0, 3, 0);
    cout << zero.size() << endl;  // 0

    return 0;
}

// Special cases:
// - no path -> empty result
// - k = 0 -> empty result
// - fewer than k loopless paths -> return all that exist
// - banning root nodes prevents cycles in the spur path
// - duplicate candidates are removed by the set
