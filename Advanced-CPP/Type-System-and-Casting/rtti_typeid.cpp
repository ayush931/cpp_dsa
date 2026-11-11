/**
 * RTTI (Run-Time Type Information)
 * typeid(expression) -> std::type_info
 *   .name() -> implementation-defined mangled name
 *   .hash_code() -> hash of the type
 *   comparison with == compares types
 * typeid on a polymorphic object uses the dynamic type
 * typeid on a non-polymorphic object uses the static type
 *
 * dynamic_cast and typeid both need RTTI; some builds disable it with
 * -fno-rtti (common in game engines and embedded builds)
 */

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
using namespace std;

class Shape {
   public:
    virtual ~Shape() {}

    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius;

   public:
    Circle(double r) {
        radius = r;
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
    double width;
    double height;

   public:
    Rectangle(double w, double h) {
        width = w;
        height = h;
    }

    double area() const override {
        return width * height;
    }
};

int main() {
    Circle circle(2);
    Rectangle rectangle(3, 4);

    Shape *shape = &circle;

    cout << typeid(*shape).name() << endl;                  // Circle (mangled)
    cout << (typeid(*shape) == typeid(Circle)) << endl;     // 1
    cout << (typeid(*shape) == typeid(Rectangle)) << endl;  // 0

    // typeid on a pointer does NOT look at the dynamic type
    Shape *pointer = &circle;
    cout << (typeid(pointer) == typeid(Shape *)) << endl;  // 1

    vector<Shape *> shapes = {&circle, &rectangle};

    for (Shape *current : shapes) {
        cout << typeid(*current).name() << " area " << current->area() << endl;
    }

    // typeid on a null pointer dereference throws std::bad_typeid
    Shape *nullShape = nullptr;

    try {
        cout << typeid(*nullShape).name() << endl;
    } catch (bad_typeid &error) {
        cout << "bad_typeid" << endl;
    }

    // type_info cannot be copied, but it can be compared and hashed
    const type_info &info = typeid(int);
    cout << info.hash_code() << endl;
    cout << (typeid(int) == typeid(long)) << endl;  // 0

    return 0;
}

// Special cases: typeid of a polymorphic null pointer -> bad_typeid;
// typeid during construction returns the base type;
// type_info::name() is not portable, use a demangler for readable output;
// RTTI can be disabled, then dynamic_cast and typeid do not work
