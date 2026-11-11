/**
 * Monotone Queue DP Optimization
 *
 * Many 1D DPs have the form
 *
 *   dp[i] = value[i] + best(dp[j]) for j in [i - k, i - 1]
 *
 * The window slides by one at every step. A brute force scans the whole window
 * -> O(n * k). A monotonic deque stores only the candidates that can still be
 * the best, in decreasing (for maximum) or increasing (for minimum) order.
 *
 * Deque invariant for a maximum:
 *   - indices are increasing
 *   - dp values are decreasing
 *   - the front is the best candidate in the current window
 *
 * Before using the front, remove indices that fell out of the window. Before
 * pushing a new index, pop from the back every index whose dp value is no
 * better, because the new index is both better and lasts longer.
 *
 * Each index enters and leaves the deque once -> O(n) time, O(k) space.
 *
 * This file shows three common shapes:
 *   1. Jump Game VI: dp[i] = nums[i] + max(dp[j]) over the last k steps
 *   2. Constrained Subsequence Sum: same, but a subsequence may start at i
 *   3. Minimum cost path: dp[i] = cost[i] + min(dp[j]) over the last k steps
 */

#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

// 1. Jump Game VI: start at index 0, jump at most k steps, maximize the sum.
int maxResultBrute(vector<int> &nums, int k) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, INT_MIN);
    dp[0] = nums[0];

    for (int i = 1; i < n; i++) {
        for (int j = max(0, i - k); j < i; j++) {
            if (dp[j] != INT_MIN) {
                dp[i] = max(dp[i], dp[j] + nums[i]);
            }
        }
    }

    return dp[n - 1];
}

int maxResult(vector<int> &nums, int k) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 0);
    deque<int> candidates;  // indices, dp decreasing

    dp[0] = nums[0];
    candidates.push_back(0);

    for (int i = 1; i < n; i++) {
        // Drop indices that are too far behind.
        while (!candidates.empty() && candidates.front() < i - k) {
            candidates.pop_front();
        }

        dp[i] = nums[i] + dp[candidates.front()];

        // The new index dominates every index with a smaller or equal value.
        while (!candidates.empty() && dp[candidates.back()] <= dp[i]) {
            candidates.pop_back();
        }

        candidates.push_back(i);
    }

    return dp[n - 1];
}

// 2. Constrained Subsequence Sum: choose a subsequence where consecutive
// chosen indices differ by at most k. The answer is the maximum sum.
int constrainedSubsetSumBrute(vector<int> &nums, int k) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 0);
    int answer = INT_MIN;

    for (int i = 0; i < n; i++) {
        dp[i] = nums[i];

        for (int j = max(0, i - k); j < i; j++) {
            dp[i] = max(dp[i], dp[j] + nums[i]);
        }

        answer = max(answer, dp[i]);
    }

    return answer;
}

int constrainedSubsetSum(vector<int> &nums, int k) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 0);
    deque<int> candidates;  // indices, dp decreasing
    int answer = INT_MIN;

    for (int i = 0; i < n; i++) {
        while (!candidates.empty() && candidates.front() < i - k) {
            candidates.pop_front();
        }

        dp[i] = nums[i];

        if (!candidates.empty()) {
            dp[i] = max(dp[i], dp[candidates.front()] + nums[i]);
        }

        while (!candidates.empty() && dp[candidates.back()] <= dp[i]) {
            candidates.pop_back();
        }

        candidates.push_back(i);
        answer = max(answer, dp[i]);
    }

    return answer;
}

// 3. Minimum cost to reach the last index with jumps of at most k.
// dp[i] = cost[i] + min(dp[j]) for j in [i - k, i - 1].
int minCostJumpBrute(vector<int> &cost, int k) {
    int n = cost.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, INT_MAX);
    dp[0] = cost[0];

    for (int i = 1; i < n; i++) {
        for (int j = max(0, i - k); j < i; j++) {
            if (dp[j] != INT_MAX) {
                dp[i] = min(dp[i], dp[j] + cost[i]);
            }
        }
    }

    return dp[n - 1];
}

int minCostJump(vector<int> &cost, int k) {
    int n = cost.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 0);
    deque<int> candidates;  // indices, dp increasing

    dp[0] = cost[0];
    candidates.push_back(0);

    for (int i = 1; i < n; i++) {
        while (!candidates.empty() && candidates.front() < i - k) {
            candidates.pop_front();
        }

        dp[i] = cost[i] + dp[candidates.front()];

        while (!candidates.empty() && dp[candidates.back()] >= dp[i]) {
            candidates.pop_back();
        }

        candidates.push_back(i);
    }

    return dp[n - 1];
}

int main() {
    vector<int> first = {1, -1, -2, 4, -7, 3};
    cout << maxResultBrute(first, 2) << " " << maxResult(first, 2) << endl;
    // 7 7 (1 -> -1 -> 4 -> 3)

    vector<int> second = {10, -5, -2, 4, 0, 3};
    cout << maxResultBrute(second, 3) << " " << maxResult(second, 3) << endl;
    // 17 17 (10 + 4 + 3)

    vector<int> third = {1, -5, -20, 4, -1, 3, -6, -3};
    cout << maxResultBrute(third, 2) << " " << maxResult(third, 2) << endl;
    // 0 0

    vector<int> fourth = {10, 2, -10, 5, 20};
    cout << constrainedSubsetSumBrute(fourth, 2) << " "
         << constrainedSubsetSum(fourth, 2) << endl;
    // 37 37 (10 + 2 + 5 + 20)

    vector<int> fifth = {-1, -2, -3};
    cout << constrainedSubsetSumBrute(fifth, 1) << " "
         << constrainedSubsetSum(fifth, 1) << endl;
    // -1 -1

    vector<int> sixth = {10, -2, -10, -5, 20};
    cout << constrainedSubsetSumBrute(sixth, 2) << " "
         << constrainedSubsetSum(sixth, 2) << endl;
    // 23 23 (10 - 2 - 5 + 20)

    vector<int> cost = {1, 2, 3, 4, 5};
    cout << minCostJumpBrute(cost, 2) << " " << minCostJump(cost, 2) << endl;
    // 9 9 (1 + 3 + 5)

    // k >= n: the window is the whole prefix, so the deque never drops an index
    // for distance and each dp[i] can use the best earlier dp[j].
    cout << maxResult(first, 100) << endl;   // 8 (1 + 4 + 3)
    cout << minCostJump(cost, 100) << endl;  // 6 (1 + 5)

    // Single element.
    vector<int> single = {42};
    cout << maxResult(single, 3) << " " << minCostJump(single, 3) << endl;
    // 42 42

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n * k)
 *   monotone deque -> O(n), every index enters and leaves at most once
 * Space complexity -> O(n) for dp and O(k) for the deque
 *
 * Special cases:
 *   - k = 1: only the previous index is allowed, so the DP is a simple prefix
 *     sum (for maxResult) or a direct walk (for minCostJump)
 *   - k >= n: the window is the whole prefix
 *   - negative values: the deque still works; for constrained subsequence sum
 *     the option to start a new subsequence at i is handled by initializing
 *     dp[i] = nums[i] before adding the best predecessor
 *   - single element: the answer is that element's value
 *   - empty input: returns 0
 *
 * The key difference between maximum and minimum is the comparison:
 *   maximum -> pop the back while dp[back] <= dp[new]
 *   minimum -> pop the back while dp[back] >= dp[new]
 * Using the wrong comparison silently keeps dominated candidates and can
 * produce a wrong answer, not just a slower one.
 */
