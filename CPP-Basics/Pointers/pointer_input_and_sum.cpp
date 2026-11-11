#include <iostream>
using namespace std;

int main() {
    int x, y;       // declare x and y, so x, y has garbage value
    cin >> x >> y;  // taking input of x and y

    int *ptrx = &x;  // &x fetches the address of x and then store it in ptrx.
    int *ptry = &y;  // &y fetches the address of y and then stores it in ptry.

    int result;  // declares result so it has  garbage value

    int *ptr_result = &result;

    /*
     * *ptrx -> 10
     * *ptry -> 20
     * *ptrx + *ptry -> 30
     * *ptr_result -> 30; we store 30 on the address stored in *ptr_result
     * result -> 30
     */

    *ptr_result = *ptrx + *ptry;

    cout << result << " " << *ptr_result << endl;

    return 0;
}