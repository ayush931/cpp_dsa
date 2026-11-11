/**
 * Median of Two Sorted Arrays
 *
 * Given two sorted arrays nums1 and nums2 of sizes m and n, return the median
 * of the two sorted arrays. The overall run time should be O(log (m + n)).
 *
 * Example:
 * Input:  nums1 = [1, 3], nums2 = [2]
 * Output: 2.0
 *
 * Input:  nums1 = [1, 2], nums2 = [3, 4]
 * Output: 2.5
 *
 * Constraints: 0 <= m, n <= 1000, 1 <= m + n, -10^6 <= values <= 10^6.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: merge both arrays, then pick the middle -> O(m + n)
double findMedianBrute(vector<int> &nums1, vector<int> &nums2) {
    vector<int> merged;
    int i = 0;
    int j = 0;

    while (i < nums1.size() && j < nums2.size()) {
        if (nums1[i] <= nums2[j]) {
            merged.push_back(nums1[i]);
            i++;
        } else {
            merged.push_back(nums2[j]);
            j++;
        }
    }

    while (i < nums1.size()) {
        merged.push_back(nums1[i++]);
    }
    while (j < nums2.size()) {
        merged.push_back(nums2[j++]);
    }

    int total = merged.size();

    if (total % 2 == 1) {
        return merged[total / 2];
    }

    return (merged[total / 2 - 1] + merged[total / 2]) / 2.0;
}

// Optimized: binary search the partition of the smaller array -> O(log min(m,
// n))
double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
    // Always binary search the smaller array.
    if (nums1.size() > nums2.size()) {
        return findMedianSortedArrays(nums2, nums1);
    }

    int m = nums1.size();
    int n = nums2.size();
    int low = 0;
    int high = m;

    while (low <= high) {
        int cut1 = (low + high) / 2;
        int cut2 = (m + n + 1) / 2 - cut1;

        int left1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int right1 = (cut1 == m) ? INT_MAX : nums1[cut1];

        int left2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int right2 = (cut2 == n) ? INT_MAX : nums2[cut2];

        if (left1 <= right2 && left2 <= right1) {
            // Correct partition: every left value <= every right value.
            if ((m + n) % 2 == 1) {
                return max(left1, left2);
            }

            return (max(left1, left2) + min(right1, right2)) / 2.0;
        } else if (left1 > right2) {
            high = cut1 - 1;  // move the cut to the left
        } else {
            low = cut1 + 1;  // move the cut to the right
        }
    }

    return 0.0;
}

int main() {
    vector<int> a = {1, 3};
    vector<int> b = {2};
    vector<int> c = {1, 2};
    vector<int> d = {3, 4};
    vector<int> e = {};
    vector<int> f = {7};

    cout << findMedianBrute(a, b) << endl;         // 2
    cout << findMedianSortedArrays(a, b) << endl;  // 2

    cout << findMedianBrute(c, d) << endl;         // 2.5
    cout << findMedianSortedArrays(c, d) << endl;  // 2.5

    cout << findMedianSortedArrays(e, f) << endl;  // 7
    cout << findMedianSortedArrays(f, e) << endl;  // 7

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(m + n) time, O(m + n) space
 *   optimized -> O(log min(m, n)) time, O(1) space
 *
 * Special cases: one array empty, both arrays have one element, even vs odd
 * total length, all values in one array are smaller, duplicate values,
 * negative values, INT_MIN / INT_MAX sentinels.
 */
