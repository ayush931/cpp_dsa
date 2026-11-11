#include <iostream>
using namespace std;

int main() {
    int *ptr = nullptr;   // declared, but not pointing to any object yet
    cout << ptr << "\n";  // prints 0, which represents the null pointer

    int marks = 90;
    ptr = &marks;  // now the pointer holds a valid address

    cout << ptr << "\n";  // re-assignment / update

    return 0;
}