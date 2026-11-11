// Static members
//
// A `static` data member belongs to the class, not to any object. There is
// exactly one copy shared by every instance. It must be defined outside the
// class (or declared `inline static` in C++17).
//
// A `static` member function has no `this` pointer, so it can only access
// static members. Call it as ClassName::function().
//
// A `static` local variable inside a function is created once and keeps its
// value between calls. It is initialized on the first call (thread-safe since
// C++11).

#include <iostream>
#include <string>
using namespace std;

class Employee {
    string name;
    int id;

    static int nextId;  // shared counter
    inline static int alive =
        0;  // C++17 inline static: no out-of-class definition

   public:
    Employee(string employeeName) {
        name = employeeName;
        id = nextId++;
        alive++;
    }

    ~Employee() {
        alive--;
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    // Static member function: no `this`, only static members are accessible.
    static int totalCreated() {
        return nextId - 1;  // nextId is the id that will be assigned next
    }

    static int currentlyAlive() {
        return alive;
    }
};

int Employee::nextId = 1;  // definition of the static data member

int nextCallNumber() {
    static int calls = 0;  // created once, survives between calls
    calls++;
    return calls;
}

class Counter {
   public:
    static int count;  // declaration only

    static void increment() {
        count++;
    }
};

int Counter::count = 0;

int main() {
    cout << nextCallNumber() << endl;  // 1
    cout << nextCallNumber() << endl;  // 2
    cout << nextCallNumber() << endl;  // 3

    Employee first("Ayush");
    Employee second("Riya");

    cout << first.getId() << " " << first.getName() << endl;    // 1 Ayush
    cout << second.getId() << " " << second.getName() << endl;  // 2 Riya

    cout << Employee::totalCreated() << endl;    // 2
    cout << Employee::currentlyAlive() << endl;  // 2

    {
        Employee temporary("Temp");
        cout << Employee::currentlyAlive() << endl;  // 3
    }

    cout << Employee::currentlyAlive() << endl;  // 2

    Counter::increment();
    Counter::increment();

    Counter object;
    object.increment();  // works, but Counter::increment() is clearer

    cout << Counter::count << endl;  // 3

    return 0;
}

// Special cases:
//   - a static member must be defined exactly once (the linker catches
//     duplicates); `inline static` avoids the separate definition
//   - static members exist even if no object is ever created
//   - a static function cannot be `const`, because there is no object
//   - a static local variable is not destroyed until the program ends
//   - static initialization order across translation units is unspecified, so
//     do not depend on another file's static object during construction
