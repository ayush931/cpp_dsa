/**
 * Object lifetime and dangling references
 *
 * An object's lifetime begins when storage is obtained and initialization
 * completes, and ends at destruction (or when storage is reused).
 * Accessing an object outside its lifetime is undefined behavior.
 *
 * Dangling cases:-
 * - Reference to a local returned from a function
 * - Pointer to a vector element after push_back reallocates
 * - Iterator invalidated by insert/erase
 * - Lambda capturing a local by reference and outliving it
 * - string_view into a temporary string
 *
 * RAII -> Resource Acquisition Is Initialization
 * Tie every resource to an object's constructor and destructor, so cleanup
 * happens on every return path and on exceptions.
 */

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

int &danglingReference() {
    int local = 42;
    return local;  // UB: local dies when the function returns
};

vector<int> &danglingVectorReference() {
    vector<int> local = {1, 2, 3};
    return local;  // UB
};

class FileHandle {
    FILE *file;

   public:
    FileHandle(const char *path) {
        file = fopen(path, "w");

        if (file == nullptr) {
            throw runtime_error("cannot open file");
        }
    }

    ~FileHandle() {
        if (file != nullptr) {
            fclose(file);
        }
    }

    FileHandle(const FileHandle &) = delete;
    FileHandle &operator=(const FileHandle &) = delete;

    void write(const string &text) {
        fputs(text.c_str(), file);
    }
};

int main() {
    // safe: return by value
    auto makeVector = []() { return vector<int>{1, 2, 3}; };

    vector<int> values = makeVector();
    cout << values.size() << endl;  // 3

    // reference into a vector becomes invalid after reallocation
    vector<int> numbers = {10, 20, 30};
    int &first = numbers[0];

    numbers.push_back(40);  // may reallocate
    // first may now dangle

    cout << numbers[0] << endl;  // safe, index again

    // iterator invalidation
    vector<int> data = {1, 2, 3, 4};

    for (auto it = data.begin(); it != data.end();) {
        if (*it % 2 == 0) {
            it = data.erase(it);  // erase returns the next valid iterator
        } else {
            ++it;
        }
    }

    for (int value : data) {
        cout << value << " ";  // 1 3
    }
    cout << endl;

    // lambda capturing by reference must not outlive the captured object
    auto makeCounter = []() {
        int count = 0;
        return [&count]() { return ++count; };  // dangling capture
    };

    // auto counter = makeCounter();   // UB when called
    // counter();

    // capture by value instead
    auto makeSafeCounter = []() {
        int count = 0;
        return [count]() mutable { return ++count; };
    };

    auto counter = makeSafeCounter();
    cout << counter() << " " << counter() << endl;  // 1 2

    // RAII: the file is closed even if write throws
    try {
        FileHandle file("/tmp/raii_demo.txt");
        file.write("hello");
    } catch (exception &error) {
        cout << error.what() << endl;
    }

    return 0;
}

// Special cases: returning a reference to a temporary is UB;
// std::string_view and std::span do not own their data;
// weak_ptr detects an expired shared_ptr, shared_ptr cycles leak;
// destructors run in reverse order of construction;
// a destructor must not throw during stack unwinding
