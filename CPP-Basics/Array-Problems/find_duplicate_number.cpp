// Find the duplicate number
//
// The array has n + 1 integers. Every value is in the range [1, n], and one
// value appears more than once. Return a repeated value without modifying the
// array.
//
// Example:
//   input  {1, 3, 4, 2, 2}
//   output 2
//
//   input  {3, 1, 3, 4, 2}
//   output 3
//
// Brute force: compare every pair -> O(n^2) time, O(1) space.
//
// Hash set: remember values already seen -> O(n) time, O(n) space.
//
// Floyd's cycle detection: treat index i as a node with a single outgoing
// edge to values[i]. Because there are n + 1 values in [1, n], the graph must
// contain a cycle, and the entrance to the cycle is the repeated value.
// First find a meeting point with slow and fast pointers, then move one
// pointer back to the start and advance both one step at a time.
//
// Edge cases:
//   - the smallest valid array is {1, 1} -> 1
//   - the duplicate can be at the beginning, middle, or end
//   - values may repeat more than twice; Floyd still returns a repeated value
//   - if the problem guarantees exactly one duplicated value, all methods
//     return that value
//   - an empty array or an out-of-range value violates the problem contract
//
// Complexity:
//   brute     -> O(n^2) time, O(1) space
//   hash set  -> O(n) time, O(n) space
//   Floyd     -> O(n) time, O(1) space

#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

int findDuplicateBrute(const vector<int> &values) {
    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(values.size()); j++) {
            if (values[i] == values[j]) {
                return values[i];
            }
        }
    }

    return -1;
}

int findDuplicateHash(const vector<int> &values) {
    unordered_set<int> seen;

    for (int value : values) {
        if (seen.count(value) != 0) {
            return value;
        }

        seen.insert(value);
    }

    return -1;
}

int findDuplicateFloyd(const vector<int> &values) {
    if (values.size() < 2) {
        return -1;
    }

    int slow = values[0];
    int fast = values[0];

    do {
        slow = values[slow];
        fast = values[values[fast]];
    } while (slow != fast);

    slow = values[0];

    while (slow != fast) {
        slow = values[slow];
        fast = values[fast];
    }

    return slow;
}

int main() {
    vector<vector<int>> tests = {
        {1, 3, 4, 2, 2}, {3, 1, 3, 4, 2}, {1, 1}, {1, 2, 3, 4, 5, 5}};

    for (const vector<int> &values : tests) {
        cout << findDuplicateBrute(values) << " " << findDuplicateHash(values)
             << " " << findDuplicateFloyd(values) << endl;
    }

    return 0;
}

// Output:
// 2 2 2
// 3 3 3
// 1 1 1
// 5 5 5
//
// Floyd's algorithm is the only version here that is both O(n) time and O(1)
// space. It works because the values are valid indices. Do not use it when the
// values can be outside [1, n].
