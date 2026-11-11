/**
 * Bit manipulation -> works directly on the binary representation of numbers
 * AND  & -> 1 if both bits are 1
 * OR   | -> 1 if at least one bit is 1
 * XOR  ^ -> 1 if the bits are different
 * NOT  ~ -> flips every bit
 * Left shift  << -> multiply by 2
 * Right shift >> -> divide by 2
 *
 * Bit i is 0-indexed from the right
 * Get bit i    -> (n >> i) & 1
 * Set bit i    -> n | (1 << i)
 * Clear bit i  -> n & ~(1 << i)
 * Toggle bit i -> n ^ (1 << i)
 */

#include <iostream>
using namespace std;

bool isEven(int n) {
    return (n & 1) == 0;
};

bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
};

int countSetBits(int n) {
    int count = 0;

    while (n > 0) {
        n = n & (n - 1);  // removes the lowest set bit
        count++;
    }

    return count;
};

void swapWithoutTemp(int &a, int &b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
};

int main() {
    int n = 10;  // 1010

    cout << isEven(n) << endl;         // 1
    cout << isPowerOfTwo(16) << endl;  // 1
    cout << countSetBits(n) << endl;   // 2

    cout << ((n >> 1) & 1) << endl;   // get bit 1 -> 1
    cout << (n | (1 << 0)) << endl;   // set bit 0 -> 11
    cout << (n & ~(1 << 1)) << endl;  // clear bit 1 -> 8
    cout << (n ^ (1 << 2)) << endl;   // toggle bit 2 -> 14

    int a = 5, b = 7;
    swapWithoutTemp(a, b);
    cout << a << " " << b << endl;  // 7 5

    cout << __builtin_popcount(n) << endl;  // number of set bits
    cout << __builtin_clz(n) << endl;  // leading zeros (n must be non-zero)
    cout << __builtin_ctz(n) << endl;  // trailing zeros (n must be non-zero)

    return 0;
}
