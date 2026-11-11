/**
 * Trapping rain water using a stack
 *
 * Given non-negative bar heights of width 1, compute how much rainwater is
 * trapped between the bars after it rains.
 *
 * Example:
 * Input:  [0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]
 * Output: 6
 *
 * Input:  [4, 2, 0, 3, 2, 5]
 * Output: 9
 *
 * Brute force: for every index, scan both sides for the tallest bars
 *              -> O(n^2) time, O(1) space
 * Optimized: monotonic decreasing stack of indices
 *            -> O(n) time, O(n) space
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int trapBrute(const vector<int> &height) {
    int n = height.size();
    int water = 0;

    for (int i = 0; i < n; i++) {
        int leftMax = 0;
        int rightMax = 0;

        for (int j = 0; j <= i; j++) {
            leftMax = max(leftMax, height[j]);
        }

        for (int j = i; j < n; j++) {
            rightMax = max(rightMax, height[j]);
        }

        water += min(leftMax, rightMax) - height[i];
    }

    return water;
}

int trap(const vector<int> &height) {
    stack<int> bars;  // indices with non-increasing heights
    int water = 0;

    for (int i = 0; i < (int)height.size(); i++) {
        while (!bars.empty() && height[i] > height[bars.top()]) {
            int bottom = bars.top();
            bars.pop();

            if (bars.empty()) {
                break;
            }

            int left = bars.top();
            int boundedHeight = min(height[left], height[i]) - height[bottom];
            int width = i - left - 1;

            water += boundedHeight * width;
        }

        bars.push(i);
    }

    return water;
}

int main() {
    vector<int> first = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    vector<int> second = {4, 2, 0, 3, 2, 5};

    cout << trapBrute(first) << " " << trap(first) << endl;    // 6 6
    cout << trapBrute(second) << " " << trap(second) << endl;  // 9 9

    cout << trap(vector<int>{}) << endl;               // 0
    cout << trap(vector<int>{3}) << endl;              // 0
    cout << trap(vector<int>{5, 4, 3, 2, 1}) << endl;  // 0
    cout << trap(vector<int>{2, 0, 2}) << endl;        // 2

    return 0;
}

/**
 * The stack stores indices whose heights are non-increasing. When a taller
 * bar arrives, it closes the valley above the popped bottom bar. The left
 * wall is the new stack top and the right wall is the current bar.
 *
 * Special cases: empty array, one bar, strictly decreasing heights (no
 * water), flat heights, and several valleys closed by one tall bar.
 */
