/**
 * Decorator pattern -> add behavior to an object by wrapping it
 *
 * The wrapper has the same interface as the wrapped object, so clients do
 * not notice the difference. Decorators can be stacked in any order.
 *
 * Prefer composition over inheritance: instead of a class for every
 * combination (CoffeeWithMilkAndSugar, CoffeeWithMocha...), compose wrappers.
 *
 * Each wrapper delegates to the next one and adds its own behavior.
 */

#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Beverage {
   public:
    virtual ~Beverage() {}

    virtual string description() const = 0;
    virtual double cost() const = 0;
};

class Coffee : public Beverage {
   public:
    string description() const override {
        return "coffee";
    }

    double cost() const override {
        return 2.00;
    }
};

class Tea : public Beverage {
   public:
    string description() const override {
        return "tea";
    }

    double cost() const override {
        return 1.50;
    }
};

class AddOn : public Beverage {
   protected:
    unique_ptr<Beverage> beverage;

   public:
    AddOn(unique_ptr<Beverage> b) {
        beverage = move(b);
    }
};

class Milk : public AddOn {
   public:
    Milk(unique_ptr<Beverage> b) : AddOn(move(b)) {}

    string description() const override {
        return beverage->description() + " + milk";
    }

    double cost() const override {
        return beverage->cost() + 0.50;
    }
};

class Sugar : public AddOn {
   public:
    Sugar(unique_ptr<Beverage> b) : AddOn(move(b)) {}

    string description() const override {
        return beverage->description() + " + sugar";
    }

    double cost() const override {
        return beverage->cost() + 0.25;
    }
};

class Mocha : public AddOn {
   public:
    Mocha(unique_ptr<Beverage> b) : AddOn(move(b)) {}

    string description() const override {
        return beverage->description() + " + mocha";
    }

    double cost() const override {
        return beverage->cost() + 0.75;
    }
};

int main() {
    unique_ptr<Beverage> plain = make_unique<Coffee>();
    cout << plain->description() << " " << plain->cost() << endl;  // coffee 2

    unique_ptr<Beverage> fancy = make_unique<Mocha>(
        make_unique<Milk>(make_unique<Sugar>(make_unique<Coffee>())));

    cout << fancy->description() << " " << fancy->cost() << endl;
    // coffee + sugar + milk + mocha 3.5

    // decorators are transparent: the client only sees Beverage
    unique_ptr<Beverage> order = make_unique<Sugar>(make_unique<Tea>());
    cout << order->description() << " " << order->cost()
         << endl;  // tea + sugar 1.75

    return 0;
}

// Special cases: a decorator must forward every method, or behavior is lost;
// decorators add one object and one virtual call per layer; ownership must be
// clear (unique_ptr here); decorators are not the same as inheritance:
// the wrapped type is not a subclass of the concrete component;
// stream manipulators and Java I/O are real-world decorators
