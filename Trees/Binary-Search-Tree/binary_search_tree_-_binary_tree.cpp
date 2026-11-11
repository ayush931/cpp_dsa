/**
 * Binary search tree -> Binary tree where all values in left subtree < values
 * of node and all values in right subtree > value of node Time complexity ->
 * O(log n base 2) This property is true for root node of all subtree in a
 * binary tree Time -> O(n) -> worst case, unbalanced tree, skewed in one
 * direction
 */

// Insertion

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

void insertBST(Node *&root, int val) {
    Node *newNode = new Node(val);
    if (root == NULL) {
        root = newNode;
        return;
    }

    Node *current = root;
    while (true) {
        if (current->value > val) {
            if (current->left == NULL) {
                current->left = newNode;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = newNode;
                return;
            }
            current = current->right;
        }
    }
};

Node *insertBSTrecursive(Node *root, int val) {
    // base case
    if (root == NULL) {
        Node *new_node = new Node(val);
        return new_node;
    }

    // recursive case
    if (root->value > val) {
        root->left = insertBSTrecursive(root->left, val);
    } else if (root->value < val) {
        root->right = insertBSTrecursive(root->right, val);
    }

    return root;
};

bool searchBST(Node *root, int key) {
    // base case
    if (root == NULL) {
        return false;
    }

    if (root->value == key) {
        return true;
    }

    // recursive case
    if (root->value < key) {
        return searchBST(root->right, key);
    }

    if (root->value > key) {
        return searchBST(root->left, key);
    }

    return false;
};

void inorderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }
    inorderTraversal(root->left);
    cout << root->value << endl;
    inorderTraversal(root->right);
};

Node *largestNodeBST(Node *root) {
    Node *curr = root;
    while (curr && curr->right != NULL) {
        curr = curr->right;
    }
    return curr;
};

Node *deleteBST(Node *root, int key) {
    if (root == NULL) {
        return root;
    }

    if (root->value < key) {
        root->right = deleteBST(root->right, key);
    } else if (root->value > key) {
        root->left = deleteBST(root->left, key);
    } else {  // root is the node to be deleted
        if (root->left == NULL &&
            root->right == NULL) {  // node has 0 child nodes
            delete root;
            return NULL;
        }

        // node has 1 child node
        else if (root->left == NULL) {
            Node *temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            delete root;
            return temp;
        }

        // node has 2 child nodes
        else {
            Node *justSmallerNode = largestNodeBST(root->left);
            root->value = justSmallerNode->value;
            root->left = deleteBST(root->left, justSmallerNode->value);
        }
    }

    return root;
};

int main() {
    BST bst1;
    insertBST(bst1.root, 3);
    insertBST(bst1.root, 1);
    insertBST(bst1.root, 4);
    insertBST(bst1.root, 6);
    insertBST(bst1.root, 2);

    inorderTraversal(bst1.root);

    bst1.root = deleteBST(bst1.root, 3);
    inorderTraversal(bst1.root);
    cout << endl;

    return 0;
}

/**
 * Consider a binary search tree with n nodes. What is the minimum number of
 * comparison required to search for a value in the worst-case scenario:-
 * O(logn)
 */