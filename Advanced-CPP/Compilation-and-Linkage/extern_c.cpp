/**
 * extern "C" -> disable C++ name mangling for a declaration
 *
 * C++ compilers mangle names to support overloading and namespaces
 *   int add(int, int) might become _Z3addii
 * C compilers do not mangle, so a C library exports add
 * extern "C" tells the C++ compiler to use the C linkage and symbol name
 *
 * __cplusplus is defined by a C++ compiler and can guard C++-only code
 * in a header shared by C and C++
 *
 * Name mangling is implementation-defined; never write mangled names by hand
 */

#include <cstddef>
#include <iostream>
#include <string>
using namespace std;

// A C-style function with C linkage
extern "C" int c_add(int a, int b) {
    return a + b;
}

// A function pointer with C linkage
extern "C" typedef int (*CBinaryOperation)(int, int);

// Overloading is not allowed with C linkage
// extern "C" int c_add(double a, double b);   // error

namespace {
int hiddenHelper(int value) {
    return value * 2;
}
}  // namespace

// A C++ function with normal C++ linkage can be overloaded
int process(int value) {
    return hiddenHelper(value);
}

double process(double value) {
    return value * 2.0;
}

int main() {
    cout << c_add(2, 3) << endl;  // 5

    CBinaryOperation operation = c_add;
    cout << operation(10, 20) << endl;  // 30

    cout << process(5) << endl;    // 10
    cout << process(2.5) << endl;  // 5

#ifdef __cplusplus
    cout << "compiled as C++, version " << __cplusplus << endl;
#else
    cout << "compiled as C" << endl;
#endif

    return 0;
}

// Special cases: extern "C" cannot be applied to a member function;
// templates cannot have C linkage; a C header included from C++ usually
// wraps its declarations in #ifdef __cplusplus extern "C" { ... } #endif;
// mixing C and C++ allocation across the boundary is undefined
