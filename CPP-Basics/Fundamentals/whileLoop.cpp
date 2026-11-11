//! While loop

/*
 * while (condition) {
 *   code
 * }
 */

//? Sum of n natural number

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;

    int sum = 0;

    int i = 1;  // loop variable

    while (i <= n) {  // condition
        sum += i;
        i++;  // updating loop variable
    }
    cout << "The sum of n natural number is: " << sum << endl;

    return 0;
}