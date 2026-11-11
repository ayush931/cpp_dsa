/**
 * Given an array of integers, replace every element with the least greater
 * element on its right side in the array. if there are no greater elements on
 * the right side, replace it with -1. Input: 8, 3, 10, 1, 6, 9, 14 Output: 9,
 * 6, 14, 6, 9, 14, -1
 *
 * For 3, the elements on its right are 10, 1, 6, 9, 14; the least one
 * greater than 3 is 6.
 */

#include <bits/stdc++.h>
using namespace std;

class TreeNode {
   public:
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int v) {
        val = v;
        left = right = NULL;
    }
};

TreeNode *insertNode(TreeNode *root, int element, int &successor) {
    if (root == NULL) {
        return new TreeNode(element);
    }

    if (element < root->val) {
        successor = root->val;
        root->left = insertNode(root->left, element, successor);
    } else if (element > root->val) {
        root->right = insertNode(root->right, element, successor);
    }

    return root;
};

void replaceWithLeastGreaterElement(vector<int> &v) {
    TreeNode *root = NULL;
    for (int i = v.size() - 1; i >= 0; i--) {
        int successor = -1;
        root = insertNode(root, v[i], successor);
        v[i] = successor;
    }
};

int main() {
    cout << "Enter input - " << endl;
    int n;
    cin >> n;
    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    replaceWithLeastGreaterElement(v);

    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}