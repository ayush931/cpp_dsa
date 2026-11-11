// Parent class inheritance by multiple child class

#include <iostream>
using namespace std;

class Parent {
   public:
    Parent() {
        cout << "Parent called \n";
    }
};

class Child1 : public Parent {
   public:
    Child1() {
        cout << "Child1 called \n";
    }
};

class Child2 : public Parent {
   public:
    Child2() {
        cout << "Child2 called \n";
    }
};

int main() {
    Child1 c1;  // First Parent called then Child1 called.
    Child2 c2;  // First Parent called then Child2 called.

    return 0;
}

// Hybrid inheritance -> Combination of more than one inheritance type.
