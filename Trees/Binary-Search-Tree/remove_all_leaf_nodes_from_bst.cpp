/**
 * Remove all leaf nodes from BST
 * Input: The input to the program is the number of nodes in the BST, followed
 * by the node values. Output: The program outputs the preorder traversal of the
 * BST before and other removing the leaf nodes.
 */

#include <bits/stdc++.h>
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

class BST {
   public:
    Node *root;

    BST() {
        root = NULL;
    }
};

Node *insertBST(Node *root, int val) {
    // base case
    if (root == NULL) {
        Node *new_node = new Node(val);
        return new_node;
    }

    // recursive case
    if (root->value > val) {
        root->left = insertBST(root->left, val);
    } else if (root->value < val) {
        root->right = insertBST(root->right, val);
    }

    return root;
};

Node *removeLeafNodes(Node *root) {
    // base case
    if (root == NULL) {
        return NULL;
    }

    if (root->left == NULL && root->right == NULL) {
        return NULL;  // when root is a leaf node
    }

    // recursive case
    root->left = removeLeafNodes(root->left);
    root->right = removeLeafNodes(root->right);
    return root;
};

void preorderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }

    cout << root->value << endl;

    preorderTraversal(root->left);
    preorderTraversal(root->right);
};

int main() {
    BST bst1;
    bst1.root = insertBST(bst1.root, 3);
    insertBST(bst1.root, 1);
    insertBST(bst1.root, 4);
    insertBST(bst1.root, 6);
    insertBST(bst1.root, 2);

    preorderTraversal(bst1.root);
    cout << endl;

    bst1.root = removeLeafNodes(bst1.root);
    preorderTraversal(bst1.root);
    cout << endl;

    return 0;
}

/**
 * Time -> O(n)
 * Space -> O(h) -> height of the tree
 */