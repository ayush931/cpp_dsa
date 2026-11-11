/**
 * Non-overlapping Intervals
 *
 * Given an array of intervals where intervals[i] = [start, end], return the
 * minimum number of intervals you need to remove to make the rest of the
 * intervals non-overlapping.
 *
 * Example:
 * Input:  [[1,2], [2,3], [3,4], [1,3]]
 * Output: 1  (remove [1,3])
 *
 * Constraints: 1 <= intervals.length <= 10^5,
 * -5 * 10^4 <= start < end <= 5 * 10^4.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: DP over intervals sorted by end -> O(n^2)
int eraseOverlapIntervalsBrute(vector<vector<int>> intervals) {
    if (intervals.empty()) {
        return 0;
    }

    sort(intervals.begin(), intervals.end(),
         [](vector<int> &a, vector<int> &b) { return a[1] < b[1]; });

    int n = intervals.size();
    vector<int> dp(n, 1);  // dp[i] = max non-overlapping intervals ending at i
    int best = 1;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (intervals[j][1] <= intervals[i][0]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }

        best = max(best, dp[i]);
    }

    return n - best;
}

// Greedy: always keep the interval that ends first -> O(n log n)
int eraseOverlapIntervals(vector<vector<int>> &intervals) {
    if (intervals.empty()) {
        return 0;
    }

    sort(intervals.begin(), intervals.end(),
         [](vector<int> &a, vector<int> &b) { return a[1] < b[1]; });

    int removed = 0;
    int lastEnd = intervals[0][1];

    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < lastEnd) {
            // Overlaps the last kept interval, so remove this one.
            removed++;
        } else {
            lastEnd = intervals[i][1];
        }
    }

    return removed;
}

int main() {
    vector<vector<int>> first = {{1, 2}, {2, 3}, {3, 4}, {1, 3}};
    vector<vector<int>> second = {{1, 2}, {1, 2}, {1, 2}};
    vector<vector<int>> third = {{1, 2}, {2, 3}};
    vector<vector<int>> fourth = {{1, 100}, {11, 22}, {1, 11}, {2, 12}};

    cout << eraseOverlapIntervalsBrute(first) << " "
         << eraseOverlapIntervals(first) << endl;  // 1 1

    cout << eraseOverlapIntervals(second) << endl;  // 2
    cout << eraseOverlapIntervals(third) << endl;   // 0
    cout << eraseOverlapIntervals(fourth) << endl;  // 2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2) after sorting
 *   greedy -> O(n log n) for sorting + O(n) scan
 *
 * Special cases: empty input, one interval, intervals that only touch at an
 * endpoint (not overlapping), all intervals overlap, duplicate intervals,
 * negative coordinates, nested intervals.
 */
