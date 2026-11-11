// Absraction -> enables us to display only essential information while hiding
// implementation details. Inheritance -> a class inherits properties of another
// class

// Access specifiers and Mode of inheritance
// public, private, protected

// Public -> Data and functions can be accessed from anywhere in the code
// Protected -> Accessible in own class, parent class and derived class
// Private -> Accessible only in own class

#include <iostream>
using namespace std;

class Parent {
   public:
    int x;

   protected:
    int y;

   private:
    int z;
};

class Child1 : public Parent {
    // x will remain public
    // y will remain protected
    // z will not be accessible
};

class Child2 : private Parent {
    // x will be private
    // y will be private
    // z will remain inaccessible
};

class Child3 : protected Parent {
    // x will be protected
    // y will be protected
    // z will remain inaccessible
};

int main() {
    Parent p;
    p.x = 10;
    cout << p.x << endl;  // 10

    return 0;
}