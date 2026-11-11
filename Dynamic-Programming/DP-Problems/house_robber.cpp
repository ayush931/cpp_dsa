/**
 * The thief has found himself a new place for his thievery again. There is only
 * one entrance to this area, called root.
 * Besides the root, each house has one and only one parent house. After a tour,
 * the smart thief realized that all houses in this place form a binary tree. It
 * will automatically contact the police if two directly-linked houses were
 * broken into on the same night. Given the root of the binary tree, return the
 * maximum amount of money the thief can rob without alerting the police.
 *
 * Example 1: root = [3,2,3,null,3,null,1] -> 7 (3 + 3 + 1)
 * Example 2: root = [3,4,5,1,3,null,1] -> 9 (4 + 5)
 *
 * Constraints: 1 <= nodes <= 10^4, 0 <= Node.val <= 10^4
 */

#include <algorithm>
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

// Recursively release the nodes allocated in main(). The tree is small here,
// but every "new" still needs a matching "delete" to avoid a leak.
void deleteTree(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

class Solution {
   public:
    // returns {best when this node is robbed, best when it is not robbed}
    pair<int, int> robHelper(TreeNode *node) {
        if (node == NULL) {
            return {0, 0};
        }

        pair<int, int> left = robHelper(node->left);
        pair<int, int> right = robHelper(node->right);

        // rob this node -> children cannot be robbed
        int robCurrent = node->val + left.second + right.second;

        // skip this node -> children may or may not be robbed
        int skipCurrent =
            max(left.first, left.second) + max(right.first, right.second);

        return {robCurrent, skipCurrent};
    };

    int rob(TreeNode *root) {
        pair<int, int> result = robHelper(root);
        return max(result.first, result.second);
    }
};

int main() {
    TreeNode *first = new TreeNode(3);
    first->left = new TreeNode(2);
    first->right = new TreeNode(3);
    first->left->right = new TreeNode(3);
    first->right->right = new TreeNode(1);

    TreeNode *second = new TreeNode(3);
    second->left = new TreeNode(4);
    second->right = new TreeNode(5);
    second->left->left = new TreeNode(1);
    second->left->right = new TreeNode(3);
    second->right->right = new TreeNode(1);

    Solution solution;
    cout << solution.rob(first) << endl;   // 7
    cout << solution.rob(second) << endl;  // 9

    TreeNode *single = new TreeNode(5);
    cout << solution.rob(single) << endl;  // 5

    deleteTree(first);
    deleteTree(second);
    deleteTree(single);

    return 0;
}

// Time -> O(n), Space -> O(h) for the recursion
// Special cases: empty tree (0), single node, all zero values, negative
// values (skipping is allowed), a path graph, a star-shaped tree
