/**
 * Invert Binary Tree
 *
 * Given the root of a binary tree, invert the tree and return its root.
 * Inverting means swapping the left and right child of every node.
 *
 * Example:
 * Input:  [4, 2, 7, 1, 3, 6, 9]
 * Output: [4, 7, 2, 9, 6, 3, 1]
 */

#include <algorithm>
#include <iostream>
#include <queue>
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

// Recursive: swap children, then invert both subtrees -> O(n)
Node *invertTree(Node *root) {
    if (root == NULL) {
        return NULL;
    }

    swap(root->left, root->right);

    invertTree(root->left);
    invertTree(root->right);

    return root;
}

// Iterative: use a queue (BFS) and swap children at every node -> O(n)
Node *invertTreeIterative(Node *root) {
    if (root == NULL) {
        return NULL;
    }

    queue<Node *> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        Node *current = nodes.front();
        nodes.pop();

        swap(current->left, current->right);

        if (current->left != NULL) {
            nodes.push(current->left);
        }
        if (current->right != NULL) {
            nodes.push(current->right);
        }
    }

    return root;
}

void levelOrder(Node *root) {
    if (root == NULL) {
        return;
    }

    queue<Node *> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        int size = nodes.size();

        while (size--) {
            Node *current = nodes.front();
            nodes.pop();

            cout << current->value << " ";

            if (current->left != NULL) {
                nodes.push(current->left);
            }
            if (current->right != NULL) {
                nodes.push(current->right);
            }
        }
    }

    cout << endl;
}

Node *buildSample() {
    Node *root = new Node(4);
    root->left = new Node(2);
    root->right = new Node(7);
    root->left->left = new Node(1);
    root->left->right = new Node(3);
    root->right->left = new Node(6);
    root->right->right = new Node(9);

    return root;
}

int main() {
    Node *first = buildSample();
    invertTree(first);
    levelOrder(first);  // 4 7 2 9 6 3 1

    Node *second = buildSample();
    invertTreeIterative(second);
    levelOrder(second);  // 4 7 2 9 6 3 1

    Node *single = new Node(1);
    invertTree(single);
    levelOrder(single);  // 1

    cout << (invertTree(NULL) == NULL) << endl;  // 1

    return 0;
}

/**
 * Time complexity -> O(n), every node is visited once
 * Space complexity -> O(h) for recursion or O(w) for the queue,
 * where h is the height and w is the maximum width
 *
 * Special cases: empty tree, single node, a left-skewed tree (becomes right
 * skewed), a right-skewed tree (becomes left skewed), calling invert twice
 * restores the original tree.
 */
