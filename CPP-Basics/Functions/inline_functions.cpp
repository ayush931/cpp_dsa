// Inline functions and inline variables
//
// A normal function call has overhead: push arguments, jump, return.
// The `inline` keyword suggests that the compiler replace a call with the
// function body. Modern compilers decide on their own, so `inline` is mostly
// a linkage specification today:
//   - it allows the same function definition in multiple translation units
//   - functions defined inside a class are implicitly inline
//   - `inline` variables (C++17) can be defined in a header without ODR errors
//
// Macros are textual replacement and have no type checking. Prefer inline
// functions (or constexpr functions) over macros.

#include <iostream>
using namespace std;

#define SQUARE_MACRO(x) ((x) * (x))

inline int square(int value) {
    return value * value;
}

// C++17 inline variable: one definition shared by every translation unit.
inline int functionCallCount = 0;

inline int squareAndCount(int value) {
    functionCallCount++;
    return value * value;
}

class Circle {
    double radius;

   public:
    Circle(double r) {
        radius = r;
    }

    // Defined inside the class -> implicitly inline.
    double area() {
        return 3.141592653589793 * radius * radius;
    }
};

int readNumber() {
    cout << "readNumber called" << endl;
    return 3;
}

int main() {
    cout << square(5) << endl;        // 25
    cout << SQUARE_MACRO(5) << endl;  // 25

    // The macro evaluates its argument twice, so side effects are dangerous.
    cout << SQUARE_MACRO(readNumber()) << endl;  // readNumber called twice
    cout << square(readNumber()) << endl;        // readNumber called once

    squareAndCount(2);
    squareAndCount(3);
    cout << functionCallCount << endl;  // 2

    Circle circle(2.0);
    cout << circle.area() << endl;  // 12.5664

    return 0;
}

// `inline` is a hint, not a command. The compiler may ignore it for large
// functions, and it may inline small functions even without the keyword.
//
// Special cases: recursive functions are usually not inlined, taking the
// address of an inline function forces a real function to exist, and inline
// variables must be initialized before use across translation units.
