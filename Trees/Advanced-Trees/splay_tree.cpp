/**
 * Splay tree -> self balancing BST where every accessed node is moved to the
 * root The move is called splaying and uses zig, zig-zig and zig-zag rotations
 * Amortized cost per operation -> O(log n)
 * Good for locality: recently accessed keys stay near the root
 * Used in caches and some memory allocators
 */

#include <iostream>
using namespace std;

class Node {
   public:
    int key;
    Node *left;
    Node *right;

    Node(int k) {
        key = k;
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

Node *splay(Node *root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (key < root->key) {
        if (root->left == NULL) {
            return root;
        }

        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL) {
                root->left = rotateLeft(root->left);
            }
        }

        return (root->left == NULL) ? root : rotateRight(root);
    } else {
        if (root->right == NULL) {
            return root;
        }

        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        } else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL) {
                root->right = rotateRight(root->right);
            }
        }

        return (root->right == NULL) ? root : rotateLeft(root);
    }
};

Node *insert(Node *root, int key) {
    if (root == NULL) {
        return new Node(key);
    }

    root = splay(root, key);

    if (root->key == key) {
        return root;
    }

    Node *newNode = new Node(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
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

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);

    inorder(root);
    cout << endl;

    root = splay(root, 20);
    cout << root->key << endl;  // 20

    return 0;
}

// Special cases: empty tree, duplicate keys, accessing the minimum or maximum,
// splaying a missing key (last accessed node comes to the root)
