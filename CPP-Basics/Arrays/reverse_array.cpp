// Reverse an array
//
// Reverse the order of the elements. Two common versions:
//
//   1. in place: swap the first and last, then move both pointers inward
//   2. with a copy: write values into a new array from the end
//
// Example:
//   input  {1, 2, 3, 4, 5}
//   output {5, 4, 3, 2, 1}
//
// The in-place version is usually preferred because it needs O(1) extra space.
// The copy version is useful when the original array must stay unchanged.
//
// Edge cases:
//   - empty array: nothing to do
//   - one element: already reversed
//   - even length: every element is swapped exactly once
//   - odd length: the middle element stays in place
//   - a subrange [left, right] can be reversed the same way
//
// Complexity:
//   in place -> O(n) time, O(1) auxiliary space
//   with copy -> O(n) time, O(n) auxiliary space

#include <iostream>
#include <vector>
using namespace std;

void reverseInPlace(vector<int> &values) {
    int left = 0;
    int right = static_cast<int>(values.size()) - 1;

    while (left < right) {
        int temporary = values[left];
        values[left] = values[right];
        values[right] = temporary;

        left++;
        right--;
    }
}

vector<int> reverseWithCopy(const vector<int> &values) {
    vector<int> result(values.size());

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        result[i] = values[values.size() - 1 - i];
    }

    return result;
}

void reverseRange(vector<int> &values, int left, int right) {
    while (left < right) {
        int temporary = values[left];
        values[left] = values[right];
        values[right] = temporary;

        left++;
        right--;
    }
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> first = {1, 2, 3, 4, 5};
    reverseInPlace(first);
    print(first);  // 5 4 3 2 1

    vector<int> second = {1, 2, 3, 4};
    print(reverseWithCopy(second));  // 4 3 2 1
    print(second);                   // 1 2 3 4 (unchanged)

    vector<int> empty;
    reverseInPlace(empty);
    print(empty);  // (blank line)

    vector<int> single = {7};
    reverseInPlace(single);
    print(single);  // 7

    vector<int> range = {1, 2, 3, 4, 5, 6};
    reverseRange(range, 1, 4);
    print(range);  // 1 5 4 3 2 6

    return 0;
}

// Output:
// 5 4 3 2 1
// 4 3 2 1
// 1 2 3 4
//
// 7
// 1 5 4 3 2 6
//
// The standard library already provides std::reverse(begin, end) and
// std::reverse_copy. Use this file to understand the two-pointer pattern,
// then prefer the standard algorithm in production code.
