/**
 * std::span, std::bit_cast, and <bit> (C++20)
 *
 * std::span<T> is a non-owning view over a contiguous range. It lets a
 * function accept an array, a vector, or a C array without copying and without
 * writing a template for every container.
 *
 * <bit> provides portable bit operations:
 *   - bit_cast<To>(from): reinterpret the bits (same size, trivially copyable)
 *   - popcount(x): number of set bits
 *   - countl_zero(x) / countr_zero(x): leading/trailing zero bits
 *   - midpoint(a, b): (a + b) / 2 without overflow
 */

#include <bit>
#include <climits>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>
using namespace std;

int sum(span<const int> values) {
    int total = 0;

    for (int value : values) {
        total += value;
    }

    return total;
}

void addOne(span<int> values) {
    for (int &value : values) {
        value++;
    }
}

int main() {
    int rawArray[] = {1, 2, 3, 4};
    vector<int> dynamic = {5, 6, 7};

    cout << sum(rawArray) << endl;  // 10
    cout << sum(dynamic) << endl;   // 18

    addOne(dynamic);
    cout << sum(dynamic) << endl;  // 21

    // Reinterpret the bits of a float as a 32-bit integer and back.
    float pi = 3.1415927f;
    uint32_t bits = bit_cast<uint32_t>(pi);
    float restored = bit_cast<float>(bits);

    cout << bits << endl;      // 1078530011
    cout << restored << endl;  // 3.14159

    uint32_t value = 0b101101;
    cout << popcount(value) << endl;     // 4
    cout << countl_zero(value) << endl;  // 26 (32-bit type)
    cout << countr_zero(value) << endl;  // 0

    cout << midpoint(10, 20) << endl;                // 15
    cout << midpoint(INT_MAX, INT_MAX - 1) << endl;  // no overflow

    return 0;
}

/**
 * Time complexity -> O(n) for span traversal, O(1) for every bit operation
 * Space complexity -> O(1); a span does not own its data
 *
 * Special cases: an empty span, a span over a const range (read-only), a
 * span whose underlying container is modified or destroyed (dangling), a
 * bit_cast between different sizes (ill-formed), popcount on a signed type
 * (use an unsigned type), midpoint with negative values.
 *
 * A span is a pointer plus a length. It does not extend the lifetime of the
 * data it points to, so never return a span to a local container.
 */
