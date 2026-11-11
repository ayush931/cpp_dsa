// Insert a value into a sorted array recursively
//
// The array is stored in a vector with no fixed capacity, so the function can
// grow it by one. Recursion finds the insertion point; a second recursion
// shifts the larger elements one slot to the right.
//
// Example:
//   values = {1, 3, 5, 7}, insert 4 -> {1, 3, 4, 5, 7}
//   values = {1, 3, 5, 7}, insert 0 -> {0, 1, 3, 5, 7}
//   values = {1, 3, 5, 7}, insert 9 -> {1, 3, 5, 7, 9}
//
// The alternative is to build a new array, which also takes O(n) time but
// needs O(n) extra space instead of just the recursion stack.

#include <iostream>
#include <vector>

// Shift the element at `from` one slot right, then recurse toward `stop`.
void shiftRight(std::vector<int> &values, int from, int stop) {
    if (from <= stop) {
        return;
    }

    values[from] = values[from - 1];
    shiftRight(values, from - 1, stop);
}

void insertSorted(std::vector<int> &values, int index, int value) {
    // base case: the value belongs at the end
    if (index == (int)values.size()) {
        values.push_back(value);
        return;
    }

    if (values[index] >= value) {
        // grow by duplicating the last element, then shift the suffix right
        values.push_back(values.back());
        shiftRight(values, (int)values.size() - 1, index);
        values[index] = value;
        return;
    }

    insertSorted(values, index + 1, value);
}

void print(const std::vector<int> &values) {
    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> values = {1, 3, 5, 7};

    insertSorted(values, 0, 4);
    print(values);  // 1 3 4 5 7

    insertSorted(values, 0, 0);
    print(values);  // 0 1 3 4 5 7

    insertSorted(values, 0, 9);
    print(values);  // 0 1 3 4 5 7 9

    std::vector<int> empty;
    insertSorted(empty, 0, 42);
    print(empty);  // 42

    std::vector<int> duplicates = {2, 2, 4};
    insertSorted(duplicates, 0, 2);
    print(duplicates);  // 2 2 2 4

    return 0;
}

// Output:
// 1 3 4 5 7
// 0 1 3 4 5 7
// 0 1 3 4 5 7 9
// 42
// 2 2 2 4
//
// Time complexity: O(n) to find the position and shift the suffix.
// Space complexity: O(n) recursion stack.
//
// Edge cases: empty array, inserting at the front, inserting at the end,
// duplicate values, and a single-element array. The input must already be
// sorted; the function does not sort it.
