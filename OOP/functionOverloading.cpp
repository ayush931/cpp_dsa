// Polymorphism => ability of objects / methods to take different forms

// Compile time Polymorphism and Runtime Polymorphism

// Function overloading -> Define a number of Functions with same Function name,
// they perform differently according to the arguments passed.

#include <iostream>
using namespace std;

class Sum {
   public:
    void add(int x, int y) {
        int sum = x + y;
        cout << sum << endl;
    }

    void add(int x, int y, int z) {
        int sum = x + y + z;
        cout << sum << endl;
    }

    void add(float x, float y) {
        float sum = x + y;
        cout << sum << endl;
    }
};

int main() {
    Sum s;

    s.add(2, 3);
    s.add(3, 5, 6);
    s.add(float(2.6), float(5.5));

    return 0;
}
