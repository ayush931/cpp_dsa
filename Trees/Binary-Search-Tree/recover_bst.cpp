/**
 * Recover a BST with Two Swapped Nodes
 *
 * Exactly two nodes of a BST were swapped by mistake. Restore the BST without
 * changing its structure. The inorder traversal of a valid BST is strictly
 * increasing, so the swapped nodes appear as inversions in the inorder order.
 *
 * Example:
 *         3
 *        / \
 *       1   4
 *          /
 *         2
 *
 * Inorder before: 1 3 2 4
 * Inorder after:  1 2 3 4
 */

#include <algorithm>
#include <iostream>
#include <vector>
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

void inorderValues(Node *root, vector<int> &values) {
    if (root == NULL) {
        return;
    }

    inorderValues(root->left, values);
    values.push_back(root->value);
    inorderValues(root->right, values);
}

void assignInorder(Node *root, vector<int> &values, int &index) {
    if (root == NULL) {
        return;
    }

    assignInorder(root->left, values, index);
    root->value = values[index++];
    assignInorder(root->right, values, index);
}

// Brute force: read all values, sort them, and write them back in inorder.
// O(n log n) time and O(n) space.
void recoverBSTBrute(Node *root) {
    vector<int> values;
    inorderValues(root, values);
    sort(values.begin(), values.end());

    int index = 0;
    assignInorder(root, values, index);
}

void findSwapped(Node *root, Node *&previous, Node *&first, Node *&second) {
    if (root == NULL) {
        return;
    }

    findSwapped(root->left, previous, first, second);

    if (previous != NULL && previous->value > root->value) {
        if (first == NULL) {
            first = previous;
        }
        second = root;
    }

    previous = root;

    findSwapped(root->right, previous, first, second);
}

// Optimized: one inorder pass finds the two misplaced nodes.
// O(n) time and O(h) recursion space.
void recoverBST(Node *root) {
    Node *previous = NULL;
    Node *first = NULL;
    Node *second = NULL;

    findSwapped(root, previous, first, second);

    if (first != NULL && second != NULL) {
        swap(first->value, second->value);
    }
}

void printInorder(Node *root) {
    vector<int> values;
    inorderValues(root, values);

    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    Node *first = new Node(3);
    first->left = new Node(1);
    first->right = new Node(4);
    first->right->left = new Node(2);

    printInorder(first);  // 1 3 2 4
    recoverBSTBrute(first);
    printInorder(first);  // 1 2 3 4

    cout << "---\n";

    Node *second = new Node(3);
    second->left = new Node(1);
    second->right = new Node(4);
    second->right->left = new Node(2);

    printInorder(second);  // 1 3 2 4
    recoverBST(second);
    printInorder(second);  // 1 2 3 4

    cout << "---\n";

    // Adjacent swap: inorder 1 4 3 5 has one inversion, but the two nodes are
    // still recovered because first and second are updated at the inversion.
    Node *adjacent = new Node(3);
    adjacent->left = new Node(1);
    adjacent->right = new Node(5);
    adjacent->right->left = new Node(4);
    swap(adjacent->value, adjacent->right->left->value);  // 4 and 3
    printInorder(adjacent);                               // 1 4 3 5
    recoverBST(adjacent);
    printInorder(adjacent);  // 1 3 4 5

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n log n) for sorting the inorder values
 *   optimized -> O(n) for one inorder traversal
 * Space complexity:
 *   brute -> O(n) for the values
 *   optimized -> O(h) recursion depth
 *
 * Special cases: the two swapped nodes are adjacent in inorder, they are the
 * root and a leaf, the tree has two nodes, and duplicate values are not part
 * of a strict BST.
 */
