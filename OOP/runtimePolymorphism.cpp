// Resolved at runtime
// Using function overriding

// Child class defines a function of parent class

#include <iostream>
using namespace std;

class Parent {
   public:
    virtual void print() {  // virtual keyword uses for function overriding,
                            // decides on runtime
        cout << "Parent class" << endl;
    }

    void show() {
        cout << "Parent class" << endl;
    }
};

class Child : public Parent {
   public:
    void print() {
        cout << "Child class" << endl;
    }

    void show() {
        cout << "Child class" << endl;
    }
};

int main() {
    Parent *p;
    Child c;

    p = &c;
    p->print();
    p->show();

    return 0;
}

// Compile time ->
// 1. Through function overloading and operator overloading
// 2. function name should be same but params can be different
// 3. Faster execution time
// 4. More memory efficient

// Runtime ->
// 1. Through function overriding
// 2. function name and params should be same
// 3. Slower time execution
// 4. Less memory efficient
