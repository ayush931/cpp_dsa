/**
 * Binary lifting on a functional graph
 *
 * A functional graph gives every node exactly one outgoing edge:
 *   next[node] = successor of node
 *
 * The graph is a collection of components, each containing one directed cycle
 * with trees feeding into it.
 *
 * Jump table:
 *   up[node][0] = next[node]
 *   up[node][j] = up[up[node][j - 1]][j - 1]
 *
 * k-th successor query -> O(log k)
 * Cycle detection -> walk with a visited timestamp, O(n)
 *
 * Edge cases: self-loop, two-node cycle, k = 0, k larger than n, forest of
 * components, duplicate successors.
 */

#include <iostream>
#include <vector>
using namespace std;

class FunctionalGraph {
    int n;
    int log;
    vector<int> nextNode;
    vector<vector<int>> up;

   public:
    FunctionalGraph(vector<int> &successor) {
        nextNode = successor;
        n = successor.size();

        log = 1;
        while ((1 << log) <= n) {
            log++;
        }

        up.assign(n, vector<int>(log, 0));

        for (int node = 0; node < n; node++) {
            up[node][0] = nextNode[node];
        }

        for (int j = 1; j < log; j++) {
            for (int node = 0; node < n; node++) {
                up[node][j] = up[up[node][j - 1]][j - 1];
            }
        }
    }

    int kthSuccessor(int node, long long k) {
        for (int j = 0; j < log; j++) {
            if (k & (1LL << j)) {
                node = up[node][j];
            }
        }
        return node;
    }

    // Returns the length of the cycle reachable from start, or 0 if no cycle
    // exists (impossible in a finite functional graph, kept for clarity).
    int cycleLength(int start) {
        int slow = start;
        int fast = start;

        do {
            slow = nextNode[slow];
            fast = nextNode[nextNode[fast]];
        } while (slow != fast);

        int length = 1;
        int current = nextNode[slow];
        while (current != slow) {
            current = nextNode[current];
            length++;
        }

        return length;
    }

    // First node on the cycle reachable from start.
    int cycleEntry(int start) {
        int slow = start;
        int fast = start;

        do {
            slow = nextNode[slow];
            fast = nextNode[nextNode[fast]];
        } while (slow != fast);

        slow = start;
        while (slow != fast) {
            slow = nextNode[slow];
            fast = nextNode[fast];
        }

        return slow;
    }
};

int main() {
    // 0 -> 1 -> 2 -> 3 -> 1  (cycle 1 -> 2 -> 3)
    // 4 -> 2                 (feeds into the same cycle)
    // 5 -> 5                 (self-loop)
    vector<int> successor = {1, 2, 3, 1, 2, 5};
    FunctionalGraph graph(successor);

    cout << graph.kthSuccessor(0, 0) << endl;  // 0
    cout << graph.kthSuccessor(0, 1) << endl;  // 1
    cout << graph.kthSuccessor(0, 4) << endl;  // 1 (0->1->2->3->1)
    cout << graph.kthSuccessor(4, 3) << endl;  // 1 (4->2->3->1)

    cout << graph.cycleLength(0) << endl;  // 3
    cout << graph.cycleEntry(4) << endl;   // 2 (4 joins the cycle at 2)
    cout << graph.cycleLength(5) << endl;  // 1

    return 0;
}

// Special cases:
// - k = 0 -> the node itself
// - self-loop -> cycle length 1, entry is the node
// - all nodes in one cycle -> every node is a cycle entry
// - k can be larger than n; the jump table still works if k fits in 64 bits
