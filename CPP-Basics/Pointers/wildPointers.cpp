// A wild pointer is declared but never initialized. Its value is
// indeterminate, so reading it or dereferencing it is undefined behavior.
// Always initialize a pointer, either with nullptr or with a real address.

#include <iostream>
using namespace std;

int main() {
    int *wild;  // uninitialized: do not read or dereference this

    int value = 42;
    wild = &value;  // assign a valid address before the first use

    cout << *wild << "\n";  // 42

    int *safe = nullptr;  // preferred default when no object is available yet
    if (safe == nullptr) {
        cout << "safe is null, so do not dereference it\n";
    }

    return 0;
}

// Output:
// 42
// safe is null, so do not dereference it
//
// Printing an uninitialized pointer would also be undefined behavior, even
// though it often looks harmless.
