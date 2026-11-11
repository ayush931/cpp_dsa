/**
 * Path Sum II
 *
 * Given the root of a binary tree and a target sum, return every root-to-leaf
 * path whose node values add up to the target. A path must start at the root
 * and end at a leaf.
 *
 * Example tree:
 *           5
 *          / \
 *         4   8
 *        /   / \
 *       11  13  4
 *      / \     / \
 *     7   2   5   1
 *
 * Input:  targetSum = 22
 * Output: [5,4,11,2], [5,8,4,5]
 *
 * Constraints: the number of nodes is in [0, 5000],
 * -1000 <= Node value, targetSum <= 1000.
 */

#include <iostream>
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

void collectPaths(Node *root, vector<int> &current,
                  vector<vector<int>> &allPaths) {
    if (root == NULL) {
        return;
    }

    current.push_back(root->value);

    if (root->left == NULL && root->right == NULL) {
        allPaths.push_back(current);
    } else {
        collectPaths(root->left, current, allPaths);
        collectPaths(root->right, current, allPaths);
    }

    current.pop_back();
}

// Brute force: collect every root-to-leaf path, then filter by sum.
// O(n * h) time and O(n * h) space in the worst case.
vector<vector<int>> pathSumBrute(Node *root, int targetSum) {
    vector<vector<int>> allPaths;
    vector<int> current;

    collectPaths(root, current, allPaths);

    vector<vector<int>> result;

    for (const vector<int> &path : allPaths) {
        int sum = 0;
        for (int value : path) {
            sum += value;
        }

        if (sum == targetSum) {
            result.push_back(path);
        }
    }

    return result;
}

// Optimized: carry the running sum and backtrack the current path.
// O(n) time and O(h) recursion space (plus the output).
void dfs(Node *root, int targetSum, int currentSum, vector<int> &current,
         vector<vector<int>> &result) {
    if (root == NULL) {
        return;
    }

    currentSum += root->value;
    current.push_back(root->value);

    if (root->left == NULL && root->right == NULL && currentSum == targetSum) {
        result.push_back(current);
    }

    dfs(root->left, targetSum, currentSum, current, result);
    dfs(root->right, targetSum, currentSum, current, result);

    current.pop_back();
}

vector<vector<int>> pathSum(Node *root, int targetSum) {
    vector<vector<int>> result;
    vector<int> current;

    dfs(root, targetSum, 0, current, result);

    return result;
}

void print(const vector<vector<int>> &paths) {
    if (paths.empty()) {
        cout << "(none)\n";
        return;
    }

    for (const vector<int> &path : paths) {
        cout << "[";
        for (int i = 0; i < path.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << path[i];
        }
        cout << "]\n";
    }
}

int main() {
    Node *root = new Node(5);
    root->left = new Node(4);
    root->right = new Node(8);
    root->left->left = new Node(11);
    root->left->left->left = new Node(7);
    root->left->left->right = new Node(2);
    root->right->left = new Node(13);
    root->right->right = new Node(4);
    root->right->right->left = new Node(5);
    root->right->right->right = new Node(1);

    print(pathSumBrute(root, 22));
    // [5,4,11,2]
    // [5,8,4,5]

    cout << "---\n";
    print(pathSum(root, 22));
    // [5,4,11,2]
    // [5,8,4,5]

    cout << "---\n";
    print(pathSum(root, 26));
    // [5,8,13]

    cout << "---\n";

    Node *single = new Node(-3);
    print(pathSum(single, -3));
    // [-3]

    print(pathSum(NULL, 0));
    // (none)

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n * h) to copy every root-to-leaf path
 *   optimized -> O(n), because every node is visited once
 * Space complexity -> O(h) recursion depth plus the result
 *
 * Special cases: empty tree, one node, negative values, target 0, a path that
 * reaches a leaf with the wrong sum, and a node with only one child (it is not
 * a leaf, so a path cannot end there).
 */
