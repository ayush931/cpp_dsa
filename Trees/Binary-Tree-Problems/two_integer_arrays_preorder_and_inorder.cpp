/**
 * Given two integer arrays preorder and inorder where preorder is the preorder
 * traversal of a binary tree and inorder is the inorder traversal of the same
 * tree, construct and return the binary tree. Input: preorder = [3, 9, 20, 15,
 * 7], inorder = [9, 3, 15, 20, 7] Output: [3, 9, 20, null, null, 15, 7]
 */

// Time and Space complexity -> O(n)

#include <iostream>
#include <unordered_map>
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
    TreeNode *buildTreeHelper(vector<int> &preorder, int prestart, int preend,
                              vector<int> &inorder, int instart, int inend,
                              unordered_map<int, int> &inmap) {
        if (prestart > preend || instart > inend) {
            return NULL;
        }

        TreeNode *rootNode = new TreeNode(preorder[prestart]);
        int rootInorderIndex = inmap[rootNode->val];
        int leftSubtreeSize = rootInorderIndex - instart;

        rootNode->left =
            buildTreeHelper(preorder, prestart + 1, prestart + leftSubtreeSize,
                            inorder, instart, rootInorderIndex - 1, inmap);
        rootNode->right =
            buildTreeHelper(preorder, prestart + leftSubtreeSize + 1, preend,
                            inorder, rootInorderIndex + 1, inend, inmap);

        return rootNode;
    };

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
        unordered_map<int, int> inmap;
        for (int i = 0; i < inorder.size(); i++) {
            inmap[inorder[i]] = i;
        }

        return buildTreeHelper(preorder, 0, preorder.size() - 1, inorder, 0,
                               inorder.size() - 1, inmap);
    }
};

void postorder(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
};

int main() {
    vector<int> preorder = {3, 9, 20, 15, 7};
    vector<int> inorder = {9, 3, 15, 20, 7};

    Solution solution;
    TreeNode *root = solution.buildTree(preorder, inorder);

    postorder(root);
    cout << endl;  // 9 15 7 20 3

    return 0;
}

// Special cases: empty arrays, a single node, a left-skewed tree,
// a right-skewed tree, duplicate values (the map index would be ambiguous;
// the problem guarantees distinct values)
