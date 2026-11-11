/**
 * Balanced Binary Tree
 *
 * Given a binary tree, determine if it is height-balanced. A height-balanced
 * tree is one where the left and right subtrees of every node differ in height
 * by at most 1.
 *
 * Example:
 * Input:  [3, 9, 20, null, null, 15, 7]
 * Output: true
 *
 * Input:  [1, 2, 2, 3, 3, null, null, 4, 4]
 * Output: false
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
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

int heightBrute(Node *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + max(heightBrute(root->left), heightBrute(root->right));
}

// Brute force: check every node and recompute its height -> O(n^2)
bool isBalancedBrute(Node *root) {
    if (root == NULL) {
        return true;
    }

    int leftHeight = heightBrute(root->left);
    int rightHeight = heightBrute(root->right);

    if (abs(leftHeight - rightHeight) > 1) {
        return false;
    }

    return isBalancedBrute(root->left) && isBalancedBrute(root->right);
}

// Optimized: return -1 as soon as a subtree is unbalanced -> O(n)
int checkHeight(Node *root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1) {
        return -1;
    }

    if (abs(leftHeight - rightHeight) > 1) {
        return -1;
    }

    return 1 + max(leftHeight, rightHeight);
}

bool isBalanced(Node *root) {
    return checkHeight(root) != -1;
}

int main() {
    Node *balanced = new Node(3);
    balanced->left = new Node(9);
    balanced->right = new Node(20);
    balanced->right->left = new Node(15);
    balanced->right->right = new Node(7);

    Node *unbalanced = new Node(1);
    unbalanced->left = new Node(2);
    unbalanced->right = new Node(2);
    unbalanced->left->left = new Node(3);
    unbalanced->left->right = new Node(3);
    unbalanced->left->left->left = new Node(4);
    unbalanced->left->left->right = new Node(4);

    cout << isBalancedBrute(balanced) << " " << isBalanced(balanced) << endl;
    // 1 1

    cout << isBalancedBrute(unbalanced) << " " << isBalanced(unbalanced)
         << endl;
    // 0 0

    cout << isBalanced(NULL) << endl;  // 1

    Node *single = new Node(1);
    cout << isBalanced(single) << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   optimized -> O(n) time, O(h) recursion space
 *
 * Special cases: empty tree (balanced), single node, a tree that is balanced
 * at the root but not in a subtree, a skewed tree (unbalanced), duplicate
 * values, a complete tree.
 */
