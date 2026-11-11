//! Do While Loop

/*
 * do {
 *    // code
 * } while (condition);
 */

//? Print the sum of the stream of N integers in the input using do-while loop.

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Total number is: ";
    cin >> n;

    int sum = 0;

    do {
        int num;
        cout << "Enter the number: ";
        cin >> num;
        sum += num;
        n--;
    } while (n > 0);
    cout << "Sum of the number is: " << sum << endl;

    return 0;
}