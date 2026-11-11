/**
 * Divide and conquer DP optimization
 * Used for transitions of the form dp[k][i] = min over j < i of
 *   dp[k - 1][j] + cost(j + 1, i)
 * If the optimal j is monotonic in i (opt[i] <= opt[i + 1]), then we can
 * compute a layer with a recursive solve(level, left, right, optLeft, optRight)
 * that only searches the allowed range of j
 *
 * Brute force -> O(k * n^2)
 * D&C optimization -> O(k * n log n)
 *
 * Problem: split an array into k non-empty segments, minimize the sum of
 * (segment sum)^2.
 *
 * Knuth optimization is a related trick for interval DP:
 * dp[i][j] = min over i < k < j of dp[i][k] + dp[k][j] + cost(i, j)
 * If opt[i][j - 1] <= opt[i][j] <= opt[i + 1][j], the complexity drops
 * from O(n^3) to O(n^2). It needs the quadrangle inequality and monotonicity.
 */

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

const long long INF = 1e18;

long long segmentCost(vector<long long> &prefix, int left, int right) {
    long long sum = prefix[right] - prefix[left - 1];
    return sum * sum;
};

long long partitionBrute(vector<int> &arr, int k) {
    int n = arr.size();
    vector<long long> prefix(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + arr[i - 1];
    }

    vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, INF));
    dp[0][0] = 0;

    for (int level = 1; level <= k; level++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[level - 1][j] == INF) {
                    continue;
                }
                dp[level][i] =
                    min(dp[level][i],
                        dp[level - 1][j] + segmentCost(prefix, j + 1, i));
            }
        }
    }

    return dp[k][n];
};

void computeLayer(vector<long long> &previous, vector<long long> &current,
                  vector<long long> &prefix, int level, int left, int right,
                  int optLeft, int optRight) {
    if (left > right) {
        return;
    }

    int middle = (left + right) / 2;
    int bestIndex = -1;
    long long bestValue = INF;

    int upper = min(middle - 1, optRight);

    for (int j = optLeft; j <= upper; j++) {
        if (previous[j] == INF) {
            continue;
        }

        long long candidate = previous[j] + segmentCost(prefix, j + 1, middle);

        if (candidate < bestValue) {
            bestValue = candidate;
            bestIndex = j;
        }
    }

    current[middle] = bestValue;

    if (bestIndex == -1) {
        bestIndex = optLeft;
    }

    computeLayer(previous, current, prefix, level, left, middle - 1, optLeft,
                 bestIndex);
    computeLayer(previous, current, prefix, level, middle + 1, right, bestIndex,
                 optRight);
};

long long partitionOptimized(vector<int> &arr, int k) {
    int n = arr.size();
    if (k <= 0 || k > n) {
        return -1;
    }

    vector<long long> prefix(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + arr[i - 1];
    }

    vector<long long> previous(n + 1, INF);
    previous[0] = 0;

    for (int level = 1; level <= k; level++) {
        vector<long long> current(n + 1, INF);
        computeLayer(previous, current, prefix, level, 1, n, 0, n - 1);
        previous = current;
    }

    return previous[n];
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    int k = 2;

    cout << partitionBrute(arr, k) << endl;      // 117
    cout << partitionOptimized(arr, k) << endl;  // 117

    cout << partitionOptimized(arr, 5) << endl;  // 55
    cout << partitionOptimized(arr, 1) << endl;  // 225

    return 0;
}

// Special cases: k = 1, k = n, k > n (invalid), zeros in the array,
// negative values (monotonicity may break), overflow (use __int128)
// D&C optimization needs: monotone opt + O(1) cost (prefix sums)
// Knuth optimization needs: quadrangle inequality + monotone opt
