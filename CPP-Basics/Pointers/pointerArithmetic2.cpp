// Incrementing a pointer moves it to the next element of an array.
// Do not increment a pointer that points to a single object: the result would
// be past one-past-the-end and using it is undefined behavior.

#include <iostream>
using namespace std;

int main() {
    int arr[3] = {10, 20, 30};
    int *ptr = arr;

    cout << *ptr << "\n";  // 10

    ptr += 1;              // now points to arr[1]
    cout << *ptr << "\n";  // 20

    ptr += 1;              // now points to arr[2]
    cout << *ptr << "\n";  // 30

    return 0;
}

// Output:
// 10
// 20
// 30
