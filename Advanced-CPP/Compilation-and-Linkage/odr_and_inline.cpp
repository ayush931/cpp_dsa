/**
 * One Definition Rule (ODR)
 * - A function, variable, class or template can have only one definition
 *   in the whole program
 * - A class can be defined once per translation unit, and all definitions
 *   must be identical
 * - An inline function can be defined in multiple translation units, but all
 *   definitions must be identical
 * - A template can be instantiated in multiple translation units, the linker
 *   keeps one copy
 *
 * inline (C++17) also applies to variables:
 *   inline int counter = 0;   // one entity shared by all translation units
 *
 * constexpr implies inline for functions and static data members
 *
 * Violating the ODR is usually not diagnosed; it causes undefined behavior
 * and strange linker errors (duplicate symbol, missing symbol)
 */

#include <iostream>
#include <string>
using namespace std;

// inline function: header-safe, one entity in the program
inline int square(int value) {
    return value * value;
}

// inline variable: header-safe global, one entity in the program
inline int sharedCounter = 0;

// constexpr function is implicitly inline
constexpr int cube(int value) {
    return value * value * value;
}

class Widget {
    int value;

   public:
    // a member function defined inside the class is implicitly inline
    Widget(int v) : value(v) {}

    int get() const {
        return value;
    }

    // a static data member defined inline (C++17)
    inline static int instanceCount = 0;
};

// a template definition must be visible in every translation unit that uses it
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    cout << square(5) << endl;  // 25
    cout << cube(3) << endl;    // 27

    sharedCounter++;
    cout << sharedCounter << endl;  // 1

    Widget first(10);
    Widget second(20);

    Widget::instanceCount = 2;
    cout << Widget::instanceCount << endl;  // 2

    cout << maximum(3, 7) << endl;                      // 7
    cout << maximum(string("a"), string("b")) << endl;  // b

    // inline namespace: members are visible in the enclosing namespace
    // and can be versioned without breaking existing code
    cout << square(1 + 1) << endl;

    return 0;
}

// Special cases: a non-inline function defined in a header included by two
// .cpp files -> duplicate symbol; a static variable in a header -> one copy
// per translation unit (probably not what you want); inline functions must
// have identical definitions, including macros; templates usually live in
// headers; inline is a hint about linkage, not a guarantee of inlining
