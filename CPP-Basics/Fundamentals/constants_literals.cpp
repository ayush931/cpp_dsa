// Constants and literals

// const        -> value cannot be changed after initialization (runtime)
// constexpr    -> value or function can be evaluated at compile time (C++11)
// consteval    -> must be evaluated at compile time (C++20)
// #define      -> preprocessor text replacement, no type, no scope
// literal      -> a fixed value written directly in the code

#include <iostream>
using namespace std;

const double PI = 3.14159;
constexpr int MAX_USERS = 100;

constexpr int square(int value) {
    return value * value;
}

#define BUFFER_SIZE 1024

int main() {
    // integer literals
    int decimal = 42;
    int octal = 052;         // leading 0 -> octal, equals 42
    int hexadecimal = 0x2A;  // 0x -> hexadecimal, equals 42
    int binary = 0b101010;   // 0b -> binary, equals 42

    cout << decimal << " " << octal << " " << hexadecimal << " " << binary
         << endl;

    // digit separators (C++14) improve readability
    long long population = 8'000'000'000LL;
    cout << population << endl;

    // suffixes
    // u / U -> unsigned, l / L -> long, ll / LL -> long long
    // f / F -> float, no suffix -> double, l / L -> long double
    auto small = 10;            // int
    auto big = 10LL;            // long long
    auto decimalNumber = 3.14;  // double
    auto single = 3.14f;        // float

    cout << sizeof(small) << " " << sizeof(big) << " " << sizeof(decimalNumber)
         << " " << sizeof(single) << endl;

    // character and string literals
    char newline = '\n';
    char tab = '\t';
    char quote = '\'';
    const char *text = "hello";
    string cppString = "hello"s;  // std::string literal (C++14)

    cout << text << cppString << quote << endl;

    // raw string literal: no escape sequences needed
    string path = R"(C:\Users\name\folder)";
    cout << path << endl;

    // const prevents accidental modification
    const int limit = 10;
    // limit = 20;   // error

    // constexpr is evaluated at compile time
    constexpr int area = square(5);
    int array[area];  // valid, area is a compile-time constant

    cout << area << " " << sizeof(array) / sizeof(array[0]) << endl;

    // const vs constexpr
    int runtimeValue = 8;
    const int runtimeConstant = runtimeValue;  // const, not compile time
    // constexpr int compileTime = runtimeValue;   // error

    cout << runtimeConstant << endl;

    // a const pointer vs a pointer to const
    int first = 1;
    int second = 2;

    const int *pointerToConst = &first;  // value cannot change through pointer
    int *const constPointer = &first;    // pointer cannot change
    const int *const both = &first;      // neither can change

    pointerToConst = &second;  // fine, pointer itself can change
    *constPointer = 10;        // fine, value can change
    // constPointer = &second;            // error

    cout << *pointerToConst << " " << *constPointer << " " << *both << endl;

    // macros have no type and no scope, avoid them for constants
    cout << BUFFER_SIZE << " " << MAX_USERS << " " << PI << endl;

    return 0;
}

// Special cases: constexpr functions can also run at runtime;
// a const local can still be changed through a pointer after const_cast
// (undefined behavior if the object is truly const);
// integer literal 0 is null pointer constant, do not use NULL in modern C++;
// floating point literals are double by default, add f to avoid a warning;
// #define ignores scope and can break code unexpectedly
