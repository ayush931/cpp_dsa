// Sum of the series, s = 1 - 2 + 3 - 4 + 5 ..... n

#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Sum of alternate series upto the digit: ";
    cin >> n;

    int result = 0;

    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            result -= i;
        } else {
            result += i;
        }
    }
    cout << "Result of sum of alternate series is: " << result << endl;

    return 0;
}