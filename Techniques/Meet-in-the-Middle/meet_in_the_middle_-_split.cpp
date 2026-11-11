/**
 * Meet in the middle -> split the search space into two halves
 * Generate all results for each half, then combine them
 * Reduces O(2^n) to O(2^(n/2) * log(2^(n/2)))
 * Useful for subset sum and counting problems with n up to about 40
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<long long> generateSubsetSums(vector<int> &arr, int start, int end) {
    int length = end - start;
    vector<long long> sums;

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

long long closestSubsetSumBrute(vector<int> &arr, int target) {
    int n = arr.size();
    long long best = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }

        if (abs(sum - target) < abs(best - target)) {
            best = sum;
        }
    }

    return best;
};

long long closestSubsetSum(vector<int> &arr, int target) {
    int n = arr.size();
    int middle = n / 2;

    vector<long long> leftSums = generateSubsetSums(arr, 0, middle);
    vector<long long> rightSums = generateSubsetSums(arr, middle, n);

    sort(rightSums.begin(), rightSums.end());

    long long best = 0;

    for (long long left : leftSums) {
        long long remaining = target - left;

        auto it = lower_bound(rightSums.begin(), rightSums.end(), remaining);

        if (it != rightSums.end()) {
            long long candidate = left + *it;
            if (abs(candidate - target) < abs(best - target)) {
                best = candidate;
            }
        }

        if (it != rightSums.begin()) {
            --it;
            long long candidate = left + *it;
            if (abs(candidate - target) < abs(best - target)) {
                best = candidate;
            }
        }
    }

    return best;
};

int main() {
    vector<int> arr = {3, 34, 4, 12, 5, 2};
    int target = 30;

    cout << closestSubsetSumBrute(arr, target) << endl;  // 30
    cout << closestSubsetSum(arr, target) << endl;       // 30

    return 0;
}

// Special cases: empty array, target smaller than every element,
// negative numbers, duplicate subset sums, n = 0
