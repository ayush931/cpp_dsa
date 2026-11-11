// Data types decide how much memory a value takes, which values it can hold,
// and which operations are allowed on it.

#include <cstdint>
#include <iostream>
#include <limits>
using namespace std;

int main() {
    // ! Built-in (fundamental) types
    int whole = 42;         // integer, usually 4 bytes
    float decimal = 3.14f;  // single precision, about 6-7 significant digits
    double precise =
        3.14159265;     // double precision, about 15-16 significant digits
    char letter = 'A';  // a single character, stored as a small integer
    bool flag = true;   // true or false

    cout << "int    : " << whole << endl;
    cout << "float  : " << decimal << endl;
    cout << "double : " << precise << endl;
    cout << "char   : " << letter << endl;
    cout << "bool   : " << boolalpha << flag << endl;

    // ! sizeof tells how many bytes a type takes on this machine
    cout << "\nSizes in bytes (may differ by platform):" << endl;
    cout << "char   : " << sizeof(char) << endl;
    cout << "bool   : " << sizeof(bool) << endl;
    cout << "int    : " << sizeof(int) << endl;
    cout << "long long : " << sizeof(long long) << endl;
    cout << "float  : " << sizeof(float) << endl;
    cout << "double : " << sizeof(double) << endl;

    // ! Ranges come from <limits>, so you never hard-code them
    cout << "\nRanges:" << endl;
    cout << "int min/max : " << numeric_limits<int>::min() << " "
         << numeric_limits<int>::max() << endl;
    cout << "char min/max : " << +numeric_limits<char>::min() << " "
         << +numeric_limits<char>::max() << endl;

    // ! Fixed-width types from <cstdint> have the same size everywhere
    int32_t fixed32 = 2000000000;
    int64_t fixed64 = 9000000000000000000LL;
    uint8_t byte = 255;
    cout << "\nFixed width: " << fixed32 << " " << fixed64 << " " << +byte
         << endl;

    // ! A char is a number too: 'A' is 65 in ASCII
    cout << "\n'A' as integer : " << static_cast<int>('A') << endl;
    cout << "'A' + 1        : " << static_cast<char>('A' + 1) << endl;

    // ! Integer division truncates; floating-point division keeps the fraction
    cout << "\n7 / 2     = " << 7 / 2 << endl;
    cout << "7 / 2.0   = " << 7 / 2.0 << endl;

    // ! auto lets the compiler deduce the type from the initializer
    auto count = 10;   // int
    auto ratio = 0.5;  // double
    cout << "\nauto count : " << count << ", auto ratio : " << ratio << endl;

    // ! Overflow wraps for unsigned types but is undefined for signed types
    unsigned int small = 0;
    small = small - 1;
    cout << "\nunsigned 0 - 1 = " << small << endl;

    return 0;
}

// ! Summary:
// ! int, char, bool, float, double are the core types for everyday programs.
// ! Use sizeof and numeric_limits instead of memorizing platform sizes.
// ! Use <cstdint> types when an exact width matters, such as binary data.
