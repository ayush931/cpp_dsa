// A struct groups related values of different types under one name.
// Example: a student has a name (string), a roll number (int) and marks
// (float).

#include <iostream>
#include <string>
using namespace std;

// ! Defining a struct: a new type, not a variable
struct Student {
    string name;
    int rollNo;
    float marks;
};

// ! Functions can take a struct like any other type
void printStudent(const Student &s) {
    cout << s.name << " (roll " << s.rollNo << ") scored " << s.marks << endl;
}

int main() {
    // ! Create a struct and set its members with the dot operator
    Student s1;
    s1.name = "Ayush";
    s1.rollNo = 1;
    s1.marks = 92.5f;

    // ! Brace initialization sets members in declaration order
    Student s2 = {"Riya", 2, 88.0f};

    printStudent(s1);
    printStudent(s2);

    // ! An array of structs stores many records of the same type
    Student batch[2] = {s1, s2};
    for (int i = 0; i < 2; i++) {
        cout << "batch[" << i << "]: " << batch[i].name << endl;
    }

    // ! Pointer to a struct uses the arrow operator
    Student *ptr = &batch[0];
    ptr->marks += 2.5f;
    cout << "after bonus: " << batch[0].marks << endl;

    return 0;
}

// ! Summary:
// ! struct Name { members };   declares the type once
// ! Name var;                  creates a variable
// ! var.member  or  ptr->member accesses a member
// ! Sorting records (sort with a comparator) builds on this.
