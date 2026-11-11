// Recursion vs iteration
//
// Recursion solves a problem by calling itself on a smaller input. Iteration
// repeats a block with a loop. Both are equivalent in power, but they trade
// time, stack space, and clarity.
//
// Example:
//   factorial(5) = 5 * 4 * 3 * 2 * 1 = 120
//   fibonacci(10) = 55
//
// Recursion usually mirrors the mathematical definition:
//
//   factorial(n) = 1                  if n == 0
//                = n * factorial(n-1) otherwise
//
// Iteration keeps the state in variables and avoids call overhead:
//
//   result = 1
//   for i = 2..n: result *= i
//
// Naive Fibonacci is a warning sign: fib(n) calls fib(n-1) and fib(n-2), so
// the same subproblems are recomputed and the number of calls grows like
// O(2^n). The iterative version is O(n) time and O(1) space.
//
// Special cases:
//   - n = 0 and n = 1 are the base cases
//   - negative input must be rejected or defined separately
//   - factorial overflows quickly; 20! is the largest value that fits in a
//     64-bit signed integer
//   - deep recursion can overflow the call stack; iterative code does not
//   - a recursive function must make progress toward a base case
//
// Complexity:
//   factorial recursion -> O(n) time, O(n) stack
//   factorial loop      -> O(n) time, O(1) space
//   fibonacci recursion -> O(2^n) time, O(n) stack
//   fibonacci loop      -> O(n) time, O(1) space

#include <iostream>
using namespace std;

long long factorialRecursive(int n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorialRecursive(n - 1);
}

long long factorialIterative(int n) {
    long long result = 1;

    for (int value = 2; value <= n; value++) {
        result *= value;
    }

    return result;
}

long long fibonacciRecursive(int n, long long &calls) {
    calls++;

    if (n <= 1) {
        return n;
    }

    return fibonacciRecursive(n - 1, calls) + fibonacciRecursive(n - 2, calls);
}

long long fibonacciIterative(int n) {
    if (n <= 1) {
        return n;
    }

    long long previous = 0;
    long long current = 1;

    for (int i = 2; i <= n; i++) {
        long long next = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

int sumDigitsRecursive(int n) {
    if (n < 10) {
        return n;
    }

    return n % 10 + sumDigitsRecursive(n / 10);
}

int sumDigitsIterative(int n) {
    int sum = 0;

    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }

    return sum;
}

int main() {
    cout << factorialRecursive(6) << " " << factorialIterative(6) << endl;

    long long calls = 0;
    cout << fibonacciRecursive(10, calls) << " " << fibonacciIterative(10)
         << endl;
    cout << "recursive calls: " << calls << endl;

    cout << sumDigitsRecursive(12345) << " " << sumDigitsIterative(12345)
         << endl;

    // Edge cases.
    cout << factorialRecursive(0) << " " << factorialIterative(0) << endl;
    cout << fibonacciIterative(0) << " " << fibonacciIterative(1) << endl;

    return 0;
}

// Output:
// 720 720
// 55 55
// recursive calls: 177
// 15 15
// 1 1
// 0 1
//
// Use recursion when the recursive structure is natural (trees, divide and
// conquer, backtracking) and the depth is bounded. Use iteration when the
// problem is a simple sequence, when the depth could be large, or when the
// call overhead matters. Any recursion can be converted to iteration with an
// explicit stack, but that is not always simpler.
