// Base class has multiple parent class having a common ancestor class

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

class GrandChild : public Child1, public Child2 {
   public:
    GrandChild() {
        cout << "GrandChild called \n";
    }
};

int main() {
    GrandChild gc;  // First the Child1 called with Parent class then Child2
                    // called with Parent class and finally GrandChild
    return 0;
}
