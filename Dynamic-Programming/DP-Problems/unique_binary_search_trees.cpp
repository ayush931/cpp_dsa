/**
 * Unique Binary Search Trees
 *
 * Given an integer n, return the number of structurally unique binary search
 * trees that store the values 1..n.
 *
 * Example 1:
 * Input: n = 3
 * Output: 5
 *
 * Example 2:
 * Input: n = 1
 * Output: 1
 *
 * Constraints: 1 <= n <= 19.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every value as the root and multiply the left and right
// subtree counts. This recomputes the same subproblems many times.
// O(Catalan(n)) time, which grows faster than 2^n.
int numTreesBrute(int n) {
    if (n <= 1) {
        return 1;
    }

    int total = 0;

    for (int root = 1; root <= n; root++) {
        total += numTreesBrute(root - 1) * numTreesBrute(n - root);
    }

    return total;
}

// Optimized: Catalan-number DP. The number of trees with n nodes is
//   sum over root of dp[root - 1] * dp[n - root]
// O(n^2) time, O(n) space.
int numTrees(int n) {
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;

    for (int nodes = 1; nodes <= n; nodes++) {
        for (int root = 1; root <= nodes; root++) {
            dp[nodes] += dp[root - 1] * dp[nodes - root];
        }
    }

    return dp[n];
}

int main() {
    cout << numTreesBrute(3) << " " << numTrees(3) << endl;  // 5 5

    for (int n = 1; n <= 10; n++) {
        cout << n << " -> " << numTrees(n) << endl;
    }

    cout << numTrees(19) << endl;  // 1767263190 (fits in int)

    return 0;
}

/**
 * Time complexity:
 *   brute -> exponential, because the same subtree sizes are recomputed
 *   DP -> O(n^2): n sizes, and each size tries every root
 * Space complexity -> O(n) for the DP table, O(n) recursion depth for brute
 *
 * Special cases: n = 0 (one empty tree, by convention), n = 1 (one tree), and
 * n = 19 (the largest value in the constraints, 1,767,263,190).
 *
 * The sequence is the Catalan numbers: 1, 1, 2, 5, 14, 42, 132, ... The DP is
 * exactly the same recurrence used to count balanced bracket sequences and
 * full binary trees.
 */
