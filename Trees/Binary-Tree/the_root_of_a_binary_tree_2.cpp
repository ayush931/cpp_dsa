/**
 * Given the root of a binary tree, return the number of leaf nodes present in
 * it. A leaf node is a node which has no left and right child.
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

int leafNodes(Node *root) {
    // base case
    if (root == NULL) {
        return 0;
    }

    if (root->left == NULL && root->right == NULL) {
        return 1;  // leaf node
    }

    // recursive case
    int leftSubtreeLeafNodes = leafNodes(root->left);
    int rightSubtreeLeafNodes = leafNodes(root->right);

    return leftSubtreeLeafNodes + rightSubtreeLeafNodes;
};

int main() {
    Node *rootNode = new Node(2);
    rootNode->left = new Node(4);
    rootNode->right = new Node(10);
    rootNode->left->left = new Node(6);
    rootNode->left->right = new Node(5);
    rootNode->right->left = new Node(12);
    rootNode->right->right = new Node(11);

    cout << leafNodes(rootNode) << endl;

    return 0;
}