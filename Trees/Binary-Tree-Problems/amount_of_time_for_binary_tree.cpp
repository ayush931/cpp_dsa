/**
 * Amount of Time for Binary tree to be infected
 * You are given the root of a binary tree with unique values, and an integer
 * start. At minute 0, an infection starts from the node with the value start.
 * Return the number of minutes needed for the entire tree to be infected.
 * Input: root = [1, 2, 5, 3, 4, null, 6]
 * Output: 4
 */

// Time -> O(n)
// space -> O(n)

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class TreeNode {
   public:
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) {
        val = x;
        left = NULL;
        right = NULL;
    }
};

class Solution {
   public:
    int calculateTime(TreeNode *startTime,
                      unordered_map<TreeNode *, TreeNode *> &parent) {
        if (startTime == NULL) {
            return -1;
        }

        unordered_set<TreeNode *> infected;
        queue<TreeNode *> q;
        q.push(startTime);
        infected.insert(startTime);
        int time = 0;

        while (!q.empty()) {
            int currLevelNode = q.size();
            bool infectFlag = false;

            while (currLevelNode--) {
                TreeNode *currNode = q.front();
                q.pop();

                if (currNode->left && !infected.count(currNode->left)) {
                    infectFlag = true;
                    infected.insert(currNode->left);
                    q.push(currNode->left);
                }

                if (currNode->right && !infected.count(currNode->right)) {
                    infectFlag = true;
                    infected.insert(currNode->right);
                    q.push(currNode->right);
                }

                if (parent[currNode] && !infected.count(parent[currNode])) {
                    infectFlag = true;
                    infected.insert(parent[currNode]);
                    q.push(parent[currNode]);
                }
            }

            if (infectFlag) {
                time++;
            }
        }

        return time;
    };

    TreeNode *makeParent(TreeNode *root,
                         unordered_map<TreeNode *, TreeNode *> &parent,
                         int start) {
        if (root == NULL) {
            return NULL;
        }

        queue<TreeNode *> q;
        q.push(root);
        TreeNode *startNode = NULL;

        while (!q.empty()) {
            TreeNode *currNode = q.front();
            q.pop();

            if (currNode->val == start) {
                startNode = currNode;
            }

            if (currNode->left) {
                parent[currNode->left] = currNode;
                q.push(currNode->left);
            }

            if (currNode->right) {
                parent[currNode->right] = currNode;
                q.push(currNode->right);
            }
        }

        return startNode;
    };

    int amountOfTime(TreeNode *root, int start) {
        unordered_map<TreeNode *, TreeNode *> parent;
        TreeNode *startNode = makeParent(root, parent, start);

        if (startNode == NULL) {
            return -1;  // start value not present
        }

        return calculateTime(startNode, parent);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->right = new TreeNode(6);

    Solution solution;
    cout << solution.amountOfTime(root, 3) << endl;   // 4
    cout << solution.amountOfTime(root, 1) << endl;   // 3
    cout << solution.amountOfTime(root, 99) << endl;  // -1

    return 0;
}

// Special cases: empty tree, start not present (-1), start at a leaf,
// start at the root, a single node (0 minutes), a skewed tree
