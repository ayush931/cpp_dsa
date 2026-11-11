// Preprocessor -> runs before compilation
// Directives start with #
// #include -> includes a file
// #define -> defines a macro
// #undef -> removes a macro
// #ifdef, #ifndef, #if, #else, #elif, #endif -> conditional compilation
// #pragma -> implementation defined (for example #pragma once)

#include <iostream>
using namespace std;

#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Include guard -> prevents a header from being included multiple times
#ifndef MY_HEADER
#define MY_HEADER

int headerValue = 10;

#endif

int main() {
    cout << PI << endl;
    cout << SQUARE(5) << endl;
    cout << MAX(3, 7) << endl;

#ifdef PI
    cout << "PI is defined" << endl;
#endif

#ifndef DEBUG
    cout << "Debug mode is off" << endl;
#endif

#if 0
  cout << "This code is not compiled" << endl;
#endif

    // Macro arguments are substituted, not evaluated
    // SQUARE(i++) becomes ((i++) * (i++)), so i is incremented twice
    // Prefer inline functions or constexpr functions over macros

    return 0;
}
