// Const and pointers
//
// The position of const decides what is protected:
//   const int *p          -> pointer to const int: *p cannot change, p can.
//   int * const p         -> const pointer to int: p cannot change, *p can.
//   const int * const p   -> neither p nor *p can change.
//
// Read the declaration from right to left, for example:
//   int * const p  -> p is a const pointer to int.
//
// Example:
//   int x = 10;
//   const int *p = &x;  // *p = 20 is an error
//   p = &y;             // allowed
//
//   int * const q = &x; // q = &y is an error
//   *q = 20;            // allowed

#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    // Pointer to const: the value is read-only through this pointer.
    const int *ptrToConst = &x;
    std::cout << *ptrToConst << "\n";  // 10
    ptrToConst = &y;                   // the pointer itself can be reassigned
    std::cout << *ptrToConst << "\n";  // 20
    // *ptrToConst = 30;              // error: value is const through the
    // pointer

    // Const pointer: the address is fixed, the value is mutable.
    int *const constPtr = &x;
    *constPtr = 30;
    std::cout << x << "\n";  // 30
    // constPtr = &y;        // error: the pointer itself is const

    // Const pointer to const: neither the address nor the value can change.
    const int *const constPtrToConst = &y;
    std::cout << *constPtrToConst << "\n";  // 20

    // A const pointer can still point to a non-const object, which is useful
    // when a function must not change where it points but may update the value.
    return 0;
}

// Output:
// 10
// 20
// 30
// 20
//
// Complexity: O(1) time and O(1) space.
//
// Edge cases: a const pointer must be initialized when declared; a pointer to
// const may point to a non-const object, but writing through it is still not
// allowed. Prefer const references for read-only function parameters.
