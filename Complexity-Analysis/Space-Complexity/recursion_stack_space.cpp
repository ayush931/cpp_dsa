// Space complexity of recursion
//
// Every active function call keeps a stack frame with its parameters, locals,
// and return address. Recursion therefore uses O(depth) stack space, even when
// the function allocates nothing on the heap.
//
// Important distinction:
//   - time complexity counts the number of calls
//   - space complexity counts the maximum number of frames alive at once
//
// Naive Fibonacci makes O(2^n) calls but only O(n) frames are alive at the
// deepest point, so its auxiliary space is O(n), not O(2^n).

#include <iostream>
#include <vector>
using namespace std;

int maximumDepth = 0;

long long sumRecursive(vector<int> &values, int index, int depth) {
    if (depth > maximumDepth) {
        maximumDepth = depth;
    }

    if (index == values.size()) {
        return 0;
    }

    return values[index] + sumRecursive(values, index + 1, depth + 1);
}

long long sumIterative(vector<int> &values) {
    long long total = 0;

    for (int value : values) {
        total += value;
    }

    return total;
}

long long fibonacciNaive(int n, int depth, int &maxDepth) {
    if (depth > maxDepth) {
        maxDepth = depth;
    }

    if (n <= 1) {
        return n;
    }

    return fibonacciNaive(n - 1, depth + 1, maxDepth) +
           fibonacciNaive(n - 2, depth + 1, maxDepth);
}

long long fibonacciMemo(int n, vector<long long> &memo, int depth,
                        int &maxDepth) {
    if (depth > maxDepth) {
        maxDepth = depth;
    }

    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }

    return memo[n] = fibonacciMemo(n - 1, memo, depth + 1, maxDepth) +
                     fibonacciMemo(n - 2, memo, depth + 1, maxDepth);
}

int main() {
    vector<int> values;

    for (int i = 1; i <= 10; i++) {
        values.push_back(i);
    }

    maximumDepth = 0;
    cout << sumRecursive(values, 0, 1) << endl;  // 55
    cout << maximumDepth << endl;                // 11 frames (index 0..10)

    cout << sumIterative(values) << endl;  // 55, O(1) extra space

    int naiveDepth = 0;
    cout << fibonacciNaive(20, 1, naiveDepth) << endl;  // 6765
    cout << naiveDepth << endl;                         // 20

    vector<long long> memo(21, -1);
    int memoDepth = 0;
    cout << fibonacciMemo(20, memo, 1, memoDepth) << endl;  // 6765
    cout << memoDepth << endl;                              // 20

    // Both versions use O(n) stack space, but the naive version makes about
    // 2^n calls while the memoized version makes about n calls.
    return 0;
}

// Space complexity:
//   sumRecursive -> O(n) stack, O(1) heap
//   sumIterative -> O(1) extra space
//   fibonacciNaive -> O(n) stack, O(1) heap
//   fibonacciMemo -> O(n) stack + O(n) heap for the memo table
//
// Special cases: n = 0 (one frame), a very deep recursion (stack overflow,
// often around 10^5 frames on the default stack), tail recursion (some
// compilers reuse the frame, but C++ does not guarantee it), and recursion on
// a linked list or tree (space is the height, which can be n for a skewed
// tree).
//
// To reduce stack usage, convert the recursion to a loop or use an explicit
// stack on the heap.
