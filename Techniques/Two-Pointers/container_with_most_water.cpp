/**
 * Container With Most Water
 * Given n non-negative integers height[i], find two lines that together
 * with the x-axis form a container that holds the most water.
 *
 * Input: height = [1, 8, 6, 2, 5, 4, 8, 3, 7]
 * Output: 49
 *
 * Brute force -> check every pair, O(n^2)
 * Optimized -> two pointers, O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

int maxAreaBrute(vector<int> &height) {
    int n = height.size();
    int maxWater = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int width = j - i;
            int water = min(height[i], height[j]) * width;
            maxWater = max(maxWater, water);
        }
    }

    return maxWater;
};

int maxAreaOptimized(vector<int> &height) {
    int left = 0;
    int right = height.size() - 1;
    int maxWater = 0;

    while (left < right) {
        int water = min(height[left], height[right]) * (right - left);
        maxWater = max(maxWater, water);

        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return maxWater;
};

int main() {
    vector<int> height = {1, 8, 6, 2, 5, 4, 8, 3, 7};

    cout << maxAreaBrute(height) << endl;      // 49
    cout << maxAreaOptimized(height) << endl;  // 49

    return 0;
}
