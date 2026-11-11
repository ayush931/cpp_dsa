/**
 * Type traits -> compile-time information about types
 * Defined in <type_traits>
 *
 * is_integral<T>, is_floating_point<T>, is_pointer<T>, is_same<A, B>
 * remove_reference<T>, remove_const<T>, decay<T>
 * enable_if<condition, T> -> SFINAE (Substitution Failure Is Not An Error)
 * conditional<condition, A, B>
 *
 * if constexpr (C++17) -> compile-time branch, the discarded branch is not
 * instantiated, which replaces many SFINAE tricks
 *
 * Concepts (C++20) express requirements directly and give better errors
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

template <typename T>
typename enable_if<is_integral<T>::value, T>::type doubleValue(T value) {
    return value * 2;
}

template <typename T>
typename enable_if<is_floating_point<T>::value, T>::type doubleValue(T value) {
    return value * 2.0;
}

template <typename T>
string describe(T value) {
    if constexpr (is_integral<T>::value) {
        return "integral";
    } else if constexpr (is_floating_point<T>::value) {
        return "floating point";
    } else if constexpr (is_pointer<T>::value) {
        return "pointer";
    } else {
        return "other";
    }
}

// C++17 if constexpr replaces many SFINAE overloads.
// C++20 concepts express the same requirement with shorter errors:
//   template <typename T>
//   concept Addable = requires(T a, T b) { { a + b } -> convertible_to<T>; };

template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    cout << doubleValue(21) << endl;   // 42
    cout << doubleValue(1.5) << endl;  // 3

    cout << describe(10) << endl;   // integral
    cout << describe(2.5) << endl;  // floating point
    int *pointer = nullptr;
    cout << describe(pointer) << endl;       // pointer
    cout << describe(string("hi")) << endl;  // other

    cout << add(3, 4) << endl;                      // 7
    cout << add(string("a"), string("b")) << endl;  // ab

    // trait values are available at compile time
    static_assert(is_integral<int>::value, "int must be integral");
    static_assert(!is_integral<double>::value, "double is not integral");
    static_assert(is_same<remove_reference<int &>::type, int>::value,
                  "remove_reference");

    cout << is_signed<int>::value << endl;             // 1
    cout << is_unsigned<unsigned int>::value << endl;  // 1
    cout << is_arithmetic<string>::value << endl;      // 0

    // conditional selects a type at compile time
    using Number = conditional<sizeof(void *) == 8, long long, int>::type;
    cout << sizeof(Number) << endl;  // 8 on 64-bit

    // decay converts arrays to pointers and removes references / cv qualifiers
    cout << is_same<decay<int[10]>::type, int *>::value << endl;  // 1

    return 0;
}

// Special cases: enable_if must depend on a template parameter, otherwise it
// is a hard error; two overloads that differ only in the return type are
// ambiguous; is_integral<bool> is true; use decay before storing a type;
// concepts produce shorter errors than SFINAE
