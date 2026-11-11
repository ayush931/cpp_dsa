/**
 * Lowest common ancestor of BST
 * Given a Binary Search Tree (BST) and two values. You need to find the LCA
 * i.e. lowest common ancestor of the two nodes provided both the nodes exists
 * in the BST. Input: n = 9, values = {8, 3, 1, 6, 4, 7, 10, 14, 13}, node-1 =
 * 3, node-2 = 13 Output: Lowest common ancestor = 8
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

Node *lowestCommonAncestor(Node *root, Node *node1, Node *node2) {
    // base case
    if (root == NULL) {
        return NULL;
    }

    if (root->value > node1->value && root->value > node2->value) {
        // lca will lie in left subtree
        return lowestCommonAncestor(root->left, node1, node2);
    }

    if (root->value < node1->value && root->value < node2->value) {
        // lca will be lie in right subtree
        return lowestCommonAncestor(root->right, node1, node2);
    }

    // if root value lies between node1 and node2 value
    // or if root value is equal to any of node values
    return root;
};

int main() {
    BST bst1;
    bst1.root = insertBST(bst1.root, 3);
    insertBST(bst1.root, 1);
    insertBST(bst1.root, 4);
    insertBST(bst1.root, 6);
    insertBST(bst1.root, 2);

    Node *node1 = bst1.root->left->right;   // 2
    Node *node2 = bst1.root->right->right;  // 6
    Node *temp = lowestCommonAncestor(bst1.root, node1, node2);
    cout << temp->value << endl;  // 3

    return 0;
}