/**
 * Templated lambdas (C++20)
 *
 * A generic lambda uses `auto` parameters:
 *
 *   auto add = [](auto a, auto b) { return a + b; };
 *
 * A templated lambda names the template parameters explicitly:
 *
 *   auto add = []<typename T>(T a, T b) { return a + b; };
 *
 * The explicit form is useful when you need:
 *   - the same type for several parameters (T a, T b)
 *   - access to T inside the body (T::value_type, sizeof(T))
 *   - constraints with concepts
 *   - perfect forwarding with T&& and std::forward<T>
 *
 * The explicit template parameter list comes right after the capture list.
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

int main() {
    // Generic lambda: each parameter is deduced independently.
    auto addGeneric = [](auto a, auto b) { return a + b; };

    cout << addGeneric(2, 3) << endl;                      // 5
    cout << addGeneric(string("a"), string("b")) << endl;  // ab

    // Templated lambda: both parameters must have the same type T.
    auto addSameType = []<typename T>(T a, T b) { return a + b; };

    cout << addSameType(2, 3) << endl;                      // 5
    cout << addSameType(string("a"), string("b")) << endl;  // ab

    // The explicit T is available inside the body.
    auto describe = []<typename T>(const T &value) {
        cout << "size=" << sizeof(T) << " value=" << value << endl;
    };

    describe(42);
    describe(string("hello"));

    // A constrained templated lambda.
    auto multiplyNumbers = []<typename T>(T a, T b)
        requires is_arithmetic_v<T>
    { return a * b; };

    cout << multiplyNumbers(4, 5) << endl;  // 20

    // Perfect forwarding inside a templated lambda.
    auto forwardTo = []<typename T>(T &&value) {
        return std::forward<T>(value);
    };

    string text = "forwarded";
    cout << forwardTo(text) << endl;                 // forwarded
    cout << forwardTo(string("temporary")) << endl;  // temporary

    // A templated lambda as a visitor over a vector.
    vector<int> numbers = {1, 2, 3, 4};
    auto sum = []<typename T>(const vector<T> &values) {
        T total = 0;
        for (const T &value : values) {
            total += value;
        }
        return total;
    };

    cout << sum(numbers) << endl;  // 10

    return 0;
}

/**
 * Time complexity -> the same as the equivalent function template
 * Space complexity -> the closure object is empty or holds the captures
 *
 * Special cases:
 *   - a generic lambda with `auto` parameters can deduce different types for
 *     each parameter; the explicit template form forces one type
 *   - a templated lambda still has a unique closure type per lambda expression
 *   - explicit template arguments can be supplied at the call site:
 *     addSameType.operator()<int>(2, 3)
 *   - constraints make the error message point at the failed requirement
 *   - captureless lambdas convert to a function pointer only if the call
 *     operator is not templated; a templated lambda cannot convert
 *
 * Before C++20, the same effect needed a helper function template or a
 * struct with a templated operator(). The explicit lambda form keeps the
 * logic local without a separate declaration.
 */
