/**
 * There are N stones, numbered 1,2,....,N. The height of ith stone is h. There
 * is a frog who is initially on Stone 1. He will repeat an action some number
 * of times to reach Stone N. The action is that if the frog is currently on
 * Stone i, it jumps to one of the following: Stone i + 1, i + 2, ...., i + k.
 * Here, a cost of |hi - hj| is incurred, where j is the stone to land on. Find
 * the minimum possible total cost incurred before the frog reaches stone N.
 * Input1: n = 5, k = 3, 10 30 40 50 20
 * Output1: 30
 */

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

// Brute force recursion -> O(k^n) in the worst case
int frogBrute(vector<int> &heights, int index, int k) {
    if (index == heights.size() - 1) {
        return 0;
    }

    int best = INT_MAX;

    for (int jump = 1; jump <= k && index + jump < heights.size(); jump++) {
        int cost = abs(heights[index] - heights[index + jump]) +
                   frogBrute(heights, index + jump, k);
        best = min(best, cost);
    }

    return best;
};

// Memoized recursion -> O(n * k)
int frogMemo(vector<int> &heights, int index, int k, vector<int> &dp) {
    if (index == heights.size() - 1) {
        return 0;
    }
    if (dp[index] != -1) {
        return dp[index];
    }

    int best = INT_MAX;

    for (int jump = 1; jump <= k && index + jump < heights.size(); jump++) {
        int cost = abs(heights[index] - heights[index + jump]) +
                   frogMemo(heights, index + jump, k, dp);
        best = min(best, cost);
    }

    return dp[index] = best;
};

// Bottom-up DP -> O(n * k) time, O(n) space
int frogBottomUp(vector<int> &heights, int k) {
    int n = heights.size();
    if (n <= 1) {
        return 0;
    }

    vector<int> dp(n, INT_MAX);
    dp[n - 1] = 0;

    for (int i = n - 2; i >= 0; i--) {
        for (int jump = 1; jump <= k && i + jump < n; jump++) {
            dp[i] =
                min(dp[i], abs(heights[i] - heights[i + jump]) + dp[i + jump]);
        }
    }

    return dp[0];
};

int main() {
    vector<int> heights = {10, 30, 40, 50, 20};
    int k = 3;

    vector<int> dp(heights.size(), -1);

    cout << frogBrute(heights, 0, k) << endl;     // 30
    cout << frogMemo(heights, 0, k, dp) << endl;  // 30
    cout << frogBottomUp(heights, k) << endl;     // 30

    vector<int> single = {10};
    cout << frogBottomUp(single, k) << endl;  // 0

    vector<int> two = {10, 20};
    cout << frogBottomUp(two, k) << endl;  // 10

    return 0;
}

// Special cases: n = 1 (already at the last stone), k = 1 (only one path),
// k >= n (can jump directly to the end), equal heights (cost 0),
// negative heights (abs handles them)
