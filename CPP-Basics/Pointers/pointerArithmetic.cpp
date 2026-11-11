// Pointer arithmetic moves in units of the pointed-to type, not in bytes.
// ptr + 1 advances by sizeof(*ptr) bytes. Forming a pointer past one-past-the-
// end of an array is undefined behavior, so keep every result inside an array
// (or one past its last element).

#include <iostream>
using namespace std;

int main() {
    int values[3] = {20, 21, 22};
    double decimals[3] = {9.8, 1.2, 3.4};

    int *ptr = values;
    double *ptrd = decimals;

    cout << "Size of int: " << sizeof(int) << "\n";
    cout << "Size of double: " << sizeof(double) << "\n";

    // Difference between neighboring addresses = size of one element.
    cout << "int step: " << (ptr + 1) - ptr << " element, "
         << reinterpret_cast<char *>(ptr + 1) - reinterpret_cast<char *>(ptr)
         << " bytes\n";

    cout << "double step: " << (ptrd + 2) - ptrd << " elements, "
         << reinterpret_cast<char *>(ptrd + 2) - reinterpret_cast<char *>(ptrd)
         << " bytes\n";

    return 0;
}

// Output:
// Size of int: 4
// Size of double: 8
// int step: 1 element, 4 bytes
// double step: 2 elements, 16 bytes
//
// Incrementing a pointer shifts it by one object, so the byte step depends on
// the type: sizeof(int) for int*, sizeof(double) for double*.
