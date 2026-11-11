/**
 * Bottom View of a Binary Tree
 *
 * Given a binary tree, print the bottom view from left to right. A node is
 * included in the bottom view if it is the lowest node at its horizontal
 * distance when the tree is viewed from below.
 *
 * Horizontal distance:
 *   root -> 0
 *   left child -> parent - 1
 *   right child -> parent + 1
 *
 * Example:
 *         20
 *        /  \
 *       8    22
 *      / \   / \
 *     5   3 4  25
 *        / \
 *       10 14
 *
 * Bottom view: 5 10 4 14 25
 */

#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        left = NULL;
        right = NULL;
    }
};

// Brute force: collect every node with its horizontal distance and depth,
// then pick the deepest node for each distance. O(n^2) in the worst case.
void collectNodes(Node *root, int distance, int depth,
                  map<int, vector<pair<int, int>>> &nodes) {
    if (root == NULL) {
        return;
    }

    nodes[distance].push_back({depth, root->value});

    collectNodes(root->left, distance - 1, depth + 1, nodes);
    collectNodes(root->right, distance + 1, depth + 1, nodes);
}

vector<int> bottomViewBrute(Node *root) {
    map<int, vector<pair<int, int>>> nodes;

    collectNodes(root, 0, 0, nodes);

    vector<int> result;

    for (auto &entry : nodes) {
        int bestDepth = -1;
        int bestValue = 0;

        for (auto &candidate : entry.second) {
            // On a tie, the later node in the traversal (the rightmost) wins.
            if (candidate.first >= bestDepth) {
                bestDepth = candidate.first;
                bestValue = candidate.second;
            }
        }

        result.push_back(bestValue);
    }

    return result;
}

// Optimized: level-order BFS. The last node seen at each horizontal distance
// is the bottom-most one. O(n log n) because of the ordered map.
vector<int> bottomView(Node *root) {
    vector<int> result;

    if (root == NULL) {
        return result;
    }

    map<int, int> lastAtDistance;
    queue<pair<Node *, int>> nodes;
    nodes.push({root, 0});

    while (!nodes.empty()) {
        Node *current = nodes.front().first;
        int distance = nodes.front().second;
        nodes.pop();

        // BFS visits deeper nodes later, so overwriting keeps the bottom one.
        lastAtDistance[distance] = current->value;

        if (current->left != NULL) {
            nodes.push({current->left, distance - 1});
        }

        if (current->right != NULL) {
            nodes.push({current->right, distance + 1});
        }
    }

    for (auto &entry : lastAtDistance) {
        result.push_back(entry.second);
    }

    return result;
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    Node *root = new Node(20);
    root->left = new Node(8);
    root->right = new Node(22);
    root->left->left = new Node(5);
    root->left->right = new Node(3);
    root->right->left = new Node(4);
    root->right->right = new Node(25);
    root->left->right->left = new Node(10);
    root->left->right->right = new Node(14);

    vector<int> brute = bottomViewBrute(root);
    print(brute);  // 5 10 4 14 25

    vector<int> optimized = bottomView(root);
    print(optimized);  // 5 10 4 14 25

    Node *single = new Node(1);
    print(bottomView(single));  // 1

    Node *skewed = new Node(1);
    skewed->left = new Node(2);
    skewed->left->left = new Node(3);
    print(bottomView(skewed));  // 3 2 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n + k log k) to collect and O(n) to scan the buckets, where k
 *   is the number of distinct horizontal distances; O(n^2) if every node has a
 *   unique distance and the buckets are scanned naively.
 *   BFS -> O(n log n) because the map is ordered.
 * Space complexity -> O(n) for the map and the queue.
 *
 * Special cases: an empty tree (empty result), one node, a left-skewed tree,
 * a right-skewed tree, ties at the same horizontal distance and depth, and
 * duplicate values.
 *
 * The top view uses the same idea but keeps the first node seen at each
 * horizontal distance instead of the last.
 */
