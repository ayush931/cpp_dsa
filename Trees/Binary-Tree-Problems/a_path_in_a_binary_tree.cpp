/**
 * A path in a binary tree is a sequence of nodes where each pair of adjacent
 * nodes in the sequence has an edge connecting them. A node can only appear in
 * the sequence at most once. Note that the path does not need to pass through
 * the root. The path sum of a path is the sum of the node's values in the path.
 * Given the root of a binary tree, return the maximum path sum of any non-empty
 * path.
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

class Solution {
   public:
    int maxSumPathHelper(TreeNode *root, int &maxSum) {
        if (root == NULL) {
            return 0;
        }

        // ignore negative branches: taking them would only reduce the sum
        int leftMaxSum = max(0, maxSumPathHelper(root->left, maxSum));
        int rightMaxSum = max(0, maxSumPathHelper(root->right, maxSum));

        // best path that uses this node as the highest point
        maxSum = max(maxSum, root->val + leftMaxSum + rightMaxSum);

        // return the best downward path from this node
        return root->val + max(leftMaxSum, rightMaxSum);
    };

    int maxPathSum(TreeNode *root) {
        int maxSum = INT_MIN;
        maxSumPathHelper(root, maxSum);
        return maxSum;
    }
};

int main() {
    TreeNode *first = new TreeNode(1);
    first->left = new TreeNode(2);
    first->right = new TreeNode(3);

    TreeNode *second = new TreeNode(-10);
    second->left = new TreeNode(9);
    second->right = new TreeNode(20);
    second->right->left = new TreeNode(15);
    second->right->right = new TreeNode(7);

    Solution solution;
    cout << solution.maxPathSum(first) << endl;   // 6 (2 + 1 + 3)
    cout << solution.maxPathSum(second) << endl;  // 42 (15 + 20 + 7)

    TreeNode *negative = new TreeNode(-3);
    cout << solution.maxPathSum(negative) << endl;  // -3

    return 0;
}

// Special cases: all negative values (the answer is the least negative node),
// a single node, a path that goes through the root, a path entirely inside
// one subtree, empty tree (not allowed by the constraints)
