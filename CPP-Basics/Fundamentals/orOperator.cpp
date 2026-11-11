#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "Enter the number: ";
    cin >> num;

    if ((num % 3 == 0) || (num % 5 == 0)) {
        cout << "The number is either divisible by 3 or 5" << endl;
    } else {
        cout << "Number is neither divisible by 3 nor by 5" << endl;
    }

    return 0;
}