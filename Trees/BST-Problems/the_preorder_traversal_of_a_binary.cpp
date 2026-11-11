/**
 * Given the preorder traversal of a binary search tree, construct the BST.
 * Input: Given preorder: [10, 5, 3, 8, 20, 15, 25]
 * Output: Inorder Traversal of the BST is 3, 5, 8, 10, 15, 20, 25
 */

#include <algorithm>
#include <climits>
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
    TreeNode *bstFromPreorderHelper(vector<int> &preorder, int prestart,
                                    int preend, vector<int> &inorder,
                                    int instart, int inend,
                                    unordered_map<int, int> &inmap) {
        if (prestart > preend || instart > inend) {
            return NULL;
        }

        TreeNode *root = new TreeNode(preorder[prestart]);
        int inroot_idx = inmap[preorder[prestart]];
        int leftsubtree_elements = inroot_idx - instart;

        root->left = bstFromPreorderHelper(
            preorder, prestart + 1, prestart + leftsubtree_elements, inorder,
            instart, inroot_idx - 1, inmap);
        root->right = bstFromPreorderHelper(
            preorder, prestart + leftsubtree_elements + 1, preend, inorder,
            inroot_idx + 1, inend, inmap);

        return root;
    }

    TreeNode *bstFromPreorder(vector<int> &preorder) {
        vector<int> inorder = preorder;
        sort(inorder.begin(), inorder.end());

        unordered_map<int, int> inmap;
        for (int i = 0; i < inorder.size(); i++) {
            inmap[inorder[i]] = i;
        }

        return bstFromPreorderHelper(preorder, 0, preorder.size() - 1, inorder,
                                     0, inorder.size() - 1, inmap);
    }
};

/**
 * Time complexity -> O(nlogn) + O(n)
 * Space complexity -> O(n) + O(n) + O(n)
 */

// optimal solution
class SolutionOptimal {
   public:
    TreeNode *bstFromPreorderHelper(vector<int> &preorder, int &index,
                                    int upperBound) {
        if (index >= preorder.size()) {
            return NULL;
        }

        if (preorder[index] >= upperBound) {
            return NULL;
        }

        TreeNode *root = new TreeNode(preorder[index]);
        index++;

        root->left = bstFromPreorderHelper(preorder, index, root->val);
        root->right = bstFromPreorderHelper(preorder, index, upperBound);

        return root;
    }

    TreeNode *bstFromPreorder(vector<int> &preorder) {
        int index = 0;
        return bstFromPreorderHelper(preorder, index, INT_MAX);
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
    vector<int> preorder = {10, 5, 3, 8, 20, 15, 25};

    Solution solution;
    TreeNode *first = solution.bstFromPreorder(preorder);
    inorder(first);
    cout << endl;  // 3 5 8 10 15 20 25

    SolutionOptimal optimal;
    TreeNode *second = optimal.bstFromPreorder(preorder);
    inorder(second);
    cout << endl;  // 3 5 8 10 15 20 25

    return 0;
}

/**
 * Time and space complexity -> O(N) and O(h)
 * Special cases: empty preorder, a single node, an already sorted preorder
 * (a right-skewed tree), duplicate values (not allowed in a strict BST)
 */
