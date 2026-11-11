/**
 * Undefined behavior (UB) -> the standard imposes no requirements
 * The program may crash, produce garbage, appear to work, or do something
 * different after a compiler upgrade or at a different optimization level
 *
 * Common sources:-
 * - Signed integer overflow
 * - Out-of-bounds array access
 * - Reading an uninitialized variable
 * - Null or dangling pointer dereference
 * - Use after free / double delete
 * - Division by zero
 * - Shifting by a negative amount or by >= bit width
 * - Modifying a string literal
 * - Data race
 * - Violating the ODR
 *
 * Unsigned overflow is defined (wraps modulo 2^n)
 * Use -fsanitize=address,undefined during development
 * UB is not the same as unspecified behavior (the standard allows several
 * results) or implementation-defined behavior (the compiler documents one)
 */

#include <climits>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

int safeAdd(int a, int b) {
    if (b > 0 && a > INT_MAX - b) {
        cout << "overflow detected" << endl;
        return INT_MAX;
    }

    if (b < 0 && a < INT_MIN - b) {
        cout << "underflow detected" << endl;
        return INT_MIN;
    }

    return a + b;
};

int main() {
    // signed overflow: undefined
    int maximum = INT_MAX;
    // int overflow = maximum + 1;   // UB, do not run this

    cout << safeAdd(maximum, 1) << endl;  // INT_MAX

    // unsigned overflow: defined, wraps
    unsigned int zero = 0;
    cout << zero - 1 << endl;  // 4294967295

    // out-of-bounds: undefined
    vector<int> numbers = {1, 2, 3};
    // cout << numbers[3] << endl;   // UB
    cout << numbers.at(2) << endl;  // 3, at() throws on out-of-bounds

    try {
        cout << numbers.at(3) << endl;
    } catch (out_of_range &error) {
        cout << "out of range" << endl;
    }

    // uninitialized read: undefined
    int uninitialized;
    // cout << uninitialized << endl;   // UB
    int initialized = 0;
    cout << initialized << endl;  // 0

    // use after free: undefined
    int *value = new int(10);
    delete value;
    // cout << *value << endl;   // UB
    value = nullptr;

    if (value != nullptr) {
        cout << *value << endl;
    } else {
        cout << "pointer is null" << endl;
    }

    // division by zero: undefined for integers, inf/nan for floating point
    // int bad = 10 / 0;   // UB
    double floating = numeric_limits<double>::infinity();
    cout << floating << endl;  // inf

    // shift rules
    unsigned int one = 1;
    cout << (one << 31) << endl;  // defined for unsigned
    // int negative = -1 << 1;      // UB, left shift of a negative value
    // int tooFar = 1 << 32;        // UB, shift >= width

    // string literal is read-only
    // char *literal = "hello";     // error in C++11 and later
    const char *literal = "hello";
    cout << literal << endl;

    // data race: undefined, use a mutex or atomic
    // i = i++ is UB: the read and the write are unsequenced
    int i = 0;
    i = i + 1;  // safe
    cout << i << endl;

    return 0;
}

// Special cases: UB can be optimized away, so "it works on my machine" proves
// nothing; std::vector::operator[] does not check bounds; new throws
// bad_alloc instead of returning null (use nothrow for the null form);
// deleting the same non-null pointer twice is UB, but deleting a null pointer
// is always safe; signed char overflow is undefined, unsigned char is defined
