// lower_bound and upper_bound after sorting
//
// Both functions require a sorted range and run in O(log n):
//   lower_bound(target) -> first index with values[index] >= target
//   upper_bound(target) -> first index with values[index] > target
//
// The number of occurrences of target is upper_bound - lower_bound.
//
// Example:
//   values = {1, 3, 3, 5, 8}
//   target 3 -> lower = 1, upper = 3, count = 2
//   target 4 -> lower = 3, upper = 3, count = 0
//   target 9 -> lower = 5, upper = 5, count = 0 (both are end())
//
// The manual versions below show the same logic as std::lower_bound and
// std::upper_bound.

#include <algorithm>
#include <iostream>
#include <vector>

int lowerBound(const std::vector<int> &values, int target) {
    int low = 0;
    int high = (int)values.size();

    while (low < high) {
        int mid = low + (high - low) / 2;

        if (values[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    return low;
}

int upperBound(const std::vector<int> &values, int target) {
    int low = 0;
    int high = (int)values.size();

    while (low < high) {
        int mid = low + (high - low) / 2;

        if (values[mid] <= target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    return low;
}

void report(const std::vector<int> &values, int target) {
    int lower = lowerBound(values, target);
    int upper = upperBound(values, target);

    int stdLower =
        (int)(std::lower_bound(values.begin(), values.end(), target) -
              values.begin());
    int stdUpper =
        (int)(std::upper_bound(values.begin(), values.end(), target) -
              values.begin());

    std::cout << "target " << target << ": lower=" << lower
              << " upper=" << upper << " count=" << upper - lower << " (std "
              << stdLower << " " << stdUpper << ")\n";
}

int main() {
    std::vector<int> values = {1, 3, 3, 5, 8};

    report(values, 3);  // target 3: lower=1 upper=3 count=2 (std 1 3)
    report(values, 4);  // target 4: lower=3 upper=3 count=0 (std 3 3)
    report(values, 0);  // target 0: lower=0 upper=0 count=0 (std 0 0)
    report(values, 9);  // target 9: lower=5 upper=5 count=0 (std 5 5)

    std::vector<int> empty;
    report(empty, 5);  // target 5: lower=0 upper=0 count=0 (std 0 0)

    return 0;
}

// Output:
// target 3: lower=1 upper=3 count=2 (std 1 3)
// target 4: lower=3 upper=3 count=0 (std 3 3)
// target 0: lower=0 upper=0 count=0 (std 0 0)
// target 9: lower=5 upper=5 count=0 (std 5 5)
// target 5: lower=0 upper=0 count=0 (std 0 0)
//
// Time complexity: O(log n) per query after the O(n log n) sort.
// Space complexity: O(1) extra.
//
// Edge cases: empty range, target smaller than every element, target larger
// than every element, duplicate values, and negative values. If the range is
// not sorted, the results are meaningless.
