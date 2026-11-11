// Convert binary to number

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the binary number: ";
    cin >> n;

    int ans = 0;
    int power = 1;

    while (n > 0) {
        int lastDigit = n % 10;
        ans = ans + (lastDigit * power);
        power *= 2;
        n /= 10;
    }

    cout << "Converted the binary into decimal: " << ans << endl;

    return 0;
}