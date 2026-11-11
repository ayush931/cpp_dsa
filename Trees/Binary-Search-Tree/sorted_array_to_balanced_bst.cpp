/**
 * Sorted Array to Balanced BST
 * Given a sorted array. Created a Balanced Binary search tree out of it. A
 * balanced BST is height-balanced i.e. the difference between the height of the
 * left subtree and right subtree is not more than 1. You need to print the
 * preorder traversal of the BST created. Input: n = 7, Element = 1, 2, 3, 4, 5,
 * 6, 7 Preorder: 4, 2, 1, 3, 6, 5, 7
 */

/**
 * Time -> O(n)
 * Space -> O(n) -> recursive stack
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

Node *sortedArrayToBST(vector<int> v, int start, int end) {
    // base case
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;
    Node *root = new Node(v[mid]);

    // recursive case
    root->left = sortedArrayToBST(
        v, start,
        mid - 1);  // recursively creating left subtree from left half to array
    root->right = sortedArrayToBST(
        v, mid + 1,
        end);  // recursively creating left subtree from left half to array

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
    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    BST bst;
    bst.root = sortedArrayToBST(v, 0, n - 1);
    preorderTraversal(bst.root);

    return 0;
}