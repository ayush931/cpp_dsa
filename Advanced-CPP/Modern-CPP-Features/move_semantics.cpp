/**
 * Move semantics -> transfers resources from a temporary object instead of
 * copying them std::move casts an lvalue to an rvalue reference Move
 * constructor -> ClassName(ClassName &&other) Move assignment -> ClassName&
 * operator=(ClassName &&other) The moved-from object remains valid but its
 * value is unspecified
 */

#include <algorithm>
#include <iostream>
using namespace std;

class Buffer {
    int *data;
    int size;

   public:
    Buffer(int n) : size(n), data(new int[n]) {}

    ~Buffer() {
        delete[] data;
    }

    Buffer(const Buffer &other) : size(other.size), data(new int[other.size]) {
        copy(other.data, other.data + size, data);
        cout << "Copy constructor" << endl;
    }

    Buffer(Buffer &&other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "Move constructor" << endl;
    }

    Buffer &operator=(Buffer &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

int main() {
    Buffer b1(10);
    Buffer b2 = b1;        // copy constructor
    Buffer b3 = move(b1);  // move constructor

    Buffer b4(5);
    b4 = move(b3);  // move assignment

    return 0;
}

// Copy -> O(n), move -> O(1) for the resource transfer
// Mark move operations noexcept so containers can use them safely
