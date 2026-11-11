#include <iostream>
using namespace std;

int main() {
    int a, b;
    cout << "Enter the number: ";
    cin >> a;
    cout << "to the power: ";
    cin >> b;

    int result = 1;

    for (int i = 1; i <= b; i++) {
        result *= a;
    }
    cout << "Given number to the given power is: " << result << endl;

    return 0;
}