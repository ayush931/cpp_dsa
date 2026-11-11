// Scope and storage duration

// Scope -> where a name is visible
//   block scope, function scope, class scope, namespace scope, global scope
// Storage duration -> how long the object lives
//   automatic -> block, destroyed at the end of the block
//   static    -> whole program
//   thread_local -> per thread
//   dynamic   -> new / delete

// Storage classes / specifiers:-
// auto         -> automatic storage (the default for locals, keyword reused
//                 for type deduction in C++11)
// static       -> static storage duration, internal linkage at global scope
// extern       -> declaration with external linkage
// register     -> removed in C++17, was a hint only
// mutable      -> a member that can change inside a const method
// thread_local -> one instance per thread

#include <iostream>
using namespace std;

int globalValue = 100;       // global scope, whole program
static int fileValue = 200;  // this file only

void showScope() {
    int local = 10;
    cout << "local: " << local << endl;
    cout << "global: " << globalValue << endl;
}

int &sharedCounter() {
    static int counter = 0;  // created once, lives until the program ends
    counter++;
    return counter;
}

int main() {
    int local = 5;
    cout << local << endl;  // 5

    {
        int local = 20;         // shadows the outer local
        cout << local << endl;  // 20

        {
            int local = 30;
            cout << local << endl;  // 30
        }

        cout << local << endl;  // 20
    }

    cout << local << endl;  // 5

    showScope();

    // :: accesses the global even when a local shadows it
    cout << ::globalValue << endl;  // 100

    // static local keeps its value between calls
    sharedCounter();
    sharedCounter();
    cout << sharedCounter() << endl;  // 3

    // loop variable scope
    for (int i = 0; i < 3; i++) {
        int inside = i * 2;
        cout << inside << " ";
    }
    cout << endl;

    // int inside = 100;   // fine, the loop variable is out of scope

    // static in a function: initialized once, thread-safe since C++11
    for (int i = 0; i < 3; i++) {
        static int calls = 0;
        calls++;
        cout << calls << " ";
    }
    cout << endl;

    // global static
    cout << fileValue << endl;

    return 0;
}

// Special cases: a local variable with the same name as a global hides it;
// a static local is shared by all calls (not thread-safe to modify without
// synchronization); the initialization order of globals across translation
// units is unspecified; a variable declared in an if/for condition is scoped
// to that statement; extern declares without defining
