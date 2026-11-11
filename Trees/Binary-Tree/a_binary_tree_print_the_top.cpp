/**
 * Given a binary tree. Print the top view of the binary tree. Top view of the
 * binary tree is the set of nodes visible when the tree is viewed from the top.
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

vector<int> topViewBinaryTree(Node *root) {
    vector<int> ans;
    if (root == NULL) {
        return ans;
    }

    queue<pair<Node *, int>> q;
    q.push(make_pair(root, 0));
    map<int, int> m;

    while (!q.empty()) {
        int nodesAtCurrentLevel = q.size();

        while (nodesAtCurrentLevel--) {
            pair<Node *, int> p = q.front();
            Node *currNode = p.first;
            int currColumn = p.second;
            q.pop();

            if (m.find(currColumn) == m.end()) {
                m[currColumn] = currNode->value;
            }

            if (currNode->left) {
                q.push(make_pair(currNode->left, currColumn - 1));
            }

            if (currNode->right) {
                q.push(make_pair(currNode->right, currColumn + 1));
            }
        }
    }

    for (auto it : m) {
        ans.push_back(it.second);
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

    vector<int> ans = topViewBinaryTree(rootNode);
    for (auto i : ans) {
        cout << i << endl;
    }

    return 0;
}