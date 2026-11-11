// Union -> all members share the same memory location
// Only one member can hold a valid value at a time
// Size of a union = size of its largest member

#include <iostream>
using namespace std;

union Value {
    int integer;
    float decimal;
    char character;
};

// Enum -> user defined data type made of named integer constants
enum Day { MON, TUE, WED, THU, FRI, SAT, SUN };

// Enum class -> scoped enum, does not convert implicitly to int
enum class Color { RED, GREEN, BLUE };

// typedef -> gives a new name to an existing type
typedef unsigned long long ull;

// using -> modern alternative of typedef
using ll = long long;

int main() {
    Value v;
    v.integer = 65;
    cout << v.integer << " " << (char)v.integer << endl;

    v.decimal = 3.14;
    cout << v.decimal << endl;  // integer is no longer valid

    Day today = WED;
    cout << today << endl;  // 2
    cout << (today == WED) << endl;

    Color c = Color::GREEN;
    if (c == Color::GREEN) {
        cout << "Green" << endl;
    }
    // cout << c; -> error, enum class needs an explicit cast

    ull a = 100;
    ll b = -50;
    cout << a << " " << b << endl;

    return 0;
}
