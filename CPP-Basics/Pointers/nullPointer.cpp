// Null pointer:- If we want to have a pointer variable, which is just declared
// but will get address later to store.

#include <iostream>
using namespace std;

int main() {
    int *ptr = nullptr;
    int *ptr1 = 0;
    int *ptr2 = nullptr;

    // print the addresses, not the pointed-to values
    cout << static_cast<void *>(ptr) << "\n";
    cout << static_cast<void *>(ptr1) << endl;
    cout << static_cast<void *>(ptr2) << endl;

    // always check before dereferencing
    if (ptr == nullptr) {
        cout << "ptr is null, cannot dereference" << endl;
    }

    return 0;
}

// Dereferencing a null pointer is undefined behavior and usually crashes
// 0 is mostly a special reserved memory address in many operating systems
// Prefer nullptr over NULL and 0 in modern C++
