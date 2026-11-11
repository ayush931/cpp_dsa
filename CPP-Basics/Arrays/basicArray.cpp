// Data structure which stores a collection of homogeneous items like int, char,
// bool, etc. They have contiguous (consecutive block of memory) memory. Index
// -> location of every element.

// Array types:- Single dimensional and multidimensional array.

// Arrays are fixed in size, but the value at an index can be changed after
// initialization. You cannot delete an element or change the size of a built-in
// array.

#include <iostream>
using namespace std;

int main() {
    int array[] = {1, 2, 3, 4, 5};

    cout << sizeof(array) << endl;
    cout << sizeof(array) / sizeof(array[0]) << endl;

    int array2[4] = {};  // Zero-initialized, so every element is 0.

    cout << array2[0] << endl;
    cout << array2[1] << endl;
    cout << array2[2] << endl;
    cout << array2[3] << endl;

    return 0;
}