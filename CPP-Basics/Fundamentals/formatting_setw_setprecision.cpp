// Formatted output with <iomanip>
//
// `setw(n)` sets the minimum width of the NEXT output only. It is not sticky,
// so it must be repeated for every field.
//
// `setprecision(n)` is sticky:
//   - in defaultfloat mode it means n significant digits
//   - in fixed or scientific mode it means n digits after the decimal point
//
// Other useful manipulators:
//   setfill(c)   fill character for padding
//   left/right   alignment inside the field
//   fixed        fixed-point notation
//   scientific   scientific notation
//   showpos      print + for positive numbers
//   boolalpha    print true/false instead of 1/0
//   hex/oct/dec  integer base
//   showbase     print 0x for hexadecimal and 0 for octal
//
// Example:
//   cout << setw(6) << 42;              // "    42"
//   cout << fixed << setprecision(2) << 3.14159; // "3.14"
//
// Special cases:
//   - setw is ignored if the value is wider than the field; the value is never
//     truncated
//   - a negative number includes the sign in the field width
//   - setprecision without fixed/scientific counts significant digits, not
//     digits after the point
//   - manipulators stay active until changed, except setw
//   - mixing printf-style and stream formatting in one program is confusing
//
// Complexity: formatting is O(number of characters written).

#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

int main() {
    // A simple table: setw must be repeated for each field.
    cout << left << setw(10) << "Name" << right << setw(8) << "Score" << endl;
    cout << left << setw(10) << "Ada" << right << setw(8) << 95 << endl;
    cout << left << setw(10) << "Grace" << right << setw(8) << 88 << endl;
    cout << left << setw(10) << "Linus" << right << setw(8) << 100 << endl;

    // Fill characters and alignment.
    cout << setfill('0') << setw(5) << 42 << endl;  // 00042
    cout << setfill('.') << left << setw(10) << "total" << right << setw(6)
         << 42 << setfill(' ') << endl;

    // Floating-point precision.
    double pi = 3.141592653589793;

    cout << pi << endl;                                     // 3.14159
    cout << fixed << setprecision(2) << pi << endl;         // 3.14
    cout << scientific << setprecision(3) << pi << endl;    // 3.142e+00
    cout << defaultfloat << setprecision(4) << pi << endl;  // 3.142

    // Integer bases.
    cout << hex << 255 << " " << oct << 255 << " " << dec << 255 << endl;
    cout << showbase << hex << 255 << dec << " " << 255 << noshowbase << endl;

    // Signs and booleans.
    cout << showpos << 42 << " " << -7 << noshowpos << endl;
    cout << boolalpha << true << " " << false << noboolalpha << endl;

    // setbase works like hex/oct/dec for bases 8, 10, and 16.
    cout << setbase(16) << 255 << setbase(10) << " " << 255 << endl;

    // A money-like row: two decimals, fixed width, right aligned.
    double price = 4.5;
    cout << fixed << setprecision(2);
    cout << left << setw(12) << "coffee" << right << setw(8) << price << endl;
    cout << left << setw(12) << "tea" << right << setw(8) << 2.25 << endl;

    return 0;
}

// Output:
// Name         Score
// Ada             95
// Grace           88
// Linus          100
// 00042
// total.....    42
// 3.14159
// 3.14
// 3.142e+00
// 3.142
// ff 377 255
// 0xff 255
// +42 -7
// true false
// ff 255
// coffee          4.50
// tea             2.25
//
// Prefer streams for type-safe formatting. Use printf-style formatting only
// when interoperating with C code or when its compact syntax is clearly
// better for a large report.
