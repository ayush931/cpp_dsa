/**
 * decltype and decltype(auto)
 *
 * `decltype(expression)` asks the compiler for the type of an expression
 * without evaluating it. The rules are:
 *
 *   1. If the expression is an unparenthesized variable or member access,
 *      decltype gives its declared type.
 *   2. If the expression is an lvalue, decltype gives T&.
 *   3. If the expression is an xvalue, decltype gives T&&.
 *   4. Otherwise decltype gives T (a prvalue type).
 *
 * The extra parentheses matter:
 *   decltype(x)   -> int
 *   decltype((x)) -> int&   because (x) is an lvalue expression
 *
 * `decltype(auto)` uses decltype rules to deduce a return type or variable
 * type. Unlike `auto`, it preserves references and top-level const.
 *
 * `std::declval<T>()` produces a T&& in an unevaluated context, which is useful
 * for writing detection traits without requiring a default constructor.
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

int globalValue = 7;

int &referenceToGlobal() {
    return globalValue;
}

template <typename A, typename B>
auto addWithTrailingReturn(A a, B b) -> decltype(a + b) {
    return a + b;
}

template <typename Container>
auto firstElement(Container &container) -> decltype(container[0]) {
    return container[0];
}

template <typename T>
auto memberValue(T value) -> decltype(value.size()) {
    return value.size();
}

template <typename T>
using SizeType = decltype(declval<T>().size());

struct Sample {
    int value = 42;

    int get() const {
        return value;
    }
};

int main() {
    int x = 10;

    decltype(x) copyOfX = 5;
    decltype((x)) referenceToX = x;

    referenceToX = 20;

    cout << "x after referenceToX assignment: " << x << endl;
    cout << "copyOfX: " << copyOfX << endl;

    static_assert(is_same_v<decltype(x), int>);
    static_assert(is_same_v<decltype((x)), int &>);
    static_assert(is_same_v<decltype(copyOfX), int>);

    const int constantValue = 99;
    static_assert(is_same_v<decltype(constantValue), const int>);
    static_assert(is_same_v<decltype((constantValue)), const int &>);

    decltype(auto) referenceResult = referenceToGlobal();
    referenceResult = 21;

    cout << "global after decltype(auto) reference: " << globalValue << endl;

    decltype(auto) valueResult = globalValue;
    valueResult = 100;

    cout << "global after decltype(auto) copy: " << globalValue << endl;

    auto autoCopy = (globalValue);
    autoCopy = 200;

    cout << "global after auto copy: " << globalValue << endl;

    decltype(auto) parenthesizedReference = (globalValue);
    parenthesizedReference = 300;

    cout << "global after decltype(auto) with parentheses: " << globalValue
         << endl;

    static_assert(is_same_v<decltype(referenceResult), int &>);
    static_assert(is_same_v<decltype(valueResult), int>);
    static_assert(is_same_v<decltype(autoCopy), int>);
    static_assert(is_same_v<decltype(parenthesizedReference), int &>);

    cout << "addWithTrailingReturn(2, 3.5): " << addWithTrailingReturn(2, 3.5)
         << endl;

    vector<int> numbers = {10, 20, 30};
    firstElement(numbers) = 99;

    cout << "first element after assignment: " << numbers[0] << endl;

    static_assert(is_same_v<decltype(firstElement(numbers)), int &>);

    string text = "decltype";
    cout << "memberValue(string): " << memberValue(text) << endl;
    static_assert(is_same_v<SizeType<string>, size_t>);

    Sample sample;
    cout << "decltype(&Sample::value) is member pointer: "
         << is_member_object_pointer_v<decltype(&Sample::value)> << endl;
    cout << "decltype(&Sample::get) is member function pointer: "
         << is_member_function_pointer_v<decltype(&Sample::get)> << endl;

    int counter = 0;
    decltype(counter++) unevaluatedType = 5;
    (void)unevaluatedType;

    cout << "counter after decltype(counter++): " << counter << endl;

    return 0;
}

// Expected output:
// x after referenceToX assignment: 20
// copyOfX: 5
// global after decltype(auto) reference: 21
// global after decltype(auto) copy: 21
// global after auto copy: 21
// global after decltype(auto) with parentheses: 300
// addWithTrailingReturn(2, 3.5): 5.5
// first element after assignment: 99
// memberValue(string): 8
// decltype(&Sample::value) is member pointer: 1
// decltype(&Sample::get) is member function pointer: 1
// counter after decltype(counter++): 0
//
// Special cases: unparenthesized variables, parenthesized lvalues, const
// variables, references returned from functions, member pointers, and
// unevaluated expressions with side effects.
