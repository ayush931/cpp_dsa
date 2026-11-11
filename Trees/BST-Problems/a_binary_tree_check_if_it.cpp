/**
 * Given a binary tree, check if it is a binary search tree or not.
 *
 * Time -> O(n) and space -> O(h)
 */

#include <climits>
#include <iostream>
using namespace std;

class TreeNode {
   public:
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) {
        val = x;
        left = NULL;
        right = NULL;
    }
};

class Solution {
   public:
    bool isValidBSTHelper(TreeNode *root, long minValue, long maxValue) {
        if (root == NULL) {
            return true;
        }

        if (root->val <= minValue || root->val >= maxValue) {
            return false;
        }

        return isValidBSTHelper(root->left, minValue, root->val) &&
               isValidBSTHelper(root->right, root->val, maxValue);
    }

    bool isValidBST(TreeNode *root) {
        return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
    }
};

// another solution using inorder traversal
class SolutionInorder {
   public:
    bool isValidBSTHelper(TreeNode *root, TreeNode *&prev) {
        if (root == NULL) {
            return true;
        }

        if (!isValidBSTHelper(root->left, prev)) {
            return false;
        }

        if (prev != NULL && root->val <= prev->val) {
            return false;
        }

        prev = root;

        return isValidBSTHelper(root->right, prev);
    }

    bool isValidBST(TreeNode *root) {
        TreeNode *prev = NULL;
        return isValidBSTHelper(root, prev);
    }
};

int main() {
    TreeNode *valid = new TreeNode(10);
    valid->left = new TreeNode(5);
    valid->right = new TreeNode(15);
    valid->right->left = new TreeNode(12);
    valid->right->right = new TreeNode(20);

    TreeNode *invalid = new TreeNode(10);
    invalid->left = new TreeNode(5);
    invalid->right = new TreeNode(15);
    invalid->right->left =
        new TreeNode(7);  // 7 is in the right subtree but < 10

    Solution solution;
    SolutionInorder inorderSolution;

    cout << solution.isValidBST(valid) << endl;           // 1
    cout << inorderSolution.isValidBST(valid) << endl;    // 1
    cout << solution.isValidBST(invalid) << endl;         // 0
    cout << inorderSolution.isValidBST(invalid) << endl;  // 0

    cout << solution.isValidBST(NULL) << endl;  // 1

    return 0;
}

// Special cases: empty tree (valid), single node (valid), duplicate values
// (not allowed in a strict BST), INT_MIN / INT_MAX values (use long bounds),
// a node that violates the BST property only with an ancestor
