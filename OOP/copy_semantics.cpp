// Copy semantics: shallow copy vs deep copy
//
// The compiler-generated copy constructor and copy assignment copy every
// member. For plain values that is fine. For a raw owning pointer it copies
// only the address, so two objects point to the same memory:
//   - modifying one object changes the other
//   - both destructors call delete, causing a double free
//
// A deep copy allocates its own memory and copies the contents. Write it
// yourself whenever a class owns a resource.

#include <cstring>
#include <iostream>
using namespace std;

class ShallowBuffer {
   public:
    int *data;
    int size;

    ShallowBuffer(int count) {
        size = count;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~ShallowBuffer() {
        delete[] data;
    }
};

class DeepBuffer {
    int *data;
    int size;

   public:
    DeepBuffer(int count) {
        size = count;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    // Copy constructor: allocate new memory and copy the values.
    DeepBuffer(const DeepBuffer &other) {
        size = other.size;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Copy assignment: release the old memory, then copy.
    DeepBuffer &operator=(const DeepBuffer &other) {
        if (this == &other) {
            return *this;  // self-assignment
        }

        delete[] data;

        size = other.size;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }

        return *this;
    }

    ~DeepBuffer() {
        delete[] data;
    }

    void set(int index, int value) {
        data[index] = value;
    }

    int get(int index) const {
        return data[index];
    }

    int length() const {
        return size;
    }
};

// A non-copyable owning buffer: delete the copy operations. This is the right
// choice when a resource must have exactly one owner and should never be
// duplicated accidentally.
class ModernBuffer {
    int *data;
    int size;

   public:
    ModernBuffer(int count) {
        size = count;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~ModernBuffer() {
        delete[] data;
    }

    ModernBuffer(const ModernBuffer &) = delete;
    ModernBuffer &operator=(const ModernBuffer &) = delete;
};

int main() {
    DeepBuffer first(3);
    first.set(0, 10);
    first.set(1, 20);
    first.set(2, 30);

    DeepBuffer second = first;  // copy constructor
    second.set(0, 99);

    cout << first.get(0) << " " << first.get(1) << " " << first.get(2) << endl;
    // 10 20 30 (the original is unchanged)

    cout << second.get(0) << " " << second.get(1) << " " << second.get(2)
         << endl;
    // 99 20 30

    DeepBuffer third(2);
    third = first;                 // copy assignment
    cout << third.get(0) << endl;  // 10

    // A shallow copy would make first.data == copy.data. This demo avoids
    // running it, because both destructors would delete the same array.
    ShallowBuffer *shallow = new ShallowBuffer(2);
    ShallowBuffer *alias = shallow;  // two names for one object, still safe
    delete alias;                    // delete only once

    // ModernBuffer forbidden(2);
    // ModernBuffer copy = forbidden;  // error: copy constructor is deleted

    return 0;
}

// Rule of three: if a class needs a destructor, a copy constructor, or a copy
// assignment operator, it usually needs all three.
//
// Special cases:
//   - self-assignment (`a = a`)
//   - copying an empty or zero-size buffer
//   - exceptions during allocation (use RAII containers to avoid leaks)
//   - move semantics: a move constructor can steal the pointer and leave the
//     source empty (see 19_rule_of_three_five_zero.cpp)
//   - prefer `std::vector`, `std::string`, and `std::unique_ptr` so the rule of
//     zero applies and no manual copy code is needed
