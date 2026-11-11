/**
 * Persistent Segment Tree
 *
 * A normal segment tree is updated in place, so old versions are lost. A
 * persistent segment tree keeps every version by copying only the O(log n)
 * nodes on the update path. All other nodes are shared.
 *
 * Operations:
 *   - build(array) -> version 0
 *   - update(previousVersion, index, value) -> new version
 *   - query(version, left, right) -> range sum in that version
 *
 * Brute force -> copy the whole array for every version -> O(n) per update
 * Persistent tree -> O(log n) new nodes per update
 */

#include <iostream>
#include <vector>
using namespace std;

class Node {
   public:
    int sum;
    Node *left;
    Node *right;

    Node(int value = 0) {
        sum = value;
        left = NULL;
        right = NULL;
    }
};

class PersistentSegmentTree {
    int size;

    Node *build(vector<int> &values, int left, int right) {
        Node *node = new Node();

        if (left == right) {
            node->sum = values[left];
            return node;
        }

        int mid = left + (right - left) / 2;
        node->left = build(values, left, mid);
        node->right = build(values, mid + 1, right);
        node->sum = node->left->sum + node->right->sum;

        return node;
    }

    Node *update(Node *previous, int left, int right, int index, int value) {
        Node *current = new Node();

        if (left == right) {
            current->sum = value;
            return current;
        }

        int mid = left + (right - left) / 2;

        if (index <= mid) {
            current->left = update(previous->left, left, mid, index, value);
            current->right = previous->right;  // share the unchanged half
        } else {
            current->left = previous->left;
            current->right =
                update(previous->right, mid + 1, right, index, value);
        }

        current->sum = current->left->sum + current->right->sum;

        return current;
    }

    int query(Node *node, int left, int right, int queryLeft, int queryRight) {
        if (node == NULL || queryLeft > right || queryRight < left) {
            return 0;
        }

        if (queryLeft <= left && right <= queryRight) {
            return node->sum;
        }

        int mid = left + (right - left) / 2;

        return query(node->left, left, mid, queryLeft, queryRight) +
               query(node->right, mid + 1, right, queryLeft, queryRight);
    }

   public:
    Node *versionZero;

    PersistentSegmentTree(vector<int> &values) {
        size = values.size();
        versionZero = (size == 0) ? nullptr : build(values, 0, size - 1);
    }

    Node *update(Node *previousVersion, int index, int value) {
        if (size == 0) {
            return nullptr;
        }
        return update(previousVersion, 0, size - 1, index, value);
    }

    int query(Node *version, int left, int right) {
        if (size == 0 || version == nullptr) {
            return 0;
        }
        return query(version, 0, size - 1, left, right);
    }
};

// Brute force: store a full copy of the array for every version.
class BruteVersions {
    vector<vector<int>> versions;

   public:
    BruteVersions(vector<int> &values) {
        versions.push_back(values);
    }

    void update(int index, int value) {
        vector<int> next = versions.back();
        next[index] = value;
        versions.push_back(next);
    }

    int query(int version, int left, int right) {
        int sum = 0;

        for (int i = left; i <= right; i++) {
            sum += versions[version][i];
        }

        return sum;
    }
};

int main() {
    vector<int> values = {1, 2, 3, 4, 5};

    PersistentSegmentTree tree(values);
    Node *version0 = tree.versionZero;
    Node *version1 = tree.update(version0, 2, 10);  // [1, 2, 10, 4, 5]
    Node *version2 = tree.update(version1, 4, 7);   // [1, 2, 10, 4, 7]

    cout << tree.query(version0, 0, 4) << endl;  // 15
    cout << tree.query(version1, 0, 4) << endl;  // 22
    cout << tree.query(version2, 0, 4) << endl;  // 24

    cout << tree.query(version1, 2, 2) << endl;  // 10
    cout << tree.query(version0, 2, 2) << endl;  // 3

    BruteVersions brute(values);
    brute.update(2, 10);
    brute.update(4, 7);
    cout << brute.query(0, 0, 4) << " " << brute.query(1, 0, 4) << " "
         << brute.query(2, 0, 4) << endl;  // 15 22 24

    vector<int> empty;
    PersistentSegmentTree emptyTree(empty);
    cout << emptyTree.query(emptyTree.versionZero, 0, 0) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   build -> O(n)
 *   update -> O(log n) new nodes
 *   query -> O(log n)
 *   brute update -> O(n), brute query -> O(n)
 * Space complexity -> O(n + updates * log n)
 *
 * Special cases: empty array, one element, update the first or last index,
 * query a single point, query the whole range, many versions (share nodes),
 * memory usage grows with the number of updates.
 *
 * Persistent trees are used for range kth-smallest queries, versioned
 * documents, and undo/rollback systems.
 */
