/**
 * consteval and constinit (C++20)
 *
 * constexpr
 *   - may be evaluated at compile time or at runtime
 *   - usable in constant expressions when the arguments are constant
 *
 * consteval
 *   - must be evaluated at compile time
 *   - calling it with runtime arguments is a compile error
 *   - useful for compile-time validation, lookup tables, and DSLs
 *
 * constinit
 *   - the variable must be initialized by a constant expression
 *   - the variable itself is not const and can be modified at runtime
 *   - guarantees no dynamic initialization, so it is ready before main()
 *
 * constexpr variable -> compile-time value, implicitly const
 * constinit variable -> compile-time initialization, mutable at runtime
 *
 * std::is_constant_evaluated() (C++20) lets one function choose a different
 * implementation during constant evaluation. C++23 adds `if consteval`.
 */

#include <array>
#include <iostream>
#include <type_traits>
using namespace std;

consteval int compileTimeSquare(int value) {
    return value * value;
}

constexpr int runtimeOrCompileTimeSquare(int value) {
    return value * value;
}

constexpr int factorial(int n) {
    int result = 1;

    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

// A constexpr function can detect whether it is running at compile time.
constexpr int safeDivide(int a, int b) {
    if (is_constant_evaluated() && b == 0) {
        return 0;  // compile-time fallback; runtime zero is handled below
    }

    return b == 0 ? 0 : a / b;
}

// constinit: static storage, constant initialization, still mutable.
constinit int globalCounter = 42;

// A constexpr array is built entirely at compile time.
constexpr array<int, 5> squares = {compileTimeSquare(1), compileTimeSquare(2),
                                   compileTimeSquare(3), compileTimeSquare(4),
                                   compileTimeSquare(5)};

int main() {
    // consteval: the argument is a constant expression.
    constexpr int compileTimeValue = compileTimeSquare(7);
    cout << compileTimeValue << endl;  // 49

    // consteval with a runtime argument would not compile:
    // int runtime = 7;
    // cout << compileTimeSquare(runtime) << endl;

    // constexpr: works at compile time and at runtime.
    int runtimeValue = 7;
    cout << runtimeOrCompileTimeSquare(runtimeValue) << endl;  // 49

    cout << factorial(5) << endl;  // 120

    cout << safeDivide(10, 2) << endl;  // 5
    cout << safeDivide(10, 0) << endl;  // 0

    cout << globalCounter << endl;  // 42
    globalCounter = 100;            // constinit does not mean const
    cout << globalCounter << endl;  // 100

    for (int value : squares) {
        cout << value << " ";
    }
    cout << endl;
    // 1 4 9 16 25

    return 0;
}

/**
 * Time complexity -> consteval work happens at compile time; runtime is O(1)
 * Space complexity -> compile-time tables live in the binary, not on the stack
 *
 * Special cases:
 *   - a consteval function cannot be called through a function pointer at
 *     runtime
 *   - a constexpr function that cannot be evaluated at compile time is still
 *     valid, as long as it is not used in a constant expression
 *   - constinit requires static or thread storage duration
 *   - constinit does not imply const, and const does not imply constinit
 *   - a constinit variable cannot depend on a runtime value
 *   - std::is_constant_evaluated() is true only during constant evaluation
 *
 * Use consteval when a value must be known at compile time (for example, a
 * validated format string or a table size). Use constinit to eliminate static
 * initialization order problems without making the object immutable.
 */
