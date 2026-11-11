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

    x = 23;  // the bucket x updated the value from 18 -> 23
    // but ptr is still pointing to same bucket
    // now if we dereference ptr, we will get 23

    cout << "New updated value of x is: " << x << endl;
    cout << "ptr is still pointing to same memory which has 23 instead of 18: "
         << *ptr << endl;

    // updating x with pointer

    *ptr = 50;
    cout << "New value of x is: " << x << "\n";
    cout << "New value pointer by ptr: " << *ptr << "\n";

    int valueAtX = *ptr;
    cout << valueAtX << endl;

    return 0;
}