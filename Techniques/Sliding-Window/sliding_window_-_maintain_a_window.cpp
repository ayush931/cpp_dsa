/**
 * Sliding window -> maintain a window [left, right] and move it over the array
 * Fixed window -> window size is constant
 * Variable window -> window size grows and shrinks based on a condition
 * Converts many O(n^2) problems into O(n)
 */

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int maxSumFixedWindowBrute(vector<int> &arr, int k) {
    int n = arr.size();
    int maxSum = INT_MIN;

    for (int i = 0; i + k <= n; i++) {
        int sum = 0;
        for (int j = i; j < i + k; j++) {
            sum += arr[j];
        }
        maxSum = max(maxSum, sum);
    }

    return maxSum;
};

int maxSumFixedWindow(vector<int> &arr, int k) {
    int n = arr.size();

    int windowSum = 0;
    for (int i = 0; i < k; i++) {
        windowSum += arr[i];
    }

    int maxSum = windowSum;

    for (int i = k; i < n; i++) {
        windowSum += arr[i] - arr[i - k];
        maxSum = max(maxSum, windowSum);
    }

    return maxSum;
};

int smallestSubarrayBrute(vector<int> &arr, int target) {
    int n = arr.size();
    int minLength = INT_MAX;

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += arr[j];
            if (sum >= target) {
                minLength = min(minLength, j - i + 1);
                break;
            }
        }
    }

    return (minLength == INT_MAX) ? 0 : minLength;
};

int smallestSubarray(vector<int> &arr, int target) {
    int n = arr.size();
    int left = 0;
    int sum = 0;
    int minLength = INT_MAX;

    for (int right = 0; right < n; right++) {
        sum += arr[right];

        while (sum >= target) {
            minLength = min(minLength, right - left + 1);
            sum -= arr[left];
            left++;
        }
    }

    return (minLength == INT_MAX) ? 0 : minLength;
};

int main() {
    vector<int> arr = {2, 1, 5, 1, 3, 2};

    cout << maxSumFixedWindowBrute(arr, 3) << endl;  // 9
    cout << maxSumFixedWindow(arr, 3) << endl;       // 9

    cout << smallestSubarrayBrute(arr, 7) << endl;  // 2
    cout << smallestSubarray(arr, 7) << endl;       // 2

    return 0;
}
