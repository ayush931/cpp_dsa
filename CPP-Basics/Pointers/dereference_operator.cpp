// Dereference operator:- we can use *ptr to actually access value stored on the
// address pointed by ptr.

#include <iostream>
using namespace std;

int main() {
    int x = 18;
    float y = 5.6;

    // create a pointer that can store address of x
    // As x is an integer variable, so we need a pointer
    // that can store address of integer type

    int *ptr = &x;
    cout << "Address stored inside ptr: " << ptr << endl;
    cout << "Value present at the address stored in ptr: " << *ptr
         << "\n";  // dereference operator

    // create a pointer that can store address of y
    // y is a float variable, so we need to store it in a
    // float pointer

    float *ptrn = &y;
    cout << "Address stored inside ptrf: " << ptrn << "\n";
    cout << "Value present at the address stored in ptrf: " << *ptrn
         << "\n";  // dereference operator

    return 0;
}