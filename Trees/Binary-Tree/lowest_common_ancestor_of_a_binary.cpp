/**
 * Lowest Common Ancestor of a Binary Tree
 *
 * Given a binary tree, find the lowest common ancestor (LCA) of two given
 * nodes p and q. The LCA is the lowest node in the tree that has both p and q
 * as descendants (a node can be a descendant of itself).
 *
 * Example:
 * Input:  root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
 * Output: 3
 *
 * p = 5, q = 4 -> 5
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

bool findPath(Node *root, Node *target, vector<Node *> &path) {
    if (root == NULL) {
        return false;
    }

    path.push_back(root);

    if (root == target) {
        return true;
    }

    if (findPath(root->left, target, path) ||
        findPath(root->right, target, path)) {
        return true;
    }

    path.pop_back();
    return false;
}

// Brute force: store both root-to-node paths, then compare them -> O(n)
Node *lowestCommonAncestorBrute(Node *root, Node *p, Node *q) {
    vector<Node *> pathToP;
    vector<Node *> pathToQ;

    findPath(root, p, pathToP);
    findPath(root, q, pathToQ);

    Node *answer = NULL;
    int limit = min(pathToP.size(), pathToQ.size());

    for (int i = 0; i < limit; i++) {
        if (pathToP[i] != pathToQ[i]) {
            break;
        }
        answer = pathToP[i];
    }

    return answer;
}

// Optimized: post-order search -> O(n) time, O(h) space
Node *lowestCommonAncestor(Node *root, Node *p, Node *q) {
    if (root == NULL || root == p || root == q) {
        return root;
    }

    Node *leftResult = lowestCommonAncestor(root->left, p, q);
    Node *rightResult = lowestCommonAncestor(root->right, p, q);

    // p and q are in different subtrees, so root is the LCA.
    if (leftResult != NULL && rightResult != NULL) {
        return root;
    }

    // Both are in the same subtree.
    return (leftResult != NULL) ? leftResult : rightResult;
}

int main() {
    Node *root = new Node(3);
    root->left = new Node(5);
    root->right = new Node(1);
    root->left->left = new Node(6);
    root->left->right = new Node(2);
    root->right->left = new Node(0);
    root->right->right = new Node(8);
    root->left->right->left = new Node(7);
    root->left->right->right = new Node(4);

    Node *p = root->left;                                          // 5
    Node *q = root->right;                                         // 1
    cout << lowestCommonAncestorBrute(root, p, q)->value << endl;  // 3
    cout << lowestCommonAncestor(root, p, q)->value << endl;       // 3

    Node *r = root->left->right->right;                       // 4
    cout << lowestCommonAncestor(root, p, r)->value << endl;  // 5

    cout << lowestCommonAncestor(root, root->right, root->left->right)->value
         << endl;  // 3

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) time, O(n) space for the two paths
 *   optimized -> O(n) time, O(h) recursion space
 *
 * Special cases: empty tree, p == q, one node is an ancestor of the other,
 * p and q are the same node, both nodes are leaves, a skewed tree, duplicate
 * values (always compare node identity, not value).
 */
