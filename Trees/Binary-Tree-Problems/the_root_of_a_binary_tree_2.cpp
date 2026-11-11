/**
 * Given the root of a binary tree, flatten the tree into a 'linked list':
 * The 'linked list' should use the same TreeNode class where the right child
 * pointer points to the next node in the list and the left child pointer is
 * always null.
 * The 'linked list' should be in the same order as a pre-order traversal.
 *
 * Input 1: root = [1, 2, 5, 3, 4, null, 6]
 * Output 1: 1 -> 2 -> 3 -> 4 -> 5 -> 6
 */

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
    TreeNode *lastNode = NULL;

    void flattenHelper(TreeNode *root) {
        if (root == NULL) {
            return;
        }

        // process the right subtree first, then the left, and prepend each node
        // to the already flattened list -> the final order is preorder
        flattenHelper(root->right);
        flattenHelper(root->left);

        root->right = lastNode;
        root->left = NULL;

        lastNode = root;
    }

    void flatten(TreeNode *root) {
        lastNode = NULL;  // reset so the method can be called again
        flattenHelper(root);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->right = new TreeNode(6);

    Solution solution;
    solution.flatten(root);

    TreeNode *current = root;

    while (current != NULL) {
        cout << current->val << " ";
        current = current->right;
    }
    cout << endl;  // 1 2 3 4 5 6

    return 0;
}

// Time -> O(n), Space -> O(h) for the recursion
// Special cases: empty tree, single node, a left-skewed tree, a right-skewed
// tree, calling flatten twice (lastNode is reset)
