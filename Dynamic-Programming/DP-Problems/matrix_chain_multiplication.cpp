/**
 * Matrix Chain Multiplication
 *
 * Given the dimensions of a sequence of matrices in arr[], where the ith
 * matrix has dimensions arr[i - 1] x arr[i], find the minimum number of scalar
 * multiplications needed to multiply the whole chain.
 *
 * Input:  n = 5, arr[] = {40, 20, 30, 10, 30}
 * Output: 26000
 *
 * The four matrices are 40x20, 20x30, 30x10, and 10x30. The best parenthesizing
 * is (A(BC))D, costing 20*30*10 + 40*20*10 + 40*10*30 = 26000.
 *
 * Constraints: 2 <= n <= 500, 1 <= arr[i] <= 1000.
 *
 * Both a memoized top-down solution and a bottom-up interval DP are shown.
 * Time: O(n^3). Space: O(n^2).
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Top-down memoization. dp[i][j] is the best cost for matrices i..j-1.
long long matrixChainMemo(const vector<int> &dims, int i, int j,
                          vector<vector<long long>> &dp) {
    if (i + 1 == j) {
        return 0;  // one matrix, no multiplication
    }
    if (dp[i][j] != -1) {
        return dp[i][j];
    }

    long long best = LLONG_MAX;

    // Split the chain after matrix k - 1: (i..k) * (k..j).
    for (int k = i + 1; k < j; k++) {
        long long cost = matrixChainMemo(dims, i, k, dp) +
                         matrixChainMemo(dims, k, j, dp) +
                         1LL * dims[i] * dims[k] * dims[j];

        best = min(best, cost);
    }

    return dp[i][j] = best;
}

// Bottom-up interval DP. Intervals are processed from short to long so that
// every sub-chain is ready before it is used.
long long matrixChainBottomUp(const vector<int> &dims) {
    int n = dims.size();
    if (n < 2) {
        return 0;
    }

    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int length = 2; length < n; length++) {  // length = j - i
        for (int i = 0; i + length < n; i++) {
            int j = i + length;
            dp[i][j] = LLONG_MAX;

            for (int k = i + 1; k < j; k++) {
                long long cost =
                    dp[i][k] + dp[k][j] + 1LL * dims[i] * dims[k] * dims[j];

                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }

    return dp[0][n - 1];
}

int main() {
    int dimensions;
    if (!(cin >> dimensions)) {
        return 0;
    }

    if (dimensions < 2) {
        cout << 0 << endl;
        return 0;
    }

    vector<int> dims(dimensions);
    for (int i = 0; i < dimensions; i++) {
        cin >> dims[i];
    }

    vector<vector<long long>> memo(dimensions,
                                   vector<long long>(dimensions, -1));

    cout << matrixChainMemo(dims, 0, dimensions - 1, memo) << endl;
    cout << matrixChainBottomUp(dims) << endl;

    return 0;
}

/**
 * Sample run:
 *   5
 *   40 20 30 10 30
 *   -> 26000
 *   -> 26000
 *
 * Edge cases: two dimensions (one matrix, cost 0), all dimensions equal,
 * a single very long matrix chain, and large dimensions (long long is used
 * because the product of three dimensions can exceed 32-bit range).
 */
