// Type casting -> converting a value from one type to another

// 1. Implicit conversion (automatic)
//    int -> double, char -> int, derived* -> base*
//    Small types are promoted, larger types may lose data

// 2. Explicit conversion
//    C style       -> (int)value
//    Functional    -> int(value)
//    C++ casts     -> static_cast, dynamic_cast, const_cast, reinterpret_cast

// Prefer the C++ casts because they state the intent and are easier to search

#include <iostream>
using namespace std;

class Base {
   public:
    virtual ~Base() {}
};

class Derived : public Base {
   public:
    void hello() {
        cout << "derived" << endl;
    }
};

int main() {
    // implicit: int to double, no data loss
    int count = 7;
    double average = count / 2.0;
    cout << average << endl;  // 3.5

    // implicit narrowing: double to int drops the fraction
    double price = 9.99;
    int rupees = price;
    cout << rupees << endl;  // 9

    // C style cast
    char letter = (char)65;
    cout << letter << endl;  // A

    // static_cast: checked at compile time, preferred for numeric conversions
    int total = 100;
    double ratio = static_cast<double>(total) / 3;
    cout << ratio << endl;  // 33.3333

    // without the cast, integer division happens first
    cout << total / 3 << endl;  // 33

    // static_cast between related class pointers (no runtime check)
    Derived derived;
    Base *base = static_cast<Base *>(&derived);

    Derived *back = static_cast<Derived *>(base);
    back->hello();

    // dynamic_cast: runtime check, returns nullptr on failure
    Base *actuallyBase = new Base();
    Derived *wrong = dynamic_cast<Derived *>(actuallyBase);

    if (wrong == nullptr) {
        cout << "cast failed" << endl;
    }

    delete actuallyBase;

    // char <-> int
    char digit = '7';
    int value = digit - '0';
    cout << value << endl;  // 7

    // bool conversion: any non-zero value is true
    int zero = 0;
    int nonZero = -5;
    cout << bool(zero) << " " << bool(nonZero) << endl;  // 0 1

    // unsigned and signed mixing can surprise you
    unsigned int big = 1;
    int minusOne = -1;

    if (minusOne > big) {
        cout << "signed converted to unsigned" << endl;  // this runs
    }

    return 0;
}

// Special cases: casting a negative int to unsigned wraps around;
// static_cast downcast to the wrong type is undefined behavior;
// const_cast cannot safely modify a truly const object;
// reinterpret_cast does not convert values, it reinterprets bits;
// use std::stoi / std::to_string for string <-> number, not casts
