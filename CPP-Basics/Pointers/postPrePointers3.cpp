#include <iostream>
using namespace std;

int main() {
    int arr[2] = {7, 5};
    int *ptr = &arr[0];

    cout << *++ptr << endl;
    cout << arr[0] << " " << arr[1] << endl;

    return 0;
}