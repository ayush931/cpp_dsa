/**
 * Exception handling -> handles runtime errors without terminating the program
 * try -> block that may throw an exception
 * throw -> throws an exception
 * catch -> handles an exception of a matching type
 * If no catch matches, std::terminate is called
 * Catch exceptions by reference to avoid slicing and extra copies
 */

#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
using namespace std;

int divide(int a, int b) {
    if (b == 0) {
        throw runtime_error("Division by zero");
    }
    return a / b;
};

int main() {
    try {
        cout << divide(10, 2) << endl;
        cout << divide(10, 0) << endl;
    } catch (const runtime_error &e) {
        cout << e.what() << endl;
    } catch (const exception &e) {
        cout << e.what() << endl;
    } catch (...) {
        cout << "Unknown exception" << endl;
    }

    try {
        throw 42;
    } catch (int x) {
        cout << "Integer exception: " << x << endl;
    }

    try {
        // Ask for an impossible number of elements. The array length exceeds
        // the implementation limit, so the standard requires a bad_alloc
        // (usually the bad_array_new_length derived class) instead of undefined
        // behavior.
        size_t count = numeric_limits<size_t>::max();
        volatile int *arr = new int[count];
        arr[0] = 1;
        cout << arr[0] << endl;
        delete[] arr;
    } catch (const bad_alloc &e) {
        cout << "Allocation failed: " << e.what() << endl;
    }

    return 0;
}

// Stack unwinding -> local objects are destroyed while the stack is searched
// for a handler noexcept -> promises that a function does not throw
