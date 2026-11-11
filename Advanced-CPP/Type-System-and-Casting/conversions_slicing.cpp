/**
 * Implicit and explicit conversions
 *
 * Implicit conversion -> the compiler converts automatically
 *   int -> double, derived* -> base*, T -> T&& (binding a temporary)
 *   A converting constructor enables implicit conversion
 *
 * explicit -> disables implicit conversion
 *   explicit Foo(int) -> Foo f = 10; is an error, Foo f(10); is fine
 *   explicit operator bool() -> allows `if (obj)` but not `int x = obj;`
 *
 * Conversion operator -> defines how a class converts to another type
 *
 * Slicing -> assigning a derived object to a base object copies only the base
 * part, losing the derived data and virtual dispatch
 */

#include <iostream>
#include <string>
using namespace std;

class Meters {
    double value;

   public:
    Meters(double v) : value(v) {}  // implicit conversion from double

    double get() const {
        return value;
    }
};

class SafeMeters {
    double value;

   public:
    explicit SafeMeters(double v) : value(v) {}  // no implicit conversion

    double get() const {
        return value;
    }
};

class Flag {
    bool active;

   public:
    Flag(bool value) : active(value) {}

    explicit operator bool() const {
        return active;
    }
};

class Base {
   public:
    virtual ~Base() {}

    virtual string name() const {
        return "Base";
    }
};

class Derived : public Base {
    int extra = 7;

   public:
    string name() const override {
        return "Derived";
    }

    int getExtra() const {
        return extra;
    }
};

void printBase(Base base) {  // takes by value -> slicing
    cout << base.name() << endl;
}

void printBaseReference(Base &base) {  // no slicing
    cout << base.name() << endl;
}

int main() {
    Meters meters = 5;  // implicit, allowed
    cout << meters.get() << endl;

    // SafeMeters safe = 5;   // error: constructor is explicit
    SafeMeters safe(5);
    cout << safe.get() << endl;

    Flag flag(true);

    if (flag) {
        cout << "active" << endl;  // explicit operator bool allows this
    }

    // int number = flag;   // error: no implicit bool -> int

    Derived derived;
    Base base = derived;          // slicing
    printBase(derived);           // slicing happens at the call
    printBaseReference(derived);  // Derived

    cout << base.name() << endl;  // Base, virtual dispatch is lost

    // avoid slicing with references or pointers
    Base &reference = derived;
    cout << reference.name() << endl;  // Derived

    return 0;
}

// Special cases: vector<Base> holding Derived objects slices them (store
// pointers or reference_wrapper instead); a constructor with one argument is
// implicit unless marked explicit; explicit operator bool is the safe pattern
// for smart pointers; narrowing conversions in braces are errors
