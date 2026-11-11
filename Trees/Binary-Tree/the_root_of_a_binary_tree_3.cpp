/**
 * Given the root of a binary tree, imagine yourself standing on the right side
 * of it, return the values of the nodes you can see from top to bottom.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        left = right = NULL;
    }
};

vector<int> rightViewBinaryTree(Node *root) {
    vector<int> ans;

    if (root == NULL) {
        return ans;
    }

    queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
        int nodesAtCurrentLevel = q.size();

        while (nodesAtCurrentLevel) {
            Node *currNode = q.front();
            q.pop();

            if (nodesAtCurrentLevel == 1) {
                ans.push_back(currNode->value);
            }

            if (currNode->left) {
                q.push(currNode->left);
            }

            if (currNode->right) {
                q.push(currNode->right);
            }

            nodesAtCurrentLevel--;
        }
    }

    return ans;
};

int main() {
    Node *rootNode = new Node(2);
    rootNode->left = new Node(4);
    rootNode->right = new Node(10);
    rootNode->left->left = new Node(6);
    rootNode->left->right = new Node(5);
    rootNode->right->right = new Node(11);

    vector<int> ans = rightViewBinaryTree(rootNode);
    for (auto i : ans) {
        cout << i << endl;
    }

    return 0;
}