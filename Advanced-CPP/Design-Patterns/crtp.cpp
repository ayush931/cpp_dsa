/**
 * CRTP (Curiously Recurring Template Pattern)
 * class Derived : public Base<Derived>
 *
 * The base knows the derived type at compile time, so it can call derived
 * methods without a virtual function. This is static polymorphism.
 *
 * Uses:-
 * - Mixins and reusable behavior
 * - Compile-time interfaces (no vtable)
 * - Instance counters
 * - Comparison operators generated from one operator
 *
 * Cost: no runtime polymorphism, every derived type gets its own base
 * instantiation (code bloat), and errors mention the template stack.
 */

#include <iostream>
#include <string>
using namespace std;

template <typename Derived>
class Shape {
   public:
    double area() const {
        return static_cast<const Derived *>(this)->areaImpl();
    }

    string name() const {
        return static_cast<const Derived *>(this)->nameImpl();
    }
};

class Circle : public Shape<Circle> {
    double radius;

   public:
    Circle(double r) {
        radius = r;
    }

    double areaImpl() const {
        return 3.14159 * radius * radius;
    }

    string nameImpl() const {
        return "circle";
    }
};

class Square : public Shape<Square> {
    double side;

   public:
    Square(double s) {
        side = s;
    }

    double areaImpl() const {
        return side * side;
    }

    string nameImpl() const {
        return "square";
    }
};

// a generic function that accepts any CRTP shape
template <typename T>
void printShape(Shape<T> &shape) {
    cout << shape.name() << " " << shape.area() << endl;
}

// instance counter mixin
template <typename T>
class Countable {
   public:
    static int count;

    Countable() {
        count++;
    }

    Countable(const Countable &) {
        count++;
    }

    ~Countable() {
        count--;
    }
};

template <typename T>
int Countable<T>::count = 0;

class Widget : public Countable<Widget> {
   public:
    int value = 0;
};

// comparison operators from one definition
template <typename Derived>
class Comparable {
   public:
    bool operator>(const Derived &other) const {
        return other < static_cast<const Derived &>(*this);
    }

    bool operator<=(const Derived &other) const {
        return !(static_cast<const Derived &>(*this) > other);
    }
};

class Version : public Comparable<Version> {
   public:
    int major;
    int minor;

    Version(int a, int b) {
        major = a;
        minor = b;
    }

    bool operator<(const Version &other) const {
        if (major != other.major) {
            return major < other.major;
        }
        return minor < other.minor;
    }
};

int main() {
    Circle circle(2);
    Square square(3);

    printShape(circle);  // circle 12.5664
    printShape(square);  // square 9

    {
        Widget first;
        Widget second;
        Widget third;

        cout << Widget::count << endl;  // 3
    }

    cout << Widget::count << endl;  // 0

    Version first(1, 2);
    Version second(2, 0);

    cout << (first < second) << endl;   // 1
    cout << (second > first) << endl;   // 1
    cout << (first <= second) << endl;  // 1

    return 0;
}

// Special cases: static_cast<Derived*>(this) is invalid if the object is not
// really a Derived; CRTP cannot be used through a base pointer; each
// instantiation duplicates code; a CRTP base destructor is not virtual, so
// never delete a derived object through the base; C++20 concepts make CRTP
// requirements explicit
