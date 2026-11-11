// Given a number n. Print if it is an Armstrong number or not.
// An Armstrong number equals the sum of its digits, each raised to the
// number of digits: 153 = 1^3 + 5^3 + 3^3.

#include <iostream>
using namespace std;

long long pow_recursive(long long p, int q) {
    // base case
    if (q == 0) {
        return 1;
    }
    if (q % 2 == 0) {
        // if q is even
        long long result = pow_recursive(p, q / 2);
        return result * result;
    } else {
        // if q is odd
        long long result = pow_recursive(p, (q - 1) / 2);
        return p * result * result;
    }
}

long long f(long long n, int d) {
    // base case
    if (n == 0) {
        return 0;
    }
    return pow_recursive(n % 10, d) + f(n / 10, d);
}

int main() {
    long long n;
    cin >> n;

    if (n <= 0) {
        cout << "NO" << endl;
        return 0;
    }

    int no_of_digits = 0;
    long long temp = n;

    while (temp > 0) {
        temp = temp / 10;
        no_of_digits++;
    }

    long long result = f(n, no_of_digits);

    if (result == n) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}

// Sample input:  153
// Sample output: YES
//
// Sample input:  9474
// Sample output: YES
//
// Sample input:  123
// Sample output: NO
//
// Time complexity -> O(d log d), where d is the number of digits
// Space complexity -> O(d)
