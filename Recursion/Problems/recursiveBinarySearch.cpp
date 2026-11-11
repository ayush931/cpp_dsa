// Recursive binary search
//
// Binary search requires a sorted range. Compare the target with the middle
// element and discard the half that cannot contain it.
//
// Example:
//   array  = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91}
//   target = 23 -> index 5
//   target = 100 -> -1
//
// The iterative version uses O(1) extra space; the recursive version uses
// O(log n) stack frames.

#include <iostream>
#include <vector>

int binarySearchRecursive(const std::vector<int> &values, int low, int high,
                          int target) {
    if (low > high) {
        return -1;
    }

    int mid = low + (high - low) / 2;

    if (values[mid] == target) {
        return mid;
    }
    if (values[mid] < target) {
        return binarySearchRecursive(values, mid + 1, high, target);
    }
    return binarySearchRecursive(values, low, mid - 1, target);
}

int binarySearchIterative(const std::vector<int> &values, int target) {
    int low = 0;
    int high = (int)values.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (values[mid] == target) {
            return mid;
        }
        if (values[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

int main() {
    std::vector<int> values = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    std::cout << binarySearchRecursive(values, 0, values.size() - 1, 23)
              << "\n";  // 5
    std::cout << binarySearchRecursive(values, 0, values.size() - 1, 2)
              << "\n";  // 0
    std::cout << binarySearchRecursive(values, 0, values.size() - 1, 91)
              << "\n";  // 9
    std::cout << binarySearchRecursive(values, 0, values.size() - 1, 100)
              << "\n";  // -1

    std::cout << binarySearchIterative(values, 38) << "\n";  // 6
    std::cout << binarySearchIterative(values, 1) << "\n";   // -1

    std::vector<int> empty;
    std::cout << binarySearchRecursive(empty, 0, -1, 5) << "\n";  // -1

    return 0;
}

// Output:
// 5
// 0
// 9
// -1
// 6
// -1
// -1
//
// Time complexity: O(log n) for both versions.
// Space complexity:
//   recursive -> O(log n) stack
//   iterative -> O(1)
//
// Edge cases: empty array, target smaller than every element, target larger
// than every element, one element, and duplicate values (this version returns
// any matching index, not necessarily the first one).
