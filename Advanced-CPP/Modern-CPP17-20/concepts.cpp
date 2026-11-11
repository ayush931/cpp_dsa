/**
 * Concepts (C++20) -> named requirements on template parameters
 * A concept is a compile-time predicate on types
 *
 * requires expression -> describes valid operations
 *   requires(T a, T b) { a + b; }
 * requires clause -> template <typename T> requires Addable<T>
 * shorthand -> template <Addable T>
 *
 * Benefits:-
 * - Requirements are part of the interface
 * - Errors point at the failed requirement instead of a deep template trace
 * - Overload resolution prefers the more constrained overload
 *
 * Standard concepts: integral, floating_point, same_as, derived_from,
 * convertible_to, invocable, ranges::range, ranges::random_access_range
 */

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> convertible_to<T>;
};

template <typename T>
concept Printable = requires(T value, ostream &out) {
    { out << value } -> convertible_to<ostream &>;
};

template <typename T>
concept Container = requires(T container) {
    typename T::value_type;
    typename T::iterator;
    { container.begin() } -> convertible_to<typename T::iterator>;
    { container.end() } -> convertible_to<typename T::iterator>;
    { container.size() } -> convertible_to<size_t>;
};

template <Addable T>
T add(T a, T b) {
    return a + b;
}

template <Printable T>
void print(const T &value) {
    cout << value << endl;
}

template <Container T>
void printAll(const T &container) {
    for (const auto &value : container) {
        cout << value << " ";
    }
    cout << endl;
}

// requires clause with multiple constraints
template <typename T>
    requires integral<T> && (sizeof(T) >= 4)
T doubled(T value) {
    return value * 2;
}

// constrained overloads: the more specific one wins
template <typename T>
void describe(T value) {
    cout << "generic" << endl;
}

template <integral T>
void describe(T value) {
    cout << "integral" << endl;
}

int main() {
    cout << add(2, 3) << endl;                      // 5
    cout << add(string("a"), string("b")) << endl;  // ab

    print(42);
    print(string("concept"));

    vector<int> numbers = {1, 2, 3};
    printAll(numbers);

    cout << doubled(21) << endl;  // 42

    describe(10);   // integral
    describe(2.5);  // generic

    static_assert(Addable<int>);
    static_assert(Addable<string>);
    static_assert(!Addable<vector<int>>);
    static_assert(Container<vector<int>>);

    // add(1, string("x"));   // error: constraints not satisfied

    return 0;
}

// Special cases: a concept is not a type, it cannot be used as a variable;
// constraints are checked after substitution, so hard errors inside the body
// are still possible; concepts do not replace static_assert for values;
// requires expressions check syntax only, not semantics;
// abbreviated templates: void f(Addable auto x)
