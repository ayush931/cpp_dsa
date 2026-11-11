/**
 * AVL tree -> self balancing binary search tree
 * Balance factor of a node = height(left subtree) - height(right subtree)
 * Balance factor of every node must be -1, 0 or 1
 * If it becomes 2 or -2, rotations restore balance
 *
 * Rotations:-
 * LL -> right rotation
 * RR -> left rotation
 * LR -> left rotation on left child, then right rotation on node
 * RL -> right rotation on right child, then left rotation on node
 *
 * Search, insertion and deletion -> O(log n)
 */

#include <algorithm>
#include <iostream>
using namespace std;

class Node {
   public:
    int value;
    int height;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        height = 1;
        left = right = NULL;
    }
};

int height(Node *node) {
    return (node == NULL) ? 0 : node->height;
};

int balanceFactor(Node *node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
};

void updateHeight(Node *node) {
    node->height = 1 + max(height(node->left), height(node->right));
};

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
};

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
};

Node *insert(Node *node, int key) {
    // normal BST insertion
    if (node == NULL) {
        return new Node(key);
    }

    if (key < node->value) {
        node->left = insert(node->left, key);
    } else if (key > node->value) {
        node->right = insert(node->right, key);
    } else {
        return node;  // duplicate keys are not inserted
    }

    updateHeight(node);

    int bf = balanceFactor(node);

    // left left
    if (bf > 1 && key < node->left->value) {
        return rightRotate(node);
    }

    // right right
    if (bf < -1 && key > node->right->value) {
        return leftRotate(node);
    }

    // left right
    if (bf > 1 && key > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // right left
    if (bf < -1 && key < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
};

Node *minValueNode(Node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
};

Node *deleteNode(Node *root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->value) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->value) {
        root->right = deleteNode(root->right, key);
    } else {
        // node with 0 or 1 child
        if (root->left == NULL || root->right == NULL) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            delete temp;
        }
        // node with 2 children
        else {
            Node *temp = minValueNode(root->right);
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
    }

    if (root == NULL) {
        return root;
    }

    updateHeight(root);

    int bf = balanceFactor(root);

    if (bf > 1 && balanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    if (bf > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && balanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    if (bf < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
};

void inorder(Node *root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    cout << root->value << " ";
    inorder(root->right);
};

int main() {
    Node *root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);  // LL rotation
    root = insert(root, 40);
    root = insert(root, 50);  // RR rotation
    root = insert(root, 25);  // RL rotation

    inorder(root);
    cout << endl;

    root = deleteNode(root, 40);

    inorder(root);
    cout << endl;

    cout << "Height: " << height(root) << endl;

    return 0;
}
