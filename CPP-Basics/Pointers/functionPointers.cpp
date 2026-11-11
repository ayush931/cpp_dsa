// Function pointers
//
// A function name decays to a pointer to the function. A function pointer can
// be stored, passed to another function (a callback), and called later.
//
// Declaration:
//   int (*op)(int, int);  // op points to a function taking two ints
//
// The parentheses around *op are required: without them the declaration would
// describe a function returning int*.
//
// Example:
//   int add(int a, int b) { return a + b; }
//   int (*op)(int, int) = add;
//   op(2, 3); // 5

#include <functional>
#include <iostream>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

// Run any binary integer operation supplied by the caller.
int apply(int (*operation)(int, int), int a, int b) {
    return operation(a, b);
}

int main() {
    int (*op)(int, int) = add;
    std::cout << op(2, 3) << "\n";  // 5

    op = multiply;
    std::cout << op(2, 3) << "\n";  // 6

    std::cout << apply(subtract, 9, 4) << "\n";  // 5

    // Array of function pointers, selected by an index.
    int (*operations[3])(int, int) = {add, subtract, multiply};

    for (int i = 0; i < 3; i++) {
        std::cout << operations[i](6, 2) << " ";
    }
    std::cout << "\n";  // 8 4 12

    // A captureless lambda converts to a function pointer.
    int (*increment)(int, int) = [](int value, int step) {
        return value + step;
    };
    std::cout << increment(10, 5) << "\n";  // 15

    // std::function can also hold lambdas that capture local state.
    int offset = 100;
    std::function<int(int)> addOffset = [offset](int value) {
        return value + offset;
    };
    std::cout << addOffset(7) << "\n";  // 107

    return 0;
}

// Output:
// 5
// 6
// 5
// 8 4 12
// 15
// 107
//
// Time complexity: O(1) per call.
// Space complexity: O(1).
//
// Edge cases: always check a function pointer for nullptr before calling it;
// the signatures must match exactly. Prefer std::function when the callable
// needs to capture state, and prefer lambdas for short local callbacks.
