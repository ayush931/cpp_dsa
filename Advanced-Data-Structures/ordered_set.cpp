/**
 * Order statistic tree (portable treap)
 *
 * Supports:
 *   - insert(x)          -> add x
 *   - erase(x)           -> remove x
 *   - findByOrder(k)     -> the k-th smallest element (0-based)
 *   - orderOfKey(x)      -> number of elements strictly smaller than x
 *
 * All operations -> O(log n) expected.
 *
 * The GNU PBDS `tree` container provides the same operations, but it is a
 * GCC extension and is not available in standard C++ or on clang. This file
 * implements a treap instead: a binary search tree where every node also has
 * a random priority, and the heap property on priorities keeps the tree
 * balanced with high probability.
 *
 * Each node stores the size of its subtree, which is what makes the order
 * queries possible.
 */

#include <cstdint>
#include <iostream>
using namespace std;

class OrderedSet {
    struct Node {
        int key;
        uint32_t priority;
        int size;
        Node *left;
        Node *right;

        Node(int value, uint32_t p) {
            key = value;
            priority = p;
            size = 1;
            left = nullptr;
            right = nullptr;
        }
    };

    Node *root;
    uint32_t randomState;

    uint32_t nextRandom() {
        // xorshift32: deterministic, fast, and good enough for a demo.
        randomState ^= randomState << 13;
        randomState ^= randomState >> 17;
        randomState ^= randomState << 5;
        return randomState;
    }

    int sizeOf(Node *node) {
        return (node == nullptr) ? 0 : node->size;
    }

    void update(Node *node) {
        if (node != nullptr) {
            node->size = 1 + sizeOf(node->left) + sizeOf(node->right);
        }
    }

    Node *rotateRight(Node *node) {
        Node *newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        update(node);
        update(newRoot);

        return newRoot;
    }

    Node *rotateLeft(Node *node) {
        Node *newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        update(node);
        update(newRoot);

        return newRoot;
    }

    Node *insert(Node *node, int key) {
        if (node == nullptr) {
            return new Node(key, nextRandom());
        }

        if (key < node->key) {
            node->left = insert(node->left, key);

            if (node->left->priority > node->priority) {
                node = rotateRight(node);
            }
        } else if (key > node->key) {
            node->right = insert(node->right, key);

            if (node->right->priority > node->priority) {
                node = rotateLeft(node);
            }
        }
        // Equal keys are ignored: this is a set, not a multiset.

        update(node);
        return node;
    }

    Node *erase(Node *node, int key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            node->left = erase(node->left, key);
        } else if (key > node->key) {
            node->right = erase(node->right, key);
        } else {
            if (node->left == nullptr) {
                Node *rightChild = node->right;
                delete node;
                return rightChild;
            }

            if (node->right == nullptr) {
                Node *leftChild = node->left;
                delete node;
                return leftChild;
            }

            // Rotate the target down until it has at most one child.
            if (node->left->priority > node->right->priority) {
                node = rotateRight(node);
                node->right = erase(node->right, key);
            } else {
                node = rotateLeft(node);
                node->left = erase(node->left, key);
            }
        }

        update(node);
        return node;
    }

    int orderOfKey(Node *node, int key) {
        if (node == nullptr) {
            return 0;
        }

        if (key <= node->key) {
            return orderOfKey(node->left, key);
        }

        return sizeOf(node->left) + 1 + orderOfKey(node->right, key);
    }

    int findByOrder(Node *node, int index) {
        if (node == nullptr) {
            return -1;
        }

        int leftSize = sizeOf(node->left);

        if (index < leftSize) {
            return findByOrder(node->left, index);
        }
        if (index == leftSize) {
            return node->key;
        }

        return findByOrder(node->right, index - leftSize - 1);
    }

    void destroy(Node *node) {
        if (node == nullptr) {
            return;
        }

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

   public:
    OrderedSet() {
        root = nullptr;
        randomState = 2463534242u;
    }

    ~OrderedSet() {
        destroy(root);
    }

    OrderedSet(const OrderedSet &) = delete;
    OrderedSet &operator=(const OrderedSet &) = delete;

    void insert(int key) {
        root = insert(root, key);
    }

    void erase(int key) {
        root = erase(root, key);
    }

    // 0-based; returns -1 when the index is out of range.
    int findByOrder(int index) {
        if (index < 0 || index >= sizeOf(root)) {
            return -1;
        }
        return findByOrder(root, index);
    }

    // Number of elements strictly smaller than key.
    int orderOfKey(int key) {
        return orderOfKey(root, key);
    }

    int size() {
        return sizeOf(root);
    }
};

int main() {
    OrderedSet values;

    values.insert(10);
    values.insert(20);
    values.insert(30);
    values.insert(40);

    cout << values.findByOrder(1) << endl;  // 20
    cout << values.orderOfKey(30) << endl;  // 2
    cout << values.orderOfKey(10) << endl;  // 0
    cout << values.orderOfKey(45) << endl;  // 4

    values.erase(20);
    cout << values.orderOfKey(30) << endl;  // 1
    cout << values.size() << endl;          // 3

    cout << values.findByOrder(0) << endl;   // 10
    cout << values.findByOrder(2) << endl;   // 40
    cout << values.findByOrder(3) << endl;   // -1 (out of range)
    cout << values.findByOrder(-1) << endl;  // -1

    values.insert(10);              // duplicate is ignored
    cout << values.size() << endl;  // 3

    return 0;
}

/**
 * Time complexity -> O(log n) expected for insert, erase, findByOrder, and
 * orderOfKey; O(n) worst case if the random priorities happen to be monotonic.
 * Space complexity -> O(n) for the nodes, O(log n) expected recursion depth.
 *
 * Special cases: an empty set, an out-of-range order index, duplicate keys
 * (ignored here; use a count or a pair<int, int> for a multiset), erasing a
 * missing key, and deep recursion if the tree becomes unbalanced.
 *
 * Alternatives:
 *   - GNU PBDS tree with tree_order_statistics_node_update (GCC only)
 *   - a Fenwick tree over coordinate-compressed values
 *   - a balanced BST with subtree sizes (AVL, red-black, splay)
 *   - a skip list with a width field per link
 */
