#include <iostream>
using namespace std;

int main() {
    int x = 9;
    int y = 2;

    int *ptr = &x;

    // ptr = 5; we can't do this as ptr only stores address nothing else.

    // *ptr = &y; this won't work as we cannot store address in an int bucket.

    cout << "Address of x: " << &x << "\n";
    cout << "Address stored inside ptr: " << ptr << endl;
    cout << "Address of ptr itself: " << &ptr << endl;

    return 0;
}