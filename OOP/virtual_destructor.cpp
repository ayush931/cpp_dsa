// Virtual destructors
//
// When a derived object is deleted through a base-class pointer, the base
// destructor must be virtual. Otherwise only the base destructor runs and the
// derived part leaks. The behavior is undefined, not just a leak.
//
// Rule: a base class with virtual functions should have a virtual destructor.
// If a class is not meant to be deleted polymorphically, make the destructor
// protected and non-virtual instead.

#include <iostream>
#include <string>
using namespace std;

class Base {
   public:
    Base() {
        cout << "Base constructed" << endl;
    }

    virtual ~Base() {  // virtual: derived destructor also runs
        cout << "Base destroyed" << endl;
    }

    virtual void describe() {
        cout << "Base" << endl;
    }
};

class Derived : public Base {
    string *payload;

   public:
    Derived() {
        payload = new string("heavy resource");
        cout << "Derived constructed" << endl;
    }

    ~Derived() override {
        delete payload;
        cout << "Derived destroyed" << endl;
    }

    void describe() override {
        cout << "Derived: " << *payload << endl;
    }
};

// A pure virtual destructor is allowed. It still needs a body, because the
// derived destructor calls it.
class AbstractBase {
   public:
    virtual ~AbstractBase() = 0;
};

AbstractBase::~AbstractBase() {
    cout << "AbstractBase destroyed" << endl;
}

class Concrete : public AbstractBase {
   public:
    ~Concrete() {
        cout << "Concrete destroyed" << endl;
    }
};

int main() {
    Base *pointer = new Derived();
    pointer->describe();  // Derived: heavy resource
    delete pointer;       // Derived destroyed, then Base destroyed

    cout << "---" << endl;

    AbstractBase *abstractPointer = new Concrete();
    delete abstractPointer;  // Concrete destroyed, then AbstractBase destroyed

    cout << "---" << endl;

    // Stack objects destroy in reverse order of construction.
    Derived stackObject;

    return 0;
}

// Without `virtual ~Base()`, `delete pointer` would call only ~Base(), the
// `payload` string would never be freed, and the program has undefined
// behavior.
//
// Special cases:
//   - a virtual destructor makes the class polymorphic and adds a vtable
//     pointer to every object
//   - do not call virtual functions from a destructor: the dynamic type is
//     already the current class, so the derived override will not run
//   - `final` on a class means it cannot be derived from, so its destructor
//     does not need to be virtual for polymorphic deletion
//   - shared ownership via `shared_ptr` remembers the original deleter, so it
//     deletes the derived type correctly even without a virtual destructor,
//     but a virtual destructor is still the clearer design
