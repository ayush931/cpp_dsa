/**
 * Initialization: prefer initialized objects and brace initialization when
 * unintended numeric conversions would lose information. C++17.
 * int x; leaves an automatic scalar indeterminate; reading it is unsafe.
 * int x{}; value-initializes it to zero. Containers initialize their elements.
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int zero{};
    double fractional{3.75};
    // int rejected{fractional}; // compile error: narrowing conversion
    int truncated =
        static_cast<int>(fractional);  // intentional loss of fraction
    vector<int> threeCopies(3, 7);     // [7, 7, 7]
    vector<int> twoValues{3, 7};       // [3, 7]: initializer_list overload
    assert(zero == 0 && truncated == 3);
    assert(threeCopies.size() == 3 && twoValues.size() == 2);
    cout << zero << ' ' << truncated << '\n';                       // 0 3
    cout << threeCopies.size() << ' ' << twoValues.size() << '\n';  // 3 2
}
