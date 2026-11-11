/**
 * B+ Tree
 *
 * A B+ tree is a balanced multi-way search tree designed for storage systems.
 * Unlike a plain B-tree, every key is stored in a leaf:
 *
 *   - Internal nodes contain only separator keys and child pointers.
 *   - Leaves contain the actual keys and values.
 *   - Leaves are linked left-to-right, so a range query is a short scan.
 *   - All leaves are at the same depth.
 *
 * This implementation uses order 4 (at most 3 keys and 4 children per node).
 * It supports insertion with node splitting, deletion with borrowing and
 * merging, point search, and ordered range scans.
 *
 * Time: O(log n) for search, insert, and delete; O(log n + k) for a range
 * query returning k entries.
 */

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

class BPlusTree {
   private:
    static const int ORDER = 4;

    struct Node {
        bool leaf;
        vector<int> keys;
        vector<int> values;
        vector<Node *> children;
        Node *next;

        explicit Node(bool isLeaf) : leaf(isLeaf), next(nullptr) {}
    };

    Node *root;

    int minLeafKeys() const {
        return (ORDER + 1) / 2 - 1;
    }

    int minChildren() const {
        return (ORDER + 1) / 2;
    }

    int maxKeys() const {
        return ORDER - 1;
    }

    Node *findLeaf(int key) const {
        Node *current = root;

        while (current != nullptr && !current->leaf) {
            int index = static_cast<int>(
                upper_bound(current->keys.begin(), current->keys.end(), key) -
                current->keys.begin());
            current = current->children[index];
        }

        return current;
    }

    bool insertRec(Node *node, int key, int value, int &newKey,
                   Node *&newNode) {
        if (node->leaf) {
            auto position =
                lower_bound(node->keys.begin(), node->keys.end(), key);
            int index = static_cast<int>(position - node->keys.begin());

            if (position != node->keys.end() && *position == key) {
                node->values[index] = value;
                return false;
            }

            node->keys.insert(position, key);
            node->values.insert(node->values.begin() + index, value);

            if (static_cast<int>(node->keys.size()) <= maxKeys()) {
                return false;
            }

            int middle = static_cast<int>(node->keys.size()) / 2;
            Node *right = new Node(true);

            right->keys.assign(node->keys.begin() + middle, node->keys.end());
            right->values.assign(node->values.begin() + middle,
                                 node->values.end());

            node->keys.resize(middle);
            node->values.resize(middle);

            right->next = node->next;
            node->next = right;

            newKey = right->keys.front();
            newNode = right;
            return true;
        }

        int index = static_cast<int>(
            upper_bound(node->keys.begin(), node->keys.end(), key) -
            node->keys.begin());

        int childKey = 0;
        Node *childNode = nullptr;
        bool split =
            insertRec(node->children[index], key, value, childKey, childNode);

        if (!split) {
            return false;
        }

        node->keys.insert(node->keys.begin() + index, childKey);
        node->children.insert(node->children.begin() + index + 1, childNode);

        if (static_cast<int>(node->keys.size()) <= maxKeys()) {
            return false;
        }

        int middle = static_cast<int>(node->keys.size()) / 2;
        int promoted = node->keys[middle];

        Node *right = new Node(false);
        right->keys.assign(node->keys.begin() + middle + 1, node->keys.end());
        right->children.assign(node->children.begin() + middle + 1,
                               node->children.end());

        node->keys.resize(middle);
        node->children.resize(middle + 1);

        newKey = promoted;
        newNode = right;
        return true;
    }

    void borrowFromLeft(Node *parent, int index) {
        Node *child = parent->children[index];
        Node *left = parent->children[index - 1];

        if (child->leaf) {
            child->keys.insert(child->keys.begin(), left->keys.back());
            child->values.insert(child->values.begin(), left->values.back());
            left->keys.pop_back();
            left->values.pop_back();
            parent->keys[index - 1] = child->keys.front();
        } else {
            child->keys.insert(child->keys.begin(), parent->keys[index - 1]);
            parent->keys[index - 1] = left->keys.back();
            left->keys.pop_back();

            child->children.insert(child->children.begin(),
                                   left->children.back());
            left->children.pop_back();
        }
    }

    void borrowFromRight(Node *parent, int index) {
        Node *child = parent->children[index];
        Node *right = parent->children[index + 1];

        if (child->leaf) {
            child->keys.push_back(right->keys.front());
            child->values.push_back(right->values.front());
            right->keys.erase(right->keys.begin());
            right->values.erase(right->values.begin());
            parent->keys[index] = right->keys.front();
        } else {
            child->keys.push_back(parent->keys[index]);
            parent->keys[index] = right->keys.front();
            right->keys.erase(right->keys.begin());

            child->children.push_back(right->children.front());
            right->children.erase(right->children.begin());
        }
    }

    void mergeWithLeft(Node *parent, int index) {
        Node *child = parent->children[index];
        Node *left = parent->children[index - 1];

        if (child->leaf) {
            left->keys.insert(left->keys.end(), child->keys.begin(),
                              child->keys.end());
            left->values.insert(left->values.end(), child->values.begin(),
                                child->values.end());
            left->next = child->next;
        } else {
            left->keys.push_back(parent->keys[index - 1]);
            left->keys.insert(left->keys.end(), child->keys.begin(),
                              child->keys.end());
            left->children.insert(left->children.end(), child->children.begin(),
                                  child->children.end());
        }

        parent->keys.erase(parent->keys.begin() + index - 1);
        parent->children.erase(parent->children.begin() + index);
        delete child;
    }

    void mergeWithRight(Node *parent, int index) {
        Node *child = parent->children[index];
        Node *right = parent->children[index + 1];

        if (child->leaf) {
            child->keys.insert(child->keys.end(), right->keys.begin(),
                               right->keys.end());
            child->values.insert(child->values.end(), right->values.begin(),
                                 right->values.end());
            child->next = right->next;
        } else {
            child->keys.push_back(parent->keys[index]);
            child->keys.insert(child->keys.end(), right->keys.begin(),
                               right->keys.end());
            child->children.insert(child->children.end(),
                                   right->children.begin(),
                                   right->children.end());
        }

        parent->keys.erase(parent->keys.begin() + index);
        parent->children.erase(parent->children.begin() + index + 1);
        delete right;
    }

    void fixChild(Node *parent, int index) {
        Node *child = parent->children[index];

        if (child->leaf) {
            int minimum = minLeafKeys();

            if (index > 0 &&
                static_cast<int>(parent->children[index - 1]->keys.size()) >
                    minimum) {
                borrowFromLeft(parent, index);
                return;
            }

            if (index + 1 < static_cast<int>(parent->children.size()) &&
                static_cast<int>(parent->children[index + 1]->keys.size()) >
                    minimum) {
                borrowFromRight(parent, index);
                return;
            }

            if (index > 0) {
                mergeWithLeft(parent, index);
            } else {
                mergeWithRight(parent, index);
            }

            return;
        }

        int minimum = minChildren();

        if (index > 0 &&
            static_cast<int>(parent->children[index - 1]->children.size()) >
                minimum) {
            borrowFromLeft(parent, index);
            return;
        }

        if (index + 1 < static_cast<int>(parent->children.size()) &&
            static_cast<int>(parent->children[index + 1]->children.size()) >
                minimum) {
            borrowFromRight(parent, index);
            return;
        }

        if (index > 0) {
            mergeWithLeft(parent, index);
        } else {
            mergeWithRight(parent, index);
        }
    }

    bool eraseRec(Node *node, int key, bool &found) {
        if (node->leaf) {
            auto position =
                lower_bound(node->keys.begin(), node->keys.end(), key);

            if (position == node->keys.end() || *position != key) {
                found = false;
                return false;
            }

            found = true;
            int index = static_cast<int>(position - node->keys.begin());
            node->keys.erase(position);
            node->values.erase(node->values.begin() + index);

            return static_cast<int>(node->keys.size()) < minLeafKeys();
        }

        int index = static_cast<int>(
            upper_bound(node->keys.begin(), node->keys.end(), key) -
            node->keys.begin());

        bool underflow = eraseRec(node->children[index], key, found);
        if (!underflow) {
            return false;
        }

        fixChild(node, index);

        return static_cast<int>(node->keys.size()) < minChildren() - 1;
    }

    void destroy(Node *node) {
        if (node == nullptr) {
            return;
        }

        if (!node->leaf) {
            for (Node *child : node->children) {
                destroy(child);
            }
        }

        delete node;
    }

    void collect(Node *node, vector<pair<int, int>> &entries) const {
        if (node == nullptr) {
            return;
        }

        if (node->leaf) {
            for (int i = 0; i < static_cast<int>(node->keys.size()); i++) {
                entries.push_back({node->keys[i], node->values[i]});
            }
            return;
        }

        for (Node *child : node->children) {
            collect(child, entries);
        }
    }

   public:
    BPlusTree() : root(nullptr) {}

    // Nodes have one owner. A default shallow copy would delete them twice.
    BPlusTree(const BPlusTree &) = delete;
    BPlusTree &operator=(const BPlusTree &) = delete;

    ~BPlusTree() {
        destroy(root);
    }

    bool empty() const {
        return root == nullptr || (root->leaf && root->keys.empty());
    }

    void insert(int key, int value) {
        if (root == nullptr) {
            root = new Node(true);
        }

        int newKey = 0;
        Node *newNode = nullptr;

        if (insertRec(root, key, value, newKey, newNode)) {
            Node *newRoot = new Node(false);
            newRoot->keys.push_back(newKey);
            newRoot->children.push_back(root);
            newRoot->children.push_back(newNode);
            root = newRoot;
        }
    }

    bool contains(int key) const {
        if (root == nullptr) {
            return false;
        }

        Node *leaf = findLeaf(key);
        return binary_search(leaf->keys.begin(), leaf->keys.end(), key);
    }

    int get(int key, int defaultValue = -1) const {
        if (root == nullptr) {
            return defaultValue;
        }

        Node *leaf = findLeaf(key);
        auto position = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);

        if (position == leaf->keys.end() || *position != key) {
            return defaultValue;
        }

        return leaf->values[static_cast<int>(position - leaf->keys.begin())];
    }

    bool erase(int key) {
        if (root == nullptr) {
            return false;
        }

        bool found = false;
        eraseRec(root, key, found);

        while (root != nullptr && !root->leaf && root->children.size() == 1) {
            Node *oldRoot = root;
            root = root->children.front();
            oldRoot->children.clear();
            delete oldRoot;
        }

        return found;
    }

    vector<pair<int, int>> rangeQuery(int low, int high) const {
        vector<pair<int, int>> result;
        if (root == nullptr || low > high) {
            return result;
        }

        Node *leaf = findLeaf(low);

        while (leaf != nullptr) {
            for (int i = 0; i < static_cast<int>(leaf->keys.size()); i++) {
                if (leaf->keys[i] < low) {
                    continue;
                }
                if (leaf->keys[i] > high) {
                    return result;
                }
                result.push_back({leaf->keys[i], leaf->values[i]});
            }

            if (!leaf->keys.empty() && leaf->keys.back() > high) {
                break;
            }
            leaf = leaf->next;
        }

        return result;
    }

    vector<pair<int, int>> entries() const {
        vector<pair<int, int>> result;
        collect(root, result);
        return result;
    }
};

void printEntries(const vector<pair<int, int>> &entries) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(entries.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << entries[i].first << ":" << entries[i].second;
    }

    cout << "]";
}

int main() {
    BPlusTree tree;

    for (int key = 1; key <= 20; key++) {
        tree.insert(key, key * 10);
    }

    cout << "all entries: ";
    printEntries(tree.entries());
    cout << endl;

    cout << "contains 7: " << (tree.contains(7) ? "yes" : "no") << endl;
    cout << "get 13: " << tree.get(13) << endl;
    cout << "get 99: " << tree.get(99) << endl;

    cout << "range 5..12: ";
    printEntries(tree.rangeQuery(5, 12));
    cout << endl;

    for (int key = 2; key <= 20; key += 2) {
        tree.erase(key);
    }

    cout << "after deleting evens: ";
    printEntries(tree.entries());
    cout << endl;

    cout << "contains 4: " << (tree.contains(4) ? "yes" : "no") << endl;
    cout << "contains 5: " << (tree.contains(5) ? "yes" : "no") << endl;
    cout << "range 1..20: ";
    printEntries(tree.rangeQuery(1, 20));
    cout << endl;

    for (int key = 1; key <= 20; key += 2) {
        tree.erase(key);
    }

    cout << "empty after deleting odds: " << (tree.empty() ? "yes" : "no")
         << " (root leaf is empty)" << endl;
    cout << "entries: ";
    printEntries(tree.entries());
    cout << endl;

    return 0;
}

// Expected output:
// all entries: [1:10, 2:20, ..., 20:200]
// contains 7: yes
// get 13: 130
// get 99: -1
// range 5..12: [5:50, 6:60, ..., 12:120]
// after deleting evens: [1:10, 3:30, 5:50, ..., 19:190]
// contains 4: no
// contains 5: yes
// range 1..20: [1:10, 3:30, ..., 19:190]
// empty after deleting odds: yes
// entries: []
//
// Special cases: duplicate insert (value update), deleting a missing key,
// borrowing from a left or right sibling, merging leaves, merging internal
// nodes, root collapse after deletion, and range queries that start or end
// between stored keys.
