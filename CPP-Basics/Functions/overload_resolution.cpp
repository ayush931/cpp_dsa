// Function overloading and overload resolution
//
// Several functions may share a name if their parameter lists differ. The
// compiler chooses one by ranking the conversions needed for the arguments:
//
//   1. exact match (including trivial qualification conversions)
//   2. promotion (char/short -> int, float -> double)
//   3. standard conversion (int -> double, double -> int, pointer conversions)
//   4. user-defined conversion (constructors, conversion operators)
//
// A better rank on one argument cannot always make up for a worse rank on
// another. If no function is best for every argument, the call is ambiguous
// and does not compile.
//
// Example:
//   void show(int);
//   void show(double);
//   show(5);     // int   -> exact match
//   show(5.0f);  // double -> float promotion beats int conversion
//   show('A');   // int   -> char promotion beats double conversion
//
// Return type is NOT part of the signature. These two cannot coexist:
//   int  read();
//   void read();
//
// Default arguments are not part of the signature either. This pair makes
// `area(5)` ambiguous:
//   int area(int, int = 1);
//   int area(int);
//
// Special cases:
//   - `const` on a by-value parameter is ignored for the signature
//   - `int` and `long` may be the same size, but they are distinct types
//   - a non-const reference overload is preferred for a mutable lvalue
//   - a const reference overload accepts const objects and temporaries
//   - overloads that differ only by pointer vs array are the same parameter
//     type after adjustment
//
// Complexity: overload resolution happens at compile time and costs nothing
// at runtime.

#include <iostream>
#include <string>
using namespace std;

void show(int value) {
    cout << "int: " << value << endl;
}

void show(double value) {
    cout << "double: " << value << endl;
}

void show(const string &value) {
    cout << "string: " << value << endl;
}

void show(const char *value) {
    cout << "c-string: " << value << endl;
}

void describe(int &value) {
    cout << "mutable reference: " << value << endl;
}

void describe(const int &value) {
    cout << "const reference: " << value << endl;
}

int area(int width, int height = 1) {
    return width * height;
}

int main() {
    show(5);                // exact int
    show(5.5);              // exact double
    show(5.5f);             // float promotes to double
    show('A');              // char promotes to int (prints 65)
    show(true);             // bool promotes to int (prints 1)
    show("hello");          // exact const char*
    show(string("world"));  // exact string

    int mutableValue = 10;
    const int constValue = 20;

    describe(mutableValue);      // mutable reference
    describe(constValue);        // const reference
    describe(30);                // temporary binds to const reference
    describe(mutableValue + 1);  // expression is a temporary

    cout << area(5) << endl;     // 5
    cout << area(5, 3) << endl;  // 15

    return 0;
}

// Output:
// int: 5
// double: 5.5
// double: 5.5
// int: 65
// int: 1
// c-string: hello
// string: world
// mutable reference: 10
// const reference: 20
// const reference: 30
// const reference: 11
// 5
// 15
//
// Keep overloads conceptually similar. If two functions with the same name do
// unrelated things, different names are easier to read and debug. Avoid
// overloads that force the reader to memorize subtle conversion rules.
