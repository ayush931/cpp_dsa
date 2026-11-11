/**
 * Serialize and Deserialize a Binary Tree
 *
 * Serialization turns a tree into a string; deserialization rebuilds exactly
 * the same tree from that string. This implementation uses preorder traversal
 * with '#' marking every null child, so the structure is preserved even when
 * the tree is skewed.
 *
 * Example tree:
 *         1
 *        / \
 *       2   3
 *          / \
 *         4   5
 *
 * Serialized: 1,2,#,#,3,4,#,#,5,#,#
 */

#include <iostream>
#include <sstream>
#include <string>
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

string serialize(Node *root) {
    if (root == NULL) {
        return "#";
    }

    return to_string(root->value) + "," + serialize(root->left) + "," +
           serialize(root->right);
}

Node *deserialize(istringstream &stream) {
    string token;

    if (!getline(stream, token, ',')) {
        return NULL;
    }
    if (token == "#") {
        return NULL;
    }

    Node *root = new Node(stoi(token));
    root->left = deserialize(stream);
    root->right = deserialize(stream);

    return root;
}

Node *deserialize(const string &data) {
    istringstream stream(data);
    return deserialize(stream);
}

void preorder(Node *root, vector<int> &values) {
    if (root == NULL) {
        return;
    }

    values.push_back(root->value);
    preorder(root->left, values);
    preorder(root->right, values);
}

void inorder(Node *root, vector<int> &values) {
    if (root == NULL) {
        return;
    }

    inorder(root->left, values);
    values.push_back(root->value);
    inorder(root->right, values);
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

void deleteTree(Node *root) {
    if (root == NULL) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->right->left = new Node(4);
    root->right->right = new Node(5);

    string data = serialize(root);
    cout << data << endl;  // 1,2,#,#,3,4,#,#,5,#,#

    Node *rebuilt = deserialize(data);

    vector<int> beforePreorder;
    vector<int> afterPreorder;
    preorder(root, beforePreorder);
    preorder(rebuilt, afterPreorder);
    print(afterPreorder);  // 1 2 3 4 5

    vector<int> beforeInorder;
    vector<int> afterInorder;
    inorder(root, beforeInorder);
    inorder(rebuilt, afterInorder);
    print(afterInorder);  // 2 1 4 3 5

    cout << (beforePreorder == afterPreorder && beforeInorder == afterInorder)
         << endl;  // 1

    cout << "---\n";

    cout << serialize(NULL) << endl;  // #
    Node *empty = deserialize("#");
    cout << (empty == NULL) << endl;  // 1

    cout << "---\n";

    Node *single = new Node(7);
    string singleData = serialize(single);
    cout << singleData << endl;  // 7,#,#
    Node *singleRebuilt = deserialize(singleData);
    vector<int> singleValues;
    preorder(singleRebuilt, singleValues);
    print(singleValues);  // 7

    deleteTree(root);
    deleteTree(rebuilt);
    deleteTree(single);
    deleteTree(singleRebuilt);

    return 0;
}

/**
 * Time complexity -> O(n) to serialize and O(n) to deserialize
 * Space complexity -> O(n) for the string and O(h) recursion depth
 *
 * Special cases: empty tree ("#"), one node, a left-skewed tree, a right-
 * skewed tree, negative values, and duplicate values (the '#' markers keep
 * the structure unambiguous).
 */
