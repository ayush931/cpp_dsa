/**
 * Type casting in C++
 *
 * static_cast
 * - Compile-time conversion between related types
 * - int <-> double, void* -> T*, base* -> derived* (no runtime check)
 * - Downcasting to the wrong type is undefined behavior
 *
 * dynamic_cast
 * - Runtime-checked cast, requires a polymorphic base (at least one virtual
 * function)
 * - Pointer form returns nullptr on failure
 * - Reference form throws std::bad_cast on failure
 * - Has a runtime cost (RTTI lookup)
 *
 * const_cast
 * - Adds or removes const / volatile
 * - Modifying a truly const object through const_cast is undefined behavior
 *
 * reinterpret_cast
 * - Reinterprets the bit pattern, no conversion
 * - Pointer <-> integer, unrelated pointer types
 * - Only safe in narrow, well-defined cases (e.g. uintptr_t round trip)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
using namespace std;

class Base {
   public:
    virtual ~Base() {}

    virtual string name() {
        return "Base";
    }
};

class Derived : public Base {
   public:
    string name() override {
        return "Derived";
    }

    void derivedOnly() {
        cout << "derived only" << endl;
    }
};

class Other : public Base {
   public:
    string name() override {
        return "Other";
    }
};

int main() {
    // static_cast: numeric conversion
    double pi = 3.14159;
    int truncated = static_cast<int>(pi);
    cout << truncated << endl;  // 3

    // static_cast: upcast is always safe
    Derived derived;
    Base *base = static_cast<Base *>(&derived);
    cout << base->name() << endl;  // Derived

    // static_cast: downcast with no check
    Derived *back = static_cast<Derived *>(base);
    back->derivedOnly();

    // dynamic_cast: checked downcast
    Base *actuallyOther = new Other();
    Derived *wrong = dynamic_cast<Derived *>(actuallyOther);

    if (wrong == nullptr) {
        cout << "dynamic_cast failed safely" << endl;
    }

    try {
        Derived &reference = dynamic_cast<Derived &>(*actuallyOther);
        cout << reference.name() << endl;
    } catch (bad_cast &error) {
        cout << "bad_cast: " << error.what() << endl;
    }

    // const_cast
    const int constantValue = 10;
    int &mutableReference = const_cast<int &>(constantValue);
    // writing through mutableReference would be undefined behavior

    cout << mutableReference << endl;  // 10

    // reinterpret_cast
    int number = 0x41424344;
    char *bytes = reinterpret_cast<char *>(&number);
    cout << bytes[0] << bytes[1] << bytes[2] << bytes[3]
         << endl;  // DCBA on little endian

    delete actuallyOther;

    return 0;
}

// Special cases: dynamic_cast on a non-polymorphic type does not compile;
// dynamic_cast during construction/destruction sees the current type only;
// casting away const from a const object and writing is UB;
// reinterpret_cast between function pointer types is not portable;
// prefer virtual functions or std::variant over downcasting
