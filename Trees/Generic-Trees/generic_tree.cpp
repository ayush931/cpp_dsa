/**
 * Generic Tree
 */

// DFS (Depth First Search)

/**
 * Preorder Traversal:-
 * 1. Print root
 * 2. Recursively call for child nodes (Left -> Right)
 */

/**
 * Inorder Traversal:-
 * 1. Recursively visit all child nodes except last
 * 2. Print root node
 * 3. Recursively visit last child node
 */

/**
 * Breadth First Order (BFS):-
 * 1. Inorder Traversal
 */

#include <bits/stdc++.h>
using namespace std;

class Node {
   public:
    char data;
    vector<Node *> children;

    Node(char data) {
        this->data = data;
    }
};

void preorderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }

    cout << root->data << " ";

    for (Node *child : root->children) {
        preorderTraversal(child);
    }

    return;
};

void inorderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }

    int childNodes = root->children.size();
    for (int i = 0; i < childNodes - 1; i++) {
        inorderTraversal(root->children[i]);
    }

    cout << root->data << " ";

    if (childNodes > 0) {
        inorderTraversal(root->children[childNodes - 1]);
    }
};

void postorderTraversal(Node *root) {
    if (root == NULL) {
        return;
    }

    for (Node *child : root->children) {
        postorderTraversal(child);
    }

    cout << root->data << " ";
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

int main() {
    Node *root = new Node('A');
    root->children.push_back(new Node('B'));
    root->children.push_back(new Node('C'));
    root->children.push_back(new Node('D'));
    root->children[0]->children.push_back(new Node('E'));
    root->children[0]->children.push_back(new Node('F'));
    root->children[2]->children.push_back(new Node('G'));

    cout << root->data << endl;
    cout << "Printing child nodes of A: ";

    for (Node *child : root->children) {
        cout << child->data << " ";
    }
    cout << endl;

    preorderTraversal(root);
    cout << endl;
    inorderTraversal(root);
    cout << endl;
    postorderTraversal(root);
    cout << endl;
    levelOrderTraversal(root);

    return 0;
}