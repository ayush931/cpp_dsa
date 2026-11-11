/**
 * Interpolation search
 *
 * Interpolation search estimates the target's position instead of always
 * probing the middle:
 *
 *   position = low + (target - arr[low]) * (high - low) / (arr[high] -
 * arr[low])
 *
 * The estimate is accurate when values are uniformly distributed, giving
 * O(log log n) expected time. On skewed data it can degrade to O(n).
 *
 * Time        -> O(log log n) average for uniformly distributed sorted data,
 *                O(n) worst
 * Space       -> O(1)
 * Requirement -> the array must be sorted
 *
 * Linear search is included as the brute-force reference. The implementation
 * moves left after a hit so it returns the first occurrence of a duplicate.
 */

#include <iostream>
#include <vector>
using namespace std;

int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int interpolationSearch(const vector<int> &arr, int target) {
    int low = 0;
    int high = (int)arr.size() - 1;

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        // All values in the range are equal, so avoid dividing by zero.
        if (arr[high] == arr[low]) {
            return arr[low] == target ? low : -1;
        }

        // Use long long: the product can exceed the range of int.
        long long numerator = (long long)(target - arr[low]) * (high - low);
        int position = low + (int)(numerator / (arr[high] - arr[low]));

        if (arr[position] == target) {
            while (position > low && arr[position - 1] == target) {
                position--;
            }
            return position;
        }

        if (arr[position] < target) {
            low = position + 1;
        } else {
            high = position - 1;
        }
    }

    return -1;
}

int main() {
    vector<int> uniform = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int target : {10, 50, 100, 35, 5, 105}) {
        int interpolation = interpolationSearch(uniform, target);
        int linear = linearSearch(uniform, target);

        cout << "uniform target " << target
             << ": interpolation = " << interpolation << ", linear = " << linear
             << ", agree = " << (interpolation == linear ? "yes" : "no")
             << endl;
    }

    // Duplicates and negative values.
    vector<int> duplicates = {-8, -3, -3, -3, 0, 4, 4, 9, 15};
    cout << "first duplicate -3: " << interpolationSearch(duplicates, -3)
         << endl;
    cout << "first duplicate 4: " << interpolationSearch(duplicates, 4) << endl;

    // All equal values: the division guard is required here.
    vector<int> equal(6, 7);
    cout << "all equal hit: " << interpolationSearch(equal, 7) << endl;
    cout << "all equal miss: " << interpolationSearch(equal, 8) << endl;

    vector<int> empty;
    cout << "empty array: " << interpolationSearch(empty, 1) << endl;

    return 0;
}

// Expected output:
// uniform target 10: interpolation = 0, linear = 0, agree = yes
// uniform target 50: interpolation = 4, linear = 4, agree = yes
// uniform target 100: interpolation = 9, linear = 9, agree = yes
// uniform target 35: interpolation = -1, linear = -1, agree = yes
// uniform target 5: interpolation = -1, linear = -1, agree = yes
// uniform target 105: interpolation = -1, linear = -1, agree = yes
// first duplicate -3: 1
// first duplicate 4: 5
// all equal hit: 0
// all equal miss: -1
// empty array: -1
//
// Special cases: empty array, one element, duplicates, all equal values,
// negative values, target outside the range, and non-uniform gaps (where the
// estimate can be poor but the loop still narrows the range).
//
// Interpolation search is a bad fit for data with huge gaps or a very skewed
// distribution; binary search has a better worst case.
