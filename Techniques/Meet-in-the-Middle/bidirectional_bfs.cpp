/**
 * Bidirectional BFS
 *
 * Search for the shortest path between source and target by running BFS from
 * both ends at the same time. At each step expand the smaller frontier.
 *
 * A node is visited from the front or from the back. When a node is reached
 * from both sides, the shortest path length is distanceFront + distanceBack.
 *
 * Time -> O(b^(d/2)) instead of O(b^d) for a branching factor b and depth d
 * Space -> O(b^(d/2))
 *
 * Edge cases: source == target, disconnected graph, self-loop, duplicate
 * edges, target not present.
 */

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

int expandFrontier(queue<int> &frontier,
                   unordered_map<int, int> &distanceThisSide,
                   unordered_map<int, int> &distanceOtherSide,
                   vector<vector<int>> &graph) {
    int size = frontier.size();

    for (int i = 0; i < size; i++) {
        int node = frontier.front();
        frontier.pop();

        for (int neighbour : graph[node]) {
            if (distanceThisSide.count(neighbour)) {
                continue;
            }

            distanceThisSide[neighbour] = distanceThisSide[node] + 1;

            if (distanceOtherSide.count(neighbour)) {
                return distanceThisSide[neighbour] +
                       distanceOtherSide[neighbour];
            }

            frontier.push(neighbour);
        }
    }

    return -1;
}

int bidirectionalBfs(vector<vector<int>> &graph, int source, int target) {
    if (source < 0 || source >= (int)graph.size()) {
        return -1;
    }
    if (target < 0 || target >= (int)graph.size()) {
        return -1;
    }
    if (source == target) {
        return 0;
    }

    queue<int> frontQueue;
    queue<int> backQueue;

    unordered_map<int, int> frontDistance;
    unordered_map<int, int> backDistance;

    frontQueue.push(source);
    backQueue.push(target);

    frontDistance[source] = 0;
    backDistance[target] = 0;

    while (!frontQueue.empty() && !backQueue.empty()) {
        int result;

        if (frontQueue.size() <= backQueue.size()) {
            result =
                expandFrontier(frontQueue, frontDistance, backDistance, graph);
        } else {
            result =
                expandFrontier(backQueue, backDistance, frontDistance, graph);
        }

        if (result != -1) {
            return result;
        }
    }

    return -1;
}

int main() {
    // 0 - 1 - 2 - 3 - 4
    // |       |
    // 5 - 6 - 7
    vector<vector<int>> graph = {{1, 5}, {0, 2}, {1, 3, 7}, {2, 4},
                                 {3},    {0, 6}, {5, 7},    {6, 2}};

    cout << bidirectionalBfs(graph, 0, 4) << endl;  // 4 (0-1-2-3-4)
    cout << bidirectionalBfs(graph, 0, 6) << endl;  // 2 (0-5-6)
    cout << bidirectionalBfs(graph, 3, 3) << endl;  // 0
    cout << bidirectionalBfs(graph, 0, 9) << endl;  // -1 (target outside graph)

    return 0;
}

// Special cases:
// - source == target -> 0
// - disconnected graph -> -1
// - expanding the smaller frontier keeps the search balanced
// - a node reached from both sides gives the shortest path because BFS
//   explores in non-decreasing distance order on each side
