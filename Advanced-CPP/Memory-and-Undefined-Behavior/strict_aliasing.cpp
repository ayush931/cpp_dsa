/**
 * Strict aliasing -> the compiler assumes pointers of different types do not
 * point to the same object
 *
 * This is why accessing an int through a float* is undefined behavior, except
 * for char, unsigned char and std::byte, which may alias anything.
 *
 * Safe ways to reinterpret bits:-
 * - std::memcpy (the compiler optimizes it away)
 * - std::bit_cast (C++20, requires trivially copyable types of equal size)
 * - a union is common in C but type punning through a union is UB in C++
 *
 * The optimizer can cache a value across a write through another pointer type,
 * producing results that look impossible at -O2 but fine at -O0.
 *
 * Compile with -fno-strict-aliasing only as a last resort; fix the code
 * instead.
 */

#include <bit>
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

float intToFloatUnsafe(uint32_t bits) {
    // return *reinterpret_cast<float*>(&bits);   // UB: violates strict
    // aliasing
    return 0;
};

float intToFloatSafe(uint32_t bits) {
    float value;
    memcpy(&value, &bits, sizeof(value));
    return value;
};

uint32_t floatToIntSafe(float value) {
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
};

struct Packet {
    uint8_t version;
    uint8_t flags;
    uint16_t length;
};

// serialization must not depend on struct layout
void serialize(Packet &packet, unsigned char *buffer) {
    buffer[0] = packet.version;
    buffer[1] = packet.flags;
    buffer[2] = packet.length & 0xFF;
    buffer[3] = (packet.length >> 8) & 0xFF;
};

Packet deserialize(unsigned char *buffer) {
    Packet packet;
    packet.version = buffer[0];
    packet.flags = buffer[1];
    packet.length = buffer[2] | (buffer[3] << 8);
    return packet;
};

int main() {
    uint32_t bits = 0x3F800000;  // IEEE-754 representation of 1.0f

    float value = intToFloatSafe(bits);
    cout << value << endl;  // 1

    uint32_t roundTrip = floatToIntSafe(value);
    cout << hex << roundTrip << dec << endl;  // 3f800000

    // C++20 bit_cast, compile-time friendly and explicit
    float fromBits = bit_cast<float>(bits);
    cout << fromBits << endl;  // 1

    uint32_t backToBits = bit_cast<uint32_t>(fromBits);
    cout << hex << backToBits << dec << endl;

    Packet packet = {1, 0, 300};
    unsigned char buffer[4];

    serialize(packet, buffer);
    Packet copy = deserialize(buffer);

    cout << (int)copy.version << " " << copy.length << endl;  // 1 300

    // char may alias any object
    int number = 0x01020304;
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&number);
    cout << hex << (int)bytes[0] << dec << endl;

    return 0;
}

// Special cases: memcpy with overlapping regions is UB (use memmove);
// bit_cast requires equal sizes and trivially copyable types;
// accessing a base class object through a derived pointer is UB;
// casting away const and writing is UB if the object is truly const;
// char aliasing is allowed for reading, writing object representation is
// limited to trivially copyable objects
