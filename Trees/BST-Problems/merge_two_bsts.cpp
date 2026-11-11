/**
 * Merge Two Binary Search Trees
 *
 * Given two BSTs, return all their values in sorted order. The trees may have
 * different shapes and sizes. This is the first step toward building a single
 * balanced BST from both trees.
 *
 * Example:
 * Tree 1:      5          Tree 2:      4
 *             / \                     / \
 *            3   8                   7   9
 *
 * Merged values: 3 4 5 7 8 9
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    Node *left;
    Node *right;

    Node(int v) {
        value = v;
        left = NULL;
        right = NULL;
    }
};

void inorder(Node *root, vector<int> &values) {
    if (root == NULL) {
        return;
    }

    inorder(root->left, values);
    values.push_back(root->value);
    inorder(root->right, values);
}

// Brute force: materialize both inorder traversals and merge the arrays.
// O(m + n) time and O(m + n) extra space.
vector<int> mergeBSTsBrute(Node *first, Node *second) {
    vector<int> leftValues;
    vector<int> rightValues;

    inorder(first, leftValues);
    inorder(second, rightValues);

    vector<int> merged;
    int i = 0;
    int j = 0;

    while (i < leftValues.size() && j < rightValues.size()) {
        if (leftValues[i] <= rightValues[j]) {
            merged.push_back(leftValues[i++]);
        } else {
            merged.push_back(rightValues[j++]);
        }
    }

    while (i < leftValues.size()) {
        merged.push_back(leftValues[i++]);
    }
    while (j < rightValues.size()) {
        merged.push_back(rightValues[j++]);
    }

    return merged;
}

void pushLeftSpine(Node *root, stack<Node *> &pending) {
    while (root != NULL) {
        pending.push(root);
        root = root->left;
    }
}

// Optimized: run both inorder traversals at the same time with two stacks.
// O(m + n) time and O(h1 + h2) extra space.
vector<int> mergeBSTs(Node *first, Node *second) {
    stack<Node *> leftStack;
    stack<Node *> rightStack;

    pushLeftSpine(first, leftStack);
    pushLeftSpine(second, rightStack);

    vector<int> merged;

    while (!leftStack.empty() || !rightStack.empty()) {
        if (rightStack.empty() ||
            (!leftStack.empty() &&
             leftStack.top()->value <= rightStack.top()->value)) {
            Node *current = leftStack.top();
            leftStack.pop();

            merged.push_back(current->value);
            pushLeftSpine(current->right, leftStack);
        } else {
            Node *current = rightStack.top();
            rightStack.pop();

            merged.push_back(current->value);
            pushLeftSpine(current->right, rightStack);
        }
    }

    return merged;
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    Node *first = new Node(5);
    first->left = new Node(3);
    first->right = new Node(8);

    Node *second = new Node(4);
    second->left = new Node(7);
    second->right = new Node(9);

    print(mergeBSTsBrute(first, second));  // 3 4 5 7 8 9
    print(mergeBSTs(first, second));       // 3 4 5 7 8 9

    cout << "---\n";

    print(mergeBSTs(first, NULL));   // 3 5 8
    print(mergeBSTs(NULL, second));  // 4 7 9
    print(mergeBSTs(NULL, NULL));    // (empty line)

    cout << "---\n";

    Node *duplicate = new Node(5);
    print(mergeBSTs(first, duplicate));  // 3 5 5 8

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(m + n) after both inorder traversals
 *   optimized -> O(m + n), every node is visited once
 * Space complexity:
 *   brute -> O(m + n) for the two arrays
 *   optimized -> O(h1 + h2) for the two stacks
 *
 * Special cases: one tree empty, both trees empty, duplicate values across
 * the trees, very different heights, and a skewed tree.
 */
