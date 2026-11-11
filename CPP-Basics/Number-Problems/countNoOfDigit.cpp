#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;

    int digits = 0;

    // do-while makes sure that 0 still counts as one digit.
    do {
        digits++;
        n = n / 10;
    } while (n > 0);
    cout << "Number of digits in the given number is: " << digits << endl;

    return 0;
}