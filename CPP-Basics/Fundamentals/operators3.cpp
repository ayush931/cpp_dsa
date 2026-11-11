//! Bitwise operators

/*
 * ~ Bitwise complement:- flips every bit, ~0101 = 1010 (shown as 4 bits).
 * << Left shift:- 0101<<1 = 101'0', The number will be multiplied by 2 when we
 * do left shift by 1.
 * >> Right shift:- 0100>>1 = 0010, The number will be divided by 2.
 * | Bitwise OR:- 1 | 1 = 1, 1 | 0 = 1, 0 | 1 = 1, 0 | 0 = 0.
 * & Bitwise AND:- 1010 & 0110 = 0010
 * ^ Bitwise exclusive OR:- It the given bit is same then it will give 0
 * otherwise it will give 1.
 */

//? a << b = a * 2^b
//? a >> b = a / 2^b

#include <iostream>
using namespace std;

int main() {
    int num1 = 5;  // 0101

    cout << (num1 << 1) << endl;  // 10
    cout << (num1 >> 1) << endl;  // 2

    int num2 = 8;  // 1000

    cout << (num1 & num2) << endl;  // 0
    cout << (num1 | num2) << endl;  // 1101 = 13

    return 0;
}
