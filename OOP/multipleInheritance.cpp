// Multiple inheritance

#include <iostream>
using namespace std;

class Parent1 {
   public:
    Parent1() {
        cout << "Parent1 called \n";
    }
};

class Parent2 {
   public:
    Parent2() {
        cout << "Parent2 called \n";
    }
};

class Child : public Parent1,
              public Parent2 {  // Multiple inheritance of class.
   public:
    Child() {
        cout << "Child called \n";
    }
};

class GrandChild : public Child {
   public:
    GrandChild() {
        cout << "GrandChild class \n";
    }
};

int main() {
    Child c1;  // First Parent1 will called then Parent2 will called and last
               // Child class will called.
    GrandChild
        gc;  // First Parent1 will called then Parent2 will called and then
             // Child class will called and last GrandChild class will called.

    return 0;
}
