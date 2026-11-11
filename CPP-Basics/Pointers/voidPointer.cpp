// These are special pointers that can point to any datatype value.
// void pointer cannot be dereferenced.

#include <iostream>
using namespace std;

int main() {
    float f = 10.2;
    int x = 10;

    void *ptr = &f;
    ptr = &x;

    int *integerPointer = (int *)ptr;

    cout << *integerPointer << endl;
    cout << ptr << endl;

    return 0;
}