/**
 * Factory pattern -> create objects without exposing the concrete class
 *
 * Simple factory -> a function with a switch on a key
 * Factory method -> a virtual function that subclasses override
 * Abstract factory -> a family of related factories
 * Registry -> map from a string key to a creation function
 *
 * Benefits: the caller depends on an interface, not on concrete types;
 * adding a new type does not change the calling code
 *
 * Return unique_ptr<Base> so ownership is explicit and there is no leak
 */

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Shape {
   public:
    virtual ~Shape() {}

    virtual string name() const = 0;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius;

   public:
    Circle(double r) {
        radius = r;
    }

    string name() const override {
        return "circle";
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

    string name() const override {
        return "rectangle";
    }

    double area() const override {
        return width * height;
    }
};

class Triangle : public Shape {
    double base;
    double height;

   public:
    Triangle(double b, double h) {
        base = b;
        height = h;
    }

    string name() const override {
        return "triangle";
    }

    double area() const override {
        return 0.5 * base * height;
    }
};

// simple factory
unique_ptr<Shape> createShape(const string &type) {
    if (type == "circle") {
        return make_unique<Circle>(2);
    }
    if (type == "rectangle") {
        return make_unique<Rectangle>(3, 4);
    }
    if (type == "triangle") {
        return make_unique<Triangle>(6, 5);
    }
    return nullptr;
};

// registry: map a key to a factory function
class ShapeFactory {
    unordered_map<string, function<unique_ptr<Shape>()>> creators;

   public:
    void registerShape(const string &type,
                       function<unique_ptr<Shape>()> creator) {
        creators[type] = creator;
    }

    unique_ptr<Shape> create(const string &type) {
        auto it = creators.find(type);

        if (it == creators.end()) {
            return nullptr;
        }

        return it->second();
    }
};

int main() {
    vector<string> types = {"circle", "rectangle", "triangle", "hexagon"};

    for (string &type : types) {
        unique_ptr<Shape> shape = createShape(type);

        if (shape) {
            cout << shape->name() << " " << shape->area() << endl;
        } else {
            cout << type << " is unknown" << endl;
        }
    }

    ShapeFactory factory;
    factory.registerShape("circle", []() { return make_unique<Circle>(1); });
    factory.registerShape("rectangle",
                          []() { return make_unique<Rectangle>(2, 5); });

    unique_ptr<Shape> square = factory.create("rectangle");
    cout << square->area() << endl;  // 10

    cout << (factory.create("oval") == nullptr) << endl;  // 1

    return 0;
}

// Special cases: unknown key -> return nullptr or throw;
// constructor arguments vary between types -> use a config struct or a
// builder; registration order matters if creators depend on each other;
// static initialization order for registries (register in main or use a
// function-local registry); avoid returning raw owning pointers
