/**
 * C++17 vocabulary types
 *
 * std::optional<T> -> a value that may or may not be present
 *   has_value(), value(), value_or(default), operator*, operator->
 *   Throws std::bad_optional_access if value() is called on an empty optional
 *
 * std::variant<A, B, C> -> a type-safe union
 *   std::get<T>, std::get<index>, std::holds_alternative<T>, std::visit
 *   Never empty unless you use std::monostate as the first alternative
 *
 * std::any -> a value of any copyable type
 *   std::any_cast<T>, has_value(), type()
 *   Use variant when the set of types is known; any has a runtime cost
 */

#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>
using namespace std;

optional<int> findIndex(vector<int> &arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return nullopt;
}

variant<int, double, string> parseValue(string input) {
    if (input == "true") {
        return 1;
    }
    if (input == "3.14") {
        return 3.14;
    }
    return input;
}

int main() {
    vector<int> arr = {4, 8, 15, 16, 23, 42};

    optional<int> index = findIndex(arr, 15);

    if (index) {
        cout << "found at " << *index << endl;  // 2
    }

    cout << findIndex(arr, 99).value_or(-1) << endl;  // -1

    try {
        cout << findIndex(arr, 99).value() << endl;
    } catch (bad_optional_access &error) {
        cout << "no value" << endl;
    }

    // variant
    variant<int, double, string> value = parseValue("3.14");

    if (holds_alternative<double>(value)) {
        cout << get<double>(value) << endl;  // 3.14
    }

    value = string("hello");
    cout << get<string>(value) << endl;  // hello

    // std::visit calls the right overload based on the active alternative
    visit([](auto &current) { cout << "visiting: " << current << endl; },
          value);

    // any
    any data = 42;
    cout << any_cast<int>(data) << endl;  // 42

    data = string("text");
    cout << any_cast<string>(data) << endl;  // text
    cout << data.type().name() << endl;

    try {
        cout << any_cast<int>(data) << endl;
    } catch (bad_any_cast &error) {
        cout << "wrong any type" << endl;
    }

    return 0;
}

// Special cases: optional<optional<int>> is allowed but confusing;
// variant valueless_by_exception after a throwing move during assignment;
// std::get<T> throws bad_variant_access on the wrong alternative;
// any_cast<T> on a pointer returns nullptr instead of throwing;
// optional does not allocate, but it always stores sizeof(T) plus padding
