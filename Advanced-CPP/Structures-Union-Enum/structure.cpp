// Structure -> user defined data type used to group variables of different data
// types Members of a structure are public by default Structure is used to
// represent a record

#include <iostream>
#include <string>
using namespace std;

struct Student {
    string name;
    int rollNo;
    float marks;
};

struct Address {
    string city;
    int pincode;
};

struct Employee {
    string name;
    Address address;  // nested structure
};

void printStudent(Student s) {
    cout << s.name << " " << s.rollNo << " " << s.marks << endl;
};

int main() {
    Student s1 = {"Ayush", 1, 92.5};

    Student s2;
    s2.name = "Riya";
    s2.rollNo = 2;
    s2.marks = 88.0;

    printStudent(s1);
    printStudent(s2);

    Student students[2] = {s1, s2};
    cout << students[1].name << endl;

    Student *ptr = &s1;
    cout << ptr->name << " " << ptr->rollNo << endl;

    Employee e1 = {"Naman", {"Delhi", 110001}};
    cout << e1.name << " " << e1.address.city << " " << e1.address.pincode
         << endl;

    cout << sizeof(Student)
         << endl;  // may be larger than the sum of members due to padding

    return 0;
}
