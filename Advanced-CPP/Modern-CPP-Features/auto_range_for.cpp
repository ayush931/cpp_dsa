// auto -> asks the compiler to deduce the type from the initializer
// Range-based for loop -> visits every element of a range
// Use references to modify elements and const references to avoid copies

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

int main() {
    auto x = 10;       // int
    auto y = 3.14;     // double
    auto z = "hello";  // const char*
    auto v = vector<int>{1, 2, 3};

    for (int el : v) {
        cout << el << " ";
    }
    cout << endl;

    for (auto el : v) {
        cout << el << " ";
    }
    cout << endl;

    for (auto &el : v) {
        el *= 2;  // modifies the vector
    }
    for (const auto &el : v) {
        cout << el << " ";
    }
    cout << endl;

    map<string, int> m = {{"a", 1}, {"b", 2}};
    for (const auto &p : m) {
        cout << p.first << " " << p.second << endl;
    }

    int arr[] = {4, 5, 6};
    for (auto el : arr) {
        cout << el << " ";
    }
    cout << endl;

    return 0;
}

// auto drops references and top-level const
// auto & keeps a reference, const auto & keeps a const reference
