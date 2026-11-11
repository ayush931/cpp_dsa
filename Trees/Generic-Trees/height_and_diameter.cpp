/**
 * Height and Diameter of a Generic Tree
 *
 * A generic (N-ary) tree lets every node have any number of children. The
 * height is the number of edges on the longest root-to-leaf path. The diameter
 * is the number of edges on the longest path between any two nodes; it does
 * not have to pass through the root.
 *
 * Example tree:
 *            10
 *          /  |  \
 *         2  34   56
 *        / \      / \
 *       1   7    8   9
 *
 * Height:   2  (10 -> 2 -> 1)
 * Diameter: 4  (1 -> 2 -> 10 -> 56 -> 9)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Node {
   public:
    int data;
    vector<Node *> children;

    Node(int value) {
        data = value;
    }
};

int heightBrute(Node *root) {
    if (root == NULL) {
        return 0;
    }

    int best = 0;
    for (Node *child : root->children) {
        best = max(best, heightBrute(child));
    }

    return best + 1;  // height in nodes
}

// Brute force: recompute the height of every child for every node.
// O(n^2) time in a skewed tree.
void diameterBrute(Node *root, int &best) {
    if (root == NULL) {
        return;
    }

    int tallest = 0;
    int secondTallest = 0;

    for (Node *child : root->children) {
        int childHeight = heightBrute(child);

        if (childHeight > tallest) {
            secondTallest = tallest;
            tallest = childHeight;
        } else if (childHeight > secondTallest) {
            secondTallest = childHeight;
        }
    }

    best = max(best, tallest + secondTallest + 1);

    for (Node *child : root->children) {
        diameterBrute(child, best);
    }
}

// Optimized: one postorder pass returns the height and updates the diameter.
// O(n) time and O(h) recursion space.
int heightAndDiameter(Node *root, int &best) {
    if (root == NULL) {
        return 0;
    }

    int tallest = 0;
    int secondTallest = 0;

    for (Node *child : root->children) {
        int childHeight = heightAndDiameter(child, best);

        if (childHeight > tallest) {
            secondTallest = tallest;
            tallest = childHeight;
        } else if (childHeight > secondTallest) {
            secondTallest = childHeight;
        }
    }

    best = max(best, tallest + secondTallest + 1);

    return tallest + 1;
}

int main() {
    Node *root = new Node(10);

    Node *two = new Node(2);
    Node *thirtyFour = new Node(34);
    Node *fiftySix = new Node(56);

    two->children.push_back(new Node(1));
    two->children.push_back(new Node(7));

    fiftySix->children.push_back(new Node(8));
    fiftySix->children.push_back(new Node(9));

    root->children.push_back(two);
    root->children.push_back(thirtyFour);
    root->children.push_back(fiftySix);

    int bruteDiameter = 0;
    diameterBrute(root, bruteDiameter);

    cout << heightBrute(root) - 1 << endl;  // 2 edges
    cout << bruteDiameter - 1 << endl;      // 4 edges

    int optimizedDiameter = 0;
    int heightNodes = heightAndDiameter(root, optimizedDiameter);

    cout << heightNodes - 1 << endl;        // 2 edges
    cout << optimizedDiameter - 1 << endl;  // 4 edges

    cout << "---\n";

    Node *single = new Node(5);
    int singleDiameter = 0;
    heightAndDiameter(single, singleDiameter);
    cout << 0 << " " << singleDiameter - 1 << endl;  // 0 0

    cout << "---\n";

    Node *chain = new Node(1);
    chain->children.push_back(new Node(2));
    chain->children[0]->children.push_back(new Node(3));
    chain->children[0]->children[0]->children.push_back(new Node(4));

    int chainDiameter = 0;
    int chainHeight = heightAndDiameter(chain, chainDiameter);
    cout << chainHeight - 1 << " " << chainDiameter - 1 << endl;  // 3 3

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2) because heights are recomputed for every node
 *   optimized -> O(n), every node is processed once
 * Space complexity -> O(h) recursion depth, where h is the tree height
 *
 * Special cases: empty tree, one node, a chain (height equals diameter), a
 * star (height 1, diameter 2), and duplicate values.
 */
