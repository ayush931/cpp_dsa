/**
 * Given the root of a binary tree and an integer targetSum, return the number
 * of paths where the sum of the values along the path equals targetSum. The
 * path does not need to start or end at the root or a leaf, but it must go
 * downwards (i.e., traveling only from parent nodes to child nodes)
 */

/**
 * Time and Space complexity -> O(n)
 */

#include <iostream>
#include <unordered_map>
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
    int pathSumHelper(TreeNode *root, int targetSum, long long currSum,
                      unordered_map<long long, int> &pathCount) {
        if (root == NULL) {
            return 0;
        }

        currSum += root->val;

        // paths ending at this node with the required sum
        int ansCount = pathCount[currSum - targetSum];

        pathCount[currSum]++;

        ansCount += pathSumHelper(root->left, targetSum, currSum, pathCount) +
                    pathSumHelper(root->right, targetSum, currSum, pathCount);

        // backtrack: this prefix must not be visible to other branches
        pathCount[currSum]--;

        return ansCount;
    };

    int pathSum(TreeNode *root, int targetSum) {
        unordered_map<long long, int> pathCount;
        pathCount[0] = 1;
        return pathSumHelper(root, targetSum, 0, pathCount);
    }
};

int main() {
    TreeNode *root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->right = new TreeNode(-3);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(2);
    root->right->right = new TreeNode(11);
    root->left->left->left = new TreeNode(3);
    root->left->left->right = new TreeNode(-2);
    root->left->right->right = new TreeNode(1);

    Solution solution;
    cout << solution.pathSum(root, 8) << endl;  // 3

    return 0;
}

// Special cases: empty tree, target 0 (the empty path is not counted),
// negative values, a path that starts and ends inside the tree,
// prefix sums that repeat (the map stores their count)
