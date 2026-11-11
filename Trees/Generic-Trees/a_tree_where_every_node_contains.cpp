/**
 * Given a Tree where every node contains variable number of children, convert
 * the tree to its mirror.
 */

#include <bits/stdc++.h>
using namespace std;

class Node {
   public:
    int data;
    vector<Node *> children;

    Node(int data) {
        this->data = data;
    }
};

void levelOrderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }

    queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
        int currentLevelNodes = q.size();
        while (currentLevelNodes--) {
            // dequeue
            Node *curr = q.front();
            q.pop();
            cout << curr->data << " ";

            // enqueue all child nodes
            for (Node *child : curr->children) {
                q.push(child);
            }
        }
        cout << endl;
    }
};

void mirrorTree(Node *root) {
    if (root == NULL) {
        return;
    }

    reverse(root->children.begin(), root->children.end());

    // Recurse even when there is only one child; its own subtree still needs
    // to be mirrored.
    for (Node *child : root->children) {
        mirrorTree(child);
    }
};

int main() {
    Node *root = new Node(10);
    root->children.push_back(new Node(2));
    root->children.push_back(new Node(34));
    root->children.push_back(new Node(56));
    root->children.push_back(new Node(100));
    root->children[1]->children.push_back(new Node(1));
    root->children[1]->children.push_back(new Node(7));
    root->children[1]->children.push_back(new Node(8));
    root->children[1]->children.push_back(new Node(9));

    levelOrderTraversal(root);
    cout << endl;
    mirrorTree(root);
    cout << endl;
    levelOrderTraversal(root);

    return 0;
}

/**
 * Time complexity -> O(n)
 * Space complexity -> O(n) + O(m) -> max number of nodes at a level
 */