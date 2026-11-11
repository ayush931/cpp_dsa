// Custom exception -> a user defined class derived from std::exception
// Override what() to return an error message
// what() is declared noexcept in the standard library

#include <exception>
#include <iostream>
using namespace std;

class InsufficientBalance : public exception {
   public:
    const char *what() const noexcept override {
        return "Insufficient balance";
    }
};

class InvalidAmount : public exception {
   public:
    const char *what() const noexcept override {
        return "Amount must be positive";
    }
};

class Account {
    double balance;

   public:
    Account(double b) {
        balance = b;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            throw InvalidAmount();
        }
        if (amount > balance) {
            throw InsufficientBalance();
        }
        balance -= amount;
    }

    double getBalance() const {
        return balance;
    }
};

int main() {
    Account acc(1000);

    try {
        acc.withdraw(1500);
    } catch (const InsufficientBalance &e) {
        cout << e.what() << endl;
    }

    try {
        acc.withdraw(-100);
    } catch (const InvalidAmount &e) {
        cout << e.what() << endl;
    }

    acc.withdraw(400);
    cout << acc.getBalance() << endl;  // 600

    return 0;
}
