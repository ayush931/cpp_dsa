/**
 * Count Good Nodes in a Binary Tree
 *
 * A node is "good" if its value is greater than or equal to every value on the
 * path from the root to that node. Return the number of good nodes.
 *
 * Example tree:
 *         3
 *        / \
 *       1   4
 *      /   / \
 *     3   1   5
 *
 * Path maximums:
 *   3 -> 3 (good)
 *   1 -> 3 (not good)
 *   4 -> 4 (good)
 *   3 -> 3 (good)
 *   1 -> 4 (not good)
 *   5 -> 5 (good)
 *
 * Output: 4
 */

#include <algorithm>
#include <iostream>
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

// Brute force: keep the whole root-to-node path and scan it for the maximum.
// O(n * h) time, O(h) extra space.
void countBrute(Node *root, vector<int> &path, int &goodNodes) {
    if (root == NULL) {
        return;
    }

    path.push_back(root->value);

    int maximum = path[0];
    for (int value : path) {
        maximum = max(maximum, value);
    }

    if (root->value >= maximum) {
        goodNodes++;
    }

    countBrute(root->left, path, goodNodes);
    countBrute(root->right, path, goodNodes);

    path.pop_back();
}

int goodNodesBrute(Node *root) {
    int goodNodes = 0;
    vector<int> path;

    countBrute(root, path, goodNodes);

    return goodNodes;
}

// Optimized: pass the maximum seen so far down the recursion.
// O(n) time and O(h) recursion space.
int countGood(Node *root, int maximum) {
    if (root == NULL) {
        return 0;
    }

    int good = (root->value >= maximum) ? 1 : 0;
    int newMaximum = max(maximum, root->value);

    return good + countGood(root->left, newMaximum) +
           countGood(root->right, newMaximum);
}

int goodNodes(Node *root) {
    if (root == NULL) {
        return 0;
    }

    // The root always qualifies; use a value smaller than any possible node.
    return countGood(root, root->value);
}

int main() {
    Node *root = new Node(3);
    root->left = new Node(1);
    root->right = new Node(4);
    root->left->left = new Node(3);
    root->right->left = new Node(1);
    root->right->right = new Node(5);

    cout << goodNodesBrute(root) << endl;  // 4
    cout << goodNodes(root) << endl;       // 4

    cout << "---\n";

    Node *decreasing = new Node(5);
    decreasing->left = new Node(4);
    decreasing->left->left = new Node(3);
    cout << goodNodes(decreasing) << endl;  // 1

    Node *single = new Node(-10);
    cout << goodNodes(single) << endl;  // 1

    cout << goodNodes(NULL) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n * h) because the path maximum is recomputed at every node
 *   optimized -> O(n), because each node is visited once
 * Space complexity -> O(h) recursion depth
 *
 * Special cases: empty tree (0), one node (1), all values decreasing (only
 * the root is good), all values equal (every node is good), negative values,
 * and duplicate values.
 */
