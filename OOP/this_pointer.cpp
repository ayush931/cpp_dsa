// The `this` pointer
//
// Inside a non-static member function, `this` is a pointer to the object that
// called the function. Its type is `ClassName*`, or `const ClassName*` inside
// a const member function.
//
// Common uses:
//   1. disambiguate a parameter from a data member with the same name
//   2. return the current object for method chaining (`return *this;`)
//   3. compare objects (`if (this == &other) return;`)
//   4. pass the object to another function (`someFunction(*this);`)

#include <iostream>
#include <string>
using namespace std;

class Account {
    string owner;
    double balance;

   public:
    Account(string owner, double balance) {
        // `owner = owner` would assign the parameter to itself. `this->owner`
        // clearly means the data member.
        this->owner = owner;
        this->balance = balance;
    }

    Account &deposit(double amount) {
        balance += amount;
        return *this;  // enable chaining
    }

    Account &withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            cout << "insufficient funds" << endl;
        }

        return *this;
    }

    // const member function: `this` is `const Account*`, so members cannot be
    // modified. It can be called on a const object.
    void print() const {
        cout << owner << ": " << balance << endl;
    }

    double getBalance() const {
        return balance;
    }

    // Copy assignment must handle self-assignment: a = a;
    Account &operator=(const Account &other) {
        if (this == &other) {
            return *this;  // self-assignment check
        }

        owner = other.owner;
        balance = other.balance;

        return *this;
    }
};

class Chain {
   public:
    int value;

    Chain(int start) {
        value = start;
    }

    Chain &add(int amount) {
        value += amount;
        return *this;
    }

    Chain &multiply(int amount) {
        value *= amount;
        return *this;
    }
};

int main() {
    Account account("Ayush", 100.0);

    // Method chaining: every call returns a reference to the same object.
    account.deposit(50).withdraw(30).deposit(10).print();  // Ayush: 130

    const Account fixed("Fixed", 500.0);
    fixed.print();  // const objects can call const methods
    // fixed.deposit(1);       // error: deposit is not const

    Account copy("Copy", 0.0);
    copy = account;  // copy assignment
    copy.print();    // Ayush: 130

    copy = copy;   // self-assignment is safe
    copy.print();  // Ayush: 130

    Chain chain(2);
    chain.add(3).multiply(4).add(1);
    cout << chain.value << endl;  // 21

    return 0;
}

// Special cases:
//   - `this` is never null in a well-formed member function call
//   - `delete this;` is legal only for objects created with `new`, and the
//     object must not be touched afterward
//   - a static member function has no `this`
//   - in a constructor, `this` points to the object under construction; do not
//     call virtual functions there, because the dynamic type is not ready
//   - returning `*this` by reference keeps the same object; returning by value
//     would return a copy
