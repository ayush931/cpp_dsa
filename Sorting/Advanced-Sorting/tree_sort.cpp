/**
 * Tree sort
 *
 * Insert every element into a binary search tree, then read the tree with an
 * inorder traversal. Inorder visits left subtree, node, right subtree, so the
 * output is sorted.
 *
 * This implementation stores nodes in a vector and uses integer indices instead
 * of pointers. Duplicate values are sent to the right subtree, so equal keys
 * are visited in insertion order. That makes this particular tree sort stable
 * for equal keys, but a balanced tree with rotations can reorder equals, so
 * tree sort is not stable in general.
 *
 * Stability   -> stable here for equal keys; not stable in general
 * In-place    -> no, O(n) nodes are required
 * Time        -> O(n log n) average, O(n^2) worst when the input is already
 *                sorted and the tree becomes a chain
 * Space       -> O(n) for the nodes, plus O(n) recursion depth in the worst
 * case
 *
 * std::sort is included as the brute-force reference.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int value;
    int left;
    int right;
};

class BinarySearchTree {
   private:
    vector<Node> nodes;
    int root;

    int insert(int node, int value) {
        if (node == -1) {
            nodes.push_back({value, -1, -1});
            return (int)nodes.size() - 1;
        }

        if (value < nodes[node].value) {
            nodes[node].left = insert(nodes[node].left, value);
        } else {
            nodes[node].right = insert(nodes[node].right, value);
        }

        return node;
    }

    void inorder(int node, vector<int> &output) const {
        if (node == -1) {
            return;
        }

        inorder(nodes[node].left, output);
        output.push_back(nodes[node].value);
        inorder(nodes[node].right, output);
    }

   public:
    BinarySearchTree() : root(-1) {}

    void insert(int value) {
        root = insert(root, value);
    }

    vector<int> sorted() const {
        vector<int> output;
        output.reserve(nodes.size());
        inorder(root, output);
        return output;
    }
};

vector<int> treeSort(const vector<int> &arr) {
    BinarySearchTree tree;

    for (int value : arr) {
        tree.insert(value);
    }

    return tree.sorted();
}

void print(const vector<int> &arr) {
    cout << "[";

    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << arr[i];
    }

    cout << "]";
}

int main() {
    vector<vector<int>> tests = {{7, 3, 9, 1, 5, 8, 10},
                                 {},
                                 {4},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {3, 1, 3, 2, 1, 3},
                                 {-4, 0, -9, 7, -4}};

    for (const vector<int> &original : tests) {
        vector<int> tree = treeSort(original);
        vector<int> expected = original;

        sort(expected.begin(), expected.end());

        print(tree);
        cout << "  matches std::sort: " << (tree == expected ? "yes" : "no")
             << endl;
    }

    return 0;
}

// Expected output:
// [1, 3, 5, 7, 8, 9, 10]  matches std::sort: yes
// []  matches std::sort: yes
// [4]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 2, 3, 4, 5]  matches std::sort: yes
// [1, 1, 2, 3, 3, 3]  matches std::sort: yes
// [-9, -4, -4, 0, 7]  matches std::sort: yes
//
// Special cases: empty array, one element, sorted input (the worst case for an
// unbalanced BST), reverse sorted input, duplicates, and negative values.
