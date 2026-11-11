/**
 * Trapping Rain Water
 * Given n non-negative integers representing an elevation map, compute
 * how much water it can trap after raining.
 *
 * Input: height = [0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]
 * Output: 6
 *
 * Brute force -> find left and right maximum for every index, O(n^2)
 * Better -> precompute prefix and suffix maximums, O(n) time and O(n) space
 * Optimized -> two pointers, O(n) time and O(1) space
 */

#include <iostream>
#include <vector>
using namespace std;

int trapBrute(vector<int> &height) {
    int n = height.size();
    int water = 0;

    for (int i = 1; i < n - 1; i++) {
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
};

int trapPrefix(vector<int> &height) {
    int n = height.size();
    if (n == 0) {
        return 0;
    }

    vector<int> leftMax(n), rightMax(n);

    leftMax[0] = height[0];
    for (int i = 1; i < n; i++) {
        leftMax[i] = max(leftMax[i - 1], height[i]);
    }

    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        rightMax[i] = max(rightMax[i + 1], height[i]);
    }

    int water = 0;
    for (int i = 0; i < n; i++) {
        water += min(leftMax[i], rightMax[i]) - height[i];
    }

    return water;
};

int trapTwoPointers(vector<int> &height) {
    int left = 0;
    int right = height.size() - 1;
    int leftMax = 0;
    int rightMax = 0;
    int water = 0;

    while (left < right) {
        if (height[left] < height[right]) {
            leftMax = max(leftMax, height[left]);
            water += leftMax - height[left];
            left++;
        } else {
            rightMax = max(rightMax, height[right]);
            water += rightMax - height[right];
            right--;
        }
    }

    return water;
};

int main() {
    vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    cout << trapBrute(height) << endl;        // 6
    cout << trapPrefix(height) << endl;       // 6
    cout << trapTwoPointers(height) << endl;  // 6

    return 0;
}
