/**
 * Convex Hull Trick (CHT) -> optimize DP transitions of the form
 * dp[i] = min over j < i of (m[j] * x[i] + b[j])
 * Every previous state is a line, every query is an x value
 * If slopes are monotonic and queries are monotonic, a deque gives O(1)
 * amortized
 *
 * Problem: partition an array into segments.
 * Cost of a segment = (sum of segment)^2 + C
 * dp[i] = min over j < i of dp[j] + (prefix[i] - prefix[j])^2 + C
 *       = min over j < i of (-2 * prefix[j]) * prefix[i] + dp[j] + prefix[j]^2
 * + C So line j has slope -2 * prefix[j] and intercept dp[j] + prefix[j]^2
 *
 * Brute force -> O(n^2)
 * CHT -> O(n)
 */

#include <climits>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

struct Line {
    long long slope;
    long long intercept;

    long long value(long long x) {
        return slope * x + intercept;
    }
};

// true if line b is never better than line a for any x (given increasing
// slopes)
bool obsolete(Line a, Line b, Line c) {
    // intersection(a, b) >= intersection(b, c)
    return (b.intercept - a.intercept) * (b.slope - c.slope) >=
           (c.intercept - b.intercept) * (a.slope - b.slope);
}

long long minCostBrute(vector<int> &arr, int constant) {
    int n = arr.size();
    vector<long long> prefix(n + 1, 0);
    vector<long long> dp(n + 1, LLONG_MAX);

    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + arr[i - 1];
    }

    dp[0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            long long segment = prefix[i] - prefix[j];
            dp[i] = min(dp[i], dp[j] + segment * segment + constant);
        }
    }

    return dp[n];
};

long long minCostCHT(vector<int> &arr, int constant) {
    int n = arr.size();
    vector<long long> prefix(n + 1, 0);
    vector<long long> dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        prefix[i] = prefix[i - 1] + arr[i - 1];
    }

    deque<Line> hull;
    hull.push_back({0, 0});  // j = 0 -> slope 0, intercept 0

    for (int i = 1; i <= n; i++) {
        long long x = prefix[i];

        // queries are increasing, so pop lines that are no longer optimal
        while (hull.size() >= 2 && hull[1].value(x) <= hull[0].value(x)) {
            hull.pop_front();
        }

        dp[i] = hull.front().value(x) + x * x + constant;

        Line newLine = {-2 * prefix[i], dp[i] + prefix[i] * prefix[i]};

        // slopes are decreasing (-2 * prefix), so remove obsolete lines from
        // the back
        while (hull.size() >= 2 &&
               obsolete(hull[hull.size() - 2], hull.back(), newLine)) {
            hull.pop_back();
        }

        hull.push_back(newLine);
    }

    return dp[n];
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    int constant = 10;

    cout << minCostBrute(arr, constant) << endl;  // 99
    cout << minCostCHT(arr, constant) << endl;    // 99

    vector<int> single = {7};
    cout << minCostCHT(single, 0) << endl;  // 49

    return 0;
}

// Special cases: empty array, one element, constant = 0, negative values
// (queries stop being monotonic), equal slopes (keep the smaller intercept),
// overflow (use long long or __int128 for the cross multiplication)
// CHT is used for: partition DP, stock trading DP, tree distance DP
