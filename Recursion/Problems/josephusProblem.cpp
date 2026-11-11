// Josephus problem
//
// n people stand in a circle, numbered 1 to n. Starting at person 1, count k
// people and remove the k-th one, then continue from the next person. Find the
// last survivor.
//
// Examples:
//   n = 5, k = 2 -> 3
//   n = 7, k = 3 -> 4
//   n = 1, k = 5 -> 1
//
// The simulation removes one person per step and is O(n^2) with a vector.
// The recursive formula works in O(n):
//   J(1, k) = 0                    (0-indexed)
//   J(n, k) = (J(n - 1, k) + k) % n
// The answer is J(n, k) + 1 when people are numbered from 1.

#include <iostream>
#include <vector>

// Brute-force simulation using a vector of remaining people.
int josephusSimulation(int n, int k) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    std::vector<int> people(n);
    for (int i = 0; i < n; i++) {
        people[i] = i + 1;
    }

    int index = 0;

    while (people.size() > 1) {
        index = (index + k - 1) % (int)people.size();
        people.erase(people.begin() + index);
    }

    return people[0];
}

// Recursive solution in 0-indexed form.
int josephusZeroIndexed(int n, int k) {
    if (n == 1) {
        return 0;
    }
    return (josephusZeroIndexed(n - 1, k) + k) % n;
}

int josephusRecursive(int n, int k) {
    if (n <= 0) {
        return 0;
    }
    return josephusZeroIndexed(n, k) + 1;
}

int main() {
    std::cout << josephusSimulation(5, 2) << " " << josephusRecursive(5, 2)
              << "\n";  // 3 3

    std::cout << josephusSimulation(7, 3) << " " << josephusRecursive(7, 3)
              << "\n";  // 4 4

    std::cout << josephusRecursive(1, 5) << "\n";  // 1
    std::cout << josephusRecursive(0, 5) << "\n";  // 0

    return 0;
}

// Output:
// 3 3
// 4 4
// 1
// 0
//
// Time complexity:
//   simulation -> O(n^2) because each erase shifts elements
//   recursive  -> O(n)
// Space complexity:
//   simulation -> O(n)
//   recursive  -> O(n) recursion stack
//
// Edge cases: n = 0 (no survivor), n = 1 (the only person survives), k = 1
// (people are removed in order, so the last person is n), and k larger than n
// (the modulo handles the wrap-around).
