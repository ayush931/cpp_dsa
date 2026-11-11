/**
 * Virtual dispatch cost and alternatives
 *
 * A virtual call:-
 * 1. Load the vptr from the object
 * 2. Load the function pointer from the vtable
 * 3. Indirect call (harder to predict and inline)
 *
 * The cost is small per call but matters in tight loops and prevents inlining.
 * A virtual destructor is still required for polymorphic deletion.
 *
 * Alternatives:-
 * - final class / final method -> the compiler can devirtualize
 * - CRTP or templates -> static polymorphism, fully inlinable
 * - std::function -> type erasure, also an indirect call
 * - switch on an enum -> predictable branch, inlinable
 *
 * Rule: use virtual functions for extensibility, not for a hot inner loop.
 */

#include <chrono>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
using namespace chrono;

class VirtualShape {
   public:
    virtual ~VirtualShape() {}

    virtual double area() const = 0;
};

class VirtualCircle : public VirtualShape {
    double radius;

   public:
    VirtualCircle(double r) {
        radius = r;
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }
};

class VirtualSquare : public VirtualShape {
    double side;

   public:
    VirtualSquare(double s) {
        side = s;
    }

    double area() const override {
        return side * side;
    }
};

class FinalCircle final : public VirtualShape {
    double radius;

   public:
    FinalCircle(double r) {
        radius = r;
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }
};

struct PlainCircle {
    double radius;

    double area() const {
        return 3.14159 * radius * radius;
    }
};

int main() {
    const int count = 2000000;

    vector<VirtualShape *> shapes;

    for (int i = 0; i < count; i++) {
        if (i % 2 == 0) {
            shapes.push_back(new VirtualCircle(1));
        } else {
            shapes.push_back(new VirtualSquare(1));
        }
    }

    double total = 0;

    auto start = high_resolution_clock::now();

    for (VirtualShape *shape : shapes) {
        total += shape->area();
    }

    auto end = high_resolution_clock::now();

    cout << "virtual: " << duration_cast<microseconds>(end - start).count()
         << " us, total " << total << endl;

    // static polymorphism: the call is inlined
    vector<PlainCircle> plain;

    for (int i = 0; i < count; i++) {
        plain.push_back({1.0});
    }

    total = 0;
    start = high_resolution_clock::now();

    for (PlainCircle &circle : plain) {
        total += circle.area();
    }

    end = high_resolution_clock::now();

    cout << "static: " << duration_cast<microseconds>(end - start).count()
         << " us, total " << total << endl;

    // type erasure with std::function
    vector<function<double()>> callables;

    for (int i = 0; i < count; i++) {
        if (i % 2 == 0) {
            callables.push_back([]() { return 3.14159; });
        } else {
            callables.push_back([]() { return 1.0; });
        }
    }

    total = 0;
    start = high_resolution_clock::now();

    for (auto &callable : callables) {
        total += callable();
    }

    end = high_resolution_clock::now();

    cout << "std::function: "
         << duration_cast<microseconds>(end - start).count() << " us, total "
         << total << endl;

    for (VirtualShape *shape : shapes) {
        delete shape;
    }

    return 0;
}

// Special cases: a class with a virtual function needs a virtual destructor
// if it is deleted through a base pointer; final enables devirtualization;
// a virtual call on a local object can be devirtualized because the dynamic
// type is known; empty base optimization; vtable pointer increases object size;
// multiple inheritance can need pointer adjustment on a cast
