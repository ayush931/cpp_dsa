/**
 * Count subsets with a given sum using meet in the middle
 * Generate subset sums for both halves and count pairs that add to the target
 *
 * Input: arr = [1, 2, 3, 4], target = 5
 * Output: 2 ({1, 4}, {2, 3})
 *
 * Brute force -> O(2^n)
 * Meet in the middle -> O(2^(n/2) * log(2^(n/2)))
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<long long> subsetSums(vector<int> &arr, int start, int end) {
    vector<long long> sums;
    int length = end - start;

    for (int mask = 0; mask < (1 << length); mask++) {
        long long sum = 0;

        for (int i = 0; i < length; i++) {
            if (mask & (1 << i)) {
                sum += arr[start + i];
            }
        }

        sums.push_back(sum);
    }

    return sums;
};

long long countSubsetsBrute(vector<int> &arr, int target) {
    int n = arr.size();
    long long count = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }

        if (sum == target) {
            count++;
        }
    }

    return count;
};

long long countSubsets(vector<int> &arr, int target) {
    int n = arr.size();
    int middle = n / 2;

    vector<long long> left = subsetSums(arr, 0, middle);
    vector<long long> right = subsetSums(arr, middle, n);

    sort(right.begin(), right.end());

    long long count = 0;

    for (long long leftSum : left) {
        long long needed = target - leftSum;
        count += upper_bound(right.begin(), right.end(), needed) -
                 lower_bound(right.begin(), right.end(), needed);
    }

    return count;
};

int main() {
    vector<int> arr = {1, 2, 3, 4};

    cout << countSubsetsBrute(arr, 5) << endl;  // 2
    cout << countSubsets(arr, 5) << endl;       // 2

    return 0;
}

// Special cases: target 0 (empty subset counts once), negative numbers,
// duplicate values, empty array, sum overflow (use long long)
