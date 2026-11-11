// Convert number to binary.

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number: ";
    cin >> n;

    int ans = 0;
    int power = 1;

    while (n > 0) {
        int parityDigit = n % 2;
        ans = ans + parityDigit * power;
        power *= 10;
        n /= 2;
    }

    cout << "Number converted into binary: " << ans << endl;

    return 0;
}