/**
 * Hopcroft-Karp Bipartite Matching
 *
 * Given a bipartite graph with n left nodes and m right nodes, find the maximum
 * number of edges such that no two edges share a vertex.
 *
 * Kuhn's algorithm
 *   - for every free left node, run a DFS that looks for an augmenting path
 *   - O(V * E) worst case
 *
 * Hopcroft-Karp
 *   - BFS builds layers from all free left nodes
 *   - DFS finds a maximal set of shortest augmenting paths in one phase
 *   - O(E * sqrt(V)) worst case
 *
 * Both algorithms rely on Berge's theorem: a matching is maximum exactly when
 * no augmenting path exists.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Kuhn: simple augmenting-path DFS. O(V * E).
bool tryKuhn(int left, vector<vector<int>> &adjacency, vector<int> &matchRight,
             vector<bool> &seen) {
    for (int right : adjacency[left]) {
        if (seen[right]) {
            continue;
        }
        seen[right] = true;

        if (matchRight[right] == -1 ||
            tryKuhn(matchRight[right], adjacency, matchRight, seen)) {
            matchRight[right] = left;
            return true;
        }
    }

    return false;
}

int maxMatchingKuhn(int leftCount, int rightCount,
                    vector<vector<int>> &adjacency) {
    vector<int> matchRight(rightCount, -1);
    int matching = 0;

    for (int left = 0; left < leftCount; left++) {
        vector<bool> seen(rightCount, false);

        if (tryKuhn(left, adjacency, matchRight, seen)) {
            matching++;
        }
    }

    return matching;
}

class HopcroftKarp {
    int leftCount;
    int rightCount;
    vector<vector<int>> &adjacency;
    vector<int> matchLeft;
    vector<int> matchRight;
    vector<int> distance;
    int shortestPathLength;

    bool buildLayers() {
        queue<int> pending;
        shortestPathLength = INT_MAX;

        for (int left = 0; left < leftCount; left++) {
            if (matchLeft[left] == -1) {
                distance[left] = 0;
                pending.push(left);
            } else {
                distance[left] = -1;
            }
        }

        while (!pending.empty()) {
            int left = pending.front();
            pending.pop();

            // Only left nodes that can still lie on a shortest augmenting path
            // need to be expanded.
            if (distance[left] >= shortestPathLength) {
                continue;
            }

            for (int right : adjacency[left]) {
                int nextLeft = matchRight[right];

                if (nextLeft == -1) {
                    shortestPathLength = distance[left] + 1;
                } else if (distance[nextLeft] == -1) {
                    distance[nextLeft] = distance[left] + 1;
                    pending.push(nextLeft);
                }
            }
        }

        return shortestPathLength != INT_MAX;
    }

    bool augment(int left) {
        for (int right : adjacency[left]) {
            int nextLeft = matchRight[right];

            if (nextLeft == -1) {
                if (distance[left] + 1 != shortestPathLength) {
                    continue;
                }

                matchLeft[left] = right;
                matchRight[right] = left;
                return true;
            }

            if (distance[nextLeft] == distance[left] + 1 && augment(nextLeft)) {
                matchLeft[left] = right;
                matchRight[right] = left;
                return true;
            }
        }

        distance[left] = -1;  // this left node cannot reach a free right node
        return false;
    }

   public:
    HopcroftKarp(int leftSize, int rightSize, vector<vector<int>> &graph)
        : adjacency(graph) {
        leftCount = leftSize;
        rightCount = rightSize;
        matchLeft.assign(leftCount, -1);
        matchRight.assign(rightCount, -1);
        distance.assign(leftCount, -1);
        shortestPathLength = INT_MAX;
    }

    int maximumMatching() {
        int matching = 0;

        while (buildLayers()) {
            for (int left = 0; left < leftCount; left++) {
                if (matchLeft[left] == -1 && augment(left)) {
                    matching++;
                }
            }
        }

        return matching;
    }

    vector<int> &leftMatches() {
        return matchLeft;
    }
};

int main() {
    // Left 0..3, right 0..3.
    vector<vector<int>> adjacency = {{0, 1}, {0}, {1, 2}, {2, 3}};

    cout << maxMatchingKuhn(4, 4, adjacency) << endl;  // 4

    HopcroftKarp hk(4, 4, adjacency);
    cout << hk.maximumMatching() << endl;  // 4

    // A graph with a smaller maximum matching.
    vector<vector<int>> second = {{0, 1}, {0, 1}, {0}, {}};

    cout << maxMatchingKuhn(4, 2, second) << endl;  // 2
    HopcroftKarp secondHk(4, 2, second);
    cout << secondHk.maximumMatching() << endl;  // 2

    // No edges.
    vector<vector<int>> empty(3);
    cout << maxMatchingKuhn(3, 3, empty) << endl;  // 0
    HopcroftKarp emptyHk(3, 3, empty);
    cout << emptyHk.maximumMatching() << endl;  // 0

    // A complete bipartite graph K(3,3) has a perfect matching of size 3.
    vector<vector<int>> complete = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}};

    HopcroftKarp completeHk(3, 3, complete);
    cout << completeHk.maximumMatching() << endl;  // 3

    return 0;
}

/**
 * Time complexity:
 *   Kuhn -> O(V * E)
 *   Hopcroft-Karp -> O(E * sqrt(V))
 * Space complexity -> O(V + E) for the matching arrays and the graph
 *
 * Special cases:
 *   - no edges (0)
 *   - an isolated left or right node
 *   - a complete bipartite graph (min(n, m))
 *   - multiple edges between the same pair (the algorithm still works, but
 *     they are redundant)
 *   - a graph with more left nodes than right nodes (matching <= rightCount)
 *
 * Applications: assigning workers to jobs, matching students to projects,
 * scheduling, resource allocation, and as a subroutine in minimum path cover
 * on a DAG. A maximum matching in a bipartite graph also gives a minimum
 * vertex cover by Konig's theorem.
 */
