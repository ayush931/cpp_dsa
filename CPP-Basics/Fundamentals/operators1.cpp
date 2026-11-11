#include <iostream>
using namespace std;

int main() {
    int num1 = 25;
    int num2 = 5;

    cout << num1 + num2 << endl;  // 30
    cout << num1 - num2 << endl;  // 20
    cout << num1 * num2 << endl;  // 125
    cout << num1 / num2 << endl;  // 5 (quotient)
    cout << num1 % num2 << endl;  // 0 (remainder)

    cout << (num1 == num2) << endl;  // 0 (false)
    cout << (num1 != num2) << endl;  // 1 (true)
    cout << (num1 >= num2) << endl;  // 1 (true)

    num1 += 3;
    cout << num1 << endl;  // 28
    num2 -= 2;
    cout << num2 << endl;  // 3

    return 0;
}