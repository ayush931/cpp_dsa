/**
 * Iterative Preorder, Inorder and Postorder Traversal
 *
 * Recursive DFS uses the call stack. These versions use an explicit stack so
 * they can handle deep trees without risking stack overflow, and they make the
 * order of visits easy to inspect.
 *
 * Example tree:
 *         1
 *        / \
 *       2   3
 *      / \ / \
 *     4  5 6  7
 *
 * Preorder  (root, left, right): 1 2 4 5 3 6 7
 * Inorder   (left, root, right): 4 2 5 1 6 3 7
 * Postorder (left, right, root): 4 5 2 6 7 3 1
 */

#include <algorithm>
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

void preorderRecursive(Node *root, vector<int> &result) {
    if (root == NULL) {
        return;
    }

    result.push_back(root->value);
    preorderRecursive(root->left, result);
    preorderRecursive(root->right, result);
}

// Visit a node, then push its right child before its left child so the left
// child is processed first.
vector<int> preorderIterative(Node *root) {
    vector<int> result;
    if (root == NULL) {
        return result;
    }

    stack<Node *> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        Node *current = nodes.top();
        nodes.pop();

        result.push_back(current->value);

        if (current->right != NULL) {
            nodes.push(current->right);
        }
        if (current->left != NULL) {
            nodes.push(current->left);
        }
    }

    return result;
}

// Go as far left as possible, then visit and move to the right subtree.
vector<int> inorderIterative(Node *root) {
    vector<int> result;
    stack<Node *> nodes;
    Node *current = root;

    while (current != NULL || !nodes.empty()) {
        while (current != NULL) {
            nodes.push(current);
            current = current->left;
        }

        current = nodes.top();
        nodes.pop();

        result.push_back(current->value);

        current = current->right;
    }

    return result;
}

// Reverse of "root, right, left": push nodes into an output stack in that
// order, then pop everything to get "left, right, root".
vector<int> postorderIterative(Node *root) {
    vector<int> result;
    if (root == NULL) {
        return result;
    }

    stack<Node *> nodes;
    stack<int> output;
    nodes.push(root);

    while (!nodes.empty()) {
        Node *current = nodes.top();
        nodes.pop();

        output.push(current->value);

        if (current->left != NULL) {
            nodes.push(current->left);
        }
        if (current->right != NULL) {
            nodes.push(current->right);
        }
    }

    while (!output.empty()) {
        result.push_back(output.top());
        output.pop();
    }

    return result;
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);

    vector<int> recursivePreorder;
    preorderRecursive(root, recursivePreorder);
    print(recursivePreorder);  // 1 2 4 5 3 6 7

    print(preorderIterative(root));   // 1 2 4 5 3 6 7
    print(inorderIterative(root));    // 4 2 5 1 6 3 7
    print(postorderIterative(root));  // 4 5 2 6 7 3 1

    cout << "---\n";

    Node *single = new Node(10);
    print(preorderIterative(single));   // 10
    print(inorderIterative(single));    // 10
    print(postorderIterative(single));  // 10

    cout << "---\n";

    print(preorderIterative(NULL));   // (empty line)
    print(inorderIterative(NULL));    // (empty line)
    print(postorderIterative(NULL));  // (empty line)

    return 0;
}

/**
 * Time complexity -> O(n), every node is pushed and popped once
 * Space complexity -> O(h) for a balanced tree and O(n) for a skewed tree,
 * where h is the height
 *
 * Special cases: empty tree, one node, a left-skewed tree, a right-skewed
 * tree, duplicate values (traversal order does not depend on values), and a
 * complete tree.
 */
