// Count inversions
//
// An inversion is a pair of indices i < j with values[i] > values[j].
// Equal values do not form an inversion.
//
// Example:
//   {2, 4, 1, 3, 5} -> 3
//     (2,1), (4,1), (4,3)
//   {5, 4, 3, 2, 1} -> 10
//     every pair is inverted
//   {1, 2, 3, 4} -> 0
//
// Brute force checks every pair in O(n^2). Merge sort counts inversions during
// the merge: when an element from the right half is chosen, every remaining
// element in the left half is larger than it.

#include <iostream>
#include <vector>

long long countBrute(const std::vector<int> &values) {
    long long count = 0;

    for (int i = 0; i < (int)values.size(); i++) {
        for (int j = i + 1; j < (int)values.size(); j++) {
            if (values[i] > values[j]) {
                count++;
            }
        }
    }

    return count;
}

long long mergeAndCount(std::vector<int> &values, int low, int mid, int high) {
    std::vector<int> left(values.begin() + low, values.begin() + mid + 1);
    std::vector<int> right(values.begin() + mid + 1, values.begin() + high + 1);

    int i = 0;
    int j = 0;
    int k = low;
    long long count = 0;

    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i] <= right[j]) {
            values[k++] = left[i++];
        } else {
            values[k++] = right[j++];
            // left[i..end] are all greater than the element just written.
            count += (long long)left.size() - i;
        }
    }

    while (i < (int)left.size()) {
        values[k++] = left[i++];
    }
    while (j < (int)right.size()) {
        values[k++] = right[j++];
    }

    return count;
}

long long mergeSortAndCount(std::vector<int> &values, int low, int high) {
    if (low >= high) {
        return 0;
    }

    int mid = low + (high - low) / 2;

    long long count = mergeSortAndCount(values, low, mid);
    count += mergeSortAndCount(values, mid + 1, high);
    count += mergeAndCount(values, low, mid, high);

    return count;
}

long long countOptimized(std::vector<int> values) {
    if (values.empty()) {
        return 0;
    }
    return mergeSortAndCount(values, 0, (int)values.size() - 1);
}

int main() {
    std::vector<int> first = {2, 4, 1, 3, 5};
    std::cout << countBrute(first) << " " << countOptimized(first)
              << "\n";  // 3 3

    std::vector<int> second = {5, 4, 3, 2, 1};
    std::cout << countBrute(second) << " " << countOptimized(second)
              << "\n";  // 10 10

    std::vector<int> sorted = {1, 2, 3, 4};
    std::cout << countOptimized(sorted) << "\n";  // 0

    std::vector<int> duplicates = {2, 2, 1};
    std::cout << countOptimized(duplicates) << "\n";  // 2

    std::vector<int> empty;
    std::cout << countOptimized(empty) << "\n";  // 0

    return 0;
}

// Output:
// 3 3
// 10 10
// 0
// 2
// 0
//
// Time complexity:
//   brute     -> O(n^2)
//   optimized -> O(n log n)
// Space complexity:
//   brute     -> O(1) extra
//   optimized -> O(n) for the temporary halves and recursion
//
// Edge cases: empty array, one element, already sorted (0 inversions),
// reverse sorted (n * (n - 1) / 2 inversions), duplicates, and negative values.
