/**
 * All nodes exactly k edges from a target node in a binary tree.
 * Add parent pointers, then BFS over left/right/parent links with a visited
 * set. Identify nodes by POINTER, not value: duplicate values are allowed.
 * Expected O(n) time/space with hashing. Target not in tree or k<0 -> empty.
 * Prerequisites: tree level-order traversal and hash containers. This extends
 * the tree's BFS traversal by allowing parent edges as well as child edges.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

struct TreeNode {
    int value;
    TreeNode *left = nullptr, *right = nullptr;
};

vector<int> nodesDistanceK(TreeNode *root, TreeNode *target, int k) {
    if (!root || !target || k < 0) {
        return {};
    }
    unordered_map<TreeNode *, TreeNode *> parent{{root, nullptr}};
    queue<TreeNode *> frontier;
    frontier.push(root);
    while (!frontier.empty()) {
        TreeNode *node = frontier.front();
        frontier.pop();
        for (TreeNode *child : {node->left, node->right}) {
            if (child) {
                parent[child] = node;
                frontier.push(child);
            }
        }
    }
    if (!parent.count(target)) {
        return {};
    }
    frontier.push(target);
    unordered_set<TreeNode *> visited{target};
    for (int distance = 0; distance < k && !frontier.empty(); ++distance) {
        size_t layerSize = frontier.size();
        while (layerSize--) {
            TreeNode *node = frontier.front();
            frontier.pop();
            for (TreeNode *next : {node->left, node->right, parent.at(node)}) {
                if (next && visited.insert(next).second) {
                    frontier.push(next);
                }
            }
        }
    }
    vector<int> result;
    while (!frontier.empty()) {
        result.push_back(frontier.front()->value);
        frontier.pop();
    }
    return result;  // BFS order; sort only if an interface specifically needs
                    // it
}

int main() {
    TreeNode seven{7}, four{4}, two{2, &seven, &four}, six{6};
    TreeNode five{5, &six, &two}, zero{0}, eight{8}, one{1, &zero, &eight};
    TreeNode root{3, &five, &one}, outside{5};
    auto answer = nodesDistanceK(&root, &five, 2);
    sort(answer.begin(), answer.end());
    assert((answer == vector<int>{1, 4, 7}));
    assert((nodesDistanceK(&root, &five, 0) == vector<int>{5}));
    assert(nodesDistanceK(&root, &outside, 1).empty());
    assert(nodesDistanceK(&root, &five, 20).empty());
    assert(nodesDistanceK(nullptr, nullptr, 0).empty());
    for (int value : answer) {
        cout << value << ' ';
    }
    cout << '\n';  // 1 4 7
}
