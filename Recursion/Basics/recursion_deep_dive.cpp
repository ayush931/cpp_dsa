// Recursion in depth
//
// A recursive function solves a problem by calling itself on a smaller input.
// Every recursive function needs:
//   1. a base case that stops the recursion
//   2. a recursive case that moves toward the base case
//
// The call stack stores one frame per active call (arguments, locals, return
// address). Too many frames cause a stack overflow.
//
// Types of recursion:
//   - head recursion: the recursive call happens before the work
//   - tail recursion: the recursive call is the last operation
//   - tree recursion: a function makes more than one recursive call
//   - mutual recursion: two or more functions call each other
//
// C++ does not guarantee tail-call optimization. Prefer a loop when the
// recursion depth can be large.

#include <iostream>
#include <vector>
using namespace std;

// Head recursion: multiply after the recursive call returns.
long long factorialHead(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorialHead(n - 1);
}

// Tail recursion: the answer is carried in an accumulator.
long long factorialTail(int n, long long accumulator = 1) {
    if (n <= 1) {
        return accumulator;
    }
    return factorialTail(n - 1, accumulator * n);
}

// Tree recursion: two calls per level -> O(2^n).
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Mutual recursion: even calls odd and odd calls even.
bool isEven(int n);
bool isOdd(int n);

bool isEven(int n) {
    if (n == 0) {
        return true;
    }
    return isOdd(n - 1);
}

bool isOdd(int n) {
    if (n == 0) {
        return false;
    }
    return isEven(n - 1);
}

// Recursion over an array.
int sumArray(vector<int> &values, int index) {
    if (index == values.size()) {
        return 0;
    }
    return values[index] + sumArray(values, index + 1);
}

// Recursion over a string.
void printReverse(string &text, int index) {
    if (index < 0) {
        return;
    }
    cout << text[index];
    printReverse(text, index - 1);
}

int main() {
    cout << factorialHead(5) << endl;  // 120
    cout << factorialTail(5) << endl;  // 120

    cout << fibonacci(7) << endl;  // 13

    cout << isEven(10) << " " << isOdd(7) << endl;  // 1 1

    vector<int> values = {1, 2, 3, 4, 5};
    cout << sumArray(values, 0) << endl;  // 15

    string text = "recursion";
    printReverse(text, text.size() - 1);
    cout << endl;  // noisrucer

    return 0;
}

// Time complexity depends on the number of calls:
//   factorial -> O(n)
//   fibonacci -> O(2^n) without memoization, O(n) with memoization
// Space complexity -> O(n) call stack for a linear recursion
//
// Special cases: n = 0, n = 1, negative n (define a base case), deep recursion
// (stack overflow), overlapping subproblems (add memoization or use DP),
// mutual recursion needs a forward declaration.
