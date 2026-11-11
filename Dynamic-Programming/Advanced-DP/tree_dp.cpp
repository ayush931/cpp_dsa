/**
 * Tree DP -> run dynamic programming on a tree
 * The answer for a node is built from the answers of its children
 *
 * Problem: maximum independent set on a tree
 * Choose nodes such that no two chosen nodes are adjacent, maximize the sum.
 *
 * Brute force -> try every subset of nodes, O(2^n)
 * Optimized -> tree DP, O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

class Node {
   public:
    int value;
    vector<Node *> children;

    Node(int v) {
        value = v;
    }
};

// returns {maximum when root is included, maximum when root is excluded}
pair<int, int> maxIndependentSet(Node *root) {
    if (root == NULL) {
        return {0, 0};
    }

    int include = root->value;
    int exclude = 0;

    for (Node *child : root->children) {
        pair<int, int> childResult = maxIndependentSet(child);
        int childInclude = childResult.first;
        int childExclude = childResult.second;

        include += childExclude;
        exclude += max(childInclude, childExclude);
    }

    return {include, exclude};
};

int bruteForce(Node *root, bool parentIncluded) {
    if (root == NULL) {
        return 0;
    }

    int exclude = 0;
    for (Node *child : root->children) {
        exclude += bruteForce(child, false);
    }

    if (parentIncluded) {
        return exclude;
    }

    int include = root->value;
    for (Node *child : root->children) {
        include += bruteForce(child, true);
    }

    return max(include, exclude);
};

int main() {
    // Nodes live on the stack; the children vectors only store their addresses.
    Node root(10);
    Node left(20);
    Node right(30);
    Node leftChild(40);
    Node rightChild(50);

    root.children = {&left, &right};
    left.children = {&leftChild};
    right.children = {&rightChild};

    pair<int, int> result = maxIndependentSet(&root);
    cout << max(result.first, result.second) << endl;  // 100

    cout << bruteForce(&root, false) << endl;  // 100

    return 0;
}

// Special cases: empty tree, single node, negative node values
// Tree DP is also used for tree diameter, subtree sums and rerooting problems
