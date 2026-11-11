/**
 * Path Sum III: count DOWNWARD paths summing to target; paths may start/end
 * anywhere. Prefix frequencies contain only ancestors of the current node.
 * Count prefix-target, add current prefix, recurse, then BACKTRACK its count
 * so sibling branches cannot form invalid cross-branch paths.
 * Expected O(n) time (hash worst O(n^2)), O(h) active prefixes + recursion.
 * Sums/differences fit long long; skewed large trees need iterative DFS.
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
using namespace std;

struct TreeNode {
    int value;
    TreeNode *left = nullptr, *right = nullptr;
};

long long countFrom(TreeNode *node, long long prefix, long long target,
                    unordered_map<long long, long long> &frequency) {
    if (!node) {
        return 0;
    }
    prefix += node->value;
    auto found = frequency.find(prefix - target);
    long long answer = found == frequency.end() ? 0 : found->second;
    ++frequency[prefix];
    answer += countFrom(node->left, prefix, target, frequency);
    answer += countFrom(node->right, prefix, target, frequency);
    if (--frequency[prefix] == 0) {
        frequency.erase(prefix);
    }
    return answer;
}

long long pathSum(TreeNode *root, long long target) {
    unordered_map<long long, long long> frequency{{0, 1}};
    return countFrom(root, 0, target, frequency);
}

int main() {
    TreeNode three{3}, negativeTwo{-2}, one{1};
    TreeNode leftThree{3, &three, &negativeTwo}, two{2, nullptr, &one};
    TreeNode five{5, &leftThree, &two}, eleven{11},
        negativeThree{-3, nullptr, &eleven};
    TreeNode root{10, &five, &negativeThree};
    assert(pathSum(&root, 8) == 3);
    TreeNode left{0}, right{0}, zero{0, &left, &right};
    assert(pathSum(&zero, 0) == 5);  // excludes a left->root->right path
    assert(pathSum(&root, 8) == 3);  // fresh state on repeated calls
    assert(pathSum(nullptr, 0) == 0);
    cout << pathSum(&root, 8) << '\n';  // 3
}
