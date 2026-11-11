// First missing positive
//
// Given an unsorted integer array, return the smallest positive integer that
// does not appear in it.
//
// Example:
//   input  {3, 4, -1, 1}
//   output 2
//
//   input  {1, 2, 0}
//   output 3
//
//   input  {7, 8, 9, 11, 12}
//   output 1
//
// Brute force: try 1, 2, 3, ... and scan the whole array for each candidate.
// The answer is at most n + 1, so this is O(n^2) time and O(1) extra space.
//
// Optimized: use the array itself as a hash table. Put every value in the
// range [1, n] at index value - 1. After that, the first index i with
// values[i] != i + 1 gives the answer i + 1. If all positions match, the
// answer is n + 1.
//
// The cyclic placement loop looks nested, but every swap puts at least one
// value into its final position, so the total work is O(n).
//
// Edge cases:
//   - empty array -> 1
//   - all negative or zero -> 1
//   - values larger than n are ignored
//   - duplicates do not cause an infinite swap loop because the condition
//     checks values[values[i] - 1] != values[i]
//   - the answer can be n + 1, for example {1, 2, 3} -> 4
//
// Complexity:
//   brute     -> O(n^2) time, O(1) auxiliary space
//   optimized -> O(n) time, O(1) auxiliary space (the input is modified)

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int firstMissingPositiveBrute(const vector<int> &values) {
    int n = static_cast<int>(values.size());

    for (int candidate = 1; candidate <= n + 1; candidate++) {
        bool found = false;

        for (int value : values) {
            if (value == candidate) {
                found = true;
                break;
            }
        }

        if (!found) {
            return candidate;
        }
    }

    return n + 1;
}

int firstMissingPositive(vector<int> values) {
    int n = static_cast<int>(values.size());

    for (int i = 0; i < n; i++) {
        while (values[i] > 0 && values[i] <= n &&
               values[values[i] - 1] != values[i]) {
            swap(values[i], values[values[i] - 1]);
        }
    }

    for (int i = 0; i < n; i++) {
        if (values[i] != i + 1) {
            return i + 1;
        }
    }

    return n + 1;
}

int main() {
    vector<vector<int>> tests = {{3, 4, -1, 1}, {1, 2, 0}, {7, 8, 9, 11, 12},
                                 {1, 1},        {},        {2}};

    for (const vector<int> &values : tests) {
        cout << firstMissingPositiveBrute(values) << " "
             << firstMissingPositive(values) << endl;
    }

    return 0;
}

// Output:
// 2 2
// 3 3
// 1 1
// 2 2
// 1 1
// 1 1
//
// The optimized version is the standard O(n) solution. If the input must not
// be modified, copy it first, which costs O(n) extra space, or use a hash set.
