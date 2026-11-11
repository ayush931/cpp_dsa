/**
 * B-tree -> self balancing search tree optimized for disk access
 * Every node can have many keys and children
 * All leaves are at the same level
 * A B-tree of order m has at most m children and at least ceil(m / 2) children
 * Used in databases and file systems
 *
 * B+ tree -> all keys are stored in the leaves, internal nodes only guide the
 * search Leaves are linked, so range queries are efficient Most database
 * indexes use B+ trees
 *
 * Search, insert and delete -> O(log n)
 */

#include <iostream>
#include <vector>
using namespace std;

class BTreeNode {
   public:
    vector<int> keys;
    vector<BTreeNode *> children;
    bool isLeaf;

    BTreeNode(bool leaf) {
        isLeaf = leaf;
    }
};

bool search(BTreeNode *root, int key) {
    if (root == NULL) {
        return false;
    }

    int i = 0;
    while (i < root->keys.size() && key > root->keys[i]) {
        i++;
    }

    if (i < root->keys.size() && root->keys[i] == key) {
        return true;
    }

    if (root->isLeaf) {
        return false;
    }

    return search(root->children[i], key);
};

int main() {
    BTreeNode *root = new BTreeNode(false);
    root->keys = {10, 20};

    BTreeNode *left = new BTreeNode(true);
    left->keys = {1, 5, 8};

    BTreeNode *middle = new BTreeNode(true);
    middle->keys = {12, 15, 18};

    BTreeNode *right = new BTreeNode(true);
    right->keys = {22, 25, 30};

    root->children = {left, middle, right};

    cout << search(root, 15) << endl;  // 1
    cout << search(root, 17) << endl;  // 0

    return 0;
}

// Special cases: empty tree, duplicate keys, minimum and maximum keys,
// root with one child, deletion causing underflow and merging
