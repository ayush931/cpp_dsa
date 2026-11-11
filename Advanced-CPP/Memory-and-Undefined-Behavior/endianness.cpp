/**
 * Endianness -> byte order of multi-byte values in memory
 *
 * Little endian -> least significant byte first (x86, ARM in little mode)
 * Big endian -> most significant byte first (network byte order)
 *
 * C++20 std::endian -> std::endian::little / big / native
 * Before C++20, detect it at runtime or with compiler macros
 *
 * Never reinterpret a pointer to read a serialized format directly:
 * the layout depends on endianness, padding and alignment.
 * Serialize field by field, or use std::bit_cast and explicit byte swaps.
 *
 * htons / htonl / ntohs / ntohl -> host <-> network byte order
 * std::byteswap (C++23) -> reverse the bytes of a value
 */

#include <bit>
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

bool isLittleEndian() {
    uint16_t value = 1;
    unsigned char firstByte;

    memcpy(&firstByte, &value, 1);
    return firstByte == 1;
};

uint32_t swapBytes(uint32_t value) {
    return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
           ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
};

uint16_t toBigEndian(uint16_t value) {
    if (isLittleEndian()) {
        return static_cast<uint16_t>((value >> 8) | (value << 8));
    }
    return value;
};

void printBytes(const void *data, size_t size) {
    const unsigned char *bytes = static_cast<const unsigned char *>(data);

    for (size_t i = 0; i < size; i++) {
        cout << hex << (int)bytes[i] << " ";
    }
    cout << dec << endl;
};

int main() {
    cout << (isLittleEndian() ? "little" : "big") << endl;

    uint32_t value = 0x01020304;

    cout << hex << value << dec << endl;
    printBytes(&value, sizeof(value));  // 04 03 02 01 on little endian

    uint32_t swapped = swapBytes(value);
    cout << hex << swapped << dec << endl;  // 04030201
    printBytes(&swapped, sizeof(swapped));  // 01 02 03 04 on little endian

    cout << hex << toBigEndian(0x1234) << dec << endl;  // 3412

    // C++20 std::endian
    if constexpr (endian::native == endian::little) {
        cout << "compile-time: little" << endl;
    } else {
        cout << "compile-time: big" << endl;
    }

    // std::bit_cast preserves the bit pattern, unlike a numeric conversion
    float number = 1.0f;
    uint32_t bits = bit_cast<uint32_t>(number);
    cout << hex << bits << dec << endl;  // 3f800000

    return 0;
}

// Special cases: mixed-endian (rare), unaligned access, signed byte order,
// serialization must define an explicit byte order (usually big endian),
// float endianness is the same as integer on mainstream platforms,
// do not use unions for type punning in C++ (undefined behavior)
