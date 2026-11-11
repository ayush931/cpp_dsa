// const -> the value cannot be modified after initialization
// constexpr -> the value is computed at compile time
// const member function -> cannot modify the object
// const parameters -> cannot be modified inside the function

#include <iostream>
using namespace std;

constexpr int square(int x) {
    return x * x;
};

class Circle {
    const double pi = 3.14159;
    double radius;

   public:
    Circle(double r) : radius(r) {}

    double area() const {  // const member function
        return pi * radius * radius;
    }

    void setRadius(double r) {
        radius = r;
    }
};

int main() {
    const int a = 10;
    // a = 20; -> error

    const int *p = &a;  // pointer to const, value cannot change through p
    int b = 20;
    int *const q = &b;        // const pointer, q cannot point elsewhere
    const int *const r = &a;  // both are const

    *q = 30;
    cout << b << endl;  // 30

    int arr[square(5)];                            // compile-time constant size
    cout << sizeof(arr) / sizeof(arr[0]) << endl;  // 25

    static_assert(square(3) == 9, "compile-time check");

    Circle c(5);
    cout << c.area() << endl;
    c.setRadius(10);
    cout << c.area() << endl;

    return 0;
}

// constexpr functions can be evaluated at compile time when all arguments are
// constant const variables can be initialized at runtime
