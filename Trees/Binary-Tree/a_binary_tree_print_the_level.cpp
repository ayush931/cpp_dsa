/**
 * Given a binary tree, print the level order in reverse manner i.e print the
 * last row first and then the rows below.
 */

#include <bits/stdc++.h>
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

vector<int> reverseLevelOrder(Node *root) {
    vector<int> ans;
    if (root == NULL) {
        return ans;
    }

    queue<Node *> q;
    stack<Node *> s;

    q.push(root);

    while (!q.empty()) {
        Node *currNode = q.front();
        q.pop();

        s.push(currNode);

        if (currNode->right) {
            q.push(currNode->right);
        }

        if (currNode->left) {
            q.push(currNode->left);
        }
    }

    while (!s.empty()) {
        ans.push_back(s.top()->value);
        s.pop();
    }

    return ans;
}

int main() {
    Node *rootNode = new Node(2);
    rootNode->left = new Node(4);
    rootNode->right = new Node(10);
    rootNode->left->left = new Node(6);
    rootNode->left->left->right = new Node(12);
    rootNode->left->right = new Node(5);
    rootNode->right->right = new Node(11);

    vector<int> ans = reverseLevelOrder(rootNode);

    for (auto i : ans) {
        cout << i << endl;
    }

    return 0;
}