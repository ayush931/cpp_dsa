#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;

    int reverse = 0;

    while (n > 0) {
        int lastDigit = n % 10;
        reverse = reverse * 10 + lastDigit;
        n /= 10;
    }
    cout << "Reverse of the given number is: " << reverse << endl;

    return 0;
}