/**
 * Small String Optimization (SSO) and move semantics
 *
 * std::string usually stores short strings inside the object itself
 * (libc++: up to 22 characters on 64-bit, libstdc++: 15)
 * Longer strings allocate on the heap
 *
 * sizeof(std::string) is therefore larger than a pointer + size
 *
 * Moving a string is O(1) for long strings (steal the pointer) and cheap for
 * short strings (copy the inline buffer). Copying a long string is O(n).
 *
 * std::move does not move anything by itself: it is a cast to an rvalue
 * reference that enables the move constructor / move assignment.
 * A moved-from object is valid but unspecified: clear it or reassign it.
 */

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using namespace chrono;

int main() {
    string shortString = "small";
    string longString(1000, 'x');

    cout << sizeof(string) << endl;          // implementation-dependent
    cout << shortString.capacity() << endl;  // >= 5, often 22
    cout << longString.capacity() << endl;   // >= 1000

    // SSO: data points inside the object for a short string. Compare integer
    // addresses because relational comparison of unrelated pointers is not
    // portable.
    uintptr_t dataAddress = reinterpret_cast<uintptr_t>(shortString.data());
    uintptr_t objectBegin = reinterpret_cast<uintptr_t>(&shortString);
    cout << (dataAddress >= objectBegin &&
             dataAddress < objectBegin + sizeof(shortString))
         << endl;

    // move is O(1): the buffer pointer is transferred
    string source(1000000, 'a');
    char *before = source.data();

    string moved = move(source);
    char *after = moved.data();

    cout << (before == after) << endl;  // 1, no copy happened
    cout << source.size() << endl;      // 0 or unspecified, usually 0

    // vector of strings: move avoids copying every element on reallocation
    vector<string> strings;
    strings.reserve(3);

    strings.push_back(string(1000000, 'a'));
    strings.push_back(string(1000000, 'b'));
    strings.push_back(string(1000000, 'c'));

    auto start = high_resolution_clock::now();
    vector<string> copied = strings;  // deep copy
    auto end = high_resolution_clock::now();

    cout << "copy: " << duration_cast<microseconds>(end - start).count()
         << " us" << endl;

    start = high_resolution_clock::now();
    vector<string> movedVector = move(strings);  // steals the buffer
    end = high_resolution_clock::now();

    cout << "move: " << duration_cast<microseconds>(end - start).count()
         << " us" << endl;

    // std::move on a const object silently falls back to copying
    const string constant(1000, 'c');
    string copyOfConstant = move(constant);  // copy, not move

    cout << copyOfConstant.size() << endl;  // 1000

    // do not use a moved-from object except to assign or destroy it
    string reusable = "hello";
    string stolen = move(reusable);
    reusable = "new value";  // valid again

    cout << stolen << " " << reusable << endl;

    return 0;
}

// Special cases: returning a local by value uses NRVO or a move, do not
// write return std::move(local) (it disables NRVO);
// move assignment must handle self-assignment;
// moved-from containers are valid but empty is not guaranteed;
// small objects are cheaper to copy than to move;
// a move constructor should be noexcept, otherwise vector reallocation
// falls back to copying for strong exception safety
