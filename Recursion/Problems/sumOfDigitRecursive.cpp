// Given an integer, find the sum of its digits using recursion.

#include <iostream>
using namespace std;

int sumOfDigits(long long n) {
    // work with the magnitude so negative inputs are handled too
    if (n < 0) {
        return sumOfDigits(-n);
    }
    // base case: a single digit
    if (n <= 9) {
        return (int)n;
    }
    return (int)(n % 10) + sumOfDigits(n / 10);
}

int main() {
    int result = sumOfDigits(1234);
    cout << result << endl;  // 10

    cout << sumOfDigits(-1234) << endl;  // 10
    cout << sumOfDigits(0) << endl;      // 0
    return 0;
}

// Time complexity -> O(log10 n), one call per digit
// Space complexity -> O(log10 n)
