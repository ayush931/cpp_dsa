/**
 * Type punning: reinterpreting object representation
 *
 * Type punning means reading the bits of one type as another type. It is a
 * common source of undefined behavior.
 *
 * 1. reinterpret_cast<T*>(&object)
 *    - reinterprets the address, not the value
 *    - dereferencing the result violates strict aliasing unless the types are
 *      related (for example, a pointer to the first member of a standard
 *      layout object)
 *
 * 2. union
 *    - reading a member other than the one last written is UB in C++, although
 *      many compilers support it as an extension
 *    - still useful for inspecting the common initial sequence of standard
 *      layout structs
 *
 * 3. std::memcpy
 *    - copies the object representation into a different object
 *    - well-defined for trivially copyable types
 *    - the compiler usually optimizes it to a register move
 *
 * 4. std::bit_cast<To>(from) (C++20)
 *    - the type-safe, constexpr-friendly version of memcpy
 *    - requires sizeof(To) == sizeof(From) and both trivially copyable
 *    - the recommended way to reinterpret bits
 */

#include <bit>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

int main() {
    float value = 3.14159f;

    // 1. bit_cast: the portable, well-defined way.
    uint32_t bits = bit_cast<uint32_t>(value);
    cout << hex << bits << dec << endl;  // 40490fd0

    // 2. memcpy: the pre-C++20 equivalent.
    uint32_t copiedBits;
    memcpy(&copiedBits, &value, sizeof(value));
    cout << (copiedBits == bits) << endl;  // 1

    // 3. Union: reading a member other than the one last written is UB in C++.
    //    Many compilers accept it, but the result is not portable. Read the
    //    active member and convert it instead.
    union FloatBits {
        float asFloat;
        uint32_t asBits;
    };

    FloatBits pun;
    pun.asFloat = value;
    cout << hex << bit_cast<uint32_t>(pun.asFloat) << dec << endl;
    // 40490fd0 (reading pun.asBits would be UB)

    // 4. reinterpret_cast: the address is reused, not the value. Reading
    // through
    //    the pointer would violate strict aliasing, so only the address is
    //    used.
    uint32_t *pointer = reinterpret_cast<uint32_t *>(&value);
    cout << (static_cast<void *>(pointer) == static_cast<void *>(&value))
         << endl;
    // 1 (the address is unchanged; *pointer would be aliasing UB)

    // Round-trip: bits back to a float.
    float restored = bit_cast<float>(bits);
    cout << restored << endl;  // 3.14159

    // Inspect the bytes in memory. This is well-defined: unsigned char may
    // alias any object.
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&value);
    for (int i = 0; i < 4; i++) {
        cout << hex << setw(2) << setfill('0') << (int)bytes[i] << " ";
    }
    cout << dec << setfill(' ') << endl;
    // d0 0f 49 40 on little-endian

    return 0;
}

/**
 * Time complexity -> O(1)
 * Space complexity -> O(1)
 *
 * Special cases:
 *   - endianness changes the byte order but not the bit_cast result
 *   - sizeof(float) and sizeof(uint32_t) must match; bit_cast enforces it at
 *     compile time
 *   - bit_cast requires a trivially copyable source and destination
 *   - reading a union member that was not last written is UB, even if the
 *     bits happen to look correct
 *   - dereferencing a reinterpret_cast pointer to an unrelated type violates
 *     strict aliasing; the optimizer may reorder or delete the access
 *   - `unsigned char` and `std::byte` are the standard exceptions: they may
 *     inspect any object's representation
 *
 * Prefer std::bit_cast for bit-level conversions, std::memcpy when C++20 is
 * not available, and a designed conversion function (for example, a
 * fixed-point type) when the operation has semantic meaning. Avoid
 * reinterpret_cast for type punning; reserve it for address-level interfaces.
 */
