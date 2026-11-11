// Template specialization -> a separate implementation of a template for a
// specific type Explicit specialization -> specialized for one concrete type
// Partial specialization -> specialized for a family of types (class templates
// only)

#include <cstring>
#include <iostream>
using namespace std;

template <typename T>
class Printer {
   public:
    void print(T value) {
        cout << value << endl;
    }
};

// Explicit specialization for char
template <>
class Printer<char> {
   public:
    void print(char value) {
        cout << "char: " << value << endl;
    }
};

template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}

// Specialization for C strings, because > compares addresses
template <>
const char *maxValue<const char *>(const char *a, const char *b) {
    return (strcmp(a, b) > 0) ? a : b;
}

// Partial specialization for pointer types
template <typename T>
class Printer<T *> {
   public:
    void print(T *value) {
        cout << "pointer: " << *value << endl;
    }
};

int main() {
    Printer<int> p1;
    p1.print(10);

    Printer<char> p2;
    p2.print('a');

    int x = 5;
    Printer<int *> p3;
    p3.print(&x);

    cout << maxValue(3, 7) << endl;
    cout << maxValue("apple", "banana") << endl;

    return 0;
}
