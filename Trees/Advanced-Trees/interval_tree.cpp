/**
 * Interval Tree
 *
 * An interval tree is an augmented binary search tree keyed by interval start
 * points. Every node stores the maximum end point in its subtree (`maxHigh`).
 * That augmentation lets an overlap query discard an entire subtree when its
 * maximum end point lies before the query start.
 *
 * Operations:
 *   - insert:          O(log n) average, O(n) worst case (unbalanced BST)
 *   - overlap search:  O(n) worst case; augmentation prunes irrelevant subtrees
 *   - erase:           O(n) worst case
 *   - reporting also sorts its k matches in O(k log k) time
 *
 * A brute-force overlap scan is included for verification.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

struct Interval {
    int low;
    int high;

    bool operator==(const Interval &other) const {
        return low == other.low && high == other.high;
    }
};

bool overlaps(const Interval &a, const Interval &b) {
    return a.low <= b.high && b.low <= a.high;
}

class IntervalTree {
   private:
    struct Node {
        Interval interval;
        int maxHigh;
        unique_ptr<Node> left;
        unique_ptr<Node> right;

        explicit Node(Interval value)
            : interval(value),
              maxHigh(value.high),
              left(nullptr),
              right(nullptr) {}
    };

    unique_ptr<Node> root;

    void update(Node *node) {
        if (node == nullptr) {
            return;
        }

        node->maxHigh = node->interval.high;

        if (node->left != nullptr) {
            node->maxHigh = max(node->maxHigh, node->left->maxHigh);
        }

        if (node->right != nullptr) {
            node->maxHigh = max(node->maxHigh, node->right->maxHigh);
        }
    }

    Node *insert(Node *node, Interval value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value.low < node->interval.low) {
            node->left.reset(insert(node->left.release(), value));
        } else {
            node->right.reset(insert(node->right.release(), value));
        }

        update(node);
        return node;
    }

    const Node *search(const Node *node, const Interval &query) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (overlaps(node->interval, query)) {
            return node;
        }

        if (node->left != nullptr && node->left->maxHigh >= query.low) {
            const Node *found = search(node->left.get(), query);
            if (found != nullptr) {
                return found;
            }
        }

        if (node->interval.low > query.high) {
            return nullptr;
        }

        return search(node->right.get(), query);
    }

    void collect(const Node *node, const Interval &query,
                 vector<Interval> &result) const {
        if (node == nullptr) {
            return;
        }

        if (node->left != nullptr && node->left->maxHigh >= query.low) {
            collect(node->left.get(), query, result);
        }

        if (overlaps(node->interval, query)) {
            result.push_back(node->interval);
        }

        if (node->interval.low <= query.high) {
            collect(node->right.get(), query, result);
        }
    }

    unique_ptr<Node> erase(unique_ptr<Node> node, Interval value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value.low < node->interval.low) {
            node->left = erase(move(node->left), value);
        } else if (value.low > node->interval.low) {
            node->right = erase(move(node->right), value);
        } else if (value.high == node->interval.high) {
            if (node->left == nullptr) {
                return move(node->right);
            }
            if (node->right == nullptr) {
                return move(node->left);
            }

            Node *successor = node->right.get();
            while (successor->left != nullptr) {
                successor = successor->left.get();
            }

            int replacementLow = successor->interval.low;
            int replacementHigh = successor->interval.high;

            node->interval.low = replacementLow;
            node->interval.high = replacementHigh;
            node->right =
                erase(move(node->right), {replacementLow, replacementHigh});
        } else {
            // Intervals sharing a start point can live in either subtree.
            node->left = erase(move(node->left), value);
            node->right = erase(move(node->right), value);
        }

        update(node.get());
        return node;
    }

   public:
    void insert(Interval value) {
        root.reset(insert(root.release(), value));
    }

    bool overlapsAny(const Interval &query) const {
        return search(root.get(), query) != nullptr;
    }

    vector<Interval> overlapping(const Interval &query) const {
        vector<Interval> result;
        collect(root.get(), query, result);
        sort(result.begin(), result.end(),
             [](const Interval &a, const Interval &b) {
                 if (a.low != b.low) {
                     return a.low < b.low;
                 }
                 return a.high < b.high;
             });
        return result;
    }

    void erase(Interval value) {
        root = erase(move(root), value);
    }

    bool empty() const {
        return root == nullptr;
    }
};

vector<Interval> bruteForce(const vector<Interval> &intervals,
                            const Interval &query) {
    vector<Interval> result;

    for (const Interval &interval : intervals) {
        if (overlaps(interval, query)) {
            result.push_back(interval);
        }
    }

    sort(result.begin(), result.end(),
         [](const Interval &a, const Interval &b) {
             if (a.low != b.low) {
                 return a.low < b.low;
             }
             return a.high < b.high;
         });

    return result;
}

void printIntervals(const vector<Interval> &intervals) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(intervals.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "[" << intervals[i].low << ", " << intervals[i].high << "]";
    }

    cout << "]";
}

int main() {
    vector<Interval> intervals = {{15, 20}, {10, 30}, {17, 19},
                                  {5, 20},  {12, 15}, {30, 40}};

    IntervalTree tree;
    for (const Interval &interval : intervals) {
        tree.insert(interval);
    }

    vector<Interval> queries = {{14, 16}, {21, 25}, {0, 4}, {18, 18}, {30, 50}};

    for (const Interval &query : queries) {
        vector<Interval> actual = tree.overlapping(query);
        vector<Interval> expected = bruteForce(intervals, query);

        cout << "query [" << query.low << ", " << query.high << "]: ";
        printIntervals(actual);
        cout << "  matches: " << (actual == expected ? "yes" : "no") << endl;
    }

    tree.erase({10, 30});
    intervals.erase(
        find_if(intervals.begin(), intervals.end(), [](const Interval &value) {
            return value.low == 10 && value.high == 30;
        }));

    cout << "after deleting [10, 30]:" << endl;
    cout << "query [14, 16]: ";
    printIntervals(tree.overlapping({14, 16}));
    cout << "  expected: ";
    printIntervals(bruteForce(intervals, {14, 16}));
    cout << endl;

    tree.erase({100, 200});
    cout << "deleting a missing interval keeps the tree usable: "
         << (tree.overlapsAny({0, 100}) ? "yes" : "no") << endl;

    IntervalTree emptyTree;
    cout << "empty tree overlap: "
         << (emptyTree.overlapsAny({0, 10}) ? "yes" : "no") << endl;

    return 0;
}

// Expected output (abridged):
// query [14, 16]: [[5, 20], [10, 30], [12, 15], [15, 20]]  matches: yes
// query [21, 25]: [[10, 30]]  matches: yes
// query [0, 4]: []  matches: yes
// query [18, 18]: [[5, 20], [10, 30], [15, 20], [17, 19]]  matches: yes
// query [30, 50]: [[10, 30], [30, 40]]  matches: yes
// after deleting [10, 30]:
// query [14, 16]: [[5, 20], [12, 15], [15, 20]]
// deleting a missing interval keeps the tree usable: yes
// empty tree overlap: no
//
// Special cases: empty tree, intervals touching at an endpoint, a query that
// starts before every interval, a query after every interval, duplicate start
// points, deleting a missing interval, and deleting a node with two children.
