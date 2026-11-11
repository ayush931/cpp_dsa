// Fundamental unit of oops
// user defined datatype

// Object is variable of type class

#include <iostream>
#include <string>
using namespace std;

// class Class_name {
//   int data1;
//   int data2;
// };

// By default the properties of the class is private, have to write in public to
// access it.

class Fruit {
   public:
    string name;
    string color;
};

class Student {
    string name;
    int roll_no;
};

int main() {
    Fruit apple;
    apple.name = "Apple";
    apple.color = "Red";

    cout << apple.name << " " << apple.color << endl;

    // another way to create a class
    Fruit *mango = new Fruit();
    mango->name = "Mango";
    mango->color = "Yellow";

    cout << mango->name << " " << mango->color << endl;

    delete mango;  // free the object created with new

    return 0;
}