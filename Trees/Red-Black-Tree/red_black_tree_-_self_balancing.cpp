/**
 * Red black tree -> self balancing binary search tree
 * Every node stores one extra bit: red or black
 *
 * Properties:-
 * 1. Every node is either red or black
 * 2. The root is black
 * 3. Every leaf (NULL) is black
 * 4. A red node cannot have a red child
 * 5. Every path from a node to its descendant leaves has the same number of
 * black nodes
 *
 * These properties keep the longest path at most twice the shortest path,
 * so height is O(log n)
 *
 * Search, insertion and deletion -> O(log n)
 * Insertion and deletion use recoloring and rotations to restore the properties
 *
 * AVL vs Red black tree:-
 * AVL -> more strictly balanced, faster search, more rotations
 * Red black -> fewer rotations, faster insertion and deletion
 *
 * C++ std::map and std::set are usually implemented as red black trees
 */

#include <iostream>
using namespace std;

enum Color { RED, BLACK };

class Node {
   public:
    int value;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(int v) {
        value = v;
        color = RED;  // a new node is always inserted as red
        left = right = parent = NULL;
    }
};

int main() {
    Node *root = new Node(10);
    root->color = BLACK;

    Node *leftChild = new Node(5);
    leftChild->parent = root;
    root->left = leftChild;

    Node *rightChild = new Node(15);
    rightChild->parent = root;
    root->right = rightChild;

    cout << "Root: " << root->value << " "
         << (root->color == BLACK ? "black" : "red") << endl;
    cout << "Children are red: "
         << (leftChild->color == RED && rightChild->color == RED) << endl;

    return 0;
}

// Full insertion and deletion require fixing violations with recoloring
// and the same LL, RR, LR, RL rotations used by AVL trees
