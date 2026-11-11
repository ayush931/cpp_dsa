/**
 * Alignment and padding
 *
 * alignof(T) -> required alignment of T
 * sizeof(T) -> size including padding
 *
 * The compiler inserts padding so every member is properly aligned and the
 * size is a multiple of the alignment. This is why member order matters.
 *
 * struct A { char c; int i; char d; };   // 12 bytes on most platforms
 * struct B { int i; char c; char d; };   // 8 bytes
 *
 * alignas(n) -> request a stricter alignment
 * std::aligned_storage (deprecated in C++23) -> raw aligned buffer
 * A type with alignment > max_align_t needs aligned allocation (C++17
 * aligned new, or posix_memalign / std::aligned_alloc)
 *
 * Cache line -> usually 64 bytes; false sharing happens when two threads
 * write different variables on the same cache line
 */

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <new>
using namespace std;

struct BadLayout {
    char first;
    int number;
    char second;
};

struct GoodLayout {
    int number;
    char first;
    char second;
};

struct alignas(16) AlignedVector {
    float x;
    float y;
    float z;
    float w;
};

struct alignas(64) CacheLinePadded {
    int value;
    char padding[60];
};

int main() {
    cout << sizeof(BadLayout) << " " << alignof(BadLayout) << endl;    // 12 4
    cout << sizeof(GoodLayout) << " " << alignof(GoodLayout) << endl;  // 8 4

    cout << alignof(int) << endl;     // 4
    cout << alignof(double) << endl;  // 8
    cout << alignof(max_align_t)
         << endl;  // platform-dependent (8 on arm64 macOS)

    cout << sizeof(AlignedVector) << " " << alignof(AlignedVector)
         << endl;                             // 16 16
    cout << sizeof(CacheLinePadded) << endl;  // 64

    // offsets show where padding lives
    cout << offsetof(BadLayout, first) << endl;   // 0
    cout << offsetof(BadLayout, number) << endl;  // 4
    cout << offsetof(BadLayout, second) << endl;  // 8

    // a common bug: reading a struct from a byte buffer without alignment
    unsigned char buffer[sizeof(GoodLayout) + 1];
    // GoodLayout *bad = reinterpret_cast<GoodLayout*>(buffer + 1);   //
    // misaligned

    // alignment-aware allocation
    void *memory = ::operator new(sizeof(AlignedVector), align_val_t(16));
    AlignedVector *vector = new (memory) AlignedVector{1, 2, 3, 4};

    cout << vector->x << " " << vector->w << endl;  // 1 4

    vector->~AlignedVector();
    ::operator delete(memory, align_val_t(16));

    return 0;
}

// Special cases: empty struct has size 1 (every object needs a unique address);
// base class subobjects may reuse padding; #pragma pack(1) removes padding but
// can cause misaligned access and crashes on strict architectures;
// atomics and mutexes should not share a cache line under contention;
// over-aligned types need C++17 aligned new or platform-specific allocation
