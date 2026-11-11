/**
 * AVL Tree: Deletion, Order Statistics, Validation, Build from Sorted Array
 *
 * Every node stores height, subtree size, and subtree sum. The extra size and
 * sum fields support:
 *   - kth smallest value in O(log n)
 *   - count of values in [low, high] in O(log n)
 *   - sum of values in [low, high] in O(log n)
 *
 * Deletion uses ordinary BST deletion followed by rebalancing on the way back
 * up. Both the left-heavy and right-heavy cases handle the single and double
 * rotation variants.
 *
 * buildFromSorted picks the middle element as the root, so a sorted array of
 * n values becomes a balanced AVL tree in O(n).
 *
 * Example:
 *   sorted = {10, 20, 30, 40, 50, 60, 70}
 *   inorder -> 10 20 30 40 50 60 70
 *   kth(4)  -> 40
 *   count in [25, 55] -> 3, sum -> 120
 *   delete 40, delete 10, insert 35, insert 65
 *   inorder -> 20 30 35 50 60 65 70
 *
 * Time complexity  -> O(log n) for insert, delete, kth, and range queries
 * Space complexity -> O(n) for the tree, O(log n) recursion
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    int height;
    int count;
    long long sum;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        height = 1;
        count = 1;
        sum = v;
        left = right = NULL;
    }
};

int height(Node *node) {
    return (node == NULL) ? 0 : node->height;
}

int nodeCount(Node *node) {
    return (node == NULL) ? 0 : node->count;
}

long long nodeSum(Node *node) {
    return (node == NULL) ? 0 : node->sum;
}

void update(Node *node) {
    if (node == NULL) {
        return;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    node->count = 1 + nodeCount(node->left) + nodeCount(node->right);
    node->sum = node->value + nodeSum(node->left) + nodeSum(node->right);
}

int balanceFactor(Node *node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *middle = x->right;

    x->right = y;
    y->left = middle;

    update(y);
    update(x);

    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *middle = y->left;

    y->left = x;
    x->right = middle;

    update(x);
    update(y);

    return y;
}

Node *rebalance(Node *node) {
    update(node);

    int bf = balanceFactor(node);

    if (bf > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
        }
        return rightRotate(node);
    }

    if (bf < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
        }
        return leftRotate(node);
    }

    return node;
}

Node *insert(Node *node, int key) {
    if (node == NULL) {
        return new Node(key);
    }

    if (key < node->value) {
        node->left = insert(node->left, key);
    } else if (key > node->value) {
        node->right = insert(node->right, key);
    } else {
        return node;  // duplicates are ignored
    }

    return rebalance(node);
}

Node *minValueNode(Node *node) {
    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

Node *deleteNode(Node *node, int key) {
    if (node == NULL) {
        return NULL;
    }

    if (key < node->value) {
        node->left = deleteNode(node->left, key);
    } else if (key > node->value) {
        node->right = deleteNode(node->right, key);
    } else {
        if (node->left == NULL || node->right == NULL) {
            Node *child = node->left ? node->left : node->right;

            if (child == NULL) {
                delete node;
                return NULL;
            }

            *node = *child;  // move the child's contents into this node
            delete child;
        } else {
            Node *successor = minValueNode(node->right);
            node->value = successor->value;
            node->right = deleteNode(node->right, successor->value);
        }
    }

    return rebalance(node);
}

Node *buildFromSorted(vector<int> &values, int left, int right) {
    if (left > right) {
        return NULL;
    }

    int mid = left + (right - left) / 2;
    Node *node = new Node(values[mid]);

    node->left = buildFromSorted(values, left, mid - 1);
    node->right = buildFromSorted(values, mid + 1, right);

    update(node);
    return node;
}

int kthSmallest(Node *node, int k) {
    if (node == NULL || k < 1 || k > nodeCount(node)) {
        return -1;
    }

    int leftCount = nodeCount(node->left);

    if (k == leftCount + 1) {
        return node->value;
    }
    if (k <= leftCount) {
        return kthSmallest(node->left, k);
    }

    return kthSmallest(node->right, k - leftCount - 1);
}

int countLessEqual(Node *node, int value) {
    if (node == NULL) {
        return 0;
    }

    if (node->value <= value) {
        return nodeCount(node->left) + 1 + countLessEqual(node->right, value);
    }

    return countLessEqual(node->left, value);
}

long long sumLessEqual(Node *node, int value) {
    if (node == NULL) {
        return 0;
    }

    if (node->value <= value) {
        return nodeSum(node->left) + node->value +
               sumLessEqual(node->right, value);
    }

    return sumLessEqual(node->left, value);
}

int rangeCount(Node *root, int low, int high) {
    return countLessEqual(root, high) - countLessEqual(root, low - 1);
}

long long rangeSum(Node *root, int low, int high) {
    return sumLessEqual(root, high) - sumLessEqual(root, low - 1);
}

bool validate(Node *node, long long low, long long high) {
    if (node == NULL) {
        return true;
    }

    if (node->value <= low || node->value >= high) {
        return false;
    }
    if (abs(balanceFactor(node)) > 1) {
        return false;
    }

    if (node->height != 1 + max(height(node->left), height(node->right))) {
        return false;
    }

    if (node->count != 1 + nodeCount(node->left) + nodeCount(node->right)) {
        return false;
    }

    if (node->sum != node->value + nodeSum(node->left) + nodeSum(node->right)) {
        return false;
    }

    return validate(node->left, low, node->value) &&
           validate(node->right, node->value, high);
}

void inorder(Node *node) {
    if (node == NULL) {
        return;
    }

    inorder(node->left);
    cout << node->value << " ";
    inorder(node->right);
}

void freeTree(Node *node) {
    if (node == NULL) {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

int main() {
    vector<int> sorted = {10, 20, 30, 40, 50, 60, 70};
    Node *root = buildFromSorted(sorted, 0, (int)sorted.size() - 1);

    inorder(root);
    cout << endl;  // 10 20 30 40 50 60 70

    cout << height(root) << endl;          // 3
    cout << kthSmallest(root, 1) << endl;  // 10
    cout << kthSmallest(root, 4) << endl;  // 40
    cout << kthSmallest(root, 7) << endl;  // 70

    cout << rangeCount(root, 25, 55) << endl;              // 3
    cout << rangeSum(root, 25, 55) << endl;                // 120
    cout << validate(root, LLONG_MIN, LLONG_MAX) << endl;  // 1

    root = deleteNode(root, 40);
    root = deleteNode(root, 10);
    root = insert(root, 35);
    root = insert(root, 65);

    inorder(root);
    cout << endl;  // 20 30 35 50 60 65 70

    cout << kthSmallest(root, 3) << endl;                  // 35
    cout << rangeCount(root, 30, 60) << endl;              // 4
    cout << rangeSum(root, 30, 60) << endl;                // 175
    cout << validate(root, LLONG_MIN, LLONG_MAX) << endl;  // 1

    freeTree(root);
    return 0;
}
