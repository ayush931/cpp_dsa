#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;

    int sum = 0;

    while (n > 0) {
        int lastDigit = n % 10;
        sum += lastDigit;
        n /= 10;
    }
    cout << "Sum of digits of the given number is: " << sum << endl;

    return 0;
}