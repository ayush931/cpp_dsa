#include <iostream>
using namespace std;

int main() {
    int num;
    cin >> num;

    if ((num % 2 == 0) && (num % 3 == 0)) {
        cout << "The number is divisible by both 2 and 3 is: " << num << endl;
    } else {
        cout << "Number is not divisible by both 2 and 3" << endl;
    }

    return 0;
}