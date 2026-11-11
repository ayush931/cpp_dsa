// Multilevel inheritance -> parent class is derived from another class.

#include <iostream>
using namespace std;

class Parent {
   public:
    Parent() {
        cout << "Parent class" << endl;
    }
};

class Child : public Parent {
   public:
    Child() {
        cout << "Child class" << endl;
    }
};

class GrandChild : public Child {
   public:
    GrandChild() {
        cout << "GrandChild class" << endl;
    }
};

int main() {
    // Single inheritance types
    Child c;        // first come the Parent class then the Child class.
    GrandChild gc;  // so first the Parent class called then Child class and
                    // then GrandChild class.
    return 0;
}
