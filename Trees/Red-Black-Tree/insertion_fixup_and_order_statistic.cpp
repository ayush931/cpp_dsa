/**
 * Red-Black Tree: Insertion Fix-Up Walkthrough and Order Statistics
 *
 * A red-black tree is a BST with one color bit per node. The five properties
 * are listed in 01_concept.cpp. Insertion always adds a red leaf, then repairs
 * the tree with recoloring and rotations.
 *
 * Insertion fix-up has three cases (plus mirror images):
 *   1. The uncle is red -> recolor parent, uncle, and grandparent; move up.
 *   2. The uncle is black and the new node is an inner child -> rotate the
 *      parent to make it an outer child.
 *   3. The uncle is black and the new node is an outer child -> rotate the
 *      grandparent and recolor.
 *
 * Each node also stores its subtree size, so kthSmallest(k) works in
 * O(log n) by comparing k with the size of the left subtree.
 *
 * Example:
 *   insert 10, 20, 30, 15, 25, 5
 *   inorder -> 5 10 15 20 25 30
 *   kth(1) -> 5, kth(3) -> 15, kth(6) -> 30
 *
 * Time complexity  -> O(log n) for insert and kthSmallest
 * Space complexity -> O(n) for the tree, O(log n) recursion for the checker
 *
 * Deletion overview (not implemented here):
 *   - Replace a node with two children by its successor, then delete the
 *     successor, which has at most one child.
 *   - If the removed node was red, the properties still hold.
 *   - If it was black, the replacing child carries an extra black. Four cases
 *     restore the black height: red sibling, black sibling with black
 *     children, black sibling with a near red child, and black sibling with a
 *     far red child. Each case rotates and/or recolors, then either finishes
 *     or moves the extra black upward.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Color { RED, BLACK };

class Node {
   public:
    int value;
    Color color;
    int count;
    Node *left;
    Node *right;
    Node *parent;

    Node(int v) {
        value = v;
        color = RED;
        count = 1;
        left = right = parent = NULL;
    }
};

class RedBlackTree {
    Node *root;
    vector<string> log;

    int nodeCount(Node *node) {
        return (node == NULL) ? 0 : node->count;
    }

    void update(Node *node) {
        if (node == NULL) {
            return;
        }
        node->count = 1 + nodeCount(node->left) + nodeCount(node->right);
    }

    void leftRotate(Node *x) {
        Node *y = x->right;

        x->right = y->left;
        if (y->left != NULL) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NULL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;

        update(x);
        update(y);

        log.push_back("  left rotate at " + to_string(x->value));
    }

    void rightRotate(Node *x) {
        Node *y = x->left;

        x->left = y->right;
        if (y->right != NULL) {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NULL) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;

        update(x);
        update(y);

        log.push_back("  right rotate at " + to_string(x->value));
    }

    void insertFixup(Node *z) {
        while (z->parent != NULL && z->parent->color == RED) {
            Node *parent = z->parent;
            Node *grand = parent->parent;

            if (parent == grand->left) {
                Node *uncle = grand->right;

                if (uncle != NULL && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grand->color = RED;
                    log.push_back("  recolor: parent " +
                                  to_string(parent->value) + ", uncle " +
                                  to_string(uncle->value) +
                                  " -> black; grand " +
                                  to_string(grand->value) + " -> red");
                    z = grand;
                } else {
                    if (z == parent->right) {
                        z = parent;
                        leftRotate(z);
                        parent = z->parent;
                        grand = parent->parent;
                    }

                    parent->color = BLACK;
                    grand->color = RED;
                    log.push_back("  recolor: parent " +
                                  to_string(parent->value) +
                                  " -> black; grand " +
                                  to_string(grand->value) + " -> red");
                    rightRotate(grand);
                }
            } else {
                Node *uncle = grand->left;

                if (uncle != NULL && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grand->color = RED;
                    log.push_back("  recolor: parent " +
                                  to_string(parent->value) + ", uncle " +
                                  to_string(uncle->value) +
                                  " -> black; grand " +
                                  to_string(grand->value) + " -> red");
                    z = grand;
                } else {
                    if (z == parent->left) {
                        z = parent;
                        rightRotate(z);
                        parent = z->parent;
                        grand = parent->parent;
                    }

                    parent->color = BLACK;
                    grand->color = RED;
                    log.push_back("  recolor: parent " +
                                  to_string(parent->value) +
                                  " -> black; grand " +
                                  to_string(grand->value) + " -> red");
                    leftRotate(grand);
                }
            }
        }

        if (root->color != BLACK) {
            root->color = BLACK;
            log.push_back("  root " + to_string(root->value) + " -> black");
        }
    }

    bool checkProperties(Node *node, int blackCount, int &expectedBlackHeight) {
        if (node == NULL) {
            if (expectedBlackHeight == -1) {
                expectedBlackHeight = blackCount;
                return true;
            }
            return blackCount == expectedBlackHeight;
        }

        if (node->color == RED) {
            if (node->left != NULL && node->left->color == RED) {
                return false;
            }
            if (node->right != NULL && node->right->color == RED) {
                return false;
            }
        }

        if (node->color == BLACK) {
            blackCount++;
        }

        return checkProperties(node->left, blackCount, expectedBlackHeight) &&
               checkProperties(node->right, blackCount, expectedBlackHeight);
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

   public:
    RedBlackTree() {
        root = NULL;
    }

    ~RedBlackTree() {
        freeTree(root);
    }

    void insert(int value) {
        log.push_back("insert " + to_string(value));

        Node *z = new Node(value);
        Node *parent = NULL;
        Node *current = root;

        while (current != NULL) {
            parent = current;

            if (value < current->value) {
                current = current->left;
            } else if (value > current->value) {
                current = current->right;
            } else {
                delete z;
                log.back() =
                    "insert " + to_string(value) + " (duplicate ignored)";
                return;
            }
        }

        z->parent = parent;

        if (parent == NULL) {
            root = z;
        } else if (value < parent->value) {
            parent->left = z;
        } else {
            parent->right = z;
        }

        // The new node adds one element to every ancestor's subtree.
        for (Node *p = z->parent; p != NULL; p = p->parent) {
            p->count++;
        }

        insertFixup(z);
    }

    int kthSmallest(int k) {
        if (root == NULL || k < 1 || k > root->count) {
            return -1;
        }

        Node *current = root;

        while (current != NULL) {
            int leftCount = nodeCount(current->left);

            if (k == leftCount + 1) {
                return current->value;
            }
            if (k <= leftCount) {
                current = current->left;
            } else {
                k -= leftCount + 1;
                current = current->right;
            }
        }

        return -1;
    }

    bool checkProperties() {
        if (root == NULL) {
            return true;
        }
        if (root->color != BLACK) {
            return false;
        }

        int expectedBlackHeight = -1;
        return checkProperties(root, 0, expectedBlackHeight);
    }

    void printInorder() {
        inorder(root);
        cout << endl;
    }

    void printLog() {
        for (const string &line : log) {
            cout << line << endl;
        }
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    tree.insert(5);

    tree.printLog();
    // insert 10
    // insert 20
    // insert 30
    //   recolor: parent 20 -> black; grand 10 -> red
    //   left rotate at 10
    // insert 15
    //   recolor: parent 15 -> black; grand 20 -> red
    //   right rotate at 20
    // insert 25
    //   recolor: parent 25 -> black; grand 30 -> red
    //   left rotate at 30
    // insert 5

    tree.printInorder();  // 5 10 15 20 25 30

    cout << tree.kthSmallest(1) << endl;     // 5
    cout << tree.kthSmallest(3) << endl;     // 15
    cout << tree.kthSmallest(6) << endl;     // 30
    cout << tree.checkProperties() << endl;  // 1

    cout << "Operation     AVL                    Red-Black" << endl;
    cout << "Balance       height difference <= 1 black height equal" << endl;
    cout << "Search        slightly faster        slightly slower" << endl;
    cout << "Insert/Delete more rotations         fewer rotations" << endl;
    cout << "Best for      read-heavy workloads   mixed workloads" << endl;

    return 0;
}
