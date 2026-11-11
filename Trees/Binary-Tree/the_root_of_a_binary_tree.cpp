/**
 * Given the root of a binary tree, return its maximum depth. A binary tree's
 * maximum depth is the number of nodes doing the longest path from the root
 * node down to the farthest leaf node.
 */

#include <iostream>
using namespace std;

class Node {
   public:
    int value;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        left = right = NULL;
    }
};

int maxDepth(Node *root) {
    // base case
    if (root == NULL) {
        return 0;
    }

    // recursion case
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    return (max(leftDepth, rightDepth) + 1);
}

int main() {
    Node *rootNode = new Node(2);
    rootNode->left = new Node(4);
    rootNode->right = new Node(10);
    rootNode->left->left = new Node(6);
    rootNode->left->right = new Node(5);
    rootNode->right->right = new Node(11);
    rootNode->right->right->right = new Node(12);

    cout << maxDepth(rootNode) << endl;

    return 0;
}