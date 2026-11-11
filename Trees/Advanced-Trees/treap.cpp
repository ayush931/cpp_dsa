/**
 * Treap -> binary search tree + heap
 * Every node stores a key (BST property) and a random priority (heap property)
 * Expected height -> O(log n)
 * Search, insert and delete -> O(log n) expected
 * Simpler than AVL or red black tree, but uses randomness
 */

#include <cstdlib>
#include <iostream>
using namespace std;

class Node {
   public:
    int key;
    int priority;
    Node *left;
    Node *right;

    Node(int k) {
        key = k;
        priority = rand();
        left = right = NULL;
    }
};

Node *rotateRight(Node *root) {
    Node *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    return newRoot;
};

Node *rotateLeft(Node *root) {
    Node *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    return newRoot;
};

Node *insert(Node *root, int key) {
    if (root == NULL) {
        return new Node(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
        if (root->left->priority > root->priority) {
            root = rotateRight(root);
        }
    } else if (key > root->key) {
        root->right = insert(root->right, key);
        if (root->right->priority > root->priority) {
            root = rotateLeft(root);
        }
    }

    return root;
};

Node *erase(Node *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = erase(root->left, key);
    } else if (key > root->key) {
        root->right = erase(root->right, key);
    } else {
        if (root->left == NULL) {
            Node *right = root->right;
            delete root;
            return right;
        }

        if (root->right == NULL) {
            Node *left = root->left;
            delete root;
            return left;
        }

        if (root->left->priority > root->right->priority) {
            root = rotateRight(root);
            root->right = erase(root->right, key);
        } else {
            root = rotateLeft(root);
            root->left = erase(root->left, key);
        }
    }

    return root;
};

bool search(Node *root, int key) {
    if (root == NULL) {
        return false;
    }
    if (root->key == key) {
        return true;
    }
    if (key < root->key) {
        return search(root->left, key);
    }
    return search(root->right, key);
};

void inorder(Node *root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
};

int main() {
    Node *root = NULL;

    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);

    inorder(root);
    cout << endl;

    cout << search(root, 40) << endl;  // 1
    root = erase(root, 30);
    cout << search(root, 30) << endl;  // 0

    return 0;
}

// Special cases: duplicate keys (store a count), empty tree,
// deleting the root, unlucky random priorities
