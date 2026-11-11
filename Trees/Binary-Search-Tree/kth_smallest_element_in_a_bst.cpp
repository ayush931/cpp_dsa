/**
 * Kth Smallest Element in a BST
 *
 * Given the root of a binary search tree and an integer k, return the kth
 * smallest value (1-indexed) of all the values of the nodes in the tree.
 *
 * Example:
 * Input:  root = [3, 1, 4, null, 2], k = 1
 * Output: 1
 *
 * Constraints: 1 <= k <= number of nodes <= 10^4.
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

void inorder(Node *root, vector<int> &values) {
    if (root == NULL) {
        return;
    }

    inorder(root->left, values);
    values.push_back(root->value);
    inorder(root->right, values);
}

// Brute force: store the whole inorder traversal -> O(n) time, O(n) space
int kthSmallestBrute(Node *root, int k) {
    vector<int> values;
    inorder(root, values);

    if (k < 1 || k > values.size()) {
        return -1;
    }

    return values[k - 1];
}

// Optimized: iterative inorder, stop at the kth node -> O(h + k), O(h) space
int kthSmallest(Node *root, int k) {
    stack<Node *> nodes;
    Node *current = root;

    while (current != NULL || !nodes.empty()) {
        while (current != NULL) {
            nodes.push(current);
            current = current->left;
        }

        current = nodes.top();
        nodes.pop();

        k--;

        if (k == 0) {
            return current->value;
        }

        current = current->right;
    }

    return -1;  // k is out of range
}

int main() {
    Node *root = new Node(3);
    root->left = new Node(1);
    root->right = new Node(4);
    root->left->right = new Node(2);

    cout << kthSmallestBrute(root, 1) << endl;  // 1
    cout << kthSmallest(root, 1) << endl;       // 1

    cout << kthSmallest(root, 2) << endl;  // 2
    cout << kthSmallest(root, 3) << endl;  // 3
    cout << kthSmallest(root, 4) << endl;  // 4
    cout << kthSmallest(root, 5) << endl;  // -1 (out of range)

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) time, O(n) space
 *   iterative inorder -> O(h + k) time, O(h) space
 *   Morris traversal -> O(n) time, O(1) space (temporarily changes the tree)
 *
 * Special cases: k = 1 (minimum), k = n (maximum), a left-skewed BST, a
 * right-skewed BST, k out of range, duplicate values are not allowed in a
 * strict BST.
 */
