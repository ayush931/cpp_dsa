/**
 * Morris Traversal (O(1) Extra Space)
 *
 * Morris traversal temporarily threads each node's rightmost left-subtree node
 * back to the current node. This replaces the recursion stack (or explicit
 * stack) with temporary pointers, giving O(1) extra space. The threads are
 * removed before moving on, so the tree is restored.
 *
 * Example tree:
 *         4
 *        / \
 *       2   6
 *      / \ / \
 *     1  3 5  7
 *
 * Inorder:  1 2 3 4 5 6 7
 * Preorder: 4 2 1 3 6 5 7
 */

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

vector<int> morrisInorder(Node *root) {
    vector<int> result;
    Node *current = root;

    while (current != NULL) {
        if (current->left == NULL) {
            result.push_back(current->value);
            current = current->right;
        } else {
            // Find the rightmost node in the left subtree.
            Node *predecessor = current->left;
            while (predecessor->right != NULL &&
                   predecessor->right != current) {
                predecessor = predecessor->right;
            }

            if (predecessor->right == NULL) {
                // Create the thread and move left.
                predecessor->right = current;
                current = current->left;
            } else {
                // The left subtree is finished: remove the thread, visit, go
                // right.
                predecessor->right = NULL;
                result.push_back(current->value);
                current = current->right;
            }
        }
    }

    return result;
}

vector<int> morrisPreorder(Node *root) {
    vector<int> result;
    Node *current = root;

    while (current != NULL) {
        if (current->left == NULL) {
            result.push_back(current->value);
            current = current->right;
        } else {
            Node *predecessor = current->left;
            while (predecessor->right != NULL &&
                   predecessor->right != current) {
                predecessor = predecessor->right;
            }

            if (predecessor->right == NULL) {
                // Visit before going left, then create the thread.
                result.push_back(current->value);
                predecessor->right = current;
                current = current->left;
            } else {
                // Left subtree is finished; remove the thread and go right.
                predecessor->right = NULL;
                current = current->right;
            }
        }
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
    Node *root = new Node(4);
    root->left = new Node(2);
    root->right = new Node(6);
    root->left->left = new Node(1);
    root->left->right = new Node(3);
    root->right->left = new Node(5);
    root->right->right = new Node(7);

    print(morrisInorder(root));   // 1 2 3 4 5 6 7
    print(morrisPreorder(root));  // 4 2 1 3 6 5 7

    // The threads must be gone, so a second inorder traversal still works.
    print(morrisInorder(root));  // 1 2 3 4 5 6 7

    cout << "---\n";

    Node *single = new Node(42);
    print(morrisInorder(single));   // 42
    print(morrisPreorder(single));  // 42

    cout << "---\n";

    print(morrisInorder(NULL));   // (empty line)
    print(morrisPreorder(NULL));  // (empty line)

    return 0;
}

/**
 * Time complexity -> O(n): every edge is traversed a constant number of times
 * Space complexity -> O(1) extra space; the output vector is not counted
 *
 * Special cases: empty tree, one node, a left-skewed tree, a right-skewed
 * tree, duplicate values, and running the traversal twice to confirm the
 * temporary threads were removed.
 */
