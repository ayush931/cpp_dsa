// const with pointers and references
//
// `const` can qualify the pointed-to value, the pointer itself, or both:
//
//   const int *p;        // pointer to const int: *p cannot be changed
//   int *const p = &x;   // const pointer: p cannot point elsewhere
//   const int *const p;  // neither *p nor p can change
//
// Read the declaration from right to left:
//   `int *const`  -> const pointer to int
//   `const int *` -> pointer to const int
//
// A reference is an alias and cannot be reseated. `const int &` is a reference
// to const: the referred object cannot be modified through the reference.
// A const reference can bind to a temporary, which is why it is the usual
// parameter type for read-only objects:
//
//   int sum(const vector<int> &values);
//
// Example:
//   int a = 10;
//   const int &r = a;   // r cannot modify a
//   a = 20;             // fine, a itself is not const
//   cout << r;          // 20
//
// Special cases:
//   - `const` on a member function means `this` points to a const object
//   - a const object can only call const member functions
//   - top-level const on a parameter is ignored for overload resolution
//   - `const_cast` can remove const, but modifying a truly const object is
//     undefined behavior
//   - returning a reference to a local object is always wrong, const or not
//
// Complexity: const is a compile-time restriction; it has no runtime cost.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int sum(const vector<int> &values) {
    int total = 0;

    for (int value : values) {
        total += value;
    }

    return total;
}

// The const reference parameter accepts lvalues, const lvalues, and
// temporaries.
int lengthPlusOne(const string &text) {
    return static_cast<int>(text.size()) + 1;
}

class Counter {
    int count;

   public:
    Counter() {
        count = 0;
    }

    void increment() {
        count++;
    }

    // A const member function promises not to modify the object.
    int get() const {
        return count;
    }
};

int main() {
    int first = 10;
    int second = 20;

    // Pointer to const: the value cannot change through the pointer.
    const int *pointerToConst = &first;
    cout << *pointerToConst << endl;  // 10
    pointerToConst = &second;         // the pointer itself can change
    cout << *pointerToConst << endl;  // 20

    // Const pointer: the address cannot change, the value can.
    int *const constPointer = &first;
    *constPointer = 30;
    cout << first << endl;  // 30

    // Const pointer to const: neither part can change.
    const int *const both = &first;
    cout << *both << endl;  // 30

    // Reference to const.
    const int &referenceToConst = first;
    first = 40;
    cout << referenceToConst << endl;  // 40

    // A const reference can bind to a temporary.
    const int &temporaryReference = 5 + 5;
    cout << temporaryReference << endl;  // 10

    vector<int> values = {1, 2, 3, 4};
    cout << sum(values) << endl;  // 10

    // A temporary vector can bind to the const reference parameter.
    cout << sum({10, 20, 30}) << endl;  // 60

    cout << lengthPlusOne("code") << endl;  // 5

    Counter counter;
    counter.increment();
    counter.increment();

    const Counter &readOnlyCounter = counter;
    cout << readOnlyCounter.get() << endl;  // 2
    // readOnlyCounter.increment();           // error: const object

    // The original object is still mutable.
    counter.increment();
    cout << counter.get() << endl;  // 3

    return 0;
}

// Output:
// 10
// 20
// 30
// 30
// 40
// 10
// 10
// 60
// 5
// 2
// 3
//
// Use `const` for values that should not change, `const &` for read-only
// parameters that may be large, and `const` member functions for observers.
// It documents intent and lets the compiler catch accidental modification.
