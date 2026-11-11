// Pointer to pointer -> a pointer that stores the address of another pointer
// Useful when a function has to change the pointer of the caller
// Also used for dynamically allocated 2-D arrays

#include <iostream>
using namespace std;

void allocate(int **ptr) {
    *ptr = new int(100);
};

int main() {
    int x = 5;
    int *p = &x;
    int **pp = &p;

    cout << x << endl;     // 5
    cout << *p << endl;    // 5
    cout << **pp << endl;  // 5

    **pp = 10;
    cout << x << endl;  // 10

    int *q = nullptr;
    allocate(&q);
    cout << *q << endl;  // 100
    delete q;

    int *arr = new int[3]{1, 2, 3};
    int **arrPtr = &arr;
    cout << (*arrPtr)[1] << endl;  // 2
    delete[] arr;

    return 0;
}
