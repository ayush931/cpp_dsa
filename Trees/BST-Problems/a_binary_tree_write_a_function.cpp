/**
 * Given a Binary Tree, write a function that returns the size of the largest
 * subtree which is also a Binary Search Tree (BST).
 * If the complete Binary Tree is a BST, then return the size of the whole tree.
 */

#include <algorithm>
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

class NodeInfo {
   public:
    bool isBST;
    int minValue;
    int maxValue;
    int size;

    NodeInfo(bool bst, int min, int max, int s) {
        isBST = bst;
        minValue = min;
        maxValue = max;
        size = s;
    }
};

class Solution {
   public:
    int ans = 0;

    NodeInfo largestBSTHelper(TreeNode *root) {
        if (root == NULL) {
            return NodeInfo(true, INT_MAX, INT_MIN, 0);
        }

        NodeInfo leftSubtree = largestBSTHelper(root->left);
        NodeInfo rightSubtree = largestBSTHelper(root->right);

        if (leftSubtree.isBST && rightSubtree.isBST &&
            root->val > leftSubtree.maxValue &&
            root->val < rightSubtree.minValue) {
            int currentSize = leftSubtree.size + rightSubtree.size + 1;
            ans = max(ans, currentSize);

            return NodeInfo(true, min(leftSubtree.minValue, root->val),
                            max(rightSubtree.maxValue, root->val), currentSize);
        }

        return NodeInfo(false, 0, 0, 0);
    };

    int largestBSTSubtree(TreeNode *root) {
        ans = 0;
        largestBSTHelper(root);
        return ans;
    }
};

int main() {
    TreeNode *root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->right = new TreeNode(15);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(8);
    root->right->right = new TreeNode(7);

    Solution solution;
    cout << solution.largestBSTSubtree(root) << endl;  // 3 (1, 5, 8)

    TreeNode *single = new TreeNode(42);
    cout << solution.largestBSTSubtree(single) << endl;  // 1

    cout << solution.largestBSTSubtree(NULL) << endl;  // 0

    return 0;
}

/**
 * Time and Space complexity -> O(n) and O(h)
 * Special cases: empty tree, single node, the whole tree is a BST,
 * duplicate values (a strict BST rejects them), negative values
 */
