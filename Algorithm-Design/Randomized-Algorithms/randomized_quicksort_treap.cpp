/**
 * Randomized quicksort and treap
 *
 * Randomized quicksort:
 *   - choose a random pivot for every partition
 *   - expected O(n log n) time, O(log n) expected recursion depth
 *   - worst case O(n^2), but the probability is tiny
 *   - not stable, in-place
 *
 * Treap:
 *   - a binary search tree ordered by key and a heap ordered by a random
 * priority
 *   - expected O(log n) insert, erase and search
 *   - the random priority keeps the tree balanced with high probability
 *
 * Edge cases: empty array, one element, duplicates, already sorted input,
 * adversarial input that defeats a fixed pivot.
 */

#include <iostream>
#include <random>
#include <vector>
using namespace std;

class RandomizedQuicksort {
    mt19937 generator;

    int partition(vector<int> &arr, int left, int right) {
        uniform_int_distribution<int> distribution(left, right);
        int pivotIndex = distribution(generator);
        swap(arr[pivotIndex], arr[right]);

        int pivot = arr[right];
        int smaller = left;

        for (int i = left; i < right; i++) {
            if (arr[i] < pivot) {
                swap(arr[i], arr[smaller]);
                smaller++;
            }
        }

        swap(arr[smaller], arr[right]);
        return smaller;
    }

    void sort(vector<int> &arr, int left, int right) {
        if (left >= right) {
            return;
        }

        int pivot = partition(arr, left, right);
        sort(arr, left, pivot - 1);
        sort(arr, pivot + 1, right);
    }

   public:
    RandomizedQuicksort() : generator(12345) {}

    void sort(vector<int> &arr) {
        sort(arr, 0, (int)arr.size() - 1);
    }
};

class Treap {
    struct Node {
        int key;
        int priority;
        Node *left;
        Node *right;

        Node(int k, int p)
            : key(k), priority(p), left(nullptr), right(nullptr) {}
    };

    Node *root;
    mt19937 generator;

    void split(Node *node, int key, Node *&left, Node *&right) {
        if (node == nullptr) {
            left = nullptr;
            right = nullptr;
            return;
        }

        if (node->key < key) {
            split(node->right, key, node->right, right);
            left = node;
        } else {
            split(node->left, key, left, node->left);
            right = node;
        }
    }

    Node *merge(Node *left, Node *right) {
        if (left == nullptr) {
            return right;
        }
        if (right == nullptr) {
            return left;
        }

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        }

        right->left = merge(left, right->left);
        return right;
    }

    Node *insert(Node *node, int key, int priority) {
        if (node == nullptr) {
            return new Node(key, priority);
        }
        if (key == node->key) {
            return node;
        }

        if (priority > node->priority) {
            Node *left;
            Node *right;
            split(node, key, left, right);
            Node *newNode = new Node(key, priority);
            newNode->left = left;
            newNode->right = right;
            return newNode;
        }

        if (key < node->key) {
            node->left = insert(node->left, key, priority);
        } else if (key > node->key) {
            node->right = insert(node->right, key, priority);
        }

        return node;
    }

    bool search(Node *node, int key) {
        if (node == nullptr) {
            return false;
        }
        if (node->key == key) {
            return true;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    void inorder(Node *node, vector<int> &output) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left, output);
        output.push_back(node->key);
        inorder(node->right, output);
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
    Treap() : root(nullptr), generator(67890) {}

    ~Treap() {
        destroy(root);
    }

    void insert(int key) {
        uniform_int_distribution<int> distribution(1, 1000000);
        root = insert(root, key, distribution(generator));
    }

    bool search(int key) {
        return search(root, key);
    }

    vector<int> inorder() {
        vector<int> output;
        inorder(root, output);
        return output;
    }
};

int main() {
    vector<int> values = {7, 3, 9, 1, 5, 8, 2, 6, 4, 0};
    RandomizedQuicksort sorter;
    sorter.sort(values);

    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;  // 0 1 2 3 4 5 6 7 8 9

    Treap treap;
    for (int value : values) {
        treap.insert(value);
    }

    vector<int> sorted = treap.inorder();
    for (int value : sorted) {
        cout << value << " ";
    }
    cout << endl;  // 0 1 2 3 4 5 6 7 8 9

    cout << treap.search(5) << endl;   // 1
    cout << treap.search(10) << endl;  // 0

    return 0;
}

// Special cases:
// - empty array -> sort does nothing
// - duplicate keys -> quicksort keeps them, treap ignores exact duplicates
// - already sorted input -> random pivots avoid the worst case with high
// probability
// - treap priorities are random, so the shape is not deterministic
