/**
 * Count Inversions
 *
 * An inversion is a pair (i, j) such that i < j and arr[i] > arr[j]. Count the
 * number of inversions in an array.
 *
 * Example:
 * Input:  [2, 4, 1, 3, 5]
 * Output: 3  (pairs: (2,1), (4,1), (4,3))
 *
 * Constraints: 1 <= n <= 10^5, -10^9 <= arr[i] <= 10^9.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: check every pair -> O(n^2) time, O(1) space
long long countInversionsBrute(vector<int> &arr) {
    long long count = 0;

    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] > arr[j]) {
                count++;
            }
        }
    }

    return count;
}

// Merge step: when the right value is smaller, every remaining left value
// forms an inversion with it.
void mergeAndCount(vector<int> &arr, int left, int mid, int right,
                   vector<int> &temp, long long &count) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            count += (mid - i + 1);
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int index = left; index <= right; index++) {
        arr[index] = temp[index];
    }
}

void mergeSortAndCount(vector<int> &arr, int left, int right, vector<int> &temp,
                       long long &count) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSortAndCount(arr, left, mid, temp, count);
    mergeSortAndCount(arr, mid + 1, right, temp, count);
    mergeAndCount(arr, left, mid, right, temp, count);
}

// Optimized: merge sort counts inversions while sorting -> O(n log n)
long long countInversions(vector<int> arr) {
    vector<int> temp(arr.size());
    long long count = 0;

    mergeSortAndCount(arr, 0, arr.size() - 1, temp, count);

    return count;
}

int main() {
    vector<int> first = {2, 4, 1, 3, 5};
    vector<int> second = {5, 4, 3, 2, 1};
    vector<int> third = {1, 2, 3, 4, 5};
    vector<int> fourth = {1, 1, 1};
    vector<int> fifth = {3};

    cout << countInversionsBrute(first) << endl;  // 3
    cout << countInversions(first) << endl;       // 3

    cout << countInversions(second) << endl;  // 10
    cout << countInversions(third) << endl;   // 0
    cout << countInversions(fourth) << endl;  // 0
    cout << countInversions(fifth) << endl;   // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   merge sort -> O(n log n) time, O(n) space
 *
 * Special cases: already sorted (0), reverse sorted (n * (n - 1) / 2),
 * duplicate values (not inversions because equal values are not >), one
 * element, empty array, counts that exceed 32-bit range (use long long).
 */
