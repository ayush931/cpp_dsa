/**
 * Given an array arr[] of length N, find the length of the longest subarray
 with a sum equal to 0.

 * Input 1:- n = 8;
 * arr[] = {15, -2, 2, -8, 1, 7, 10, 23}
 * Output 1: 5
 */

#include <bits/stdc++.h>
using namespace std;

int maxLengthZeroSumSubarray(vector<int> &v) {
    // firstIndex[sum] is the earliest index where this prefix sum appeared.
    // Prefix sum 0 exists before the array starts, at index -1.
    unordered_map<int, int> firstIndex;
    firstIndex[0] = -1;

    int prefixSum = 0;
    int maxlen = 0;

    for (int i = 0; i < (int)v.size(); i++) {
        prefixSum += v[i];

        if (firstIndex.find(prefixSum) != firstIndex.end()) {
            maxlen = max(maxlen, i - firstIndex[prefixSum]);
        } else {
            firstIndex[prefixSum] = i;
        }
    }

    return maxlen;
};

int main() {
    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    cout << maxLengthZeroSumSubarray(v) << endl;  // 5

    return 0;
}

// Time and space -> O(n) average
// Example: [1, -1] has a zero-sum subarray of length 2.