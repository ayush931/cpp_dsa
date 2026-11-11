/**
 * Binary Search Tree Iterator
 *
 * Implement an iterator over a BST that returns values in ascending order.
 * next() returns the next smallest value and hasNext() reports whether one
 * exists. The optimized version uses O(h) memory and O(1) amortized time per
 * call; it does not copy the whole tree.
 *
 * Example tree:
 *         7
 *        / \
 *       3   15
 *          /  \
 *         9    20
 *
 * Iteration: 3 7 9 15 20
 */

#include <iostream>
#include <stack>
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

// Brute force: store the full inorder traversal in a vector.
class BSTIteratorBrute {
   private:
    vector<int> values;
    int index;

    void inorder(Node *root) {
        if (root == NULL) {
            return;
        }

        inorder(root->left);
        values.push_back(root->value);
        inorder(root->right);
    }

   public:
    BSTIteratorBrute(Node *root) {
        index = 0;
        inorder(root);
    }

    int next() {
        return values[index++];
    }

    bool hasNext() const {
        return index < values.size();
    }
};

// Optimized: keep only the current left spine on a stack.
class BSTIterator {
   private:
    stack<Node *> pending;

    void pushLeftSpine(Node *root) {
        while (root != NULL) {
            pending.push(root);
            root = root->left;
        }
    }

   public:
    BSTIterator(Node *root) {
        pushLeftSpine(root);
    }

    int next() {
        Node *current = pending.top();
        pending.pop();

        // After visiting a node, its right subtree contains the next values.
        pushLeftSpine(current->right);

        return current->value;
    }

    bool hasNext() const {
        return !pending.empty();
    }
};

int main() {
    Node *root = new Node(7);
    root->left = new Node(3);
    root->right = new Node(15);
    root->right->left = new Node(9);
    root->right->right = new Node(20);

    BSTIteratorBrute brute(root);
    while (brute.hasNext()) {
        cout << brute.next() << " ";
    }
    cout << endl;  // 3 7 9 15 20

    BSTIterator iterator(root);
    while (iterator.hasNext()) {
        cout << iterator.next() << " ";
    }
    cout << endl;  // 3 7 9 15 20

    cout << "---\n";

    BSTIterator empty(NULL);
    cout << empty.hasNext() << endl;  // 0

    Node *single = new Node(42);
    BSTIterator one(single);
    cout << one.hasNext() << " " << one.next() << " " << one.hasNext() << endl;
    // 1 42 0

    return 0;
}

/**
 * Time complexity:
 *   constructor -> O(h)
 *   next() -> O(1) amortized, O(h) worst case for one call
 *   hasNext() -> O(1)
 *   brute force -> O(n) construction and O(1) per call, but O(n) memory
 * Space complexity -> O(h) for the stack, where h is the tree height
 *
 * Special cases: empty tree, one node, a left-skewed BST, a right-skewed BST,
 * and calling next() only while hasNext() is true.
 */
