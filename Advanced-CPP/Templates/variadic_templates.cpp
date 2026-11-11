/**
 * Variadic Templates
 *
 * A variadic template accepts any number of template arguments.
 *   - `sizeof...(args)` gives the number of arguments
 *   - a fold expression (C++17) reduces a parameter pack in one line
 *   - recursive variadic templates handle one argument at a time
 *   - perfect forwarding preserves value categories with `T&&` + `forward<T>`
 *
 * These tools are used by std::tuple, std::make_unique, emplace_back, and
 * logging libraries.
 */

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
using namespace std;

// C++17 fold expression: sum any number of values.
template <typename... Values>
auto sumAll(Values... values) {
    return (values + ...);  // unary right fold
}

// Recursive variadic template: print one value, then recurse on the rest.
void printAll() {
    cout << endl;
}

template <typename First, typename... Rest>
void printAll(First first, Rest... rest) {
    cout << first << " ";
    printAll(rest...);
}

// Count the arguments at compile time.
template <typename... Values>
int argumentCount(Values... values) {
    return sizeof...(values);
}

// Perfect forwarding: pass arguments to another function unchanged.
template <typename... Values>
void forwardAll(Values &&...values) {
    printAll(forward<Values>(values)...);
}

// Build a tuple and read it back.
template <typename... Values>
auto makePairLike(Values... values) {
    return tuple<Values...>(values...);
}

int main() {
    cout << sumAll(1, 2, 3, 4) << endl;                  // 10
    cout << sumAll(2.5, 3.5) << endl;                    // 6
    cout << sumAll(string("ab"), string("cd")) << endl;  // abcd

    printAll(1, "two", 3.0, '4');  // 1 two 3 4

    cout << argumentCount(1, 2, 3) << endl;  // 3
    cout << argumentCount() << endl;         // 0

    forwardAll(10, "twenty", 30.5);  // 10 twenty 30.5

    auto values = makePairLike(1, string("hello"), 2.5);
    cout << get<0>(values) << " " << get<1>(values) << " " << get<2>(values)
         << endl;  // 1 hello 2.5

    return 0;
}

/**
 * Time complexity -> O(number of arguments)
 * Space complexity -> O(number of arguments) for the parameter pack
 *
 * Special cases: empty parameter pack (sumAll() is ill-formed for the fold
 * expression, so provide an overload or an identity value), one argument,
 * mixed types (the return type is deduced from the fold), references and
 * move-only types (use perfect forwarding), recursion depth equal to the
 * number of arguments.
 */
