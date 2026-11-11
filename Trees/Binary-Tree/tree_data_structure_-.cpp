/**
 * Tree data structure:-
 * Non-linear data structures stores hierarchical data.
 * Elements are stored at different levels.
 * Elements are called nodes - which are connected/linked together to represent
 * hierarchy
 */

/**
 * Terminology:-
 * Root, Child Node, Parent Node, Sibling Node, Leaf, Number of edges, level,
 * height, size, subtree
 */

/**
 * Types of Trees:-
 * 1. Generic tree:- A node can have any number of child nodes.
 * 2. Binary Trees:- Tree in which a node can have maximum 2 child nodes.
 * 3. Binary search tree:- Binary tree in which all nodes in left subtree will
 * have volume less than root node value and all nodes in right subtree will
 * have value more than root node volume
 * 4. AVL trees:- Self balancing trees
 * Difference between heights of left subtree and right subtree can be 1, 0, -1
 */

/**
 * Binary Tree:-
 * Every node can have maximum 2 child nodes.
 * Every node will contain:- data, link to left child, link to right child
 * If any child does not exists, pointer will point to NULL
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

int main() {
    Node *root = new Node(2);
    root->left = new Node(3);
    root->right = new Node(4);

    cout << "Root Node: " << root->value << endl;
    cout << "Left Node: " << root->left->value << endl;
    cout << "Right Node: " << root->right->value << endl;

    return 0;
}