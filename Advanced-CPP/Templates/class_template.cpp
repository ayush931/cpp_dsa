// Class template -> a class whose data members and member functions can use a
// generic type The type is supplied when an object is created

#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Box {
    T value;

   public:
    Box(T v) {
        value = v;
    }

    void set(T v) {
        value = v;
    }

    T get() {
        return value;
    }
};

template <typename T, typename U>
class Pair {
    T first;
    U second;

   public:
    Pair(T a, U b) {
        first = a;
        second = b;
    }

    void display() {
        cout << first << " " << second << endl;
    }
};

int main() {
    Box<int> b1(10);
    Box<string> b2("hello");
    Box<double> b3(3.14);

    cout << b1.get() << " " << b2.get() << " " << b3.get() << endl;

    b1.set(20);
    cout << b1.get() << endl;

    Pair<int, string> p1(1, "one");
    Pair<string, double> p2("pi", 3.14);
    p1.display();
    p2.display();

    return 0;
}
