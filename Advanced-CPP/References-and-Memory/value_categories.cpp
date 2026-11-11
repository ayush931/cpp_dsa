/**
 * C++17: lvalues identify objects; prvalues compute values; xvalues identify
 * objects whose resources may be reused. lvalue/rvalue is about expressions,
 * not simply whether an object has a name or lives on the stack.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
using namespace std;

const char *category(const string &) {
    return "lvalue or const rvalue";
}

const char *category(string &&) {
    return "mutable rvalue";
}

int main() {
    string name = "hello";
    string &&reference = std::move(name);
    // A named rvalue-reference variable is an lvalue expression.
    static_assert(is_same_v<decltype((reference)), string &>);
    static_assert(is_same_v<decltype(std::move(name)), string &&>);
    cout << category(reference) << '\n';
    cout << category(std::move(reference)) << '\n';
    // std::move is a cast, not a move operation. No resource moved above.
    assert(name == "hello");
    string destination = std::move(name);  // move constructor may reuse data
    assert(destination == "hello");
    // Moved-from standard containers remain valid; don't assume they are empty.
    name = "reused";
    const string fixed = "constant";
    cout << category(std::move(fixed)) << '\n';  // const cannot bind to T&&
    const string &extended = string("temporary");
    assert(extended == "temporary");  // lifetime extended to reference's scope
    // Returning a reference to a local, or forwarding a temporary reference
    // through a function, does not extend that local/temporary's lifetime.
}
