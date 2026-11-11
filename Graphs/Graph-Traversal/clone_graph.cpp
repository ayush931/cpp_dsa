/**
 * Clone Graph
 * Given a node of a connected undirected graph, return a deep copy of it.
 *
 * Brute force -> copy all nodes first, then copy every edge, O(V + E)
 * Optimized -> DFS/BFS with a map from original node to clone, O(V + E)
 */

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    vector<Node *> neighbours;

    Node(int v) {
        value = v;
    }
};

Node *cloneGraphBrute(Node *node) {
    if (node == NULL) {
        return NULL;
    }

    unordered_map<Node *, Node *> copies;
    queue<Node *> q;
    q.push(node);
    copies[node] = new Node(node->value);

    while (!q.empty()) {
        Node *current = q.front();
        q.pop();

        for (Node *neighbour : current->neighbours) {
            if (!copies.count(neighbour)) {
                copies[neighbour] = new Node(neighbour->value);
                q.push(neighbour);
            }
            copies[current]->neighbours.push_back(copies[neighbour]);
        }
    }

    return copies[node];
};

Node *cloneGraphDFS(Node *node, unordered_map<Node *, Node *> &copies) {
    if (node == NULL) {
        return NULL;
    }
    if (copies.count(node)) {
        return copies[node];
    }

    Node *clone = new Node(node->value);
    copies[node] = clone;

    for (Node *neighbour : node->neighbours) {
        clone->neighbours.push_back(cloneGraphDFS(neighbour, copies));
    }

    return clone;
};

int main() {
    Node *n1 = new Node(1);
    Node *n2 = new Node(2);
    Node *n3 = new Node(3);

    n1->neighbours = {n2, n3};
    n2->neighbours = {n1, n3};
    n3->neighbours = {n1, n2};

    Node *copy1 = cloneGraphBrute(n1);
    unordered_map<Node *, Node *> copies;
    Node *copy2 = cloneGraphDFS(n1, copies);

    cout << copy1->value << " " << copy1->neighbours.size() << endl;
    cout << copy2->value << " " << copy2->neighbours.size() << endl;

    return 0;
}

// Special cases: empty graph, single node, self loops, disconnected graph
// The map is necessary to handle cycles without infinite recursion
