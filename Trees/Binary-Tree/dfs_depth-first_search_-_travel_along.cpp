/**
 * DFS(Depth-first search):- Travel along the height of the binary tree
 */

/**
 * Preorder traversal:-
 * 1. Visit the root node
 * 2. Left subtree
 * 3. Right subtree
 * recursively
 */

/**
 * Inorder:-
 * 1. visit left subtree
 * 2. Root node
 * 3. Right subtree
 * Recursively
 */

/**
 * Postorder:-
 * 1. Visit left subtree
 * 2. Right subtree
 * 3. Root node
 * Recursively
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

void preorderTraversal(Node *rootNode) {
    // base case
    if (rootNode == NULL) {
        return;
    }

    cout << rootNode->value << endl;

    // recursive call
    preorderTraversal(rootNode->left);
    preorderTraversal(rootNode->right);
};

void inorderTraversal(Node *rootNode) {
    // base case
    if (rootNode == NULL) {
        return;
    }

    // recursive case
    inorderTraversal(rootNode->left);
    cout << rootNode->value << endl;
    inorderTraversal(rootNode->right);
};

void postorderTraversal(Node *rootNode) {
    // base case
    if (rootNode == NULL) {
        return;
    }

    // recursive case
    postorderTraversal(rootNode->left);
    postorderTraversal(rootNode->right);
    cout << rootNode->value << endl;
};

int main() {
    Node *rootNode = new Node(2);
    rootNode->left = new Node(4);
    rootNode->right = new Node(10);
    rootNode->left->left = new Node(6);
    rootNode->left->right = new Node(5);
    rootNode->right->right = new Node(11);

    preorderTraversal(rootNode);
    cout << "*****" << endl;
    inorderTraversal(rootNode);
    cout << "*****" << endl;
    postorderTraversal(rootNode);

    return 0;
}