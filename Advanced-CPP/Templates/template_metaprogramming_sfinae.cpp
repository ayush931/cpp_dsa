/**
 * Template Metaprogramming, Type Traits, and SFINAE
 *
 * Core Concepts:
 *   1. SFINAE (Substitution Failure Is Not An Error):
 *      When substituting a deduced template argument into a function template
 *      fails to form a valid type or expression, the compiler does NOT emit an
 * error; instead, that candidate is silently dropped from the overload set.
 *
 *   2. std::enable_if and std::enable_if_t:
 *      A type trait that defines a nested type 'type' only when its boolean
 *      condition evaluates to true. Used to conditionally enable or disable
 * overloads.
 *
 *   3. Expression SFINAE and std::void_t (C++17 Detection Idiom):
 *      Allows compile-time introspection to check whether a type supports an
 * operation or possesses a specific method (e.g., checking if `T.serialize()`
 * exists).
 *
 *   4. Tag Dispatching:
 *      Passing compile-time tags (like `std::true_type` or `std::false_type`)
 *      to select appropriate helper overloads.
 *
 *   5. Evolution to C++20:
 *      While SFINAE is ubiquitous in C++11/14/17, C++20 introduces `concepts`
 *      and `requires` clauses to achieve cleaner, more expressive constraints.
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

// ============================================================================
// 1. Basic SFINAE with std::enable_if
// ============================================================================

// Overload enabled ONLY for integral types (int, long, short, etc.)
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void printCategory(T val) {
    cout << "  [Integral Type] value = " << val << " (size: " << sizeof(T)
         << " bytes)" << endl;
}

// Overload enabled ONLY for floating-point types (float, double)
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>,
          typename = void>
void printCategory(T val) {
    cout << "  [Floating Point Type] value = " << val << endl;
}

// ============================================================================
// 2. Detection Idiom with std::void_t (C++17)
// ============================================================================

// Primary template: assumes the method does NOT exist
template <typename T, typename = void>
struct has_serialize : std::false_type {};

// Specialization: valid only if t.serialize() compiles!
template <typename T>
struct has_serialize<T, std::void_t<decltype(std::declval<T>().serialize())>>
    : std::true_type {};

// Helper inline variable template
template <typename T>
inline constexpr bool has_serialize_v = has_serialize<T>::value;

// Classes to test detection idiom
class SerializableUser {
   public:
    string serialize() const {
        return "{\"user_id\": 101, \"name\": \"Ayush\"}";
    }
};

class PlainData {
   public:
    int id = 42;
};

// Function taking advantage of compile-time introspection
template <typename T>
void transmit(const T &obj) {
    if constexpr (has_serialize_v<T>) {
        cout << "  Transmitting serialized payload: " << obj.serialize()
             << endl;
    } else {
        cout << "  Transmitting raw binary buffer (sizeof: " << sizeof(T)
             << " bytes)" << endl;
    }
}

// ============================================================================
// 3. Compile-Time Factorial using Template Recursion (Pure Metaprogramming)
// ============================================================================

template <unsigned int N>
struct Factorial {
    static constexpr unsigned long long value = N * Factorial<N - 1>::value;
};

// Base case specialization
template <>
struct Factorial<0> {
    static constexpr unsigned long long value = 1;
};

// ============================================================================
// 4. Tag Dispatching Example
// ============================================================================

namespace detail {
template <typename T>
void processHelper(const T &val, std::true_type /* is_pointer */) {
    if (val != nullptr) {
        cout << "  [Tag Dispatch] Processing pointer pointing to: " << *val
             << endl;
    } else {
        cout << "  [Tag Dispatch] Processing null pointer" << endl;
    }
}

template <typename T>
void processHelper(const T &val, std::false_type /* is_pointer */) {
    cout << "  [Tag Dispatch] Processing direct value: " << val << endl;
}
}  // namespace detail

template <typename T>
void smartProcess(const T &val) {
    detail::processHelper(val, std::is_pointer<T>{});
}

int main() {
    cout << "=== 1. std::enable_if Overload Resolution ===" << endl;
    printCategory(42);             // Dispatches to Integral overload
    printCategory(3.14159);        // Dispatches to Floating-Point overload
    printCategory(10000000000LL);  // Dispatches to Integral overload

    cout << "\n=== 2. std::void_t Detection Idiom ===" << endl;
    SerializableUser user;
    PlainData data;

    cout << "Checking has_serialize trait:" << endl;
    cout << "  SerializableUser has serialize()? "
         << (has_serialize_v<SerializableUser> ? "YES" : "NO") << endl;
    cout << "  PlainData has serialize()?        "
         << (has_serialize_v<PlainData> ? "YES" : "NO") << endl;

    cout << "\nTransmission dispatch:" << endl;
    transmit(user);
    transmit(data);

    cout << "\n=== 3. Compile-Time Metaprogramming ===" << endl;
    constexpr unsigned long long fact7 = Factorial<7>::value;
    cout << "Compile-time Factorial<7>: " << fact7
         << " (evaluated at compile time!)" << endl;

    cout << "\n=== 4. Tag Dispatching ===" << endl;
    int number = 999;
    smartProcess(number);
    smartProcess(&number);

    return 0;
}

/**
 * Summary:
 *   - SFINAE allows overloading based on arbitrary type characteristics.
 *   - std::void_t provides a clean, robust way to detect expressions and member
 * functions.
 *   - Modern C++ (C++17/20) `if constexpr` and `concepts` significantly
 * simplify code that previously required cumbersome template specializations.
 */
