// Reference -> another name (alias) for an existing variable
// A reference must be initialized when declared and cannot be reseated
// It is safer and cleaner than a pointer when NULL is not needed

#include <iostream>
using namespace std;

void swapByReference(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
};

int &getElement(int arr[], int index) {
    return arr[index];
};

int main() {
    int x = 10;
    int &ref = x;  // ref is another name of x

    ref = 20;
    cout << x << endl;                  // 20
    cout << &x << " " << &ref << endl;  // same address

    int y = 5;
    // int &ref2; -> error, reference must be initialized
    ref = y;  // assigns the value of y, does not make ref refer to y
    cout << x << " " << y << endl;  // 5 5

    swapByReference(x, y);
    cout << x << " " << y << endl;  // 5 5 -> values were equal

    // Use distinct values so the swap is visible.
    int a = 3, b = 8;
    swapByReference(a, b);
    cout << a << " " << b << endl;  // 8 3

    int arr[3] = {1, 2, 3};
    getElement(arr, 1) = 10;  // returned reference can be modified
    cout << arr[1] << endl;   // 10

    const int &cref = x;  // reference to const, cannot modify through cref
    cout << cref << endl;

    return 0;
}
