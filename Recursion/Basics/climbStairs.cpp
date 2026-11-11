// Climbing stairs: count the ways to reach step n when each move is either
// 1 step or 2 steps.
//
// This is the Fibonacci recurrence with different initial values:
//   ways(0) = 1  (do nothing)
//   ways(1) = 1
//   ways(n) = ways(n - 1) + ways(n - 2)
//
// Example:
//   n = 1 -> 1
//   n = 2 -> 2   (1+1, 2)
//   n = 3 -> 3   (1+1+1, 1+2, 2+1)
//   n = 5 -> 8
//   n = 10 -> 89
//
// The plain recursive version recomputes the same states many times. Memoizing
// or iterating reduces the time to O(n).

#include <iostream>
#include <vector>

long long climbBrute(int n) {
    if (n < 0) {
        return 0;
    }
    if (n <= 1) {
        return 1;
    }
    return climbBrute(n - 1) + climbBrute(n - 2);
}

long long climbMemo(int n, std::vector<long long> &memo) {
    if (n < 0) {
        return 0;
    }
    if (n <= 1) {
        return 1;
    }
    if (memo[n] != -1) {
        return memo[n];
    }

    memo[n] = climbMemo(n - 1, memo) + climbMemo(n - 2, memo);
    return memo[n];
}

long long climbIterative(int n) {
    if (n < 0) {
        return 0;
    }

    long long previous = 1;  // ways(0)
    long long current = 1;   // ways(1)

    for (int step = 2; step <= n; step++) {
        long long next = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

int main() {
    std::cout << climbBrute(10) << "\n";  // 89

    std::vector<long long> memo(11, -1);
    std::cout << climbMemo(10, memo) << "\n";  // 89
    std::cout << climbIterative(10) << "\n";   // 89

    std::cout << climbIterative(0) << "\n";  // 1
    std::cout << climbIterative(1) << "\n";  // 1
    std::cout << climbIterative(2) << "\n";  // 2
    std::cout << climbIterative(5) << "\n";  // 8

    return 0;
}

// Output:
// 89
// 89
// 89
// 1
// 1
// 2
// 8
//
// Time complexity:
//   brute      -> O(2^n)
//   memoized   -> O(n)
//   iterative  -> O(n)
// Space complexity:
//   brute      -> O(n) stack
//   memoized   -> O(n) table + O(n) stack
//   iterative  -> O(1)
//
// Edge cases: n = 0 returns 1 by convention; negative n returns 0. Use long
// long because the count grows like Fibonacci numbers and overflows int early.
