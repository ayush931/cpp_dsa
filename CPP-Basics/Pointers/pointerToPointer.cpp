// Pointer to pointer
//
// A pointer stores an address. A pointer to pointer stores the address of a
// pointer, so a function can change which object the caller's pointer refers
// to. This is also how a function can allocate memory and hand the result back.
//
// Example:
//   int x = 5;
//   int *p = &x;
//   int **pp = &p;
//   **pp = 9;  // changes x through two levels of indirection

#include <iostream>

// Allocate one int and give the caller ownership through *out.
void allocate(int **out, int value) {
    *out = new int(value);
}

// Point the caller's pointer at an existing object.
void reassign(int **out, int *target) {
    *out = target;
}

int main() {
    int x = 5;
    int *p = &x;
    int **pp = &p;

    std::cout << **pp << "\n";  // 5
    **pp = 9;                   // same as x = 9
    std::cout << x << "\n";     // 9

    int *heapValue = nullptr;
    allocate(&heapValue, 42);
    std::cout << *heapValue << "\n";  // 42
    delete heapValue;
    heapValue = nullptr;

    int y = 7;
    reassign(&p, &y);
    std::cout << *p << "\n";  // 7

    // p now points at y, so changing *p changes y, not x.
    *p = 11;
    std::cout << x << " " << y << "\n";  // 9 11

    return 0;
}

// Output:
// 5
// 9
// 42
// 7
// 9 11
//
// Time complexity: O(1) for every operation.
// Space complexity: O(1), plus one heap allocation that is deleted.
//
// Edge cases: always initialize the pointer to nullptr before passing its
// address to an allocating function; check for allocation failure when using
// nothrow new; never dereference a null pointer-to-pointer.
