/**
 * Given two integer arrays, preorder and postorder where preorder is the
 * preorder traversal of a binary tree of distinct values and postorder is the
 * postorder traversal of the same tree, reconstruct and return the binary tree.
 * If there exists multiple answer, you can return any of them.
 *
 * Input:- preorder = [1, 2, 4, 5, 3, 6, 7], postorder = [4, 5, 2, 6, 7, 3, 1]
 * Output: [1, 2, 3, 4, 5, 6, 7]
 */

// Time and Space complexity -> O(N)

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
    TreeNode *constructFromPrePostHelper(vector<int> &preorder, int prestart,
                                         int preend, vector<int> &postorder,
                                         int poststart, int postend,
                                         unordered_map<int, int> &postmap) {
        if (prestart > preend || poststart > postend) {
            return NULL;
        }

        TreeNode *rootNode = new TreeNode(preorder[prestart]);

        if (prestart == preend) {
            return rootNode;
        }

        int leftChildValue = preorder[prestart + 1];
        int leftChildIndex = postmap[leftChildValue];
        int leftSubtreeSize = leftChildIndex - poststart + 1;

        rootNode->left = constructFromPrePostHelper(
            preorder, prestart + 1, prestart + leftSubtreeSize, postorder,
            poststart, leftChildIndex, postmap);

        rootNode->right = constructFromPrePostHelper(
            preorder, prestart + leftSubtreeSize + 1, preend, postorder,
            leftChildIndex + 1, postend - 1, postmap);

        return rootNode;
    }

    TreeNode *constructFromPrePost(vector<int> &preorder,
                                   vector<int> &postorder) {
        unordered_map<int, int> postmap;

        for (int i = 0; i < postorder.size(); i++) {
            postmap[postorder[i]] = i;
        }

        return constructFromPrePostHelper(preorder, 0, preorder.size() - 1,
                                          postorder, 0, postorder.size() - 1,
                                          postmap);
    }
};

void inorder(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
};

int main() {
    vector<int> preorder = {1, 2, 4, 5, 3, 6, 7};
    vector<int> postorder = {4, 5, 2, 6, 7, 3, 1};

    Solution solution;
    TreeNode *root = solution.constructFromPrePost(preorder, postorder);

    inorder(root);
    cout << endl;  // 4 2 5 1 6 3 7

    return 0;
}

// Special cases: empty arrays, one node, only a left child, only a right
// child, duplicate values (the problem guarantees distinct values)
