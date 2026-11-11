/**
 * Connected components
 *
 * An undirected graph may be disconnected. A connected component is a maximal
 * set of vertices where every pair is connected by a path.
 *
 * BFS/DFS from every unvisited vertex:
 *   - each traversal visits exactly one component
 *   - componentCount counts the traversals
 *   - componentId labels every vertex with its component number
 *
 * Time -> O(V + E), Space -> O(V)
 *
 * Edge cases: empty graph, isolated vertices, self-loops, parallel edges,
 * and a graph that is already connected.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class ConnectedComponents {
    vector<vector<int>> graph;
    vector<int> componentId;
    int count;

    void bfs(int source, int id) {
        queue<int> q;
        q.push(source);
        componentId[source] = id;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbour : graph[node]) {
                if (componentId[neighbour] == -1) {
                    componentId[neighbour] = id;
                    q.push(neighbour);
                }
            }
        }
    }

   public:
    ConnectedComponents(vector<vector<int>> &input) {
        graph = input;
        int n = graph.size();
        componentId.assign(n, -1);
        count = 0;

        for (int node = 0; node < n; node++) {
            if (componentId[node] == -1) {
                bfs(node, count);
                count++;
            }
        }
    }

    int numberOfComponents() const {
        return count;
    }

    vector<int> ids() const {
        return componentId;
    }

    vector<vector<int>> groups() const {
        vector<vector<int>> result(count);

        for (int node = 0; node < (int)componentId.size(); node++) {
            result[componentId[node]].push_back(node);
        }

        return result;
    }
};

int main() {
    // 0 - 1     2 - 3     4
    vector<vector<int>> graph = {{1}, {0}, {3}, {2}, {}};

    ConnectedComponents components(graph);
    cout << components.numberOfComponents() << endl;  // 3

    vector<int> ids = components.ids();
    for (int id : ids) {
        cout << id << " ";
    }
    cout << endl;  // 0 0 1 1 2

    vector<vector<int>> groups = components.groups();
    for (auto &group : groups) {
        for (int node : group) {
            cout << node << " ";
        }
        cout << endl;
    }
    // 0 1
    // 2 3
    // 4

    vector<vector<int>> empty;
    ConnectedComponents none(empty);
    cout << none.numberOfComponents() << endl;  // 0

    return 0;
}

// Special cases:
// - empty graph -> 0 components
// - every vertex isolated -> V components
// - self-loops do not change the component count
// - parallel edges are harmless because visited checks prevent revisits
