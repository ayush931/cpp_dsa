// Count subsequences with a given sum
//
// A subsequence keeps the original order and may skip elements. Count how many
// subsequences (including the empty one) add up to the target.
//
// Examples:
//   values = {1, 2, 3, 4, 5}, target = 5 -> 3
//     {5}, {1, 4}, {2, 3}
//   values = {2, 4, 6}, target = 6 -> 2
//     {6}, {2, 4}
//   values = {1, 2, 3}, target = 0 -> 1
//     the empty subsequence
//
// Brute force enumerates all 2^n subsets. The recursive version makes the same
// two choices (take or skip) but does not build the subsets explicitly.

#include <iostream>
#include <vector>

long long countRecursive(const std::vector<int> &values, int index,
                         long long remaining) {
    // base case: no elements left
    if (index == (int)values.size()) {
        return remaining == 0 ? 1 : 0;
    }

    // skip the current element
    long long skip = countRecursive(values, index + 1, remaining);

    // take the current element
    long long take =
        countRecursive(values, index + 1, remaining - values[index]);

    return skip + take;
}

long long countBrute(const std::vector<int> &values, int target) {
    int n = (int)values.size();
    long long count = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += values[i];
            }
        }

        if (sum == target) {
            count++;
        }
    }

    return count;
}

int main() {
    std::vector<int> first = {1, 2, 3, 4, 5};
    std::vector<int> second = {2, 4, 6};
    std::vector<int> third = {1, 2, 3};

    std::cout << countRecursive(first, 0, 5) << " " << countBrute(first, 5)
              << "\n";  // 3 3

    std::cout << countRecursive(second, 0, 6) << " " << countBrute(second, 6)
              << "\n";  // 2 2

    std::cout << countRecursive(third, 0, 0) << "\n";    // 1
    std::cout << countRecursive(third, 0, 100) << "\n";  // 0

    std::vector<int> empty;
    std::cout << countRecursive(empty, 0, 0) << " "
              << countRecursive(empty, 0, 1) << "\n";  // 1 0

    return 0;
}

// Output:
// 3 3
// 2 2
// 1
// 0
// 1 0
//
// Time complexity: O(2^n) for both versions.
// Space complexity:
//   recursive -> O(n) recursion stack
//   brute     -> O(1) extra (the bit mask is the only state)
//
// Edge cases: empty array, target 0 (the empty subsequence counts once),
// duplicate values (they are distinct positions, so both are counted), and
// negative values (the recursion still works; pruning on remaining < 0 would
// not be safe unless all values are non-negative).
