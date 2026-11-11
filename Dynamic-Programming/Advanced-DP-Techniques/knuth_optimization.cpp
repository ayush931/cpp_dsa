/**
 * Knuth's Optimization
 *
 * Some interval DPs have the form
 *
 *   dp[i][j] = min over root in [i, j] of
 *              (dp[i][root - 1] + dp[root + 1][j]) + weight[i][j]
 *
 * The direct implementation is O(n^3). If the cost satisfies the quadrangle
 * inequality and the monotonicity condition, the optimal split point also
 * satisfies
 *
 *   opt[i][j - 1] <= opt[i][j] <= opt[i + 1][j]
 *
 * Then the root loop only needs to scan that range. The total work becomes
 * O(n^2), because the range lengths telescope.
 *
 * The classic example is the Optimal Binary Search Tree:
 *   - p[i] is the probability of searching for key i
 *   - q[i] is the probability of searching for a dummy key between key i and
 *     key i + 1 (q[0] is before the first key, q[n] is after the last)
 *   - weight[i][j] is the sum of all probabilities in the subtree
 *   - dp[i][j] is the minimum expected search cost for keys i..j
 *
 * Knuth's optimization also applies to other interval DPs of the same shape,
 * where the added cost depends only on the interval [i, j] and satisfies the
 * quadrangle inequality. Matrix chain multiplication and polygon triangulation
 * add a cost that depends on the split point, so they do not fit this
 * recurrence directly.
 */

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Direct interval DP: O(n^3).
long long optimalCostBrute(vector<long long> &p, vector<long long> &q) {
    int n = p.size() - 1;  // keys are 1..n, p[0] is unused

    vector<vector<long long>> weight(n + 2, vector<long long>(n + 2, 0));
    vector<vector<long long>> cost(n + 2, vector<long long>(n + 2, 0));

    for (int i = 1; i <= n + 1; i++) {
        weight[i][i - 1] = q[i - 1];
        cost[i][i - 1] = q[i - 1];  // an empty key interval still has one dummy
    }

    for (int length = 1; length <= n; length++) {
        for (int i = 1; i + length - 1 <= n; i++) {
            int j = i + length - 1;

            weight[i][j] = weight[i][j - 1] + p[j] + q[j];
            cost[i][j] = LLONG_MAX;

            for (int root = i; root <= j; root++) {
                cost[i][j] =
                    min(cost[i][j], cost[i][root - 1] + cost[root + 1][j]);
            }

            cost[i][j] += weight[i][j];
        }
    }

    return n == 0 ? q[0] : cost[1][n];
}

// Knuth's optimization: O(n^2).
long long optimalCostKnuth(vector<long long> &p, vector<long long> &q,
                           int &bestRoot) {
    int n = p.size() - 1;

    if (n == 0) {
        bestRoot = -1;
        return q[0];
    }

    vector<vector<long long>> weight(n + 2, vector<long long>(n + 2, 0));
    vector<vector<long long>> cost(n + 2, vector<long long>(n + 2, 0));
    vector<vector<int>> opt(n + 2, vector<int>(n + 2, 0));

    for (int i = 1; i <= n + 1; i++) {
        weight[i][i - 1] = q[i - 1];
        cost[i][i - 1] = q[i - 1];  // an empty key interval still has one dummy
    }

    for (int i = 1; i <= n; i++) {
        weight[i][i] = weight[i][i - 1] + p[i] + q[i];
        cost[i][i] = cost[i][i - 1] + cost[i + 1][i] + weight[i][i];
        opt[i][i] = i;
    }

    for (int length = 2; length <= n; length++) {
        for (int i = 1; i + length - 1 <= n; i++) {
            int j = i + length - 1;

            weight[i][j] = weight[i][j - 1] + p[j] + q[j];
            cost[i][j] = LLONG_MAX;

            // The monotonicity bound: the best root cannot move left of the
            // best root for [i, j - 1] or right of the best root for [i + 1,
            // j].
            int start = opt[i][j - 1];
            int end = opt[i + 1][j];

            for (int root = start; root <= end; root++) {
                long long candidate = cost[i][root - 1] + cost[root + 1][j];

                if (candidate < cost[i][j]) {
                    cost[i][j] = candidate;
                    opt[i][j] = root;
                }
            }

            cost[i][j] += weight[i][j];
        }
    }

    bestRoot = opt[1][n];
    return cost[1][n];
}

void check(vector<long long> p, vector<long long> q) {
    int root = -1;
    long long brute = optimalCostBrute(p, q);
    long long optimized = optimalCostKnuth(p, q, root);

    cout << brute << " " << optimized << " root=" << root << endl;
}

int main() {
    // The CLRS example. Probabilities are multiplied by 100 so everything is
    // an integer: expected cost 275, optimal root key 2.
    vector<long long> p = {0, 15, 10, 5, 10, 20};
    vector<long long> q = {5, 10, 5, 5, 5, 10};

    int root = -1;
    cout << optimalCostBrute(p, q) << " " << optimalCostKnuth(p, q, root) << " "
         << root << endl;
    // 275 275 2

    // One key: cost = p[1] + 2*q[0] + 2*q[1] = 10 + 10 + 10 = 30.
    check({0, 10}, {5, 5});  // 30 30 root=1

    // Two equal keys: dp[1][1] = 1, dp[2][2] = 1, weight[1][2] = 2.
    // Root 1 gives 0 + 1 + 2 = 3, and the tie keeps the smaller root.
    check({0, 1, 1}, {0, 0, 0});  // 3 3 root=1

    // Three equal keys: the balanced root 2 gives 5.
    check({0, 1, 1, 1}, {0, 0, 0, 0});  // 5 5 root=2

    // A skewed example where the best root is the first key.
    check({0, 100, 1, 1}, {0, 0, 0, 0});  // 105 105 root=1

    // Empty tree with one dummy key: the search fails after one comparison.
    check({0}, {5});  // 5 5 root=-1

    // Empty tree with zero probability.
    check({0}, {0});  // 0 0 root=-1

    // A larger random-looking example: the two methods must agree.
    check({0, 4, 9, 2, 7, 3, 8, 1, 6, 5},
          {2, 3, 1, 4, 2, 3, 1, 2, 3, 1});  // 205 205 root=4

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^3): O(n^2) intervals, O(n) roots per interval
 *   Knuth -> O(n^2): the root ranges telescope
 * Space complexity -> O(n^2) for the cost, weight, and opt tables
 *
 * Special cases:
 *   - n = 0: one empty tree with dummy probability q[0], cost q[0], no root
 *   - n = 1: the only key is the root
 *   - all probabilities zero: every tree has cost 0; the first valid root wins
 *   - equal keys/probabilities: many roots tie; the comparison uses strict
 *     less-than, so the smallest root in the allowed range is kept
 *   - integer probabilities: multiply floating-point probabilities by a common
 *     denominator to avoid floating-point comparison errors
 *   - negative weights would break the quadrangle inequality; probabilities
 *     must be non-negative
 *
 * Why the total is O(n^2): for a fixed interval length, the sum of the range
 * lengths [opt[i][j-1], opt[i+1][j]] telescopes to O(n), because consecutive
 * ranges share endpoints. There are O(n) lengths, hence O(n^2).
 *
 * The quadrangle inequality is the real precondition. It is not enough for the
 * DP to look like the formula above; a cost function that violates the
 * inequality can make the monotonicity bound skip the true optimum.
 */
