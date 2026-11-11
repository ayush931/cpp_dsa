// bitset -> fixed size sequence of bits
// Size must be known at compile time
// Supports fast bitwise operations and bit counting

#include <bitset>
#include <iostream>
#include <string>
using namespace std;

int main() {
    bitset<8> b(10);  // 00001010
    cout << b << endl;

    cout << b[1] << endl;       // 1
    cout << b.count() << endl;  // number of set bits -> 2
    cout << b.size() << endl;   // 8
    cout << b.any() << " " << b.none() << endl;

    b.set(0);    // set bit 0
    b.reset(3);  // clear bit 3
    b.flip(2);   // toggle bit 2
    cout << b << endl;

    cout << b.to_ulong() << endl;
    cout << b.to_string() << endl;

    bitset<4> x(string("1010"));
    bitset<4> y(6);  // 0110
    cout << (x & y) << endl;
    cout << (x | y) << endl;
    cout << (x ^ y) << endl;
    cout << (~x) << endl;

    x <<= 1;
    cout << x << endl;

    return 0;
}

// Each bitwise operation on a bitset of size n -> O(n / word size)
