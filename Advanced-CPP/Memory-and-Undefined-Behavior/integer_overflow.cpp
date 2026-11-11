/**
 * Integer overflow and checked arithmetic
 *
 * Unsigned integers wrap modulo 2^N. That is well-defined:
 *   UINT_MAX + 1 == 0
 *
 * Signed integer overflow is undefined behavior:
 *   INT_MAX + 1        // UB, not necessarily INT_MIN
 *   -INT_MIN           // UB, because the positive value does not fit
 *   INT_MIN / -1       // UB
 *
 * The optimizer is allowed to assume signed overflow never happens, so the
 * generated code can do surprising things. Use unsigned types for bit patterns
 * and modular arithmetic, and checked arithmetic for values that must stay in
 * range.
 *
 * Checked helpers:
 *   - __builtin_add_overflow / sub / mul (GCC and Clang)
 *   - std::numeric_limits<T>::max() / min()
 *   - wider types (long long) when the range is known
 *   - std::cmp_* (C++20) for comparing integers without signed/unsigned bugs
 */

#include <climits>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
using namespace std;

// Portable checked addition. Returns false on overflow.
bool safeAdd(int a, int b, int &result) {
    if (b > 0 && a > INT_MAX - b) {
        return false;
    }
    if (b < 0 && a < INT_MIN - b) {
        return false;
    }

    result = a + b;
    return true;
}

bool safeMultiply(int a, int b, int &result) {
    if (a == 0 || b == 0) {
        result = 0;
        return true;
    }

    long long product = (long long)a * b;

    if (product < INT_MIN || product > INT_MAX) {
        return false;
    }

    result = (int)product;
    return true;
}

// The compiler builtin is shorter and often faster.
bool builtinAdd(int a, int b, int &result) {
    return !__builtin_add_overflow(a, b, &result);
}

bool builtinMultiply(int a, int b, int &result) {
    return !__builtin_mul_overflow(a, b, &result);
}

int main() {
    unsigned int maximum = numeric_limits<unsigned int>::max();
    cout << maximum << endl;      // 4294967295
    cout << maximum + 1 << endl;  // 0 (defined wraparound)

    int result = 0;

    cout << safeAdd(INT_MAX, 1, result) << endl;              // 0 (overflow)
    cout << safeAdd(40, 2, result) << " " << result << endl;  // 1 42

    cout << safeMultiply(100000, 100000, result) << endl;  // 0
    cout << safeMultiply(1000, 1000, result) << " " << result
         << endl;  // 1 1000000

    cout << builtinAdd(INT_MAX, 1, result) << endl;         // 0
    cout << builtinMultiply(50000, 50000, result) << endl;  // 0

    // size_t is unsigned: subtracting past zero wraps to a huge value.
    size_t small = 1;
    size_t large = 2;
    cout << (small - large > 1000000) << endl;  // 1 (wrapped)

    // A safe average avoids overflowing a + b.
    int a = INT_MAX;
    int b = INT_MAX - 2;
    int average = a / 2 + b / 2 + (a % 2 + b % 2) / 2;
    cout << average << endl;  // 2147483646

    // std::midpoint (C++20) does the same thing without overflow.
    cout << std::midpoint(a, b) << endl;  // 2147483646

    return 0;
}

/**
 * Time complexity -> O(1) per check
 * Space complexity -> O(1)
 *
 * Special cases:
 *   - INT_MIN / -1 and INT_MIN % -1 are UB
 *   - -INT_MIN is UB
 *   - left-shifting a negative value is UB before C++20; shifting by >= the
 *     width is always UB
 *   - signed/unsigned comparison converts the signed value to unsigned, which
 *     can turn -1 into a huge positive number
 *   - `size()` returns size_t; `for (int i = 0; i < v.size(); i++)` can warn
 *     or misbehave when the container is huge
 *   - overflow in floating point is not UB; it produces infinity or a NaN
 *
 * Production code often uses a checked integer library, a wider type, or
 * explicit range checks at the boundary where untrusted input enters the
 * system. Do not rely on wraparound for signed types, and do not assume the
 * compiler will "do the obvious thing" after overflow.
 */
