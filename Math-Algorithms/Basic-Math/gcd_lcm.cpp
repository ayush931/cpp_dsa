/**
 * GCD -> largest number that divides both numbers
 * Euclidean algorithm -> gcd(a, b) = gcd(b, a % b)
 * Time complexity -> O(log min(a, b))
 * LCM -> (a * b) / gcd(a, b)
 *
 * Extended Euclidean algorithm also finds x and y such that
 * a * x + b * y = gcd(a, b)
 */

#include <iostream>
using namespace std;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
};

int gcdRecursive(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcdRecursive(b, a % b);
};

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;  // divide first to avoid overflow
};

int extendedGcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int g = extendedGcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
};

int main() {
    cout << gcd(36, 60) << endl;           // 12
    cout << gcdRecursive(36, 60) << endl;  // 12
    cout << lcm(12, 18) << endl;           // 36

    int x, y;
    int g = extendedGcd(30, 18, x, y);
    cout << g << " " << x << " " << y << endl;  // 6 -1 2

    return 0;
}

// gcd(a, 0) = a
// gcd(a, b) = gcd(b, a % b)
