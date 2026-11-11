/**
 * Write a C++ program to construct all unique binary search trees (BSTs) for
 * keys ranging from 1 to N.
 */

#include <iostream>
#include <vector>
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
    vector<TreeNode *> generateTreesHelper(int start, int end) {
        vector<TreeNode *> treesList;

        if (start > end) {
            treesList.push_back(NULL);
            return treesList;
        }

        for (int i = start; i <= end; i++) {
            vector<TreeNode *> leftSubtrees = generateTreesHelper(start, i - 1);
            vector<TreeNode *> rightSubtrees = generateTreesHelper(i + 1, end);

            for (TreeNode *leftSubtree : leftSubtrees) {
                for (TreeNode *rightSubtree : rightSubtrees) {
                    TreeNode *rootNode = new TreeNode(i);
                    rootNode->left = leftSubtree;
                    rootNode->right = rightSubtree;
                    treesList.push_back(rootNode);
                }
            }
        }

        return treesList;
    };

    vector<TreeNode *> generateTrees(int n) {
        if (n == 0) {
            return {};
        }
        return generateTreesHelper(1, n);
    }
};

void preorder(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
};

int main() {
    Solution solution;
    vector<TreeNode *> trees = solution.generateTrees(3);

    cout << trees.size() << endl;  // 5

    for (TreeNode *root : trees) {
        preorder(root);
        cout << endl;
    }

    // 1 2 3
    // 1 3 2
    // 2 1 3
    // 3 1 2
    // 3 2 1

    return 0;
}

/**
 * Time complexity -> Catalan number C(n), which is O(4^n / n^(3/2))
 * Space complexity -> O(C(n) * n) for the result
 * Special cases: n = 0 (empty list), n = 1 (one tree), large n (the count
 * grows exponentially), duplicate keys are not possible because keys are 1..N
 */
