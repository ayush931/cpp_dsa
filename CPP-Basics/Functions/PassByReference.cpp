#include <iostream>
using namespace std;

int main() {
    int p = 5;
    int &q = p;  // It will allocate the memory of p to q.

    q++;
    cout << p << endl;

    cout << &p << endl;  // Printing the memory address.
    cout << &q << endl;

    return 0;
}