/**
 * Lower bound and upper bound
 *
 * For a sorted array and a target:
 *
 *   lower_bound -> first index i with arr[i] >= target
 *   upper_bound -> first index i with arr[i] >  target
 *
 * Both return arr.size() when no such index exists. Together they describe the
 * half-open range [lower, upper) that contains every occurrence of target, so
 * upper - lower is the frequency.
 *
 * The manual implementations are the classic "first true" binary search:
 * keep the answer in a variable and move the boundary past every element that
 * does not satisfy the predicate.
 *
 * Time        -> O(log n)
 * Space       -> O(1)
 * Requirement -> the array must be sorted
 *
 * The standard library functions are included for comparison.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int lowerBoundManual(const vector<int> &arr, int target) {
    int low = 0;
    int high = (int)arr.size();

    while (low < high) {
        int middle = low + (high - low) / 2;

        if (arr[middle] < target) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }

    return low;
}

int upperBoundManual(const vector<int> &arr, int target) {
    int low = 0;
    int high = (int)arr.size();

    while (low < high) {
        int middle = low + (high - low) / 2;

        if (arr[middle] <= target) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }

    return low;
}

void report(const vector<int> &arr, int target) {
    int lower = lowerBoundManual(arr, target);
    int upper = upperBoundManual(arr, target);

    int stdLower =
        (int)(std::lower_bound(arr.begin(), arr.end(), target) - arr.begin());
    int stdUpper =
        (int)(std::upper_bound(arr.begin(), arr.end(), target) - arr.begin());

    cout << "target " << target << ": manual = [" << lower << ", " << upper
         << "), std = [" << stdLower << ", " << stdUpper << ")"
         << ", count = " << upper - lower << ", agree = "
         << (lower == stdLower && upper == stdUpper ? "yes" : "no") << endl;
}

int main() {
    vector<int> arr = {-8, -3, -3, 0, 4, 4, 4, 9, 15};

    for (int target : {-8, -3, 4, 0, 9, 15, -20, 5, 20}) {
        report(arr, target);
    }

    vector<int> empty;
    report(empty, 3);

    vector<int> allEqual(5, 6);
    report(allEqual, 6);

    return 0;
}

// Expected output:
// target -8: manual = [0, 1), std = [0, 1), count = 1, agree = yes
// target -3: manual = [1, 3), std = [1, 3), count = 2, agree = yes
// target 4: manual = [4, 7), std = [4, 7), count = 3, agree = yes
// target 0: manual = [3, 4), std = [3, 4), count = 1, agree = yes
// target 9: manual = [7, 8), std = [7, 8), count = 1, agree = yes
// target 15: manual = [8, 9), std = [8, 9), count = 1, agree = yes
// target -20: manual = [0, 0), std = [0, 0), count = 0, agree = yes
// target 5: manual = [7, 7), std = [7, 7), count = 0, agree = yes
// target 20: manual = [9, 9), std = [9, 9), count = 0, agree = yes
// target 3: manual = [0, 0), std = [0, 0), count = 0, agree = yes
// target 6: manual = [0, 5), std = [0, 5), count = 5, agree = yes
//
// Special cases: empty array, duplicates, all equal values, negative values,
// target smaller than every element, target larger than every element, and a
// target that is absent but falls between two elements.
//
// lower_bound and upper_bound are the building blocks for counting, finding
// the first/last occurrence, and many "search on the answer" problems.
