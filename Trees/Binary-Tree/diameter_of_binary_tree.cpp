/**
 * Diameter of Binary Tree
 *
 * Given the root of a binary tree, return the length of its diameter. The
 * diameter is the number of edges on the longest path between any two nodes.
 * The path does not have to pass through the root.
 *
 * Example:
 * Input:  [1, 2, 3, 4, 5]
 * Output: 3  (path 4 -> 2 -> 1 -> 3)
 */

#include <algorithm>
#include <iostream>
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

int heightBrute(Node *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + max(heightBrute(root->left), heightBrute(root->right));
}

// Brute force: compute the height of every node separately -> O(n^2)
int diameterBrute(Node *root) {
    if (root == NULL) {
        return 0;
    }

    int throughRoot = heightBrute(root->left) + heightBrute(root->right);
    int leftDiameter = diameterBrute(root->left);
    int rightDiameter = diameterBrute(root->right);

    return max(throughRoot, max(leftDiameter, rightDiameter));
}

// Optimized: one post-order pass returns the height and updates the diameter
int heightAndDiameter(Node *root, int &diameter) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = heightAndDiameter(root->left, diameter);
    int rightHeight = heightAndDiameter(root->right, diameter);

    // Longest path whose highest point is this node.
    diameter = max(diameter, leftHeight + rightHeight);

    return 1 + max(leftHeight, rightHeight);
}

int diameter(Node *root) {
    int result = 0;
    heightAndDiameter(root, result);
    return result;
}

int main() {
    Node *first = new Node(1);
    first->left = new Node(2);
    first->right = new Node(3);
    first->left->left = new Node(4);
    first->left->right = new Node(5);

    cout << diameterBrute(first) << endl;  // 3
    cout << diameter(first) << endl;       // 3

    Node *single = new Node(1);
    cout << diameter(single) << endl;  // 0

    Node *skewed = new Node(1);
    skewed->left = new Node(2);
    skewed->left->left = new Node(3);
    skewed->left->left->left = new Node(4);
    cout << diameter(skewed) << endl;  // 3

    cout << diameter(NULL) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2) because height is recomputed at every node
 *   optimized -> O(n) time, O(h) recursion space
 *
 * Special cases: empty tree (0), single node (0), a skewed tree (n - 1),
 * the diameter does not pass through the root, duplicate values, negative
 * values (they do not affect the number of edges).
 */
