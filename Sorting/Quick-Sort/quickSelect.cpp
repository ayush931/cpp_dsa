// Quickselect: find the k-th smallest or k-th largest element without fully
// sorting the array.
//
// The partition step places one pivot at its final sorted position. Only the
// side that contains the requested rank is explored, so the average work is
// O(n) instead of O(n log n).
//
// Example:
//   values = {7, 10, 4, 3, 20, 15}
//   sorted = {3, 4, 7, 10, 15, 20}
//   3rd smallest -> 7
//   2nd largest  -> 15
//
// Worst case: already sorted input with the last element as pivot -> O(n^2).
// A random or median-of-three pivot reduces the chance of that case.

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int partition(std::vector<int> &values, int low, int high) {
    int pivot = values[high];
    int i = low;

    for (int j = low; j < high; j++) {
        if (values[j] < pivot) {
            std::swap(values[i], values[j]);
            i++;
        }
    }

    std::swap(values[i], values[high]);
    return i;
}

// k is a 0-based index into the sorted order.
int quickSelect(std::vector<int> &values, int low, int high, int k) {
    if (low == high) {
        return values[low];
    }

    int pivotIndex = partition(values, low, high);

    if (pivotIndex == k) {
        return values[pivotIndex];
    }
    if (k < pivotIndex) {
        return quickSelect(values, low, pivotIndex - 1, k);
    }
    return quickSelect(values, pivotIndex + 1, high, k);
}

int kthSmallest(std::vector<int> values, int k) {
    if (k < 1 || k > (int)values.size()) {
        return -1;
    }
    return quickSelect(values, 0, (int)values.size() - 1, k - 1);
}

int kthLargest(std::vector<int> values, int k) {
    if (k < 1 || k > (int)values.size()) {
        return -1;
    }
    return quickSelect(values, 0, (int)values.size() - 1,
                       (int)values.size() - k);
}

void print(const std::vector<int> &values) {
    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> values = {7, 10, 4, 3, 20, 15};

    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    print(sorted);  // 3 4 7 10 15 20

    std::cout << kthSmallest(values, 1) << "\n";  // 3
    std::cout << kthSmallest(values, 3) << "\n";  // 7
    std::cout << kthSmallest(values, 6) << "\n";  // 20

    std::cout << kthLargest(values, 1) << "\n";  // 20
    std::cout << kthLargest(values, 2) << "\n";  // 15
    std::cout << kthLargest(values, 6) << "\n";  // 3

    std::cout << kthSmallest(values, 0) << "\n";  // -1
    std::cout << kthSmallest(values, 7) << "\n";  // -1

    std::vector<int> empty;
    std::cout << kthLargest(empty, 1) << "\n";  // -1

    return 0;
}

// Output:
// 3 4 7 10 15 20
// 3
// 7
// 20
// 20
// 15
// 3
// -1
// -1
// -1
//
// Time complexity: O(n) average, O(n^2) worst.
// Space complexity: O(log n) average recursion stack, O(n) worst.
//
// Edge cases: empty array, k smaller than 1, k larger than n, duplicate
// values, and one element. The input vector is passed by value, so the caller's
// order is not changed.
