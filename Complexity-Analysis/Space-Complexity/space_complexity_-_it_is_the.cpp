/*
 * Space complexity -> It is the extra memory space requirement of an algorithm,
 * using the asymptotic analysis.
 */

// Print the nth fibonacci number.

#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Enter the nth term for fibonacci number ";
    cin >> n;

    if (n < 0) {
        cout << "n must be non-negative" << endl;
        return 1;
    }

    int a = 0;
    int b = 1;
    int c = 0;

    if (n == 0) {
        c = 0;
    } else {
        c = 1;
        for (int i = 2; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
    }

    cout << "Fibonacci number for the nth term is " << c << endl;

    return 0;
}

/*
 *  Space complexity -> O(1)
 *  Time complexity -> O(n)
 */